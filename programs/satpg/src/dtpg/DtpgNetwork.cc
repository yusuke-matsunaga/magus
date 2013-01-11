
/// @file DtpgNetwork.cc
/// @brief DtpgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgNetwork.h"
#include "DtpgNode.h"
#include "DtpgGate.h"
#include "DtpgFFR.h"
#include "DtpgFault.h"
#include "DtpgCover.h"
#include "ym_networks/TgNetwork.h"
#include "ym_networks/TgNode.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

BEGIN_NONAMESPACE

inline
DtpgNode**
alloc_nodearray(Alloc& alloc,
		ymuint n)
{
  void* p = alloc.get_memory(sizeof(DtpgNode*) * n);
  return new (p) DtpgNode*[n];
}

void
dfs(DtpgNode* node,
    vector<bool>& mark)
{
  if ( mark[node->id()] ) {
    return;
  }

  mark[node->id()] = true;
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    dfs(node->fanin(i), mark);
  }
}

struct Lt
{
  bool
  operator()(const pair<ymuint, ymuint>& left,
	     const pair<ymuint, ymuint>& right)
  {
    return left.first < right.first;
  }

};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス DtpgNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tgnetwork もとのネットワーク
// @param[in] fault_list 故障リスト
DtpgNetwork::DtpgNetwork(const TgNetwork& tgnetwork,
			 const vector<SaFault*>& fault_list) :
  mAlloc(4096)
{
  mInputNum = tgnetwork.input_num1();
  mOutputNum = tgnetwork.output_num1();
  mFFNum = tgnetwork.ff_num();

  mGateNum = tgnetwork.node_num();
  {
    void* p = mAlloc.get_memory(sizeof(DtpgGate) * mGateNum);
    mGateArray = new (p) DtpgGate[mGateNum];
  }

  // DtpgNode の必要ノード数を計算する．
  ymuint node_num = 0;
  for (ymuint i = 0; i < mGateNum; ++ i) {
    const TgNode* tgnode = tgnetwork.node(i);
    if ( tgnode->is_cplx_logic() ) {
      // 論理式から必要なノード数を見積もる．
      LogExpr lexp = tgnetwork.get_lexp(tgnode->func_id());
      // 基本は全リテラル数 - 1
      node_num += lexp.litnum() - 1;
      ymuint n = lexp.input_size();
      // ただし否定のリテラルは NOT ゲートが必要となる．
      for (ymuint i = 0; i < n; ++ i) {
	node_num += lexp.litnum(VarId(i), kPolNega);
      }
    }
    else {
      // 外部入力，外部出力，単純な論理ノードは1対1に対応する．
      ++ node_num;
    }
  }


  //////////////////////////////////////////////////////////////////////
  // いくつかの配列用のメモリ領域を確保する．
  //////////////////////////////////////////////////////////////////////
  { // これは DtpgNode そのものの配列なので alloc_nodearray() は使えない．
    void* p = mAlloc.get_memory(sizeof(DtpgNode) * node_num);
    mNodeArray = new (p) DtpgNode[node_num];
    mNodeNum = 0;
  }

  mInputArray = alloc_nodearray(mAlloc, input_num2());
  mOutputArray = alloc_nodearray(mAlloc, output_num2());
  mOutputArray2 = alloc_nodearray(mAlloc, output_num2());

  mActNodeNum = 0;
  mActNodeArray = alloc_nodearray(mAlloc, mNodeNum);


  //////////////////////////////////////////////////////////////////////
  // 外部入力をコピーする．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < input_num2(); ++ i) {
    const TgNode* tgnode = tgnetwork.input(i);

    DtpgNode* node = new_node(0);
    node->mGateType = 1U;
    node->mLid = tgnode->lid();

    mInputArray[i] = node;

    DtpgGate* gate = &mGateArray[tgnode->gid()];
    set_gate(gate, node, 0);
  }


  //////////////////////////////////////////////////////////////////////
  // 論理ノードをコピーする．
  // ただし mNodeArray には入力からのトポロジカル順にノードを格納する．
  //////////////////////////////////////////////////////////////////////
  ymuint nl = tgnetwork.logic_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const TgNode* tgnode = tgnetwork.sorted_logic(i);
    ymuint ni = tgnode->ni();
    DtpgNode* node = NULL;
    if ( tgnode->is_cplx_logic() ) {
      LogExpr lexp = tgnetwork.get_lexp(tgnode->func_id());
      node = make_node_tree(lexp, tgnode);
    }
    else {
      DtpgNode* node = new_node(ni);

      node->mGateType = 3U | (static_cast<ymuint>(tgnode->gate_type()) << 2);

      for (ymuint i = 0; i < ni; ++ i) {
	const TgNode* itgnode = tgnode->fanin(i);
	DtpgNode* inode = mGateArray[itgnode->gid()].root_node();
	node->mFanins[i] = inode;
      }
    }

    DtpgGate* gate = &mGateArray[tgnode->gid()];
    set_gate(gate, node, ni);
  }


  //////////////////////////////////////////////////////////////////////
  // 外部出力をコピーする．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < output_num2(); ++ i) {
    const TgNode* tgnode = tgnetwork.output(i);

    DtpgNode* node = new_node(1);
    node->mGateType = 2U;
    node->mLid = tgnode->lid();

    const TgNode* itgnode = tgnode->fanin(0);
    DtpgNode* inode = mGateArray[itgnode->gid()].root_node();
    node->mFanins[0] = inode;

    mOutputArray[i] = node;

    DtpgGate* gate = &mGateArray[tgnode->gid()];
    gate->mRoot = node;
    set_gate(gate, node, 1);
  }

  assert_cond( mNodeNum == node_num, __FILE__, __LINE__);


  //////////////////////////////////////////////////////////////////////
  // ファンアウトの情報を設定する．
  //////////////////////////////////////////////////////////////////////

  // Phase-1: 各ノードのファンアウト数を数える．
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    DtpgNode* node = &mNodeArray[i];
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      DtpgNode* inode = node->fanin(j);
      ++ inode->mFanoutNum;
    }
  }

  // Phase-2: 各ノードのファンアウト用の配列を確保する．
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    DtpgNode* node = &mNodeArray[i];
    node->mFanouts = alloc_nodearray(mAlloc, node->mFanoutNum);
    node->mActFanouts = alloc_nodearray(mAlloc, node->mFanoutNum);
    node->mFanoutNum = 0;
  }

  // Phase-3: 各ノードのファンアウト情報を設定する．
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    DtpgNode* node = &mNodeArray[i];
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      DtpgNode* inode = node->fanin(j);
      inode->mFanouts[inode->mFanoutNum] = node;
      ++ inode->mFanoutNum;
    }
  }

#if 0
  //////////////////////////////////////////////////////////////////////
  // 故障リストを作る．
  //////////////////////////////////////////////////////////////////////
  ymuint nf = fault_list.size();
  void* p = mAlloc.get_memory(sizeof(DtpgFault) * nf);
  mFaultChunk = new (p) DtpgFault[nf];
  mFaultList.reserve(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    SaFault* f = fault_list[i];
    DtpgFault* df = &mFaultChunk[i];
    mFaultList.push_back(df);
    df->mSaFault = f;
    df->mId = i;
    const TgNode* tgnode = f->node();
    DtpgNode* node = mGateArray[tgnode->gid()].root_node();
    df->mNode = node;
    int fval = f->val();
    assert_cond( fval == 0 || fval == 1, __FILE__, __LINE__);
    if ( f->is_input_fault() ) {
      ymuint ipos = f->pos();
      const TgNode* tgsrc = f->source_node();
      df->mSrcNode = mNodeMap[tgsrc->gid()];
      df->mPosVal = (ipos << 4) | (fval << 1) | 1U;
      node->mInputFault[ipos * 2 + fval] = df;
    }
    else {
      df->mSrcNode = NULL;
      df->mPosVal = (fval << 1);
      node->mOutputFault[fval] = df;
    }
  }
#endif

  activate_all();

  // TFI のサイズの昇順に出力を並べ替える．
  vector<pair<ymuint, ymuint> > tmp_list(output_num2());
  for (ymuint i = 0; i < output_num2(); ++ i) {
    vector<bool> mark(mNodeNum, false);
    dfs(output(i), mark);
    ymuint n = 0;
    for (ymuint j = 0; j < mNodeNum; ++ j) {
      if ( mark[j] ) {
	++ n;
      }
    }
    tmp_list[i] = make_pair(n, i);
  }
  sort(tmp_list.begin(), tmp_list.end(), Lt());
  for (ymuint i = 0; i < output_num2(); ++ i) {
    mOutputArray2[i] = mOutputArray[tmp_list[i].second];
  }

#if 0
  // 各関数のカバーを作る．
  ymuint nfunc = tgnetwork.func_num();
  mFuncNum = nfunc;
  {
    void* p = mAlloc.get_memory(sizeof(DtpgCover*) * nfunc * 2);
    mCoverList = new (p) DtpgCover*[nfunc * 2];
  }
  for (ymuint i = 0; i < nfunc; ++ i) {
    const TvFunc& f = tgnetwork.get_func(i);
    ymuint ni = f.ni();
    if ( ni > 20 ) {
      // ちょっと乱暴
      cerr << "Too many inputs (" << ni << ")" << endl;
      abort();
    }
    vector<ymuint32> onset;
    vector<ymuint32> offset;
    ymuint32 nip = 1U << ni;
    for (ymuint32 p = 0; p < nip; ++ p) {
      if ( f.value(p) ) {
	onset.push_back(p);
      }
      else {
	offset.push_back(p);
      }
    }
    mCoverList[i * 2 + 0] = prime_cover(ni, onset);
    mCoverList[i * 2 + 1] = prime_cover(ni, offset);
  }
#endif
}

// @brief デストラクタ
DtpgNetwork::~DtpgNetwork()
{
}

// @brief 一つの外部出力に関係するノードのみをアクティブにする．
// @param[in] onode 外部出力ノード
void
DtpgNetwork::activate_po(DtpgNode* onode)
{
  assert_cond( onode->is_output(), __FILE__, __LINE__);

  vector<bool> mark(mNodeNum, false);

  // pos 番めの出力から到達可能なノードにマークをつける．
  dfs(onode, mark);

  activate_sub(mark);
}

// @brief 全てのノードをアクティブにする．
void
DtpgNetwork::activate_all()
{
  // といっても PO に到達できないノードは除外する．

  vector<bool> mark(mNodeNum, false);

  // すべての PO から到達可能なノードにマークをつける．
  for (ymuint i = 0; i < mOutputNum + mFFNum; ++ i) {
    dfs(output(i), mark);
  }

  activate_sub(mark);
}


BEGIN_NONAMESPACE

DtpgNode*
merge(DtpgNode* node1,
      DtpgNode* node2)
{
  for ( ; ; ) {
    if ( node1 == node2 ) {
      return node1;
    }
    if ( node1 == NULL || node2 == NULL ) {
      return NULL;
    }
    ymuint id1 = node1->id();
    ymuint id2 = node2->id();
    if ( id1 < id2 ) {
      node1 = node1->imm_dom();
    }
    else if ( id1 > id2 ) {
      node2 = node2->imm_dom();
    }
  }
}

END_NONAMESPACE

// @brief activate_po(), activate_all() の下請け関数
void
DtpgNetwork::activate_sub(const vector<bool>& mark)
{
  // マークにしたがってファンアウトなどの情報をセットする．
  mActNodeNum = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    DtpgNode* node = &mNodeArray[i];
    if ( !mark[i] ) {
      node->clear_active();
      continue;
    }
    node->set_active();
    mActNodeArray[mActNodeNum] = node;
    ++ mActNodeNum;

    ymuint nfo = node->fanout_num();
    ymuint act_nfo = 0;
    for (ymuint i = 0; i < nfo; ++ i) {
      DtpgNode* onode = node->fanout(i);
      if ( mark[onode->id()] ) {
	node->mActFanouts[act_nfo] = onode;
	++ act_nfo;
      }
    }
    node->mActFanoutNum = act_nfo;
  }

  // immediate dominator を求める．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint nfo = node->active_fanout_num();
    switch ( nfo ) {
    case 0:
      assert_cond( node->is_output(), __FILE__, __LINE__);
      node->mImmDom = NULL;
      break;

    case 1:
      node->mImmDom = node->active_fanout(0);
      break;

    default:
      {
	DtpgNode* node0 = node->active_fanout(0);
	for (ymuint i = 1; i < nfo; ++ i) {
	  node0 = merge(node0, node->active_fanout(i));
	  if ( node0 == NULL ) {
	    break;
	  }
	}
	node->mImmDom = node0;
      }
      break;
    }
  }
}

// @brief ノードを得る．
DtpgNode*
DtpgNetwork::node(ymuint pos)
{
  // アドレス計算のために DtpgNode の定義が必要なのでインライン関数にしないでここに置く．
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return &mNodeArray[pos];
}

// @brief アクティブな部分に対して FFR を求める．
void
DtpgNetwork::get_ffr_list(vector<DtpgFFR*>& ffr_list)
{
  // FFR を求める．
  vector<DtpgFFR*> ffr_array(mNodeNum);
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint id = node->id();
    if ( node->is_output() || node->active_fanout_num() > 1 ) {
      DtpgFFR* ffr = new DtpgFFR;
      ffr_array[id] = ffr;
      ffr_list.push_back(ffr);
      ffr->mRoot = node;
    }
    else if ( node->active_fanout_num() == 1 ) {
      DtpgNode* fonode = node->active_fanout(0);
      DtpgFFR* ffr = ffr_array[fonode->id()];
      assert_cond( ffr != NULL, __FILE__, __LINE__);
      ffr_array[id] = ffr;
    }
  }

  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[i];
    DtpgFFR* ffr = ffr_array[node->id()];
    assert_cond( ffr != NULL, __FILE__, __LINE__);
    ffr->mNodeList.push_back(node);
  }
}

// @brief アクティブな部分に対して MFFC を求める．
void
DtpgNetwork::get_mffc_list(vector<DtpgFFR*>& mffc_list)
{
  vector<DtpgFFR*> mffc_array(mNodeNum, NULL);
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint id = node->id();
    DtpgNode* root = node->imm_dom();
    if ( root == NULL ) {
      DtpgFFR* mffc = new DtpgFFR;
      mffc_array[id] = mffc;
      mffc_list.push_back(mffc);
      mffc->mRoot = node;
    }
    else {
      mffc_array[id] = mffc_array[root->id()];
    }
  }

  // 入力からのトポロジカル順にMFFC内のノードリストを作る．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[i];
    ymuint id = node->id();
    DtpgFFR* mffc = mffc_array[id];
    assert_cond( mffc != NULL, __FILE__, __LINE__);
    mffc->mNodeList.push_back(node);
  }
}

// @brief fnode の TFO の TFI にマークをつける．
// @param[in] fnode 起点となるノード
// @param[in] tfo_list TFO ノードを入れるリスト
// @param[in] tfi_list TFI ノードを入れるリスト
void
DtpgNetwork::mark_tfo_tfi(DtpgNode* fnode,
			  vector<DtpgNode*>& tfo_list,
			  vector<DtpgNode*>& tfi_list)
{
  // まず fnode の TFO にマークをつける．
  // 同時にマークの付いたノードは tfo_list につまれる．
  tfo_list.clear();
  tfo_list.reserve(mActNodeNum);
  tfo_list.push_back(fnode);
  fnode->set_mark1();
  for (ymuint rpos = 0; rpos < tfo_list.size(); ++ rpos) {
    DtpgNode* node = tfo_list[rpos];
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      DtpgNode* onode = node->active_fanout(i);
      if ( !onode->mark1() ) {
	tfo_list.push_back(onode);
	onode->set_mark1();
      }
    }
  }

  // TFO マークの付いたノードの TFI に別のマークをつける．
  tfi_list.clear();
  tfi_list.reserve(mActNodeNum);
  for (ymuint rpos = 0; rpos < tfo_list.size(); ++ rpos) {
    DtpgNode* node = tfo_list[rpos];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      if ( !inode->mark1() && !inode->mark2() ) {
	// マークの付いていないファンインがあった．
	tfi_list.push_back(inode);
	inode->set_mark2();
      }
    }
  }
  for (ymuint rpos2 = 0; rpos2 < tfi_list.size(); ++ rpos2) {
    DtpgNode* node = tfi_list[rpos2];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      if ( !inode->mark1() && !inode->mark2() ) {
	// マークの付いていないファンインがあった．
	tfi_list.push_back(inode);
	inode->set_mark2();
      }
    }
  }

  // ノードのマークをクリアする．
  for (vector<DtpgNode*>::const_iterator p = tfo_list.begin();
       p != tfo_list.end(); ++ p) {
    DtpgNode* node = *p;
    node->clear_mark1();
    node->clear_mark2();
  }
  for (vector<DtpgNode*>::const_iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    node->clear_mark1();
    node->clear_mark2();
  }
}

// @brief 論理式から DtpgNode を作る．
// @param[in] expr 論理式
// @param[in] tgnode 元のノード (ファンインの情報を得るために必要)
DtpgNode*
DtpgNetwork::make_node_tree(const LogExpr& expr,
			    const TgNode* tgnode)
{
  assert_cond( !expr.is_constant(), __FILE__, __LINE__);

  if ( expr.is_posiliteral() ) {
    ymuint ipos = expr.varid().val();
    const TgNode* itgnode = tgnode->fanin(ipos);
    DtpgNode* inode = mGateArray[itgnode->gid()].root_node();
    return inode;
  }
  if ( expr.is_negaliteral() ) {
    ymuint ipos = expr.varid().val();
    const TgNode* itgnode = tgnode->fanin(ipos);
    DtpgNode* inode = mGateArray[itgnode->gid()].root_node();
    // NOT ゲートを作る．
    DtpgNode* node = new_node(1);
    node->mGateType = 3U | (static_cast<ymuint>(kTgGateNot) << 2);
    node->mFanins[0] = inode;
    return node;
  }

  // 入力からのトポロジカル順に並べたいので
  // ファンインを先に処理しなければならない．
  ymuint nc = expr.child_num();
  vector<DtpgNode*> fanins(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    LogExpr expr1 = expr.child(i);
    fanins[i] = make_node_tree(expr1, tgnode);
  }

  DtpgNode* node = new_node(nc);
  node->mGateType = 3U;
  if ( expr.is_and() ) {
    node->mGateType |= (static_cast<ymuint>(kTgGateAnd) << 2);
  }
  else if ( expr.is_or() ) {
    node->mGateType |= (static_cast<ymuint>(kTgGateOr) << 2);
  }
  else if ( expr.is_xor() ) {
    node->mGateType |= (static_cast<ymuint>(kTgGateXor) << 2);
  }
  for (ymuint i = 0; i < nc; ++ i) {
    node->mFanins[i] = fanins[i];
  }
  return node;
}

// @brief 新しいノードを確保する．
// @param[in] ni 入力数
// @note 実際には mNodeArray はすでに確保済なのでポインタを返すだけ．
// @note 結果 mNodeNum の値が一つ増える．
// @note DtptNode は適切に初期化される．
DtpgNode*
DtpgNetwork::new_node(ymuint ni)
{
  DtpgNode* node = &mNodeArray[mNodeNum];
  node->mId = mNodeNum;
  ++ mNodeNum;

  node->mFaninNum = ni;
  node->mFanins = alloc_nodearray(mAlloc, ni);

  node->mFanoutNum = 0;
  node->mFanouts = NULL;

  node->mActFanoutNum = 0;
  node->mActFanouts = NULL;

  node->mGate = NULL;

  node->mMarks = 0U;

  node->mImmDom = NULL;

  return node;
}

// @brief DtpgGate の設定を行う．
// @param[in] gate DtpgGate
// @param[in] root 根のノード
// @param[in] ni 入力数
void
DtpgNetwork::set_gate(DtpgGate* gate,
		      DtpgNode* root,
		      ymuint ni)
{
  gate->mRoot = root;
  gate->mInputNum = ni;

  gate->mOutputFault[0] = NULL;
  gate->mOutputFault[1] = NULL;

  void* p = mAlloc.get_memory(sizeof(DtpgFault*) * ni * 2);

  gate->mInputFault = new (p) DtpgFault*[ni * 2];
  for (ymuint i = 0; i < ni; ++ i) {
    gate->mInputFault[i * 2 + 0] = NULL;
    gate->mInputFault[i * 2 + 1] = NULL;
  }
}

#if 0
// @brief DtpgNode の内容を設定する．
// @param[in] tgnode もととなる TgNode
// @param[in] node 対象のノード
// @param[in] id ID番号
void
DtpgNetwork::set_node(const TgNode* tgnode,
		      DtpgNode* node,
		      ymuint id)
{
  mNodeMap[tgnode->gid()] = node;
  node->mId = id;

  ymuint ni = tgnode->ni();
  node->mFaninNum = ni;
  node->mFanins = alloc_nodearray(mAlloc, ni);
  {
    void* p = mAlloc.get_memory(sizeof(DtpgFault*) * ni * 2);
    node->mInputFault = new (p) DtpgFault*[ni * 2];
  }

  for (ymuint i = 0; i < ni; ++ i) {
    const TgNode* itgnode = tgnode->fanin(i);
    DtpgNode* inode = mNodeMap[itgnode->gid()];
    node->mFanins[i] = inode;
  }

  ymuint no = tgnode->fanout_num();
  node->mFanoutNum = no;
  node->mFanouts = alloc_nodearray(mAlloc, no);
  node->mActFanoutNum = 0;
  node->mActFanouts = alloc_nodearray(mAlloc, no);

  for (ymuint i = 0; i < ni; ++ i) {
    node->mInputFault[i * 2 + 0] = NULL;
    node->mInputFault[i * 2 + 1] = NULL;
  }
  node->mOutputFault[0] = NULL;
  node->mOutputFault[1] = NULL;

  node->mMarks = 0U;
}
#endif

#if 0
extern
void
prime_gen(ymuint ni,
	  const vector<ymuint32>& minterm_list,
	  vector<vector<Literal> >& cover);


// @brief 最小項のリストからプライムカバーを作る．
DtpgCover*
DtpgNetwork::prime_cover(ymuint ni,
			 const vector<ymuint32>& minterm_list)
{
  vector<vector<Literal> > tmp_cover;
  prime_gen(ni, minterm_list, tmp_cover);

  void* p = mAlloc.get_memory(sizeof(DtpgCover));
  DtpgCover* cover = new (p) DtpgCover;
  ymuint nc = tmp_cover.size();
  cover->mCubeNum = nc;

  void* q = mAlloc.get_memory(sizeof(DtpgCube) * nc);
  cover->mCubeArray = new (q) DtpgCube[nc];

  for (ymuint i = 0; i < nc; ++ i) {
    const vector<Literal>& tmp_cube = tmp_cover[i];
    DtpgCube& cube = cover->mCubeArray[i];
    ymuint nl = tmp_cube.size();
    cube.mLitNum = nl;

    void* r = mAlloc.get_memory(sizeof(Literal) * nl);
    cube.mLitArray = new (r) Literal[nl];
    for (ymuint j = 0; j < nl; ++ j) {
      cube.mLitArray[j] = tmp_cube[j];
    }
  }

  return cover;
}
#endif

END_NAMESPACE_YM_SATPG_DTPG

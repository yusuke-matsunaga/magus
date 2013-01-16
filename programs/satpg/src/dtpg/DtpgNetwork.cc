
/// @file DtpgNetwork.cc
/// @brief DtpgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgNetwork.h"
#include "DtpgNode.h"
#include "DtpgFFR.h"
#include "DtpgFault.h"
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

// 論理式から必要なノード数を数える．
ymuint
subnode_count(const LogExpr& expr)
{
  if ( expr.is_posiliteral() ) {
    return 0;
  }
  if ( expr.is_negaliteral() ) {
    return 1;
  }

  ymuint n = 1;
  ymuint nc = expr.child_num();
  for (ymuint i = 0; i < nc; ++ i) {
    LogExpr expr1 = expr.child(i);
    n += subnode_count(expr1);
  }
  return n;
}

// tTgGateType と入力数から LogExpr を作る．
LogExpr
gate_type_to_expr(tTgGateType gate_type,
		  ymuint ni)
{
  vector<LogExpr> inputs(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    inputs[i] = LogExpr::make_posiliteral(VarId(i));
  }
  switch ( gate_type ) {
  case kTgGateBuff: return inputs[0];
  case kTgGateNot:  return ~inputs[0];
  case kTgGateAnd:  return LogExpr::make_and(inputs);
  case kTgGateNand: return ~LogExpr::make_and(inputs);
  case kTgGateOr:   return LogExpr::make_or(inputs);
  case kTgGateNor:  return ~LogExpr::make_or(inputs);
  case kTgGateXor:  return LogExpr::make_xor(inputs);
  case kTgGateXnor: return ~LogExpr::make_xor(inputs);
  default: assert_not_reached(__FILE__, __LINE__);
  }
  // ダミー
  return LogExpr();
}

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
  //////////////////////////////////////////////////////////////////////
  // 要素数を数え，必要なメモリ領域を確保する．
  //////////////////////////////////////////////////////////////////////

  mNodeNum = tgnetwork.node_num();
  mInputNum = tgnetwork.input_num1();
  mOutputNum = tgnetwork.output_num1();
  mFFNum = tgnetwork.ff_num();

  {
    void* p = mAlloc.get_memory(sizeof(DtpgNode) * mNodeNum);
    mNodeArray = new (p) DtpgNode[mNodeNum];
  }

  mNodeMap = alloc_nodearray(mAlloc, mNodeNum);
  mInputArray = alloc_nodearray(mAlloc, input_num2());
  mOutputArray = alloc_nodearray(mAlloc, output_num2());
  mOutputArray2 = alloc_nodearray(mAlloc, output_num2());

  mActNodeNum = 0;
  mActNodeArray = alloc_nodearray(mAlloc, mNodeNum);


  ymuint id = 0;

  //////////////////////////////////////////////////////////////////////
  // 外部入力を作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npi = input_num2();
  for (ymuint i = 0; i < npi; ++ i, ++ id) {
    const TgNode* tgnode = tgnetwork.input(i);
    DtpgNode* node = make_node(id, tgnode);
    mInputArray[i] = node;
  }


  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  // ただし mNodeArray は入力からのトポロジカル順になる．
  //////////////////////////////////////////////////////////////////////
  ymuint nl = tgnetwork.logic_num();
  for (ymuint i = 0; i < nl; ++ i, ++ id) {
    const TgNode* tgnode = tgnetwork.sorted_logic(i);
    DtpgNode* node = make_node(id, tgnode);
    if ( tgnode->is_cplx_logic() ) {
      ymuint fid = tgnode->func_id();
      LogExpr expr = tgnetwork.get_lexp(fid);
      // サブノード数を数える．
      ymuint nc = subnode_count(expr);
      node->mSubNodeNum = nc;
      void* p = mAlloc.get_memory(sizeof(DtpgNode) * nc);
      node->mSubNodeList = new (p) DtpgNode[nc];
      ymuint subid = 0;
      make_subnode(expr, tgnode, node->mSubNodeList, subid);
      assert_cond( subid == nc, __FILE__, __LINE__);
      node->mExpr = expr;
    }
    else {
      node->mExpr = gate_type_to_expr(tgnode->gate_type(), node->fanin_num());
    }
  }


  //////////////////////////////////////////////////////////////////////
  // 外部出力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_num2();
  for (ymuint i = 0; i < npo; ++ i, ++ id) {
    const TgNode* tgnode = tgnetwork.output(i);
    DtpgNode* node = make_node(id, tgnode);
    mOutputArray[i] = node;
  }

  assert_cond( id == mNodeNum, __FILE__, __LINE__);


  //////////////////////////////////////////////////////////////////////
  // ファンアウトの情報をコピーする．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    const TgNode* tgnode = tgnetwork.node(i);
    ymuint nfo = tgnode->fanout_num();
    DtpgNode* node = mNodeMap[tgnode->gid()];
    for (ymuint i = 0; i < nfo; ++ i) {
      const TgNode* otgnode = tgnode->fanout(i);
      DtpgNode* onode = mNodeMap[otgnode->gid()];
      node->mFanouts[i] = onode;
    }
  }


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
    DtpgNode* node = mNodeMap[tgnode->gid()];
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
  // アドレス計算のために DtpgNode の定義が必要なのでここに置く．
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

// @brief DtpgNode の内容を設定する．
// @param[in] id ID番号
// @param[in] tgnode もととなる TgNode
// @param[in] type ノードタイプ
// @param[in] sub_id サブID
DtpgNode*
DtpgNetwork::make_node(ymuint id,
		       const TgNode* tgnode)
{
  DtpgNode* node = &mNodeArray[id];
  node->mId = id;

  mNodeMap[tgnode->gid()] = node;

  if ( tgnode->is_input() ) {
    node->mTypeId = 1U | (tgnode->lid() << 2);
  }
  else if ( tgnode->is_output() ) {
    node->mTypeId = 2U | (tgnode->lid() << 2);
  }
  else if ( tgnode->is_logic() ) {
    node->mTypeId = 3U | (static_cast<ymuint>(tgnode->gate_type()) << 2);
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }

  node->mSubNodeNum = 0;
  node->mSubNodeList = NULL;

  ymuint ni = tgnode->ni();
  node->mFaninNum = ni;
  if ( ni > 0 ) {
    node->mFanins = alloc_nodearray(mAlloc, ni);

    void* p = mAlloc.get_memory(sizeof(DtpgFault*) * ni * 2);
    node->mInputFault = new (p) DtpgFault*[ni * 2];
  }
  else {
    node->mFanins = NULL;
    node->mInputFault = NULL;
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

  return node;
}

// @brief 複雑な論理式に対応するサブノードを作る．
DtpgNode*
DtpgNetwork::make_subnode(const LogExpr& expr,
			  const TgNode* tgnode,
			  DtpgNode* node_list,
			  ymuint& id)
{
  if ( expr.is_posiliteral() ) {
    ymuint ipos = expr.varid().val();
    const TgNode* itgnode = tgnode->fanin(ipos);
    DtpgNode* inode = mNodeMap[itgnode->gid()];
    return inode;
  }
  if ( expr.is_negaliteral() ) {
    ymuint ipos = expr.varid().val();
    const TgNode* itgnode = tgnode->fanin(ipos);
    DtpgNode* inode = mNodeMap[itgnode->gid()];
    DtpgNode* node = &node_list[id];
    ++ id;
    node->mTypeId = 0U | (kTgGateNot << 2);
    node->mFaninNum = 1;
    node->mFanins = alloc_nodearray(mAlloc, 1);
    node->mFanins[0] = inode;
    return node;
  }

  ymuint nc = expr.child_num();
  vector<DtpgNode*> fanin(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    LogExpr expr1 = expr.child(i);
    DtpgNode* inode = make_subnode(expr1, tgnode, node_list, id);
    fanin[i] = inode;
  }

  DtpgNode* node = &node_list[id];
  ++ id;
  node->mTypeId = 0U;
  if ( expr.is_and() ) {
    node->mTypeId |= (static_cast<ymuint>(kTgGateAnd) << 2);
  }
  else if ( expr.is_or() ) {
    node->mTypeId |= (static_cast<ymuint>(kTgGateOr) << 2);
  }
  else if ( expr.is_xor() ) {
    node->mTypeId |= (static_cast<ymuint>(kTgGateXor) << 2);
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }

  node->mFaninNum = nc;
  node->mFanins = alloc_nodearray(mAlloc, nc);
  for (ymuint i = 0; i < nc; ++ i) {
    node->mFanins[i] = fanin[i];
  }
  return node;
}

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

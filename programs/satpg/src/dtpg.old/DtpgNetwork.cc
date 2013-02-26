
/// @file DtpgNetwork.cc
/// @brief DtpgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgNetwork.h"
#include "DtpgNode.h"
#include "DtpgPrimitive.h"
#include "DtpgFFR.h"
#include "DtpgFault.h"
#include "ym_networks/TgNetwork.h"
#include "ym_networks/TgNode.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

BEGIN_NONAMESPACE

// 全ての論理ノードを kTgGateCplx タイプとして登録する時に true にする．
// DtpgPrimitive 関係のコードのデバッグ用
const bool FORCE_TO_CPLX_LOGIC = false;

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
primitive_count(const LogExpr& expr)
{
  if ( expr.is_posiliteral() ) {
    return 1;
  }
  if ( expr.is_negaliteral() ) {
    return 2;
  }

  ymuint n = 1;
  ymuint nc = expr.child_num();
  for (ymuint i = 0; i < nc; ++ i) {
    LogExpr expr1 = expr.child(i);
    n += primitive_count(expr1);
  }
  return n;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス DtpgNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tgnetwork もとのネットワーク
// @param[in] fault_list 故障リスト
DtpgNetwork::DtpgNetwork(const TgNetwork& tgnetwork) :
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
      // プリミティブ数を数える．
      ymuint np = primitive_count(expr);
      node->mPrimitiveNum = np;
      void* p = mAlloc.get_memory(sizeof(DtpgPrimitive) * np);
      node->mPrimitiveList = new (p) DtpgPrimitive[np];
      ymuint subid = 0;
      make_primitive(expr, tgnode, node->mPrimitiveList, subid);
      assert_cond( subid == np, __FILE__, __LINE__);
    }
    else if ( FORCE_TO_CPLX_LOGIC ) {
      // デバッグ用のコード
      // 組み込み型のゲートも DtpgPrimit を用いて表す．
      ymuint ni = tgnode->fanin_num();
      tTgGateType gate_type = tgnode->gate_type();
      ymuint np = ni + 1;
      node->mPrimitiveNum = np;
      void* p = mAlloc.get_memory(sizeof(DtpgPrimitive) * np);
      node->mPrimitiveList = new (p) DtpgPrimitive[np];
      for (ymuint i = 0; i < ni; ++ i) {
	DtpgPrimitive* prim = &node->mPrimitiveList[i];
	set_input_primitive(prim, i);
      }
      DtpgPrimitive* prim = &node->mPrimitiveList[ni];
      set_logic_primitive(prim, gate_type, ni);
      for (ymuint i = 0; i < ni; ++ i) {
	prim->mFanins[i] = &node->mPrimitiveList[i];
      }
      node->mTypeId = 3U | (kTgGateCplx << 2);
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
#if 0
  ymuint nf = fault_list.size();
  void* p = mAlloc.get_memory(sizeof(DtpgFault) * nf);
  mFaultChunk = new (p) DtpgFault[nf];
  for (ymuint i = 0; i < nf; ++ i) {
    SaFault* f = fault_list[i];
    DtpgFault* df = &mFaultChunk[i];
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
}

// @brief デストラクタ
DtpgNetwork::~DtpgNetwork()
{
}

// @brief 一つの外部出力に関係するノードのみをアクティブにする．
// @param[in] po_pos 出力番号
void
DtpgNetwork::activate_po(ymuint po_pos)
{
  DtpgNode* onode = output2(po_pos);

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
    if ( nfo == 0 ) {
      assert_cond( node->is_output(), __FILE__, __LINE__);
      node->mImmDom = NULL;
    }
    else {
      DtpgNode* node0 = node->active_fanout(0);
      for (ymuint i = 1; i < nfo; ++ i) {
	node0 = merge(node0, node->active_fanout(i));
	if ( node0 == NULL ) {
	  break;
	}
      }
      node->mImmDom = node0;
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

  node->mCval = kB3X;
  node->mNval = kB3X;
  node->mMaVal = kB3X;

  if ( tgnode->is_input() ) {
    node->mTypeId = 1U | (tgnode->lid() << 2);
  }
  else if ( tgnode->is_output() ) {
    node->mTypeId = 2U | (tgnode->lid() << 2);
  }
  else if ( tgnode->is_logic() ) {
    node->mTypeId = 3U | (static_cast<ymuint>(tgnode->gate_type()) << 2);

    switch ( tgnode->gate_type() ) {
    case kTgGateAnd:
    case kTgGateNand:
      node->mCval = kB3False;
      node->mNval = kB3True;
      break;

    case kTgGateOr:
    case kTgGateNor:
      node->mCval = kB3True;
      node->mNval = kB3False;
      break;

    default:
      break;
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }

  node->mPrimitiveNum = 0;
  node->mPrimitiveList = NULL;

  ymuint ni = tgnode->fanin_num();
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

// @brief 複雑な論理式に対応するプリミティブを作る．
DtpgPrimitive*
DtpgNetwork::make_primitive(const LogExpr& expr,
			    const TgNode* tgnode,
			    DtpgPrimitive* primitive_list,
			    ymuint& id)
{
  if ( expr.is_posiliteral() ) {
    ymuint ipos = expr.varid().val();
    DtpgPrimitive* input = &primitive_list[id];
    ++ id;
    set_input_primitive(input, ipos);
    return input;
  }
  if ( expr.is_negaliteral() ) {
    ymuint ipos = expr.varid().val();
    DtpgPrimitive* input = &primitive_list[id];
    ++ id;
    set_input_primitive(input, ipos);

    DtpgPrimitive* prim = &primitive_list[id];
    ++ id;
    set_logic_primitive(prim, kTgGateNot, 1);
    prim->mFanins[0] = input;
    return prim;
  }

  ymuint nc = expr.child_num();
  vector<DtpgPrimitive*> fanin(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    LogExpr expr1 = expr.child(i);
    DtpgPrimitive* inode = make_primitive(expr1, tgnode, primitive_list, id);
    fanin[i] = inode;
  }

  DtpgPrimitive* prim = &primitive_list[id];
  ++ id;

  tTgGateType gate_type;
  if ( expr.is_and() ) {
    gate_type = kTgGateAnd;
  }
  else if ( expr.is_or() ) {
    gate_type = kTgGateOr;
  }
  else if ( expr.is_xor() ) {
    gate_type = kTgGateXor;
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  set_logic_primitive(prim, gate_type, nc);
  for (ymuint i = 0; i < nc; ++ i) {
    prim->mFanins[i] = fanin[i];
  }
  return prim;
}

// @brief 入力プリミティブの設定を行なう．
// @param[in] id 入力番号
void
DtpgNetwork::set_input_primitive(DtpgPrimitive* prim,
				 ymuint id)
{
  prim->mTypeId = 0U | (id << 1);
  prim->mFaninNum = 0;
  prim->mFanins = NULL;
}

// @brief 論理プリミティブの設定を行なう．
void
DtpgNetwork::set_logic_primitive(DtpgPrimitive* prim,
				 tTgGateType gate_type,
				 ymuint ni)
{
  prim->mTypeId = 1U | (static_cast<ymuint32>(gate_type) << 1);
  prim->mFaninNum = ni;

  void* p = mAlloc.get_memory(sizeof(DtpgPrimitive*) * ni);
  prim->mFanins = new (p) DtpgPrimitive*[ni];
}

BEGIN_NONAMESPACE

// node のファンアウトに mark1 をつける．
void
set_tfo_mark(DtpgNode* node)
{
  if ( node->mark1() ) {
    return;
  }
  node->set_mark1();

  ymuint n = node->active_fanout_num();
  for (ymuint i = 0; i < n; ++ i) {
    DtpgNode* onode = node->active_fanout(i);
    set_tfo_mark(onode);
  }
}

// node のファンアウトに mark1 をつける．
void
clear_tfo_mark(DtpgNode* node)
{
  if ( !node->mark1() ) {
    return;
  }
  node->clear_mark1();

  ymuint n = node->active_fanout_num();
  for (ymuint i = 0; i < n; ++ i) {
    DtpgNode* onode = node->active_fanout(i);
    clear_tfo_mark(onode);
  }
}

END_NONAMESPACE

// @brief f の検出に必要な割り当てを求める．
// @param[in] f 対象の故障
// @param[in] ma_list 割り当て結果を格納するリスト
// @return 矛盾が生じたら(fが冗長故障の場合) false を返す．
// @note DtpgNetwork のメンバにはアクセスしないので static メンバになっている．
// @note ma_list の内容は DtpgNode::id() * 2 + val (0 / 1)
bool
DtpgNetwork::get_mandatory_assignment(DtpgFault* f,
				      vector<ymuint32>& ma_list)
{
  DtpgNode* fnode = f->node();
  DtpgNode* fsrc = fnode;
  if ( f->is_input_fault() ) {
    fsrc = f->source_node();
  }

  vector<DtpgNode*> node_list;
  Bool3 gval;
  if ( f->val() == 1 ) {
    gval = kB3False;
  }
  else {
    gval = kB3True;
  }
  bool stat = fsrc->bwd_prop(NULL, gval, node_list);
  if ( !stat ) {
    goto untestable;
  }

  if ( f->is_input_fault() ) {
    Bool3 nval = fnode->nval();
    if ( nval != kB3X ) {
      ymuint ni = fnode->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	if ( i == f->pos() ) continue;
	DtpgNode* inode = fnode->fanin(i);
	if ( !inode->bwd_prop(fnode, nval, node_list) ) {
	  goto untestable;
	}
      }
    }
  }
  if ( fnode->imm_dom() != NULL ) {
    set_tfo_mark(fnode);
    for (DtpgNode* dom = fnode->imm_dom(); dom != NULL; dom = dom->imm_dom()) {
      Bool3 nval = dom->nval();
      if ( nval != kB3X ) {
	ymuint ni = dom->fanin_num();
	for (ymuint i = 0; i < ni; ++ i) {
	  DtpgNode* inode = dom->fanin(i);
	  if ( inode->mark1() ) continue;
	  if ( !inode->bwd_prop(dom, nval, node_list) ) {
	    clear_tfo_mark(fnode);
	    goto untestable;
	  }
	}
      }
    }
    clear_tfo_mark(fnode);
  }

  for (vector<DtpgNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    assert_cond( node->ma_value() != kB3X, __FILE__, __LINE__);
    int val = node->ma_value() == kB3True ? 1 : 0;
    ma_list.push_back(node->id() * 2 + val);
    node->clear_ma_value();
  }
  sort(ma_list.begin(), ma_list.end());
  return true;

 untestable:
  for (vector<DtpgNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    node->clear_ma_value();
  }
  return false;
}

END_NAMESPACE_YM_SATPG_DTPG

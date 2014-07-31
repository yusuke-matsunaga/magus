
/// @file RwtOp.cc
/// @brief RwtOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "RwtOp.h"

#include "RwtMgr.h"
#include "RwtPatList.h"
#include "RwtPat.h"
#include "RwtNode.h"

#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnNode.h"
#include "YmNetworks/BdnConstNodeHandle.h"

#include "YmLogic/TvFunc.h"
#include "YmLogic/NpnMgr.h"
#include "YmLogic/NpnMap.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス RwtOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RwtOp::RwtOp(RwtMgr& mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
RwtOp::~RwtOp()
{
}

// @brief 処理の最初に呼ばれる関数
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
// @param[in] mode カット列挙モード
void
RwtOp::all_init(const BdnMgr& sbjgraph,
		ymuint limit)
{
  mNetwork = &sbjgraph;

  ymuint n = sbjgraph.max_node_id();
  mNodeInfo.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mNodeInfo[i].mMark = 0U;
  }
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
// @param[in] node 根のノード
void
RwtOp::node_init(const BdnNode* node)
{
}

// @brief カットを見つけたときに呼ばれる関数
void
RwtOp::found_cut(const BdnNode* root,
		 ymuint ni,
		 const BdnNode** inputs)
{
  if ( ni == 0 ) {
    return;
  }

  cout << "cut = {" << root->id() << ", ";
  for (ymuint i = 0; i < ni; ++ i) {
    cout << " " << inputs[i]->id();
  }
  cout << "}" << endl;

  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* inode = inputs[i];
    NodeInfo& node_info = mNodeInfo[inode->id()];
    node_info.mMark = 1U;
    node_info.mFunc = TvFunc::posi_literal(mMgr.input_num(), VarId(i));
  }
  calc_func(root);
  clear_mark(root);

  TvFunc f = mNodeInfo[root->id()].mFunc;
  NpnMgr npnmgr;
  NpnMap cmap0;
  npnmgr.cannonical(f, cmap0);
  TvFunc f1 = f.xform(cmap0);
  const RwtPatList* patlist = mMgr.find_patlist(f1);
  if ( patlist == NULL ) {
    return;
  }
  NpnMap cmap = inverse(cmap0);

  vector<BdnNodeHandle> node_map(mMgr.node_num());
  vector<bool> pat_mark(mMgr.node_num());
  ymuint np = patlist->pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    const RwtPat& pat = patlist->pat(i);
    ymuint ni = pat.input_num();
    ymuint nn = pat.node_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const RwtNode* pat_node = pat.node(i);
      assert_cond( pat_node->is_input(), __FILE__, __LINE__);
      ymuint iid = pat_node->input_id();
      NpnVmap vmap = cmap.imap(VarId(iid));
      const BdnNode* inode = inputs[vmap.var().val()];
      node_map[pat_node->id()] = BdnNodeHandle(inode, vmap.inv());
      pat_mark[pat_node->id()] = true;
      mNodeInfo[inode->id()].mMark = 2U;
    }
    ymuint cost = 0;
    for (ymuint i = ni; i < nn; ++ i) {
      const RwtNode* pat_node = pat.node(i);
      if ( pat_node->is_and() || pat_node->is_xor() ) {
	const RwtNode* pat_inode0 = pat_node->fanin0_node();
	const RwtNode* pat_inode1 = pat_node->fanin1_node();
	if ( !pat_mark[pat_inode0->id()] ||
	     !pat_mark[pat_inode1->id()] ) {
	  ++ cost;
	  continue;
	}
	BdnNodeHandle inode0 = node_map[pat_inode0->id()];
	BdnNodeHandle inode1 = node_map[pat_inode1->id()];
	if ( pat_node->fanin0_inv() ) {
	  inode0 = ~inode0;
	}
	if ( pat_node->fanin1_inv() ) {
	  inode1 = ~inode1;
	}
	if ( pat_node->is_and() ) {
	  BdnNodeHandle onode = mNetwork->find_and(inode0, inode1);
	  if ( !onode.is_error() ) {
	    node_map[pat_node->id()] = onode;
	    pat_mark[pat_node->id()] = true;
	    const BdnNode* on = onode.node();
	    mNodeInfo[on->id()].mMark = 2U;
	  }
	}
	else {
	  BdnNodeHandle onode = mNetwork->find_xor(inode0, inode1);
	  if ( !onode.is_error() ) {
	    node_map[pat_node->id()] = onode;
	    pat_mark[pat_node->id()] = true;
	    const BdnNode* on = onode.node();
	    mNodeInfo[on->id()].mMark = 2U;
	  }
	}
      }
    }
    for (ymuint i = 0; i < nn; ++ i) {
      const RwtNode* pat_node = pat.node(i);
      pat_mark[pat_node->id()] = false;
    }
    if ( mNodeInfo[root->id()].mMark & 2U ) {
      continue;
    }
    mNodeInfo[root->id()].mMark = 1U;
    assert_cond( root->is_logic(), __FILE__, __LINE__);
    ymuint n = 0;
    n += count_node(root->fanin0());
    n += count_node(root->fanin1());
    ymint gain = n - cost;
    cout << "Root: " << root->id()
	 << " " << pat.root_node()->id()
	 << " gain = " << gain << " = " << n
	 << " - " << cost << endl;
    clear_mark(root);
  }
#if 0
  cout << "Root: " << root->id() << endl
       << "  func: " << f << endl;
  for (const RwtPat* pat1 = pat; pat1; pat1 = pat1->link()) {
    cout << " " << pat1->node(pat1->node_num() - 1)->id();
  }
  cout << endl;
#endif
}

void
RwtOp::calc_func(const BdnNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( node_info.mMark == 1U ) {
    return;
  }
#if 0
  cout << "calc_func(" << node->id() << ")" << endl;
#endif
  node_info.mMark = 1U;

  assert_cond( node->is_logic(), __FILE__, __LINE__);
  const BdnNode* inode0 = node->fanin0();
  const BdnNode* inode1 = node->fanin1();
  calc_func(inode0);
  calc_func(inode1);
  bool inv0 = node->fanin0_inv();
  bool inv1 = node->fanin1_inv();
  TvFunc func0 = mNodeInfo[inode0->id()].mFunc;
  if ( inv0 ) {
    func0.negate();
  }
  TvFunc func1 = mNodeInfo[inode1->id()].mFunc;
  if ( inv1 ) {
    func1.negate();
  }
  if ( node->is_and() ) {
    node_info.mFunc = func0 & func1;
  }
  else {
    node_info.mFunc = func0 ^ func1;
  }
}

void
RwtOp::clear_mark(const BdnNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( node_info.mMark == 0U ) {
    return;
  }
#if 0
  cout << "clear_mark(" << node->id() << ")" << endl;
#endif
  node_info.mMark = 0U;

  if ( node->is_logic() ) {
    clear_mark(node->fanin0());
    clear_mark(node->fanin1());
  }
}

ymuint
RwtOp::count_node(const BdnNode* node)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  if ( node_info.mMark & 2U ) {
    return 0;
  }
  const BdnFanoutList& fo_list = node->fanout_list();
  for (BdnFanoutList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    BdnEdge* edge = *p;
    const BdnNode* to = edge->to();
    if ( (mNodeInfo[to->id()].mMark & 1U) == 0 ) {
      // このノードは削除できない．
      return 0;
    }
  }
  if ( node->is_input() ) {
    return 1;
  }
#if 0
  cout << "count_node: mark 1U (" << node->id() << ")" << endl;
#endif
  node_info.mMark = 1U;
  ymuint n = 1;
  n += count_node(node->fanin0());
  n += count_node(node->fanin1());
  return n;
}

// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
// @param[in] node 根のノード
void
RwtOp::node_end(const BdnNode* node)
{
}

// @brief 処理の最後に呼ばれる関数
void
RwtOp::all_end(const BdnMgr& sbjgraph,
	       ymuint limit)
{
}

END_NAMESPACE_YM_NETWORKS

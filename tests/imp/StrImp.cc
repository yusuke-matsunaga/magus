
/// @file StrImp.cc
/// @brief StrImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "StrImp.h"
#include "StrNode.h"
#include "SnInput.h"
#include "SnAnd.h"
#include "SnXor.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス StrImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
StrImp::StrImp()
{
}

// @brief デストラクタ
StrImp::~StrImp()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] network 対象のネットワーク
// @param[in] imp_list 間接含意のリスト
void
StrImp::learning(const BdnMgr& network,
		 vector<ImpInfo>& imp_list)
{
  ymuint n = network.max_node_id();

  // BDN の情報を StrNode にコピーする．
  mNodeArray.clear();
  mNodeArray.resize(n, NULL);

  vector<BdnNode*> node_list;
  network.sort(node_list);
  vector<ymuint> fo_count(n, 0);
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    const BdnNode* bnode0 = bnode->fanin0();
    ++ fo_count[bnode0->id()];
    const BdnNode* bnode1 = bnode->fanin1();
    ++ fo_count[bnode1->id()];
  }

  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    ymuint id = bnode->id();
    StrNode* node = new SnInput(id);
    mNodeArray[id] = node;
    node->set_nfo(fo_count[id]);
  }

  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    ymuint id = bnode->id();
    const BdnNode* bnode0 = bnode->fanin0();
    bool inv0 = bnode->fanin0_inv();
    StrNode* node0 = mNodeArray[bnode0->id()];
    assert_cond(node0 != NULL, __FILE__, __LINE__);
    const BdnNode* bnode1 = bnode->fanin1();
    bool inv1 = bnode->fanin1_inv();
    StrNode* node1 = mNodeArray[bnode1->id()];
    assert_cond(node1 != NULL, __FILE__, __LINE__);
    StrNode* node = NULL;
    if ( bnode->is_and() ) {
      node = new SnAnd(id, node0, inv0, node1, inv1);
    }
    else if ( bnode->is_xor() ) {
      node = new SnXor(id, node0, inv0, node1, inv1);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    mNodeArray[id] = node;
    node->set_nfo(fo_count[id]);
  }

  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node = mNodeArray[i];
    if ( node == NULL ) continue;
    cout << "Node#" << node->id() << ":";
    if ( node->is_input() ) {
      cout << "INPUT";
    }
    else if ( node->is_and() ) {
      cout << "AND";
    }
    else if ( node->is_xor() ) {
      cout << "XOR";
    }
    cout << endl;

    if ( node->is_and() || node->is_xor() ) {
      const StrEdge& e0 = node->fanin0();
      cout << "  Fanin0: " << e0.src_node()->id();
      if ( e0.src_inv() ) {
	cout << "~";
      }
      cout << endl;
      const StrEdge& e1 = node->fanin1();
      cout << "  Fanin1: " << e1.src_node()->id();
      if ( e1.src_inv() ) {
	cout << "~";
      }
      cout << endl;
    }

    cout << "  Fanouts: ";
    const vector<StrEdge*>& fo_list = node->fanout_list();
    for (vector<StrEdge*>::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p ) {
      StrEdge* e = *p;
      cout << " (" << e->dst_node()->id() << ", " << e->dst_pos() << ")";
    }
    cout << endl;
  }

  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node = mNodeArray[i];
    if ( node == NULL ) continue;

    // node に 0 を割り当てる．
    bool ok0 = node->bwd_prop0(NULL);
    if ( ok0 ) {
      const vector<StrNode*>& l_list = StrNode::learned_list();
      for (vector<StrNode*>::const_iterator p = l_list.begin();
	   p != l_list.end(); ++ p) {
	StrNode* src_node = *p;
	bool src_val = (node->val() == 1) ? true : false;
	imp_list.push_back(ImpInfo(src_node->id(), src_val, node->id(), true));
      }
      cout << "Node#" << node->id() << ": 0 -> "
	   << l_list.size()
	   << " learned implications" << endl;
    }
    else {
      // 単一の割り当てで矛盾が起こった．
      // node は 1 固定
    }
    StrNode::clear_imp();

    // node に 1 を割り当てる．
    bool ok1 = node->bwd_prop1(NULL);
    if ( ok1 ) {
      const vector<StrNode*>& l_list = StrNode::learned_list();
      for (vector<StrNode*>::const_iterator p = l_list.begin();
	   p != l_list.end(); ++ p) {
	StrNode* src_node = *p;
	bool src_val = (node->val() == 1) ? true : false;
	imp_list.push_back(ImpInfo(src_node->id(), src_val, node->id(), false));
      }
      cout << "Node#" << node->id() << ": 1 -> "
	   << l_list.size()
	   << " learned implications" << endl;
    }
    else {
      // 単一の割り当てで矛盾が起こった．
      // node は 0 固定
    }
    StrNode::clear_imp();
  }
}

END_NAMESPACE_YM_NETWORKS

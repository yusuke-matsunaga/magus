
/// @file libym_cec/make_miter.cc
/// @brief make_miter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cec/cec_nsdef.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnPort.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnNodeHandle.h"
#include "ym_networks/BdnConstNodeHandle.h"


BEGIN_NAMESPACE_YM_CEC

BEGIN_NONAMESPACE

bool debug = true;

ostream&
operator<<(ostream& s,
	   BdnNodeHandle nh)
{
  if ( nh.is_zero() ) {
    s << "***ZERO***";
  }
  else if ( nh.is_one() ) {
    s << "***ONE***";
  }
  else {
    BdnNode* node = nh.node();
    bool inv = nh.inv();
    if ( inv ) {
      s << "~";
    }
    s << "Node[" << node->id() << "]";
  }
  return s;
}

ostream&
operator<<(ostream& s,
	   BdnConstNodeHandle nh)
{
  if ( nh.is_zero() ) {
    s << "***ZERO***";
  }
  else if ( nh.is_one() ) {
    s << "***ONE***";
  }
  else {
    const BdnNode* node = nh.node();
    bool inv = nh.inv();
    if ( inv ) {
      s << "~";
    }
    s << "Node[" << node->id() << "]";
  }
  return s;
}

void
copy_logic(const BdnMgr& src_network,
	   BdnMgr& bdn_mgr,
	   vector<BdnNodeHandle>& bdn_array,
	   ymuint netid)
{
  vector<const BdnNode*> node_list;
  src_network.sort(node_list);

  ymuint nv = src_network.lnode_num();
  assert_cond( nv == node_list.size(), __FILE__, __LINE__);
  for (ymuint i = 0; i < nv; ++ i) {
    const BdnNode* src_node = node_list[i];
    bool is_and = src_node->is_and();

    const BdnNode* src_fanin0 = src_node->fanin0();
    BdnNodeHandle dst_handle0 = bdn_array[src_fanin0->id()];
    if ( src_node->fanin0_inv() ) {
      dst_handle0 = ~dst_handle0;
    }

    const BdnNode* src_fanin1 = src_node->fanin1();
    BdnNodeHandle dst_handle1 = bdn_array[src_fanin1->id()];
    if ( src_node->fanin1_inv() ) {
      dst_handle1 = ~dst_handle1;
    }

    if ( debug ) {
      cout << endl
	   << "registering Node#" << src_node->id() << "@network"
	   << netid << endl
	   << "  " << (is_and ? "and" : "xor") << endl
	   << "  fanins:"
	   << dst_handle0 << ", " << dst_handle1 << endl;
    }

    BdnNodeHandle anode;
    if ( is_and ) {
      anode = bdn_mgr.new_and(dst_handle0, dst_handle1);
    }
    else {
      anode = bdn_mgr.new_xor(dst_handle0, dst_handle1);
    }
    bdn_array[src_node->id()] = anode;

    if ( debug ) {
      cout << "Node#" << src_node->id() << "@network"
	   << netid << " = " << anode << endl;
    }
  }
}

END_NONAMESPACE


void
make_miter(BdnMgr& bdn_mgr,
	   const BdnMgr& src_network1,
	   const BdnMgr& src_network2,
	   const vector<pair<ymuint32, ymuint32> >& iassoc,
	   const vector<pair<ymuint32, ymuint32> >& oassoc,
	   vector<pair<BdnNodeHandle, BdnNodeHandle> >& comp_pairs)
{
  // src_network1 のノード番号をキーにして対応する bdn_mgr のノードを納める配列
  ymuint max1 = src_network1.max_node_id();
  vector<BdnNodeHandle> bdn_array1(max1);

  // src_network2 のノード番号をキーにして対応する bdn_mgr のノードを納める配列
  ymuint max2 = src_network2.max_node_id();
  vector<BdnNodeHandle> bdn_array2(max2);

  comp_pairs.clear();
  comp_pairs.resize(oassoc.size());

  // 入力ノードを作る．
  vector<BdnNodeHandle> input_array(iassoc.size());
  ymuint iid = 0;
  for (vector<pair<ymuint, ymuint> >::const_iterator p = iassoc.begin();
       p != iassoc.end(); ++ p) {
    BdnPort* port = bdn_mgr.new_input_port(string(), 1);
    BdnNodeHandle anode(port->_input(0), false);
    input_array[iid] = anode;
    ++ iid;
    bdn_array1[p->first] = anode;
    bdn_array2[p->second] = anode;
    if ( debug ) {
      const BdnNode* node1 = src_network1.node(p->first);
      const BdnNode* node2 = src_network2.node(p->second);
      cout << endl
	   << "Node#" << node1->id() << "@network1 = " << anode << endl
	   << "Node#" << node2->id() << "@network2 = " << anode << endl;
    }
  }

  // 内部ノード用を作る．
  copy_logic(src_network1, bdn_mgr, bdn_array1, 1);
  copy_logic(src_network2, bdn_mgr, bdn_array2, 2);

  // 外部出力ノードを作る．
  ymuint oid = 0;
  for (vector<pair<ymuint, ymuint> >::const_iterator p = oassoc.begin();
       p != oassoc.end(); ++ p) {
    const BdnNode* onode1 = src_network1.node(p->first);
    BdnNodeHandle ainode1 = bdn_array1[onode1->output_fanin()->id()];
    if ( onode1->output_fanin_inv() ) {
      ainode1 = ~ainode1;
    }

    const BdnNode* onode2 = src_network2.node(p->second);
    BdnNodeHandle ainode2 = bdn_array2[onode2->output_fanin()->id()];
    if ( onode2->output_fanin_inv() ) {
      ainode2 = ~ainode2;
    }

    if ( debug ) {
      cout << "Output#" << oid << ": " << endl
	   << "Node#" << onode1->id() << "@network1"
	   << " = " << ainode1 << endl
	   << "Node#" << onode2->id() << "@network2"
	   << " = " << ainode2 << endl;
    }

    comp_pairs[oid] = make_pair(ainode1, ainode2);
    ++ oid;
  }
}

END_NAMESPACE_YM_CEC

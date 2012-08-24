
/// @file libym_cec/check_ceq.cc
/// @brief check_ceq の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CecMgr.h"
#include "CecNode.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnPort.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnNodeHandle.h"


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

void
copy_logic(const BdnMgr& src_network,
	   BdnMgr& bdn_mgr,
	   vector<BdnNodeHandle>& bdn_array,
	   ymuint netid)
{
  vector<BdnNode*> node_list;
  src_network.sort(node_list);

  ymuint nv = src_network.lnode_num();
  assert_cond( nv == node_list.size(), __FILE__, __LINE__);
  for (ymuint i = 0; i < nv; ++ i) {
    BdnNode* src_node = node_list[i];
    bool is_and = src_node->is_and();
    BdnNodeHandle src_fanin0(src_node->fanin0(), src_node->fanin0_inv());
    BdnNodeHandle src_fanin1(src_node->fanin1(), src_node->fanin1_inv());

    if ( debug ) {
      cout << endl
	   << "registering Node#" << src_node->id() << "@network"
	   << netid << endl
	   << "  " << (is_and ? "and" : "xor") << endl
	   << "  fanins:"
	   << src_fanin0 << ", " << src_fanin1 << endl;
    }

    BdnNodeHandle anode;
    if ( is_and ) {
      anode = bdn_mgr.new_and(src_fanin0, src_fanin1);
    }
    else {
      anode = bdn_mgr.new_xor(src_fanin0, src_fanin1);
    }
    bdn_array[src_node->id()] = anode;

    if ( debug ) {
      cout << "Node#" << src_node->id() << "@network"
	   << netid << " = " << anode << endl;
    }
  }
}

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

// @brief handle の TFI を作る．
CecHandle
make_tficone(BdnNodeHandle handle,
	     CecMgr& cec_mgr,
	     vector<CecHandle>& nodemap)
{
  if ( handle.is_zero() ) {
    return cec_mgr.make_zero();
  }
  if ( handle.is_one() ) {
    return cec_mgr.make_one();
  }

  BdnNode* node = handle.node();
  if ( nodemap[node->id()] == CecHandle() ) {
    assert_cond(node->is_logic(), __FILE__, __LINE__);
    BdnNode* node0 = node->fanin0();
    bool inv0 = node->fanin0_inv();
    BdnNodeHandle ih0(node0, inv0);
    CecHandle fh0 = make_tficone(ih0, cec_mgr, nodemap);
    BdnNode* node1 = node->fanin1();
    bool inv1 = node->fanin1_inv();
    BdnNodeHandle ih1(node1, inv1);
    CecHandle fh1 = make_tficone(ih1, cec_mgr, nodemap);
    if ( node->is_and() ) {
      CecHandle fh = cec_mgr.make_and(fh0, fh1);
      nodemap[node->id()] = fh;
    }
    else if ( node->is_xor() ) {
      CecHandle fh2 = cec_mgr.make_and(fh0, ~fh1);
      CecHandle fh3 = cec_mgr.make_and(~fh0, fh1);
      CecHandle fh = cec_mgr.make_or(fh2, fh3);
      nodemap[node->id()] = fh;
    }
  }
  if ( handle.inv() ) {
    return ~nodemap[node->id()];
  }
  else {
    return nodemap[node->id()];
  }
}

END_NONAMESPACE


void
check_ceq(const BdnMgr& src_network1,
	  const BdnMgr& src_network2,
	  const vector<pair<ymuint32, ymuint32> >& iassoc,
	  const vector<pair<ymuint32, ymuint32> >& oassoc,
	  ymint log_level,
	  ostream* log_out,
	  const string& sat_type,
	  const string& sat_option,
	  ostream* sat_out,
	  ymuint sigsize,
	  vector<Bool3>& stats)
{
  // 2つのネットワークの miter を作る．
  BdnMgr bdn_mgr;
  vector<pair<BdnNodeHandle, BdnNodeHandle> > comp_pairs;
  make_miter(bdn_mgr, src_network1, src_network2,
	     iassoc, oassoc,
	     comp_pairs);

  CecMgr cec_mgr(sat_type, sat_option, sat_out);

  // オプションを設定する．
  cec_mgr.set_loglevel(log_level);
  cec_mgr.set_logstream(log_out);

  ymuint nn = bdn_mgr.max_node_id();
  vector<CecHandle> node_map(nn);
  const BdnNodeList& input_list = bdn_mgr.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    node_map[node->id()] = cec_mgr.make_input();
  }

  ymuint no = src_network1.output_num();
  stats.resize(no);
  for (ymuint i = 0; i < no; ++ i) {
    BdnNodeHandle handle1 = comp_pairs[i].first;
    BdnNodeHandle handle2 = comp_pairs[i].second;
    if ( handle1 == handle2 ) {
      stats[i] = kB3True;
    }
    else {
      CecHandle chandle1 = make_tficone(handle1, cec_mgr, node_map);
      CecHandle chandle2 = make_tficone(handle2, cec_mgr, node_map);
      stats[i] = cec_mgr.check_equiv(chandle1, chandle2);
    }
  }

  if ( log_level > 1 ) {
    cec_mgr.dump_stats(*log_out);
  }
}

END_NAMESPACE_YM_CEC

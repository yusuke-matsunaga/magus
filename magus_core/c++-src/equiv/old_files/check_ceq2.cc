
/// @file libym_cec/check_ceq.cc
/// @brief check_ceq の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CecMgr.h"
#include "CecNode.h"
#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnPort.h"
#include "YmNetworks/BdnNode.h"
#include "YmNetworks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_CEC

extern
void
make_miter(BdnMgr& bdn_mgr,
	   const BdnMgr& src_network1,
	   const BdnMgr& src_network2,
	   const vector<pair<ymuint32, ymuint32> >& iassoc,
	   const vector<pair<ymuint32, ymuint32> >& oassoc,
	   vector<pair<BdnNodeHandle, BdnNodeHandle> >& comp_pairs);

BEGIN_NONAMESPACE

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
    ASSERT_COND(node->is_logic() );
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
check_ceq2(const BdnMgr& src_network1,
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
    if ( log_level > 2 ) {
      (*log_out) << "Checking Output#" << (i + 1) << " / " << no << endl;
    }
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

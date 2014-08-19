
/// @file libym_cec/check_ceq.cc
/// @brief 組み合わせ回路の検証を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "cec_nsdef.h"
#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnPort.h"
#include "YmNetworks/BdnNode.h"
#include "FraigMgr.h"


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

int debug = 0;


// @brief handle の TFI を作る．
FraigHandle
make_tficone(BdnNodeHandle handle,
	     FraigMgr& fraig_mgr,
	     vector<FraigHandle>& map)
{
  if ( handle.is_zero() ) {
    return fraig_mgr.make_zero();
  }
  if ( handle.is_one() ) {
    return fraig_mgr.make_one();
  }

  BdnNode* node = handle.node();
  if ( map[node->id()] == FraigHandle() ) {
    assert_cond(node->is_logic(), __FILE__, __LINE__);
    BdnNode* node0 = node->fanin0();
    bool inv0 = node->fanin0_inv();
    BdnNodeHandle ih0(node0, inv0);
    FraigHandle fh0 = make_tficone(ih0, fraig_mgr, map);
    BdnNode* node1 = node->fanin1();
    bool inv1 = node->fanin1_inv();
    BdnNodeHandle ih1(node1, inv1);
    FraigHandle fh1 = make_tficone(ih1, fraig_mgr, map);
    if ( node->is_and() ) {
      FraigHandle fh = fraig_mgr.make_and(fh0, fh1);
      map[node->id()] = fh;
    }
    else if ( node->is_xor() ) {
      FraigHandle fh2 = fraig_mgr.make_and(fh0, ~fh1);
      FraigHandle fh3 = fraig_mgr.make_and(~fh0, fh1);
      FraigHandle fh = fraig_mgr.make_or(fh2, fh3);
      map[node->id()] = fh;
    }
  }
  if ( handle.inv() ) {
    return ~map[node->id()];
  }
  else {
    return map[node->id()];
  }
}

END_NONAMESPACE


// 新しい関数
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
  // 2つのネットワークの出力の miter 構造を BdnMgr に設定する．
  BdnMgr bdn_mgr;
  vector<pair<BdnNodeHandle, BdnNodeHandle> > comp_pairs;
  make_miter(bdn_mgr, src_network1, src_network2,
	     iassoc, oassoc,
	     comp_pairs);

  FraigMgr fraig_mgr(sigsize, sat_type, sat_option, sat_out);

  // オプションを設定する．
  fraig_mgr.set_loglevel(log_level);
  fraig_mgr.set_logstream(log_out);

  ymuint naig = bdn_mgr.max_node_id();
  vector<FraigHandle> map(naig);
  const BdnNodeList& input_list = bdn_mgr.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    map[node->id()] = fraig_mgr.make_input();
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
      FraigHandle fhandle1 = make_tficone(handle1, fraig_mgr, map);
      FraigHandle fhandle2 = make_tficone(handle2, fraig_mgr, map);
      stats[i] = fraig_mgr.check_equiv(fhandle1, fhandle2);
    }
  }
  if ( log_level > 1 ) {
    fraig_mgr.dump_stats(*log_out);
  }
}

END_NAMESPACE_YM_CEC

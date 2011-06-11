
/// @file libym_cec/check_ceq.cc
/// @brief 組み合わせ回路の検証を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: check_equiv.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cec/cec_nsdef.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnPort.h"
#include "ym_networks/BdnNode.h"
#include "FraigMgr.h"


BEGIN_NAMESPACE_YM_CEC

BEGIN_NONAMESPACE

int debug = 0;

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
    s << node->id_str();
  }
  return s;
}

/// @brief 論理式を BDN に変換する．
BdnNodeHandle
make_logic(const LogExpr& func,
	   BdnMgr& bdn_mgr,
	   const vector<BdnNodeHandle>& inputs)
{
  if ( func.is_zero() ) {
    return BdnNodeHandle::make_zero();
  }
  if ( func.is_one() ) {
    return BdnNodeHandle::make_one();
  }
  if ( func.is_posiliteral() ) {
    return inputs[func.varid()];
  }
  if ( func.is_negaliteral() ) {
    return ~inputs[func.varid()];
  }
  ymuint nc = func.child_num();
  vector<BdnNodeHandle> fanins(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    fanins[i] = make_logic(func.child(i), bdn_mgr, inputs);
  }
  if ( func.is_and() ) {
    return bdn_mgr.new_and(fanins);
  }
  if ( func.is_or() ) {
    return bdn_mgr.new_or(fanins);
  }
  if ( func.is_xor() ) {
    return bdn_mgr.new_xor(fanins);
  }
  assert_not_reached(__FILE__, __LINE__);
  return BdnNodeHandle::make_zero();
}

// @brief 2つのネットワークの miter 構造を作る．
// @param[in] src_network1, src_network2 検証対象のネットワーク
// @param[in] iassoc 対応する2つのネットワークの入力のペアのリスト
// @param[in] oassoc 対応する2つのネットワークの出力のペアのリスト
// @param[out] comp_pairs 比較対象の AigHandle のペアの配列．
void
make_miter(BdnMgr& bdn_mgr,
	   const BNetwork& src_network1,
	   const BNetwork& src_network2,
	   const vector<pair<ymuint, ymuint> >& iassoc,
	   const vector<pair<ymuint, ymuint> >& oassoc,
	   vector<pair<BdnNodeHandle, BdnNodeHandle> >& comp_pairs)
{
  // ネットワーク1のノードに対する BDN ノードを納めた配列
  ymuint max1 = src_network1.max_node_id();
  vector<BdnNodeHandle> bdn_array1(max1);

  // ネットワーク2のノードに対する BDN ノードを納めた配列
  ymuint max2 = src_network2.max_node_id();
  vector<BdnNodeHandle> bdn_array2(max2);

  comp_pairs.clear();
  comp_pairs.resize(oassoc.size());

  // 入力用の BDN ノードを作る．
  // これは2つのネットワークに共通
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
      BNode* bnode1 = src_network1.node(p->first);
      BNode* bnode2 = src_network2.node(p->second);
      cout << endl
	   << bnode1->name() << "@network1 = " << anode << endl
	   << bnode2->name() << "@network2 = " << anode << endl;
    }
  }

  // 内部のノード用の BDN ノードを作る．
  for (ymuint i = 0; i < 2; ++ i) {
    const BNetwork& network = (i == 0) ? src_network1 : src_network2;
    vector<BdnNodeHandle>& bdn_array = (i == 0) ? bdn_array1 : bdn_array2;
    // 内部ノードを作る．
    // まず入力からのトポロジカル順にソートし buff に入れる．
    BNodeVector node_list;
    network.tsort(node_list);
    ymuint nv = network.logic_node_num();
    for (ymuint j = 0; j < nv; ++ j) {
      BNode* bnode = node_list[j];
      ymuint ni = bnode->ni();
      vector<BdnNodeHandle> ianodes(ni);
      for (ymuint pos = 0; pos < ni; ++ pos) {
	ianodes[pos] = bdn_array[bnode->fanin(pos)->id()];
      }
      if ( debug ) {
	cout << endl;
	cout << "registering " << bnode->name() << "@network" << (i + 1) << endl;
	cout << " expr: " << bnode->func() << endl;
	cout << " fanins:";
	for (ymuint i = 0; i < ni; ++ i) {
	  cout << " \"" << bnode->fanin(i)->name() << "\": "
	       << ianodes[i];
	}
	cout << endl;
      }
      BdnNodeHandle anode = make_logic(bnode->func(), bdn_mgr, ianodes);
      bdn_array[bnode->id()] = anode;
      if ( debug ) {
	cout << bnode->name() << "@network" << (i + 1) << " = "
	     << anode << endl;
      }
    }
  }

  // 外部出力用の BDN ノードを作る．
  ymuint oid = 0;
  for (vector<pair<ymuint, ymuint> >::const_iterator p = oassoc.begin();
       p != oassoc.end(); ++ p) {
    BNode* onode1 = src_network1.node(p->first);
    BdnNodeHandle ainode1 = bdn_array1[onode1->fanin(0)->id()];
    BNode* onode2 = src_network2.node(p->second);
    BdnNodeHandle ainode2 = bdn_array2[onode2->fanin(0)->id()];
    if ( debug ) {
      cout << "Output#" << oid << ": " << endl
	   << onode1->name() << "@network1" << " = " << ainode1
	   << " -> " << ainode1 << endl
	   << onode2->name() << "@network2" << " = " << ainode2
	   << " -> " << ainode2 << endl;
    }
    comp_pairs[oid] = make_pair(ainode1, ainode2);
    ++ oid;
  }
}

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
check_ceq(const BNetwork& src_network1,
	  const BNetwork& src_network2,
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

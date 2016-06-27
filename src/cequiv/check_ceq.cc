
/// @file libym_cec/check_ceq.cc
/// @brief 組み合わせ回路の検証を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "cec_nsdef.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "FraigMgr.h"


BEGIN_NAMESPACE_YM_CEC

extern
void
make_miter(BdnMgr& bdn_mgr,
	   const BnNetwork& src_network1,
	   const BnNetwork& src_network2,
	   const vector<pair<ymuint, ymuint> >& iassoc,
	   const vector<pair<ymuint, ymuint> >& oassoc,
	   vector<pair<BdnNodeHandle, BdnNodeHandle> >& comp_pairs);

BEGIN_NONAMESPACE

int debug = 0;

// @brief 他入力

// @brief node に対応する Fraig を作る．
// @param[in] network 元の Boolean Network
// @param[in] node_id 変換対象のノード番号
// @param[in] fraig_mgr FrAIG マネージャ
// @param[in] map BnNode の ID 番号をキーにして FraigHandle を記録した配列
FraigHandle
make_tficone(const BnNetwork& network,
	     ymuint node_id,
	     FraigMgr& fraig_mgr,
	     vector<FraigHandle>& map)
{
  const BnNode* node = network.node(node_id);
  const BnFuncType* func_type = node->func_type();
  if ( func_type->type() == BnFuncType::kFt_C0 ) {
    // 定数0の場合
    return fraig_mgr.make_zero();
  }
  if ( func_type->type() == BnFuncType::kFt_C1 ) {
    // 定数1の場合
    return fraig_mgr.make_one();
  }

  if ( map[node->id()] == FraigHandle() ) {
    // マップに未登録だった
    // 必ず logic タイプのはず．
    ASSERT_COND(node->is_logic() );

    // ファンインに対応する FraigHandle を求める．
    ymuint ni = node->fanin_num();
    vector<FraigHandle> fanin_handles(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      ymuint inode_id = node->fanin_id(i);
      FraigHandle ih = make_tficone(network, inode_id, fraig_mgr, map);
      fanin_handles[i] = ih;
    }

    // 個々の関数タイプに従って FrAIG を生成する．
    switch ( func_type->type() ) {
    case BnFuncType::kFt_C0:
    case BnFuncType::kFt_C1:
      ASSERT_NOT_REACHED;
      break;

    case BnFuncType::kFt_BUFF:
      map[node->id()] = fanin_handles[0];
      break;

    case BnFuncType::kFt_NOT:
      map[node->id()] = ~fanin_handles[0];
      break;

    case BnFuncType::kFt_AND:
      map[node->id()] = fraig_mgr.make_and(fanin_handles);
      break;

    case BnFuncType::kFt_NAND:
      map[node->id()] = ~fraig_mgr.make_and(fanin_handles);
      break;

    case BnFuncType::kFt_OR:
      map[node->id()] = fraig_mgr.make_or(fanin_handles);
      break;

    case BnFuncType::kFt_NOR:
      map[node->id()] = ~fraig_mgr.make_or(fanin_handles);
      break;

    case BnFuncType::kFt_XOR:
      map[node->id()] = fraig_mgr.make_xor(fanin_handles);
      break;

    case BnFuncType::kFt_XNOR:
      map[node->id()] = ~fraig_mgr.make_xor(fanin_handles);
      break;

    case BnFuncType::kFt_CELL:
      {
	const Cell* cell = func_type->cell();
      }
      break;

    case BnFuncType::kFt_EXPR:
      map[node->id()] = fraig_mgr.make_logic(func_type->expr(), fanin_handles);
      break;

    case BnFuncType::kFt_TV:
      {
	TvFunc tv = func_type->truth_vector();
      }
      break;

    }
  }

  return map[node->id()];
}

END_NONAMESPACE


// 新しい関数
void
check_ceq(const BnNetwork& src_network1,
	  const BnNetwork& src_network2,
	  const vector<pair<ymuint, ymuint> >& iassoc,
	  const vector<pair<ymuint, ymuint> >& oassoc,
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

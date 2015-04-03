
/// @file fault_analyzer.cc
/// @brief fault_analyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"

#include "TpgNode.h"
#include "TpgFault.h"
#include "NodeSet.h"
#include "SatEngine.h"
#include "GenVidMap.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

bool
check_intersect(const vector<ymuint>& list1,
		const vector<ymuint>& list2)
{
  ymuint n1 = list1.size();
  ymuint n2 = list2.size();
  ymuint i1 = 0;
  ymuint i2 = 0;
  while ( i1 < n2 && i2 < n2 ) {
    ymuint v1 = list1[i1];
    ymuint v2 = list2[i2];
    if ( v1 < v2 ) {
      ++ i1;
    }
    else if ( v1 > v2 ) {
      ++ i2;
    }
    else {
      return true;
    }
  }
  return false;
}

END_NONAMESPACE

bool
check_dominance(TpgFault* f1,
		TpgFault* f2,
		ymuint max_id)
{
  SatEngine engine(string(), string(), NULL);

  NodeSet node_set1;
  NodeSet node_set2;

  TpgNode* fnode1 = f1->node();
  TpgNode* fnode2 = f2->node();

  node_set1.mark_region(max_id, vector<TpgNode*>(1, fnode1));
  node_set2.mark_region(max_id, vector<TpgNode*>(1, fnode2));

  if ( false ) {
    // 入力が共通部分を持たなければ絶対に関係はない．
    vector<ymuint> input_list1;
    for (ymuint i = 0; i < node_set1.tfo_tfi_size(); ++ i) {
      TpgNode* node = node_set1.tfo_tfi_node(i);
      if ( node->is_input() ) {
	input_list1.push_back(node->input_id());
      }
    }
    sort(input_list1.begin(), input_list1.end());

    vector<ymuint> input_list2;
    for (ymuint i = 0; i < node_set2.tfo_tfi_size(); ++ i) {
      TpgNode* node = node_set2.tfo_tfi_node(i);
      if ( node->is_input() ) {
	input_list2.push_back(node->input_id());
      }
    }
    sort(input_list2.begin(), input_list2.end());

    if ( !check_intersect(input_list1, input_list2) ) {
      return false;
    }
  }

  GenVidMap gvar_map(max_id);

  GenVidMap fvar1_map(max_id);
  GenVidMap dvar1_map(max_id);

  GenVidMap fvar2_map(max_id);
  GenVidMap dvar2_map(max_id);


  vector<bool> mark(max_id);
  vector<TpgNode*> all_list;
  all_list.reserve(node_set1.tfo_tfi_size() + node_set2.tfo_tfi_size());
  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);
    mark[node->id()] = true;
    all_list.push_back(node);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar1_map.set_vid(node, gvar);
    fvar2_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set2.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);
    if ( !mark[node->id()] ) {
      all_list.push_back(node);
      VarId gvar = engine.new_var();
      gvar_map.set_vid(node, gvar);
      fvar1_map.set_vid(node, gvar);
      fvar2_map.set_vid(node, gvar);
    }
  }

  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar1_map.set_vid(node, fvar);
    dvar1_map.set_vid(node, dvar);
  }

  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar2_map.set_vid(node, fvar);
    dvar2_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < all_list.size(); ++ i) {
    TpgNode* node = all_list[i];
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路1の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode1 ) {
      engine.make_fault_cnf(f1, gvar_map, fvar1_map);
    }
    else {
      engine.make_node_cnf(node, fvar1_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar1_map, dvar1_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障1の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo1 = node_set1.output_list().size();
  engine.tmp_lits_begin(npo1);
  for (ymuint i = 0; i < npo1; ++ i) {
    TpgNode* node = node_set1.output_list()[i];
    Literal dlit(dvar1_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  //////////////////////////////////////////////////////////////////////
  // 故障回路2の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode2 ) {
      engine.make_fault_cnf(f2, gvar_map, fvar2_map);
    }
    else {
      engine.make_node_cnf(node, fvar2_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar2_map, dvar2_map);
  }

  // f1 が検出可能で f2 が検出不可能な入力があるか調べる．
  engine.assumption_begin();

  // dominator ノードの dvar は1でなければならない．
  for (TpgNode* node = fnode1; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar1_map(node), false);
    engine.assumption_add(dlit);
  }

  ymuint npo2 = node_set2.output_list().size();
  for (ymuint i = 0; i < npo2; ++ i) {
    TpgNode* node = node_set2.output_list()[i];
    Literal dlit(dvar2_map(node), false);
    engine.assumption_add(~dlit);
  }

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = engine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3False ) {
    // f1 を検出するときは必ず f2 も検出する．
    return true;
  }
  return false;
}

bool
check_conflict(TpgFault* f1,
	       TpgFault* f2,
	       ymuint max_id)
{
  SatEngine engine(string(), string(), NULL);

  NodeSet node_set1;
  NodeSet node_set2;

  TpgNode* fnode1 = f1->node();
  TpgNode* fnode2 = f2->node();

  node_set1.mark_region(max_id, vector<TpgNode*>(1, fnode1));
  node_set2.mark_region(max_id, vector<TpgNode*>(1, fnode2));

  {
    // 入力が共通部分を持たなければ絶対に関係はない．
    vector<ymuint> input_list1;
    for (ymuint i = 0; i < node_set1.tfo_tfi_size(); ++ i) {
      TpgNode* node = node_set1.tfo_tfi_node(i);
      if ( node->is_input() ) {
	input_list1.push_back(node->input_id());
      }
    }
    sort(input_list1.begin(), input_list1.end());

    vector<ymuint> input_list2;
    for (ymuint i = 0; i < node_set2.tfo_tfi_size(); ++ i) {
      TpgNode* node = node_set2.tfo_tfi_node(i);
      if ( node->is_input() ) {
	input_list2.push_back(node->input_id());
      }
    }
    sort(input_list2.begin(), input_list2.end());

    if ( !check_intersect(input_list1, input_list2) ) {
      return false;
    }
  }

  GenVidMap gvar_map(max_id);

  GenVidMap fvar1_map(max_id);
  GenVidMap dvar1_map(max_id);

  GenVidMap fvar2_map(max_id);
  GenVidMap dvar2_map(max_id);


  vector<bool> mark(max_id);
  vector<TpgNode*> all_list;
  all_list.reserve(node_set1.tfo_tfi_size() + node_set2.tfo_tfi_size());
  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);
    mark[node->id()] = true;
    all_list.push_back(node);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar1_map.set_vid(node, gvar);
    fvar2_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set2.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);
    if ( !mark[node->id()] ) {
      all_list.push_back(node);
      VarId gvar = engine.new_var();
      gvar_map.set_vid(node, gvar);
      fvar1_map.set_vid(node, gvar);
      fvar2_map.set_vid(node, gvar);
    }
  }

  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar1_map.set_vid(node, fvar);
    dvar1_map.set_vid(node, dvar);
  }

  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar2_map.set_vid(node, fvar);
    dvar2_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < all_list.size(); ++ i) {
    TpgNode* node = all_list[i];
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路1の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    TpgNode* node = node_set1.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode1 ) {
      engine.make_fault_cnf(f1, gvar_map, fvar1_map);
    }
    else {
      engine.make_node_cnf(node, fvar1_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar1_map, dvar1_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障1の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo1 = node_set1.output_list().size();
  engine.tmp_lits_begin(npo1);
  for (ymuint i = 0; i < npo1; ++ i) {
    TpgNode* node = node_set1.output_list()[i];
    Literal dlit(dvar1_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  //////////////////////////////////////////////////////////////////////
  // 故障回路2の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    TpgNode* node = node_set2.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode2 ) {
      engine.make_fault_cnf(f2, gvar_map, fvar2_map);
    }
    else {
      engine.make_node_cnf(node, fvar2_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar2_map, dvar2_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障2の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo2 = node_set2.output_list().size();
  engine.tmp_lits_begin(npo2);
  for (ymuint i = 0; i < npo2; ++ i) {
    TpgNode* node = node_set2.output_list()[i];
    Literal dlit(dvar2_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  // 両方故障に対するテスト生成を行なう．
  engine.assumption_begin();

  // dominator ノードの dvar は1でなければならない．
  for (TpgNode* node = fnode1; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar1_map(node), false);
    engine.assumption_add(dlit);
  }
  for (TpgNode* node = fnode2; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar2_map(node), false);
    engine.assumption_add(dlit);
  }

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = engine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3False ) {
    // 矛盾していた．
    return true;
  }
  return false;
}

END_NAMESPACE_YM_SATPG

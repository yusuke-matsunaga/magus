
/// @file fault_analyzer.cc
/// @brief fault_analyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"

#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "NodeSet.h"
#include "NodeValList.h"
#include "SatEngine.h"
#include "GenVidMap.h"
#include "ModelValMap.h"
#include "Extractor.h"


BEGIN_NONAMESPACE
const bool do_verify = false;
END_NONAMESPACE

BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

void
dfs(const TpgNode* node,
    HashSet<ymuint>& mark,
    vector<const TpgNode*>& node_list)
{
  if ( mark.check(node->id()) ) {
    return;
  }
  mark.add(node->id());

  node_list.push_back(node);

  ymuint nfo = node->active_fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    const TpgNode* onode = node->active_fanout(i);
    dfs(onode, mark, node_list);
  }
}

struct NodeLt
{
  bool
  operator()(const TpgNode* left,
	     const TpgNode* right)
  {
    return left->id() < right->id();
  }
};

END_NONAMESPACE

void
verify_suf_list(TpgFault* fault,
		const NodeSet& node_set,
		const NodeValList& suf_list,
		const vector<Bool3>& ref_model);

void
analyze_fault(TpgFault* fault,
	      NodeValList& suf_list,
	      NodeValList& ma_list,
	      ymuint max_id)
{
  SatEngine engine(string(), string(), NULL);

  NodeSet node_set;

  GenVidMap gvar_map(max_id);
  GenVidMap fvar_map(max_id);
  GenVidMap dvar_map(max_id);

  const TpgNode* fnode = fault->node();

  node_set.mark_region(max_id, fnode);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      engine.make_fault_cnf(fault, gvar_map, fvar_map);
    }
    else {
      engine.make_node_cnf(node, fvar_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar_map, dvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  engine.tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    const TpgNode* node = node_set.output_list()[i];
    Literal dlit(dvar_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  // 故障に対するテスト生成を行なう．
  engine.assumption_begin();

  // dominator ノードの dvar は1でなければならない．
  for (const TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar_map(node), false);
    engine.assumption_add(dlit);
  }

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = engine.solve(sat_model, sat_stats, sat_time);
  ASSERT_COND ( sat_ans == kB3True );

  // 十分割当を求める．
  ModelValMap val_map(gvar_map, fvar_map, sat_model);
  Extractor extract(val_map);
  extract(fault, suf_list);

  if ( do_verify ) { // 検証
    verify_suf_list(fault, node_set, suf_list, sat_model);
  }

  // 必要割当を求める．
  vector<Bool3> tmp_model;

  ymuint n = suf_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = suf_list[i];

    engine.assumption_begin();

    // dominator ノードの dvar は1でなければならない．
    for (const TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(dvar_map(node), false);
      engine.assumption_add(dlit);
    }

    // node の割当の反対を試す．
    const TpgNode* node = nv.node();
    bool inv = nv.val();
    Literal alit(gvar_map(node), inv);
    engine.assumption_add(alit);

    Bool3 tmp_stat = engine.solve(tmp_model, sat_stats, sat_time);
    if ( tmp_stat == kB3True ) {
      // 反対でも検出できたので必要割当ではない．
      ;
    }
    else if ( tmp_stat == kB3False ) {
      // 失敗したということはこの割当は必要である．
      ma_list.add(node, nv.val());
    }
    else {
      // アボート．とりあえず無視
      ;
    }
  }
}

bool
check_dominance(TpgFault* f1,
		TpgFault* f2,
		ymuint max_id)
{
  SatEngine engine(string(), string(), NULL);

  NodeSet node_set1;
  NodeSet node_set2;

  const TpgNode* fnode1 = f1->node();
  const TpgNode* fnode2 = f2->node();

  node_set1.mark_region(max_id, fnode1);
  node_set2.mark_region(max_id, fnode2);

  GenVidMap gvar_map(max_id);

  GenVidMap fvar1_map(max_id);
  GenVidMap dvar1_map(max_id);

  GenVidMap fvar2_map(max_id);
  GenVidMap dvar2_map(max_id);

  // node_set1 と node_set2 の union を all_list に入れる．
  vector<bool> mark(max_id);
  vector<const TpgNode*> all_list;
  all_list.reserve(node_set1.tfo_tfi_size() + node_set2.tfo_tfi_size());
  for (ymuint i = 0; i < node_set1.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set1.tfo_tfi_node(i);
    mark[node->id()] = true;
    all_list.push_back(node);
  }
  for (ymuint i = 0; i < node_set2.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set2.tfo_tfi_node(i);
    if ( !mark[node->id()] ) {
      all_list.push_back(node);
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < all_list.size(); ++ i) {
    const TpgNode* node = all_list[i];
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar1_map.set_vid(node, gvar);
    fvar2_map.set_vid(node, gvar);
  }

  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    const TpgNode* node = node_set1.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar1_map.set_vid(node, fvar);
    dvar1_map.set_vid(node, dvar);
  }

  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    const TpgNode* node = node_set2.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar2_map.set_vid(node, fvar);
    dvar2_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < all_list.size(); ++ i) {
    const TpgNode* node = all_list[i];
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路1の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    const TpgNode* node = node_set1.tfo_tfi_node(i);

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
    const TpgNode* node = node_set1.output_list()[i];
    Literal dlit(dvar1_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  // dominator ノードの dvar は1でなければならない．
  for (const TpgNode* node = fnode1; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar1_map(node), false);
    engine.add_clause(dlit);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路2の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    const TpgNode* node = node_set2.tfo_tfi_node(i);

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

  ymuint npo2 = node_set2.output_list().size();
  for (ymuint i = 0; i < npo2; ++ i) {
    const TpgNode* node = node_set2.output_list()[i];
    Literal dlit(dvar2_map(node), false);
    engine.add_clause(~dlit);
  }

  // f1 が検出可能で f2 が検出不可能な入力があるか調べる．
  engine.assumption_begin();

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

  const TpgNode* fnode1 = f1->node();
  const TpgNode* fnode2 = f2->node();

  node_set1.mark_region(max_id, fnode1);
  node_set2.mark_region(max_id, fnode2);

  GenVidMap gvar_map(max_id);

  GenVidMap fvar1_map(max_id);
  GenVidMap dvar1_map(max_id);

  GenVidMap fvar2_map(max_id);
  GenVidMap dvar2_map(max_id);

  // node_set1 と node_set2 の union を all_list に入れる．
  vector<bool> mark(max_id);
  vector<const TpgNode*> all_list;
  all_list.reserve(node_set1.tfo_tfi_size() + node_set2.tfo_tfi_size());
  for (ymuint i = 0; i < node_set1.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set1.tfo_tfi_node(i);
    mark[node->id()] = true;
    all_list.push_back(node);
  }
  for (ymuint i = 0; i < node_set2.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set2.tfo_tfi_node(i);
    if ( !mark[node->id()] ) {
      all_list.push_back(node);
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < all_list.size(); ++ i) {
    const TpgNode* node = all_list[i];
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar1_map.set_vid(node, gvar);
    fvar2_map.set_vid(node, gvar);
  }

  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    const TpgNode* node = node_set1.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar1_map.set_vid(node, fvar);
    dvar1_map.set_vid(node, dvar);
  }

  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    const TpgNode* node = node_set2.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar2_map.set_vid(node, fvar);
    dvar2_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < all_list.size(); ++ i) {
    const TpgNode* node = all_list[i];
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路1の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    const TpgNode* node = node_set1.tfo_tfi_node(i);

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
    const TpgNode* node = node_set1.output_list()[i];
    Literal dlit(dvar1_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  // dominator ノードの dvar は1でなければならない．
  for (const TpgNode* node = fnode1; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar1_map(node), false);
    engine.add_clause(dlit);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路2の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    const TpgNode* node = node_set2.tfo_tfi_node(i);

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
    const TpgNode* node = node_set2.output_list()[i];
    Literal dlit(dvar2_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  for (const TpgNode* node = fnode2; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar2_map(node), false);
    engine.add_clause(dlit);
  }

  // 両方故障に対するテスト生成を行なう．
  engine.assumption_begin();

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

void
verify_suf_list(TpgFault* fault,
		const NodeSet& node_set,
		const NodeValList& suf_list,
		const vector<Bool3>& ref_model)
{
  SatEngine engine(string(), string(), NULL);

  const TpgNode* fnode = fault->node();

  ymuint max_id = node_set.max_id();

  GenVidMap gvar_map(max_id);
  GenVidMap fvar_map(max_id);
  GenVidMap dvar_map(max_id);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      engine.make_fault_cnf(fault, gvar_map, fvar_map);
    }
    else {
      engine.make_node_cnf(node, fvar_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar_map, dvar_map);
  }

  // 上の割当のもとでは常に故障が検出できることを検証する．
  engine.assumption_begin();

  ymuint n = suf_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = suf_list[i];
    const TpgNode* node = nv.node();
    VarId vid = gvar_map(node);
    bool inv = nv.val();
    Literal alit(vid, !inv);
    engine.assumption_add(alit);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の非検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  for (ymuint i = 0; i < npo; ++ i) {
    const TpgNode* node = node_set.output_list()[i];
    Literal dlit(dvar_map(node), false);
    engine.assumption_add(~dlit);
  }
  vector<Bool3> tmp_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 tmp_stat = engine.solve(tmp_model, sat_stats, sat_time);
  if ( tmp_stat == kB3True ) {
    cout << fault->str() << endl;
    cout << "ERROR: not a sufficient condition" << endl;
    {
      ModelValMap tmp_val(gvar_map, fvar_map, tmp_model);
      ModelValMap sat_val(gvar_map, fvar_map, ref_model);
      HashSet<ymuint> mark;
      vector<const TpgNode*> node_list;
      dfs(fault->node(), mark, node_list);
      sort(node_list.begin(), node_list.end(), NodeLt());
      for (ymuint i = 0; i < node_list.size(); ++ i) {
	const TpgNode* node = node_list[i];

	cout << "Node#" << setw(5) << node->id() << ": ";
	if ( tmp_val.gval(node) != sat_val.gval(node) ||
	     tmp_val.fval(node) != sat_val.fval(node) ) {
	  cout << "*";
	}
	else {
	  cout << " ";
	}
	cout << tmp_val.gval(node)
	     << "/" << tmp_val.fval(node)
	     << " <=> " << sat_val.gval(node)
	     << "/" << sat_val.fval(node)
	     << ": ";
	if ( node->is_input() ) {
	  cout << "INPUT#" << node->input_id();
	}
	else if ( node->is_output() ) {
	  cout << "OUTPUT#" << node->output_id();
	  const TpgNode* inode = node->fanin(0);
	  cout << " = Node#" << inode->id();
	}
	else if ( node->is_logic() ) {
	  cout << node->gate_type();
	  ymuint ni = node->fanin_num();
	  if ( ni > 0 ) {
	    cout << "(";
	    for (ymuint j = 0; j < ni; ++ j) {
	      const TpgNode* inode = node->fanin(j);
	      cout << " Node#" << inode->id();
	    }
	    cout << ")";
	  }
	}
	else {
	  ASSERT_NOT_REACHED;
	}
	cout << endl;
      }

      for (ymuint i = 0; i < n; ++ i) {
	NodeVal nv = suf_list[i];
	const TpgNode* node = nv.node();
	if ( node_set.tfo_mark(node) ) {
	  continue;
	}
	cout << "Node#" << node->id() << ": ";
	cout << ", gval = " << tmp_val.gval(node)
	     << ": ";
	if ( node->is_input() ) {
	  cout << "INPUT#" << node->input_id();
	}
	else if ( node->is_output() ) {
	  cout << "OUTPUT#" << node->output_id();
	  const TpgNode* inode = node->fanin(0);
	  cout << " = Node#" << inode->id();
	}
	else if ( node->is_logic() ) {
	  cout << node->gate_type();
	  ymuint ni = node->fanin_num();
	  if ( ni > 0 ) {
	    cout << "(";
	    for (ymuint j = 0; j < ni; ++ j) {
	      const TpgNode* inode = node->fanin(j);
	      cout << " Node#" << inode->id();
	    }
	    cout << ")";
	  }
	}
	else {
	  ASSERT_NOT_REACHED;
	}
	cout << endl;
      }
      for (ymuint i = 0; i < n; ++ i) {
	NodeVal nv = suf_list[i];
	const TpgNode* node = nv.node();
	cout << " Node#" << node->id() << ":";
	if ( nv.val() ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << endl;
    }
    exit(1);
  }
}

END_NAMESPACE_YM_SATPG

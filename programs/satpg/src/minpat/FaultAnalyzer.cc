
/// @file FaultAnalyzer.cc
/// @brief FaultAnalyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FaultAnalyzer.h"
#include "TpgFault.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "NodeSet.h"
#include "NodeValList.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FaultAnalyzer
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
FaultAnalyzer::FaultAnalyzer(const string& sat_type,
			     const string& sat_option,
			     ostream* sat_outp) :
  mEngine(sat_type, sat_option, sat_outp)
{
}

// @brief デストラクタ
FaultAnalyzer::~FaultAnalyzer()
{
}

// @brief 初期化する．
void
FaultAnalyzer::init(ymuint max_id)
{
  mNodeMark.clear();
  mNodeMark.resize(max_id, false);

  mGvarMap.init(max_id);
}

// @brief 正常回路のCNFを作る．
// @param[in] network 対象のネットワーク
void
FaultAnalyzer::make_gval_cnf(TpgNetwork& network)
{
  ymuint max_id = network.max_node_id();

  mNodeMark.clear();
  mNodeMark.resize(max_id, false);

  mGvarMap.init(max_id);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < network.active_node_num(); ++ i) {
    const TpgNode* node = network.active_node(i);
    VarId gvar = mEngine.new_var();
    mGvarMap.set_vid(node, gvar);
    mNodeMark[node->id()] = true;
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < network.active_node_num(); ++ i) {
    const TpgNode* node = network.active_node(i);
    mEngine.make_node_cnf(node, mGvarMap);
  }
}

// @brief 正常回路のCNFのもとで2つの割当が両立するか調べる．
// @param[in] list1, list2 割当リスト
bool
FaultAnalyzer::check_intersect(const NodeValList& list1,
			       const NodeValList& list2)
{
  mEngine.assumption_begin();

  add_assumptions(list1);
  add_assumptions(list2);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3True ) {
    return true;
  }
  return false;
}

// @brief 正常回路のCNFのもとで2つの割当が矛盾するか調べる．
// @param[in] list1, list2 割当リスト
bool
FaultAnalyzer::check_conflict(const NodeValList& list1,
			      const NodeValList& list2)
{
  mEngine.assumption_begin();

  add_assumptions(list1);
  add_assumptions(list2);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3False ) {
    return true;
  }
  return false;
}

// @brief 正常回路と故障回路のCNFを作る．
// @param[in] network 対象のネットワーク
// @param[in] fault 故障
void
FaultAnalyzer::make_fval_cnf(TpgNetwork& network,
			     TpgFault* fault)
{
  ymuint max_id = network.max_node_id();

  mNodeMark.clear();
  mNodeMark.resize(max_id, false);

  mGvarMap.init(max_id);
  mFvarMap.init(max_id);
  mDvarMap.init(max_id);

  TpgNode* fnode = fault->node();

  NodeSet node_set;

  node_set.mark_region(max_id, fnode);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = mEngine.new_var();
    mGvarMap.set_vid(node, gvar);
    mFvarMap.set_vid(node, gvar);
    mNodeMark[node->id()] = true;
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = mEngine.new_var();
    VarId dvar = mEngine.new_var();
    mFvarMap.set_vid(node, fvar);
    mDvarMap.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    mEngine.make_node_cnf(node, mGvarMap);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      mEngine.make_fault_cnf(fault, mGvarMap, mFvarMap);
    }
    else {
      mEngine.make_node_cnf(node, mFvarMap);
    }

    // D-Chain 制約を作る．
    mEngine.make_dchain_cnf(node, mGvarMap, mFvarMap, mDvarMap);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  mEngine.tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    const TpgNode* node = node_set.output_list()[i];
    Literal dlit(mDvarMap(node), false);
    mEngine.tmp_lits_add(dlit);
  }
  mEngine.tmp_lits_end();

  for (const TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    Literal dlit(mDvarMap(node), false);
    mEngine.add_clause(dlit);
  }
}

// @brief 故障回路のCNFのもとで割当が両立するか調べる．
// @param[in] list 割当リスト
bool
FaultAnalyzer::check_intersect(const NodeValList& list)
{
  mEngine.assumption_begin();

  add_assumptions(list);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3True ) {
    return true;
  }
  return false;
}

// @brief 故障回路のCNFのもとで割当が矛盾するか調べる．
// @param[in] list 割当リスト
bool
FaultAnalyzer::check_conflict(const NodeValList& list)
{
  mEngine.assumption_begin();

  add_assumptions(list);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3False ) {
    return true;
  }
  return false;
}

// @brief 仮定を追加する．
// @param[in] assign_list 割当リスト
void
FaultAnalyzer::add_assumptions(const NodeValList& assign_list)
{
  for (ymuint i = 0; i < assign_list.size(); ++ i) {
    NodeVal nv = assign_list[i];
    const TpgNode* node = nv.node();
    make_gval_cnf(node);
    Literal alit(mGvarMap(node), false);
    if ( nv.val() ) {
      mEngine.assumption_add(alit);
    }
    else {
      mEngine.assumption_add(~alit);
    }
  }
}

// @brief ノードのCNFを作る．
void
FaultAnalyzer::make_gval_cnf(const TpgNode* node)
{
  if ( mNodeMark[node->id()] ) {
    return;
  }
  mNodeMark[node->id()] = true;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    make_gval_cnf(inode);
  }

  VarId gvar = mEngine.new_var();
  mGvarMap.set_vid(node, gvar);
  mEngine.make_node_cnf(node, mGvarMap);
}

END_NAMESPACE_YM_SATPG

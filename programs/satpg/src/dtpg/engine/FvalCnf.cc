
/// @file FvalCnf.cc
/// @brief FvalCnf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FvalCnf.h"
#include "GvalCnf.h"
#include "NodeSet.h"
#include "SatEngine.h"
#include "TpgNode.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FvalCnf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値
// @param[in] gval_cnf 正常回路のCNFを作るクラス
FvalCnf::FvalCnf(ymuint max_node_id,
		 GvalCnf& gval_cnf) :
  mMaxId(max_node_id),
  mGvalCnf(gval_cnf),
  mMark(max_node_id),
  mFvarMap(max_node_id),
  mDvarMap(max_node_id)
{
}

// @brief デストラクタ
FvalCnf::~FvalCnf()
{
}

// @brief 初期化する．
// @param[in] max_node_id ノード番号の最大値
void
FvalCnf::init(ymuint max_node_id)
{
  mMaxId = max_node_id;
  mMark.clear();
  mMark.resize(max_node_id, false);
  mFvarMap.init(max_node_id);
  mDvarMap.init(max_node_id);
}

// @brief 故障回路のCNFを作る．
// @param[in] engine SATエンジン
// @param[in] fault 故障
// @param[in] node_set 関係するノード集合
// @param[in] detect 検出条件
//
// detect = kVal0: 検出しないCNFを作る．
//        = kVal1: 検出するCNFを作る．
//        = kValX: fd_var() で制御するCNFを作る．
void
FvalCnf::make_cnf(SatEngine& engine,
		  TpgFault* fault,
		  const NodeSet& node_set,
		  Val3 detect)
{
  mGvalCnf.make_cnf(engine, node_set);

  ymuint n = node_set.tfo_size();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    mFvarMap.set_vid(node, fvar);
    mDvarMap.set_vid(node, dvar);
  }
  ymuint n0 = node_set.tfo_tfi_size();
  for (ymuint i = n; i < n0; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    mFvarMap.set_vid(node, gvar_map()(node));
  }

  const TpgNode* fnode = fault->node();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      engine.make_fault_cnf(fault, gvar_map(), mFvarMap);
    }
    else {
      engine.make_node_cnf(node, mFvarMap);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map(), mFvarMap, mDvarMap);
  }

  const vector<const TpgNode*>& output_list = node_set.output_list();
  ymuint npo = output_list.size();

  if ( detect == kVal0 ) {
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_list[i];
      Literal dlit(mDvarMap(node));
      engine.add_clause(~dlit);
    }
  }
  else if ( detect == kVal1 ) {
    engine.tmp_lits_begin(npo);
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_list[i];
      Literal dlit(mDvarMap(node));
      engine.tmp_lits_add(dlit);
    }
    engine.tmp_lits_end();

    for (const TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(mDvarMap(node), false);
      engine.add_clause(dlit);
    }
  }
  else {
    engine.tmp_lits_begin(npo + 1);
    Literal fdlit(mFdVar);
    for (ymuint i = 0; i < npo; ++ i) {
      const TpgNode* node = output_list[i];
      Literal dlit(mDvarMap(node));
      engine.tmp_lits_add(dlit);
      engine.add_clause(fdlit, ~dlit);
    }
    engine.tmp_lits_add(~fdlit);
    engine.tmp_lits_end();

    for (const TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(mDvarMap(node), false);
      engine.add_clause(~fdlit, dlit);
    }
  }
}

// @brief 故障回路のCNFを作る．
// @param[in] engine SATエンジン
// @param[in] fault 故障
//
// detect = kVal0: 検出しないCNFを作る．
//        = kVal1: 検出するCNFを作る．
//        = kValX: fd_var() で制御するCNFを作る．
void
FvalCnf::make_cnf(SatEngine& engine,
		  TpgFault* fault,
		  Val3 detect)
{
  const TpgNode* fnode = fault->node();
  NodeSet node_set;
  node_set.mark_region(mMaxId, fnode);
  make_cnf(engine, fault, node_set, detect);
}

// @brief 割当リストに対応する仮定を追加する．
// @param[in] engine SATエンジン
// @param[in] assign_list 割当リスト
void
FvalCnf::add_assumption(SatEngine& engine,
			const NodeValList& assign_list)
{
  mGvalCnf.add_assumption(engine, assign_list);
}

#if 0
// @brief 割当リストのもとでチェックを行う．
// @param[in] engine SATエンジン
// @param[in] assign_list 割当リスト
// @param[out] suf_list 十分割当リストを格納する変数
void
FvalCnf::get_suf_list(SatEngine& engine,
		      const NodeValList& assign_list,
		      NodeValList& suf_list)
{
  engine.assumption_add();
  mGvalCnf.add_assumption(engine, assign_list);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = engine.solve(sat_model, sat_stats, sat_time);

  ModelValMap val_map(gvar_map(), fvar_map(), sat_model);
  Extractor extract(val_map);
  extract(fault, suf_list);

  return sat_ans;
}
#endif

// @brief 正常回路の変数マップを得る．
const VidMap&
FvalCnf::gvar_map() const
{
  return mGvalCnf.var_map();
}

// @brief 故障変数マップを得る．
const VidMap&
FvalCnf::fvar_map() const
{
  return mFvarMap;
}

// @brief 故障検出用の変数番号を返す．
VarId
FvalCnf::fd_var() const
{
  return mFdVar;
}

// @brief TFO にマークをつけてCNF式を作る．
// @param[in] engine SATエンジン
// @param[in] node ノード
void
FvalCnf::mark_tfo(SatEngine& engine,
		  const TpgNode* node)
{
  if ( mMark[node->id()] ) {
    return;
  }
  mMark[node->id()] = true;

  VarId fvar = engine.new_var();
  mFvarMap.set_vid(node, fvar);
  VarId dvar = engine.new_var();
  mDvarMap.set_vid(node, dvar);

  mFconeList.push_back(node);
  if ( node->is_output() ) {
    mOutputList.push_back(node);
  }

  ymuint nfo = node->active_fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    const TpgNode* onode = node->active_fanout(i);
    mark_tfo(engine, onode);
  }
}

END_NAMESPACE_YM_SATPG

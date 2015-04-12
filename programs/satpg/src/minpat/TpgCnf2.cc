
/// @file TpgCnf2.cc
/// @brief TpgCnf2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "TpgCnf2.h"
#include "TpgFault.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "NodeSet.h"
#include "NodeValList.h"
#include "ModelValMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgCnf2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
TpgCnf2::TpgCnf2(const string& sat_type,
		 const string& sat_option,
		 ostream* sat_outp) :
  mEngine(sat_type, sat_option, sat_outp)
{
}

// @brief デストラクタ
TpgCnf2::~TpgCnf2()
{
}

// @brief f1 が f2 を支配しているか調べる．
// @param[in] f1, f2 故障
// @param[in] max_id ノード番号の最大値
bool
TpgCnf2::check_dominance(TpgFault* f1,
			 NodeSet& node_set1,
			 TpgFault* f2,
			 NodeSet& node_set2,
			 ymuint max_id)
{
  make_fval_cnf(f1, node_set1, f2, node_set2, false, max_id);

  // 両方故障に対するテスト生成を行なう．
  mEngine.assumption_begin();

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3False ) {
    // f1 を検出して f2 を検出しないことはない．
    return true;
  }
  return false;
}

// @brief f1 と f2 がコンフリクトしているか調べる．
// @param[in] f1, f2 故障
// @param[in] max_id ノード番号の最大値
bool
TpgCnf2::check_conflict(TpgFault* f1,
			NodeSet& node_set1,
			TpgFault* f2,
			NodeSet& node_set2,
			ymuint max_id)
{
  make_fval_cnf(f1, node_set1, f2, node_set2, true, max_id);

  // 両方故障に対するテスト生成を行なう．
  mEngine.assumption_begin();

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3False ) {
    // 矛盾していた．
    return true;
  }
  return false;
}

// @brief 正常回路と故障回路のCNFを作る．
// @param[in] f1, f2 故障
// @param[in] detect_f2 f2 を検出するとき true とするフラグ
// @param[in] max_id ノードの最大番号
void
TpgCnf2::make_fval_cnf(TpgFault* f1,
		       NodeSet& node_set1,
		       TpgFault* f2,
		       NodeSet& node_set2,
		       bool detect_f2,
		       ymuint max_id)
{
  mGvarMap.init(max_id);

  mFvar1Map.init(max_id);
  mDvar1Map.init(max_id);

  mFvar2Map.init(max_id);
  mDvar2Map.init(max_id);

  const TpgNode* fnode1 = f1->node();
  const TpgNode* fnode2 = f2->node();

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
    VarId gvar = mEngine.new_var();
    mGvarMap.set_vid(node, gvar);
    mFvar1Map.set_vid(node, gvar);
    mFvar2Map.set_vid(node, gvar);
  }

  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    const TpgNode* node = node_set1.tfo_tfi_node(i);
    VarId fvar = mEngine.new_var();
    VarId dvar = mEngine.new_var();
    mFvar1Map.set_vid(node, fvar);
    mDvar1Map.set_vid(node, dvar);
  }

  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    const TpgNode* node = node_set2.tfo_tfi_node(i);
    VarId fvar = mEngine.new_var();
    VarId dvar = mEngine.new_var();
    mFvar2Map.set_vid(node, fvar);
    mDvar2Map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < all_list.size(); ++ i) {
    const TpgNode* node = all_list[i];
    mEngine.make_node_cnf(node, mGvarMap);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路1の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
    const TpgNode* node = node_set1.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode1 ) {
      mEngine.make_fault_cnf(f1, mGvarMap, mFvar1Map);
    }
    else {
      mEngine.make_node_cnf(node, mFvar1Map);
    }

    // D-Chain 制約を作る．
    mEngine.make_dchain_cnf(node, mGvarMap, mFvar1Map, mDvar1Map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障1の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo1 = node_set1.output_list().size();
  mEngine.tmp_lits_begin(npo1);
  for (ymuint i = 0; i < npo1; ++ i) {
    const TpgNode* node = node_set1.output_list()[i];
    Literal dlit(mDvar1Map(node), false);
    mEngine.tmp_lits_add(dlit);
  }
  mEngine.tmp_lits_end();

  // dominator ノードの dvar は1でなければならない．
  for (const TpgNode* node = fnode1; node != NULL; node = node->imm_dom()) {
    Literal dlit(mDvar1Map(node), false);
    mEngine.add_clause(dlit);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路2の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
    const TpgNode* node = node_set2.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode2 ) {
      mEngine.make_fault_cnf(f2, mGvarMap, mFvar2Map);
    }
    else {
      mEngine.make_node_cnf(node, mFvar2Map);
    }

    // D-Chain 制約を作る．
    mEngine.make_dchain_cnf(node, mGvarMap, mFvar2Map, mDvar2Map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障2の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo2 = node_set2.output_list().size();
  if ( detect_f2 ) {
    mEngine.tmp_lits_begin(npo2);
    for (ymuint i = 0; i < npo2; ++ i) {
      const TpgNode* node = node_set2.output_list()[i];
      Literal dlit(mDvar2Map(node), false);
      mEngine.tmp_lits_add(dlit);
    }
    mEngine.tmp_lits_end();

    for (const TpgNode* node = fnode2; node != NULL; node = node->imm_dom()) {
      Literal dlit(mDvar2Map(node), false);
      mEngine.add_clause(dlit);
    }
  }
  else {
    for (ymuint i = 0; i < npo2; ++ i) {
      const TpgNode* node = node_set2.output_list()[i];
      Literal dlit(mDvar2Map(node), false);
      mEngine.add_clause(~dlit);
    }
  }
}

END_NAMESPACE_YM_SATPG

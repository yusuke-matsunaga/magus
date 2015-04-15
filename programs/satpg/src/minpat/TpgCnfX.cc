
/// @file TpgCnfX.cc
/// @brief TpgCnfX の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "TpgCnfX.h"
#include "TpgFault.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "NodeSet.h"
#include "NodeValList.h"
#include "ModelValMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgCnfX
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
TpgCnfX::TpgCnfX(const string& sat_type,
		 const string& sat_option,
		 ostream* sat_outp) :
  mEngine(sat_type, sat_option, sat_outp)
{
}

// @brief デストラクタ
TpgCnfX::~TpgCnfX()
{
}

// @brief 故障グループが両立か調べる．
// @param[in] fault_group 故障のリスト
// @param[in] max_id ノード番号の最大値
bool
TpgCnfX::check_compat(const vector<TpgFault*>& fault_group,
		      ymuint max_id)
{
  ymuint nf = fault_group.size();

  mFvarMapArray.clear();
  mFvarMapArray.resize(nf);

  mDvarMapArray.clear();
  mDvarMapArray.resize(nf);

  vector<NodeSet> node_set_array(nf);

  mGvarMap.init(max_id);

  for (ymuint fid = 0; fid < nf; ++ fid) {
    TpgFault* fault = fault_group[fid];

    node_set_array[fid].mark_region(max_id, fault->node());

    mFvarMapArray[fid].init(max_id);
    mDvarMapArray[fid].init(max_id);
  }

  // いずれかの故障に関係するノードを all_list に入れる．
  vector<bool> mark(max_id);
  vector<const TpgNode*> all_list;
  for (ymuint fid = 0; fid < nf; ++ fid) {
    NodeSet& node_set = node_set_array[fid];
    for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
      const TpgNode* node = node_set.tfo_tfi_node(i);
      mark[node->id()] = true;
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
    for (ymuint fid = 0; fid < nf; ++ fid) {
      mFvarMapArray[fid].set_vid(node, gvar);
    }
  }

  for (ymuint fid = 0; fid < nf; ++ fid) {
    NodeSet& node_set = node_set_array[fid];
    for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
      const TpgNode* node = node_set.tfo_tfi_node(i);
      VarId fvar = mEngine.new_var();
      VarId dvar = mEngine.new_var();
      mFvarMapArray[fid].set_vid(node, fvar);
      mDvarMapArray[fid].set_vid(node, dvar);
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < all_list.size(); ++ i) {
    const TpgNode* node = all_list[i];
    mEngine.make_node_cnf(node, mGvarMap);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint fid = 0; fid < nf; ++ fid) {
    TpgFault* fault = fault_group[fid];
    TpgNode* fnode = fault->node();

    NodeSet& node_set = node_set_array[fid];
    GenVidMap& fvar_map = mFvarMapArray[fid];
    GenVidMap& dvar_map = mDvarMapArray[fid];

    for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
      const TpgNode* node = node_set.tfo_tfi_node(i);

      // 故障回路のゲートの入出力関係を表すCNFを作る．
      if ( node == fnode ) {
	mEngine.make_fault_cnf(fault, mGvarMap, fvar_map);
      }
      else {
	mEngine.make_node_cnf(node, fvar_map);
      }

      // D-Chain 制約を作る．
      mEngine.make_dchain_cnf(node, mGvarMap, fvar_map, dvar_map);
    }

    //////////////////////////////////////////////////////////////////////
    // 故障1の検出条件
    //////////////////////////////////////////////////////////////////////
    ymuint npo1 = node_set.output_list().size();
    mEngine.tmp_lits_begin(npo1);
    for (ymuint i = 0; i < npo1; ++ i) {
      const TpgNode* node = node_set.output_list()[i];
      Literal dlit(dvar_map(node), false);
      mEngine.tmp_lits_add(dlit);
    }
    mEngine.tmp_lits_end();

    // dominator ノードの dvar は1でなければならない．
    for (const TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(dvar_map(node), false);
      mEngine.add_clause(dlit);
    }
  }

  // 全ての故障を同時に検出するテスト生成を行なう．
  mEngine.assumption_begin();

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3True ) {
    // 両立していた．
    return true;
  }
  return false;
}

END_NAMESPACE_YM_SATPG

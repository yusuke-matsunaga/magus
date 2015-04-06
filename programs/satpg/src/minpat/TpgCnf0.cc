
/// @file TpgCnf0.cc
/// @brief TpgCnf0 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "TpgCnf0.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TestVector.h"
#include "NodeValList.h"
#include "ModelValMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgCnf0
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
TpgCnf0::TpgCnf0(const string& sat_type,
		 const string& sat_option,
		 ostream* sat_outp) :
  mEngine(sat_type, sat_option, sat_outp)
{
}

// @brief デストラクタ
TpgCnf0::~TpgCnf0()
{
}

// @brief 割当を満たすパタンを作る．
// @param[in] network ネットワーク
// @param[in] list 割当リスト
// @param[in] testvector パタン
bool
TpgCnf0::get_testvector(TpgNetwork& network,
			const NodeValList& list,
			TestVector* testvector)
{
  ymuint max_id = network.max_node_id();

  mGvarMap.init(max_id);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < network.active_node_num(); ++ i) {
    const TpgNode* node = network.active_node(i);
    VarId gvar = mEngine.new_var();
    mGvarMap.set_vid(node, gvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < network.active_node_num(); ++ i) {
    const TpgNode* node = network.active_node(i);
    mEngine.make_node_cnf(node, mGvarMap);
  }
  mEngine.assumption_begin();

  add_assumptions(list);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = mEngine.solve(sat_model, sat_stats, sat_time);
  if ( sat_ans == kB3True ) {
    ModelValMap val_map(mGvarMap, mGvarMap, sat_model);
    ymuint ni = network.input_num();
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* node = network.input(i);
      Val3 val = val_map.gval(node);
      testvector->set_val(node->input_id(), val);
    }
    return true;
  }
  return false;
}

// @brief 仮定を追加する．
// @param[in] assign_list 割当リスト
void
TpgCnf0::add_assumptions(const NodeValList& assign_list)
{
  for (ymuint i = 0; i < assign_list.size(); ++ i) {
    NodeVal nv = assign_list[i];
    const TpgNode* node = nv.node();
    Literal alit(mGvarMap(node), false);
    if ( nv.val() ) {
      mEngine.assumption_add(alit);
    }
    else {
      mEngine.assumption_add(~alit);
    }
  }
}

END_NAMESPACE_YM_SATPG

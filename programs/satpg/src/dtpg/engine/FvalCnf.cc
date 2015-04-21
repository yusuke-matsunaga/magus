
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
#include "ModelValMap.h"
#include "Extractor.h"
#include "BackTracer.h"


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
  mFvarMap.init(max_node_id);
  mDvarMap.init(max_node_id);
}

// @brief 十分割当リストを求める．
// @param[in] sat_model SAT問題の解
// @param[in] fault 故障
// @param[in] node_set 故障に関連するノード集合
// @param[out] suf_list 十分割当リストを格納する変数
// @param[out] pi_suf_list 外部入力上の十分割当リストを格納する変数
void
FvalCnf::get_pi_suf_list(const vector<Bool3>& sat_model,
			 const TpgFault* fault,
			 const NodeSet& node_set,
			 NodeValList& suf_list,
			 NodeValList& pi_suf_list)
{
  ModelValMap val_map(gvar_map(), fvar_map(), sat_model);

  Extractor extractor(val_map);
  extractor(fault, suf_list);

  BackTracer backtracer(mMaxId);
  backtracer(fault->node(), node_set, val_map, pi_suf_list);
}

END_NAMESPACE_YM_SATPG

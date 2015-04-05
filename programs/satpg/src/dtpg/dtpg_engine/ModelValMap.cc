
/// @file ModelValMap.cc
/// @brief ModelValMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ModelValMap.h"
#include "TpgNode.h"
#include "VidMap.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// @brief Bool3 から Val3 への変換
inline
Val3
bool3_to_val3(Bool3 bval)
{
  switch ( bval ) {
  case kB3True:  return kVal1;
  case kB3False: return kVal0;
  case kB3X:     return kValX;
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス ModelValMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] gvar_map 正常値の変数マップ
// @param[in] fvar_map 故障値の変数マップ
// @param[in] model SATソルバの作ったモデル
ModelValMap::ModelValMap(const VidMap& gvar_map,
			 const VidMap& fvar_map,
			 const vector<Bool3>& model) :
  mGvarMap(gvar_map),
  mFvarMap(fvar_map),
  mModel(model)
{
}

// @brief デストラクタ
ModelValMap::~ModelValMap()
{
}

// @brief ノードの正常値を返す．
// @param[in] node 対象のノード
Val3
ModelValMap::gval(const TpgNode* node) const
{
  return bool3_to_val3(mModel[mGvarMap(node).val()]);
}

// @brief ノードの故障値を返す．
// @param[in] node 対象のノード
Val3
ModelValMap::fval(const TpgNode* node) const
{
  return bool3_to_val3(mModel[mFvarMap(node).val()]);
}

END_NAMESPACE_YM_SATPG

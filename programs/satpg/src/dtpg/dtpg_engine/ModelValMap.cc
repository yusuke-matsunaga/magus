
/// @file ModelValMap.cc
/// @brief ModelValMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ModelValMap.h"


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
// @param[in] model SATソルバの作ったモデル
ModelValMap::ModelValMap(const vector<Bool3>& model) :
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
ModelValMap::gval(TpgNode* node)
{
  return bool3_to_val3(mModel[node->gvar().val()]);
}

// @brief ノードの故障値を返す．
// @param[in] node 対象のノード
Val3
ModelValMap::fval(TpgNode* node)
{
  return bool3_to_val3(mModel[node->fvar().val()]);
}

END_NAMESPACE_YM_SATPG

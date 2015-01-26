
/// @file IrFloatConst.cc
/// @brief IrFloatConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFloatConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFloatConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
IrFloatConst::IrFloatConst(double val) :
  mVal(val)
{
}

// @brief デストラクタ
IrFloatConst::~IrFloatConst()
{
}

// @brief 種類を返す．
ExprType
IrFloatConst::expr_type() const
{
  return kFloatConst;
}

// @brief 実数値を返す．
//
// kFloatConst の時のみ有効
double
IrFloatConst::float_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL

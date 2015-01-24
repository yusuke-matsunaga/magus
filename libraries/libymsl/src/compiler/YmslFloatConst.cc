
/// @file YmslFloatConst.cc
/// @brief YmslFloatConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslFloatConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslFloatConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
YmslFloatConst::YmslFloatConst(double val) :
  mVal(val)
{
}

// @brief デストラクタ
YmslFloatConst::~YmslFloatConst()
{
}

// @brief 種類を返す．
ExprType
YmslFloatConst::expr_type() const
{
  return kFloatConst;
}

// @brief 実数値を返す．
//
// kFloatConst の時のみ有効
double
YmslFloatConst::float_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL

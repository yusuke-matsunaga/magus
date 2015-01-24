
/// @file AstFloatConst.cc
/// @brief AstFloatConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFloatConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFloatConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstFloatConst::AstFloatConst(double val,
			     const FileRegion& loc) :
  AstExpr(loc),
  mVal(val)
{
}

// @brief デストラクタ
AstFloatConst::~AstFloatConst()
{
}

// @brief 種類を返す．
ExprType
AstFloatConst::expr_type() const
{
  return kFloatConst;
}

// @brief 浮動小数点値を返す．
//
// kFloatConst のみ有効
double
AstFloatConst::float_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL

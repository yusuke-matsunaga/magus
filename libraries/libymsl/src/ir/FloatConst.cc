
/// @file FloatConst.cc
/// @brief FloatConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FloatConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス FloatConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
// @param[in] val 値
FloatConst::FloatConst(const Type* type,
		       double val) :
  ConstVal(type),
  mVal(val)
{
}

// @brief デストラクタ
FloatConst::~FloatConst()
{
}

// @brief 実数値を返す．
double
FloatConst::float_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL


/// @file YmslIntConst.cc
/// @brief YmslIntConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslIntConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslIntConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
YmslIntConst::YmslIntConst(int val) :
  mVal(val)
{
}

// @brief デストラクタ
YmslIntConst::~YmslIntConst()
{
}

// @brief 種類を返す．
ExprType
YmslIntConst::expr_type() const
{
  return kIntConst;
}

// @brief 整数値を返す．
//
// kIntConst の時のみ有効
int
YmslIntConst::int_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL

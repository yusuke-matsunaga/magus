
/// @file IrIntConst.cc
/// @brief IrIntConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrIntConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrIntConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
IrIntConst::IrIntConst(int val) :
  mVal(val)
{
}

// @brief デストラクタ
IrIntConst::~IrIntConst()
{
}

// @brief 種類を返す．
ExprType
IrIntConst::expr_type() const
{
  return kIntConst;
}

// @brief 整数値を返す．
//
// kIntConst の時のみ有効
int
IrIntConst::int_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL

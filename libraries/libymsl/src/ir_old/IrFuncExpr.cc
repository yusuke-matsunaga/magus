
/// @file IrFuncExpr.cc
/// @brief IrFuncExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFuncExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFuncExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
IrFuncExpr::IrFuncExpr(Function* func) :
  mFunction(func)
{
}

// @brief デストラクタ
IrFuncExpr::~IrFuncExpr()
{
}

// @brief 種類を返す．
ExprType
IrFuncExpr::expr_type() const
{
  return kFuncExpr;
}

// @brief 関数を返す．
//
// kFuncExpr のみ有効
Function*
IrFuncExpr::function() const
{
  return mFunction;
}

END_NAMESPACE_YM_YMSL

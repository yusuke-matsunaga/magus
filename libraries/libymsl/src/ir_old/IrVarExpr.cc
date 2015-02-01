
/// @file IrVarExpr.cc
/// @brief IrVarExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrVarExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrVarExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var 変数
IrVarExpr::IrVarExpr(Var* var) :
  mVar(var)
{
}

// @brief デストラクタ
IrVarExpr::~IrVarExpr()
{
}

// @brief 種類を返す．
ExprType
IrVarExpr::expr_type() const
{
  return kSymbolExpr;
}

// @brief 変数を返す．
//
// kPrimary の時のみ有効
Var*
IrVarExpr::var() const
{
  return mVar;
}

END_NAMESPACE_YM_YMSL


/// @file IrScopeExpr.cc
/// @brief IrScopeExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrScopeExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrScopeExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] scope スコープ
IrScopeExpr::IrScopeExpr(IrScope* scope) :
  mScope(scope)
{
}

// @brief デストラクタ
IrScopeExpr::~IrScopeExpr()
{
}

// @brief 種類を返す．
ExprType
IrScopeExpr::expr_type() const
{
  return kScopeExpr;
}

// @brief スコープを返す．
//
// kScopeExpr のみ有効
IrScope*
IrScopeExpr::scope() const
{
  return mScope;
}

END_NAMESPACE_YM_YMSL

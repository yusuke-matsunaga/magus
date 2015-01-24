
/// @file YmslVarExpr.cc
/// @brief YmslVarExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslVarExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslVarExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var 変数
YmslVarExpr::YmslVarExpr(YmslVar* var) :
  mVar(var)
{
}

// @brief デストラクタ
YmslVarExpr::~YmslVarExpr()
{
}

// @brief 種類を返す．
ExprType
YmslVarExpr::expr_type() const
{
  return kSymbolExpr;
}

// @brief 変数を返す．
//
// kPrimary の時のみ有効
YmslVar*
YmslVarExpr::var() const
{
  return mVar;
}

END_NAMESPACE_YM_YMSL

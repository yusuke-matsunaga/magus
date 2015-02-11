
/// @file AstReturn.cc
/// @brief AstReturn の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstReturn.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstReturn
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 値
// @param[in] loc ファイル位置
AstReturn::AstReturn(AstExpr* expr,
		     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr)
{
}

// @brief デストラクタ
AstReturn::~AstReturn()
{
}

// @brief 種類を返す．
AstStatement::Type
AstReturn::stmt_type() const
{
  return kReturn;
}

// @brief 式を返す．
//
// kExprStmt, kReturn のみ有効
const AstExpr*
AstReturn::expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_YMSL

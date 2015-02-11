
/// @file AstExprStmt.cc
/// @brief AstExprStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstExprStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstExprStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 式
// @param[in] loc ファイル位置
AstExprStmt::AstExprStmt(AstExpr* expr,
			 const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr)
{
}

// @brief デストラクタ
AstExprStmt::~AstExprStmt()
{
}

// @brief 種類を返す．
AstStatement::Type
AstExprStmt::stmt_type() const
{
  return kExpr;
}

// @brief 式を返す．
//
// kExprStmt のみ有効
const AstExpr*
AstExprStmt::expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_YMSL

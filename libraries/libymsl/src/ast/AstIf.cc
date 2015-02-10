
/// @file AstIf.cc
/// @brief AstIf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstIf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstIf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 条件式
// @param[in] then_stmt 条件が成り立った時実行される文のリスト
// @param[in] else_stmt 条件が成り立たなかった時実行される文
// @param[in] loc ファイル位置
AstIf::AstIf(AstExpr* expr,
	     AstStatement* then_stmt,
	     AstStatement* else_stmt,
	     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr),
  mStmt(then_stmt),
  mElseStmt(else_stmt)
{
}

// @brief デストラクタ
AstIf::~AstIf()
{
}

// @brief 種類を返す．
AstStatement::Type
AstIf::stmt_type() const
{
  return kIf;
}

// @brief 式を返す．
//
// kAssignment,
// kDoWhile, kFor, kIf, kWhile, kSwitch
// kExprStmt, kReturn, kVarDecl のみ有効
const AstExpr*
AstIf::expr() const
{
  return mExpr;
}

// @brief 本体の文を返す．
//
// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
const AstStatement*
AstIf::stmt() const
{
  return mStmt;
}

// @brief else 節を得る．
//
// kIf のみ有効
const AstStatement*
AstIf::else_stmt() const
{
  return mElseStmt;
}

END_NAMESPACE_YM_YMSL

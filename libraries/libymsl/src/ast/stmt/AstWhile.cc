
/// @file AstWhile.cc
/// @brief AstWhile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstWhile.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件式
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstWhile::AstWhile(AstExpr* cond,
		   AstStatement* stmt,
		   const FileRegion& loc) :
  AstStatement(loc),
  mExpr(cond),
  mStmt(stmt)
{
}

// @brief デストラクタ
AstWhile::~AstWhile()
{
}

// @brief 種類を返す．
AstStatement::Type
AstWhile::stmt_type() const
{
  return kWhile;
}

// @brief 式を返す．
//
// kAssignment,
// kDoWhile, kFor, kIf, kWhile, kSwitch
// kExprStmt, kReturn, kVarDecl のみ有効
const AstExpr*
AstWhile::expr() const
{
  return mExpr;
}

// @brief 本体の文を返す．
//
// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
const AstStatement*
AstWhile::stmt() const
{
  return mStmt;
}

END_NAMESPACE_YM_YMSL

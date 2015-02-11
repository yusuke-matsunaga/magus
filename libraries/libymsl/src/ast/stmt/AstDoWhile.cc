
/// @file AstDoWhile.cc
/// @brief AstDoWhile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstDoWhile.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstDoWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt 本体の文
// @param[in] cond 条件式
// @param[in] loc ファイル位置
AstDoWhile::AstDoWhile(AstStatement* stmt,
		       AstExpr* cond,
		       const FileRegion& loc) :
  AstStatement(loc),
  mStmt(stmt),
  mExpr(cond)
{
}

// @brief デストラクタ
AstDoWhile::~AstDoWhile()
{
}

// @brief 種類を返す．
AstStatement::Type
AstDoWhile::stmt_type() const
{
  return kDoWhile;
}

// @brief 式を返す．
//
// kAssignment,
// kDoWhile, kFor, kIf, kWhile, kSwitch
// kExprStmt, kReturn, kVarDecl のみ有効
const AstExpr*
AstDoWhile::expr() const
{
  return mExpr;
}

// @brief 本体の文を返す．
//
// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
const AstStatement*
AstDoWhile::stmt() const
{
  return mStmt;
}

END_NAMESPACE_YM_YMSL


/// @file AstFor.cc
/// @brief AstFor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstFor.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstFor::AstFor(AstStatement* init,
	       AstExpr* cond,
	       AstStatement* next,
	       AstStatement* stmt,
	       const FileRegion& loc) :
  AstStatement(loc),
  mInit(init),
  mExpr(cond),
  mNext(next),
  mStmt(stmt)
{
}

// @brief デストラクタ
AstFor::~AstFor()
{
}

// @brief 種類を返す．
AstStatement::Type
AstFor::stmt_type() const
{
  return kFor;
}

// @brief 条件式を返す．
//
// kDoWhile, kFor, kIf, kWhile, kSwitch のみ有効
const AstExpr*
AstFor::expr() const
{
  return mExpr;
}

// @brief 初期化文を返す．
//
// kFor のみ有効
const AstStatement*
AstFor::init_stmt() const
{
  return mInit;
}

// @brief 増加文を返す．
//
// kFor のみ有効
const AstStatement*
AstFor::next_stmt() const
{
  return mNext;
}

// @brief 本体の文を返す．
//
// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
const AstStatement*
AstFor::stmt() const
{
  return mStmt;
}

END_NAMESPACE_YM_YMSL

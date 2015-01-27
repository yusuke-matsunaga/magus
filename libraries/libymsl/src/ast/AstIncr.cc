
/// @file AstIncr.cc
/// @brief AstIncr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstIncr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstIncr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 対象の式
// @param[in] loc ファイル位置
AstIncr::AstIncr(AstExpr* expr,
		 const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr)
{
}

// @brief デストラクタ
AstIncr::~AstIncr()
{
}

// @brief 種類を返す．
StmtType
AstIncr::stmt_type() const
{
  return kIncr;
}

// @brief 左辺式を返す．
//
// kAssignment のみ有効
const AstExpr*
AstIncr::lhs_expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_YMSL

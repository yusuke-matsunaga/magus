
/// @file AstAssignment.cc
/// @brief AstAssignment の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstAssignment.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left 左辺
// @param[in] right 右辺
// @param[in] loc ファイル位置
AstAssignment::AstAssignment(AstExpr* left,
			     AstExpr* right,
			     const FileRegion& loc) :
  AstStatement(loc),
  mLeft(left),
  mRight(right)
{
}

// @brief デストラクタ
AstAssignment::~AstAssignment()
{
}

// @brief 種類を返す．
AstStatement::Type
AstAssignment::stmt_type() const
{
  return kAssignment;
}

// @brief 左辺式を返す．
//
// kAssignment のみ有効
const AstExpr*
AstAssignment::lhs_expr() const
{
  return mLeft;
}

// @brief 式を返す．
//
// kAssignment,
// kDoWhile, kFor, kIf, kWhile, kSwitch
// kExprStmt, kReturn, kVarDecl のみ有効
const AstExpr*
AstAssignment::expr() const
{
  return mRight;
}

END_NAMESPACE_YM_YMSL

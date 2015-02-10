
/// @file AstDecr.cc
/// @brief AstDecr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstDecr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstDecr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 対象の式
// @param[in] loc ファイル位置
AstDecr::AstDecr(AstExpr* expr,
		 const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr)
{
}

// @brief デストラクタ
AstDecr::~AstDecr()
{
}

// @brief 種類を返す．
AstStatement::Type
AstDecr::stmt_type() const
{
  return kDecr;
}

// @brief 左辺式を返す．
//
// kAssignment のみ有効
const AstExpr*
AstDecr::lhs_expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_YMSL

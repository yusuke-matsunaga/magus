
/// @file AstEnumConst.cc
/// @brief AstEnumConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstEnumConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstEnumConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] expr 値を表す式
// @param[in] loc ファイル位置
AstEnumConst::AstEnumConst(AstSymbol* name,
			   AstExpr* expr,
			   const FileRegion& loc) :
  Ast(loc),
  mName(name),
  mExpr(expr)
{
}

// @brief デストラクタ
AstEnumConst::~AstEnumConst()
{
}

// @brief 名前を返す．
const AstSymbol*
AstEnumConst::name() const
{
  return mName;
}

// @brief 値を表す式を返す．
//
// NULL の場合もある．
const AstExpr*
AstEnumConst::expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_YMSL

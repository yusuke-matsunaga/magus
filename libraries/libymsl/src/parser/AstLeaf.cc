
/// @file AstLeaf.cc
/// @brief AstLeaf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstLeaf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstLeaf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstLeaf::AstLeaf(const FileRegion& loc) :
  Ast(loc)
{
}

// @brief デストラクタ
AstLeaf::~AstLeaf()
{
}

// @brief シンボルを返す．
//
// kSymbolExpr, kMemberRef のみ有効
const AstSymbol*
AstLeaf::symbol() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 本体を返す．
//
// kArrayRef, kMemberRef, FuncCall のみ有効
const AstLeaf*
AstLeaf::body() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 配列のインデックスを返す．
//
// kArrayRef のみ有効
const AstExpr*
AstLeaf::index() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 引数の数を返す．
//
// kFuncCall のみ有効
ymuint
AstLeaf::arglist_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 引数を返す．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
const AstExpr*
AstLeaf::arglist_elem(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 整数値を返す．
//
// kIntConst のみ有効
int
AstLeaf::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 実数値を返す．
//
// kFloatConst のみ有効
double
AstLeaf::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列値を返す．
//
// kStringConst のみ有効
const char*
AstLeaf::string_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

END_NAMESPACE_YM_YMSL


/// @file YmslExpr.cc
/// @brief YmslExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslExpr::YmslExpr()
{
}

// @brief デストラクタ
YmslExpr::~YmslExpr()
{
}

// @brief 整数値を得る．
//
// kIntConst のみ有効
int
YmslExpr::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 実数値を得る．
//
// kFloatConst のみ有効
double
YmslExpr::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列を得る．
//
// kStringConst のみ有効
const char*
YmslExpr::string_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 終端式を返す．
//
// kLeafExpr の時のみ有効
YmslLeaf*
YmslExpr::leaf() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief オペランドの数を返す．
//
// 演算子の時のみ有効
ymuint
YmslExpr::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num() )
//
// 演算子の時のみ有効
YmslExpr*
YmslExpr::operand(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数本体を返す．
//
// kFuncCall のみ有効
YmslLeaf*
YmslExpr::func() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数の引数の数を得る．
//
// kFuncCall のみ有効
ymuint
YmslExpr::arglist_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 関数の引数を得る．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
YmslExpr*
YmslExpr::arglist_elem(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

END_NAMESPACE_YM_YMSL

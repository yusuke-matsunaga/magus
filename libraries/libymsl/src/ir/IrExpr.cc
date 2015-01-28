
/// @file IrExpr.cc
/// @brief IrExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrExpr::IrExpr()
{
}

// @brief デストラクタ
IrExpr::~IrExpr()
{
}

// @brief 整数値を得る．
//
// kIntConst のみ有効
int
IrExpr::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 実数値を得る．
//
// kFloatConst のみ有効
double
IrExpr::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列を得る．
//
// kStringConst のみ有効
const char*
IrExpr::string_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 変数を返す．
//
// kSymbolExpr のみ有効
Var*
IrExpr::var() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 本体の式を返す．
//
// kMemberRef, kArrayRef のみ有効
IrExpr*
IrExpr::body() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief メンバのオフセットを返す．
//
// kMemberRef のみ有効
ymuint
IrExpr::member_offset() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @biref インデックスの式を返す．
//
// kArrayRef のみ有効
IrExpr*
IrExpr::array_index() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief オペランドの数を返す．
//
// 演算子の時のみ有効
ymuint
IrExpr::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num() )
//
// 演算子の時のみ有効
IrExpr*
IrExpr::operand(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数本体を返す．
//
// kFuncCall のみ有効
IrExpr*
IrExpr::func_expr() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数の引数の数を得る．
//
// kFuncCall のみ有効
ymuint
IrExpr::arglist_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 関数の引数を得る．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
IrExpr*
IrExpr::arglist_elem(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief スコープを返す．
//
// kIrScopeExpr のみ有効
Scope*
IrExpr::scope() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief enum 型を返す．
//
// kEnumExpr のみ返す．
const Type*
IrExpr::enum_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数を返す．
//
// kFuncExpr のみ有効
Function*
IrExpr::function() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

END_NAMESPACE_YM_YMSL

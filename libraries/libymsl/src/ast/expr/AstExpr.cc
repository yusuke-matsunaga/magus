
/// @file AstExpr.cc
/// @brief AstExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstExpr::AstExpr(const FileRegion& loc) :
  Ast(loc)
{
}

// @brief デストラクタ
AstExpr::~AstExpr()
{
}

// @brief 整数値を返す．
//
// kIntConst のみ有効
Ymsl_INT
AstExpr::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 実数値を返す．
//
// kFloatConst のみ有効
Ymsl_FLOAT
AstExpr::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列値を返す．
//
// kStringConst のみ有効
const char*
AstExpr::string_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief シンボルを返す．
//
// kSymbolExpr のみ有効
const AstSymbol*
AstExpr::symbol() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 本体を返す．
//
// kArrayRef, kMemberRef のみ有効
const AstExpr*
AstExpr::body() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief メンバ名を返す．
const AstSymbol*
AstExpr::member() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 配列のインデックスを返す．
//
// kArrayRef のみ有効
const AstExpr*
AstExpr::index() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief オペコードを返す．
//
// 演算子のみ有効
OpCode
AstExpr::opcode() const
{
  ASSERT_NOT_REACHED;
  return kOpCastBoolean;
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
AstExpr::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置
//
// pos の範囲は演算子によって異なる．
const AstExpr*
AstExpr::operand(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数本体を返す．
const AstExpr*
AstExpr::func() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 引数の数を返す．
//
// kFuncCall のみ有効
ymuint
AstExpr::arglist_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 引数を返す．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
const AstExpr*
AstExpr::arglist_elem(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief ExprType を出力する．
// @param[in] s 出力先のストリーム
// @param[in] et 式の種類
ostream&
operator<<(ostream& s,
	   AstExpr::Type et)
{
  switch ( et ) {
  }

  return s;
}

END_NAMESPACE_YM_YMSL

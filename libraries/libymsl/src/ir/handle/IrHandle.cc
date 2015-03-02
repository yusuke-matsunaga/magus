
/// @file IrHandle.cc
/// @brief IrHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrHandle::IrHandle() :
  mLink(NULL)
{
}

// @brief デストラクタ
IrHandle::~IrHandle()
{
}

// @brief スコープを返す．
//
// 他の要素の場合には NULL を返す．
Scope*
IrHandle::scope() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 型を得る．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
const Type*
IrHandle::value_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief インデックスが設定されていたら true を返す．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
bool
IrHandle::has_index() const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief インデックスを返す．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
ymuint
IrHandle::index() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief インデックスを設定する．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
void
IrHandle::set_index(ymuint index)
{
  ASSERT_NOT_REACHED;
}

// @brief グローバル変数の時に true を返す．
//
// kVar のみ有効
bool
IrHandle::is_global() const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief 変数本体を返す．
//
// kVar かつ is_global() == true の時のみ有効
VsmValue*
IrHandle::variable() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数本体を返す．
//
// kFunction, kMethodRef のみ有効
const VsmFunction*
IrHandle::function() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief ブール値を返す．
//
// kBooleanConst のみ有効
Ymsl_BOOLEAN
IrHandle::boolean_val() const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief 整数値を返す．
//
// kIntConst のみ有効
Ymsl_INT
IrHandle::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 実数値を返す．
//
// kFloatConst のみ有効
Ymsl_FLOAT
IrHandle::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列値を返す．
//
// kStringConst のみ有効
const char*
IrHandle::string_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
IrNode*
IrHandle::label() const
{
  return NULL;
}

// @brief 型を返す．
//
// 他の要素の場合には NULL を返す．
const Type*
IrHandle::named_type() const
{
  return NULL;
}

// @brief 配列本体の式を返す．
//
// kArrayRef のみ有効
IrNode*
IrHandle::array_expr() const
{
  return NULL;
}

// @brief 配列のインデックスを返す．
//
// kArrayRef のみ有効
IrNode*
IrHandle::array_index() const
{
  return NULL;
}

// @brief オブジェクトを指す式を返す．
//
// kMemberRef, kMethodRef のみ有効
IrNode*
IrHandle::obj_expr() const
{
  return NULL;
}

END_NAMESPACE_YM_YMSL

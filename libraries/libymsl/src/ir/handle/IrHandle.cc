
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

// @brief モジュールインデックスを返す．
//
// kVar, kFunction のみ有効
ymuint
IrHandle::module_index() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief ローカルインデックスを設定する．
// @param[in] index インデックス
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
void
IrHandle::set_local_index(ymuint index)
{
  ASSERT_NOT_REACHED;
}

// @brief ローカルインデックスを返す．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
ymuint
IrHandle::local_index() const
{
  ASSERT_NOT_REACHED;
  return 0;
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

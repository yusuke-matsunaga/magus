
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

// @brief 変数を返す．
//
// 他の要素の場合には NULL を返す．
const Var*
IrHandle::var() const
{
  return NULL;
}

// @brief 関数を返す．
//
// 他の要素の場合には NULL を返す．
const Function*
IrHandle::function() const
{
  return NULL;
}

// @brief 定数を返す．
//
// 他の要素の場合には NULL を返す．
IrNode*
IrHandle::constant() const
{
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

// @brief スコープを返す．
//
// 他の要素の場合には NULL を返す．
Scope*
IrHandle::scope() const
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

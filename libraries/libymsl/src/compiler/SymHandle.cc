
/// @file SymHandle.cc
/// @brief SymHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SymHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス SymHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SymHandle::SymHandle() :
  mLink(NULL)
{
}

// @brief デストラクタ
SymHandle::~SymHandle()
{
}

// @brief 変数を返す．
//
// 他の要素の場合には NULL を返す．
const Var*
SymHandle::var() const
{
  return NULL;
}

// @brief 定数を返す．
//
// 他の要素の場合には NULL を返す．
IrNode*
SymHandle::const_node() const
{
  return NULL;
}

// @brief 関数を返す．
//
// 他の要素の場合には NULL を返す．
const Function*
SymHandle::function() const
{
  return NULL;
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
IrNode*
SymHandle::label() const
{
  return NULL;
}

// @brief スコープを返す．
//
// 他の要素の場合には NULL を返す．
Scope*
SymHandle::scope() const
{
  return NULL;
}

// @brief 型を返す．
//
// 他の要素の場合には NULL を返す．
const Type*
SymHandle::named_type() const
{
  return NULL;
}

END_NAMESPACE_YM_YMSL

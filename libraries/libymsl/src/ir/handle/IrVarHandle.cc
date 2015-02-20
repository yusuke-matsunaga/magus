
/// @file IrVarHandle.cc
/// @brief IrVarHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrVarHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrVarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] value_type 型
// @param[in] global グローバル変数の時 true とするフラグ
IrVarHandle::IrVarHandle(ShString name,
			 const Type* value_type,
			 bool global) :
  mName(name),
  mValueType(value_type),
  mGlobal(global)
{
}

// @brief デストラクタ
IrVarHandle::~IrVarHandle()
{
}

// @brief 名前を返す．
ShString
IrVarHandle::name() const
{
  return mName;
}

// @brief 種類を返す．
IrHandle::HandleType
IrVarHandle::handle_type() const
{
  return kVar;
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrVarHandle::is_static() const
{
  return false;
}

// @brief 型を得る．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
const Type*
IrVarHandle::value_type() const
{
  return mValueType;
}

// @brief グローバル変数の時に true を返す．
//
// kVar のみ有効
bool
IrVarHandle::is_global() const
{
  return mGlobal;
}

// @brief インデックスを返す．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
ymuint
IrVarHandle::index() const
{
  return mIndex;
}

// @brief インデックスを設定する．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
void
IrVarHandle::set_index(ymuint index)
{
  mIndex = index;
}

END_NAMESPACE_YM_YMSL


/// @file IrFuncHandle.cc
/// @brief IrFuncHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrFuncHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] type 関数の型
IrFuncHandle::IrFuncHandle(ShString name,
			   const Type* type) :
  mName(name),
  mType(type)
{
}

// @brief デストラクタ
IrFuncHandle::~IrFuncHandle()
{
}

// @brief 名前を返す．
ShString
IrFuncHandle::name() const
{
  return mName;
}

// @brief 種類を返す．
IrHandle::HandleType
IrFuncHandle::handle_type() const
{
  return kFunction;
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrFuncHandle::is_static() const
{
  return false;
}

// @brief 型を得る．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
const Type*
IrFuncHandle::value_type() const
{
  return mType;
}

// @brief インデックスを返す．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
ymuint
IrFuncHandle::index() const
{
  return mIndex;
}

// @brief インデックスを設定する．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
void
IrFuncHandle::set_index(ymuint index)
{
  mIndex = index;
}

END_NAMESPACE_YM_YMSL

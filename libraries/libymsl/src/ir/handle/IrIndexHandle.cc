
/// @file IrIndexHandle.cc
/// @brief IrIndexHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrIndexHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrIndexHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] type 型
IrIndexHandle::IrIndexHandle(ShString name,
			     const Type* type) :
  mName(name),
  mType(type)
{
  mIndex = 0;
}

// @brief デストラクタ
IrIndexHandle::~IrIndexHandle()
{
}

// @brief 名前を返す．
ShString
IrIndexHandle::name() const
{
  return mName;
}

// @brief 型を得る．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
const Type*
IrIndexHandle::value_type() const
{
  return mType;
}

// @brief インデックスが設定されていたら true を返す．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
bool
IrIndexHandle::has_index() const
{
  return static_cast<bool>(mIndex & 1U);
}

// @brief インデックスを返す．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
ymuint
IrIndexHandle::index() const
{
  return (mIndex >> 1);
}

// @brief インデックスを設定する．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
void
IrIndexHandle::set_index(ymuint index)
{
  mIndex = (index << 1) | 1U;
}

END_NAMESPACE_YM_YMSL

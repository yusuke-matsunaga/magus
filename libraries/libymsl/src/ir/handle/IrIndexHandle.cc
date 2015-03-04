
/// @file IrIndexHandle.cc
/// @brief IrIndexHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrIndexHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrIndexHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] type 型
// @param[in] module_index モジュールインデックス
// @param[in] local_index ローカルインデックス
IrIndexHandle::IrIndexHandle(ShString name,
			     const Type* type,
			     ymuint module_index,
			     ymuint local_index) :
  mName(name),
  mType(type),
  mModuleIndex(module_index),
  mLocalIndex(local_index)
{
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

// @brief モジュールインデックスを返す．
//
// kVar, kFunction のみ有効
ymuint
IrIndexHandle::module_index() const
{
  return mModuleIndex;
}

// @brief ローカルインデックスを設定する．
// @param[in] index インデックス
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
void
IrIndexHandle::set_local_index(ymuint index)
{
  mLocalIndex = index;
}

// @brief ローカルインデックスを返す．
//
// kVar, kFunction, kMemberRef, kMethodRef のみ有効
ymuint
IrIndexHandle::local_index() const
{
  return mLocalIndex;
}

END_NAMESPACE_YM_YMSL

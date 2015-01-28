
/// @file TypeHandle.cc
/// @brief TypeHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "TypeHandle.h"
#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス TypeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
TypeHandle::TypeHandle(const Type* type) :
  mType(type)
{
}

// @brief デストラクタ
TypeHandle::~TypeHandle()
{
}

// @brief 名前を返す．
ShString
TypeHandle::name() const
{
  return mType->type_name();
}

// @brief 型を返す．
//
// 他の要素の場合には NULL を返す．
const Type*
TypeHandle::named_type() const
{
  return mType;
}

END_NAMESPACE_YM_YMSL

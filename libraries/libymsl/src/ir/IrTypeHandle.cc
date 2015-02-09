
/// @file IrTypeHandle.cc
/// @brief IrTypeHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrTypeHandle.h"
#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrTypeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
// @param[in] scope スコープ
IrTypeHandle::IrTypeHandle(const Type* type,
			   Scope* scope) :
  mType(type),
  mScope(scope)
{
}

// @brief デストラクタ
IrTypeHandle::~IrTypeHandle()
{
}

// @brief 名前を返す．
ShString
IrTypeHandle::name() const
{
  return mType->type_name();
}

// @brief 種類を返す．
IrHandle::HandleType
IrTypeHandle::handle_type() const
{
  return kNamedType;
}

// @brief スコープを返す．
//
// 他の要素の場合には NULL を返す．
Scope*
IrTypeHandle::scope() const
{
  return mScope;
}

// @brief 型を返す．
//
// 他の要素の場合には NULL を返す．
const Type*
IrTypeHandle::named_type() const
{
  return mType;
}

END_NAMESPACE_YM_YMSL

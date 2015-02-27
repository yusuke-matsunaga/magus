
/// @file IrScopeHandle.cc
/// @brief IrScopeHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrScopeHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrScopeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] scope スコープ
IrScopeHandle::IrScopeHandle(ShString name,
			     Scope* scope) :
  mName(name),
  mScope(scope)
{
}

// @brief デストラクタ
IrScopeHandle::~IrScopeHandle()
{
}

// @brief 名前を返す．
ShString
IrScopeHandle::name() const
{
  return mName;
}

// @brief 種類を返す．
IrHandle::HandleType
IrScopeHandle::handle_type() const
{
  return kScope;
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrScopeHandle::is_static() const
{
  return false;
}

// @brief スコープを返す．
//
// 他の要素の場合には NULL を返す．
Scope*
IrScopeHandle::scope() const
{
  return mScope;
}

END_NAMESPACE_YM_YMSL

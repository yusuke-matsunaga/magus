
/// @file ScopeHandle.cc
/// @brief ScopeHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ScopeHandle.h"
#include "Scope.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス ScopeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] scope スコープ
ScopeHandle::ScopeHandle(Scope* scope) :
  mScope(scope)
{
}

// @brief デストラクタ
ScopeHandle::~ScopeHandle()
{
}

// @brief 名前を返す．
ShString
ScopeHandle::name() const
{
  return mScope->name();
}

// @brief スコープを返す．
//
// 他の要素の場合には NULL を返す．
Scope*
ScopeHandle::scope() const
{
  return mScope;
}

END_NAMESPACE_YM_YMSL

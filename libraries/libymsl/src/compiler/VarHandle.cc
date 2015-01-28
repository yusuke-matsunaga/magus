
/// @file VarHandle.cc
/// @brief VarHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VarHandle.h"
#include "Var.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス VarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var 変数宣言
VarHandle::VarHandle(Var* var) :
  mVar(var)
{
}

// @brief デストラクタ
VarHandle::~VarHandle()
{
}

// @brief 名前を返す．
ShString
VarHandle::name() const
{
  return mVar->name();
}

// @brief 変数を返す．
//
// 他の要素の場合には NULL を返す．
Var*
VarHandle::var() const
{
  return mVar;
}

END_NAMESPACE_YM_YMSL

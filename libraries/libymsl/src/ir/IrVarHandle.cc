
/// @file IrVarHandle.cc
/// @brief IrVarHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrVarHandle.h"
#include "Var.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrVarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var 変数宣言
IrVarHandle::IrVarHandle(const Var* var) :
  mVar(var)
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
  return mVar->name();
}

// @brief 種類を返す．
IrHandle::HandleType
IrVarHandle::handle_type() const
{
  return kVar;
}

// @brief 変数を返す．
//
// 他の要素の場合には NULL を返す．
const Var*
IrVarHandle::var() const
{
  return mVar;
}

END_NAMESPACE_YM_YMSL


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
// @param[in] var_addr 変数のアドレス
IrVarHandle::IrVarHandle(ShString name,
			 const Type* value_type,
			 bool global,
			 VsmValue* var_addr) :
  IrIndexHandle(name, value_type),
  mGlobal(global),
  mVarAddr(var_addr)
{
}

// @brief デストラクタ
IrVarHandle::~IrVarHandle()
{
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

// @brief グローバル変数の時に true を返す．
//
// kVar のみ有効
bool
IrVarHandle::is_global() const
{
  return mGlobal;
}

// @brief 変数本体を返す．
//
// kVar かつ is_global() == true の時のみ有効
VsmValue*
IrVarHandle::variable() const
{
  return mVarAddr;
}

END_NAMESPACE_YM_YMSL

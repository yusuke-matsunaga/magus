
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
// @param[in] module_index モジュールインデックス
// @param[in] local_index ローカルインデックス
// @param[in] global グローバル変数の時 true とするフラグ
IrVarHandle::IrVarHandle(ShString name,
			 const Type* value_type,
			 ymuint module_index,
			 ymuint local_index,
			 bool global) :
  IrIndexHandle(name, value_type, module_index, local_index),
  mGlobal(global)
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

END_NAMESPACE_YM_YMSL


/// @file IrLocalVarHandle.cc
/// @brief IrLocalVarHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrLocalVarHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrLocalVarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] value_type 型
// @param[in] module_index モジュールインデックス
// @param[in] local_index ローカルインデックス
IrLocalVarHandle::IrLocalVarHandle(ShString name,
				   const Type* value_type,
				   ymuint module_index,
				   ymuint local_index) :
  IrIndexHandle(name, value_type, module_index, local_index)
{
}

// @brief デストラクタ
IrLocalVarHandle::~IrLocalVarHandle()
{
}

// @brief 種類を返す．
IrHandle::HandleType
IrLocalVarHandle::handle_type() const
{
  return kLocalVar;
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrLocalVarHandle::is_static() const
{
  return false;
}

END_NAMESPACE_YM_YMSL

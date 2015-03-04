
/// @file IrFuncHandle.cc
/// @brief IrFuncHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFuncHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] type 関数の型
// @param[in] func 関数本体
// @param[in] module_index モジュールインデックス
// @param[in] local_index ローカルインデックス
IrFuncHandle::IrFuncHandle(ShString name,
			   const Type* type,
			   ymuint module_index,
			   ymuint local_index) :
  IrIndexHandle(name, type, module_index, local_index)
{
}

// @brief デストラクタ
IrFuncHandle::~IrFuncHandle()
{
}

// @brief 種類を返す．
IrHandle::HandleType
IrFuncHandle::handle_type() const
{
  return kFunction;
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrFuncHandle::is_static() const
{
  return false;
}

END_NAMESPACE_YM_YMSL


/// @file IrFuncHandle.cc
/// @brief IrFuncHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
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
IrFuncHandle::IrFuncHandle(ShString name,
			   const Type* type,
			   const VsmFunction* func) :
  IrIndexHandle(name, type),
  mFunction(func)
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

// @brief 関数本体を返す．
//
// kFunction, kMethodRef のみ有効
const VsmFunction*
IrFuncHandle::function() const
{
  return mFunction;
}

END_NAMESPACE_YM_YMSL

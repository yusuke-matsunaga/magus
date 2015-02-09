
/// @file IrFuncRef.cc
/// @brief IrFuncRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFuncRef.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFuncRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
IrFuncRef::IrFuncRef(const Function* func) :
  IrNode(kOpFuncRef, NULL),
  mFunction(func)
{
}

// @brief デストラクタ
IrFuncRef::~IrFuncRef()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrFuncRef::is_static() const
{
  return false;
}

// @brief 関数本体を返す．
//
// kOpFuncRef のみ有効
const Function*
IrFuncRef::function() const
{
  return mFunction;
}

END_NAMESPACE_YM_YMSL

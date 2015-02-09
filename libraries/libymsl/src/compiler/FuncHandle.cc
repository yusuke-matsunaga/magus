
/// @file FuncHandle.cc
/// @brief FuncHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FuncHandle.h"
#include "Function.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス FuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
FuncHandle::FuncHandle(const Function* func) :
  mFunc(func)
{
}

// @brief デストラクタ
FuncHandle::~FuncHandle()
{
}

// @brief 名前を返す．
ShString
FuncHandle::name() const
{
  return mFunc->name();
}

// @brief 種類を返す．
IrHandle::HandleType
FuncHandle::handle_type() const
{
  return kFunction;
}

// @brief 関数を返す．
//
// 他の要素の場合には NULL を返す．
const Function*
FuncHandle::function() const
{
  return mFunc;
}

END_NAMESPACE_YM_YMSL

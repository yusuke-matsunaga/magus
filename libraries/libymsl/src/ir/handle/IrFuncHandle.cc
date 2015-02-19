
/// @file IrFuncHandle.cc
/// @brief IrFuncHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrFuncHandle.h"
#include "IrFuncBlock.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
IrFuncHandle::IrFuncHandle(IrFuncBlock* func) :
  mFunc(func)
{
}

// @brief デストラクタ
IrFuncHandle::~IrFuncHandle()
{
}

// @brief 名前を返す．
ShString
IrFuncHandle::name() const
{
  return mFunc->name();
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

// @brief 関数を返す．
//
// 他の要素の場合には NULL を返す．
IrFuncBlock*
IrFuncHandle::function() const
{
  return mFunc;
}

END_NAMESPACE_YM_YMSL

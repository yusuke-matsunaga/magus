
/// @file IrFuncBlock.cc
/// @brief IrFuncBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFuncBlock.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFuncBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] arg_list 引数のリスト
// @param[in] arg_init_list 引数のデフォルト値のリスト
// @param[in] func_handle 関数のハンドル
IrFuncBlock::IrFuncBlock(const vector<IrHandle*>& arg_list,
			 const vector<IrNode*>& arg_init_list,
			 IrHandle* func_handle) :
  mArgList(arg_list),
  mArgInitList(arg_init_list),
  mFuncHandle(func_handle)
{
}

// @brief デストラクタ
IrFuncBlock::~IrFuncBlock()
{
}

// @brief 関数のハンドルを返す．
IrHandle*
IrFuncBlock::func_handle()
{
  return mFuncHandle;
}

END_NAMESPACE_YM_YMSL

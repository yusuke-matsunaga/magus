
/// @file IrFunction.cc
/// @brief IrFunction の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFunction.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFunction
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] toplevel トップレベルブロック
// @param[in] func 関数本体
// @param[in] arg_list 引数のリスト
// @param[in] arg_init_list 引数のデフォルト値のリスト
IrFunction::IrFunction(IrToplevel& toplevel,
		       const Function* func,
		       const vector<const Var*>& arg_list,
		       const vector<IrNode*>& arg_init_list) :
  mToplevel(toplevel),
  mFunction(func),
  mArgList(arg_list),
  mArgInitList(arg_init_list)
{
}

// @brief デストラクタ
IrFunction::~IrFunction()
{
}

// @brief トップレベルのブロックを返す．
IrToplevel&
IrFunction::toplevel()
{
  return mToplevel;
}

END_NAMESPACE_YM_YMSL

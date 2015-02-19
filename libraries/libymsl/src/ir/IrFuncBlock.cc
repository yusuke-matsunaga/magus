
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
// @param[in] toplevel トップレベルブロック
// @param[in] func 関数本体
// @param[in] arg_list 引数のリスト
// @param[in] arg_init_list 引数のデフォルト値のリスト
IrFuncBlock::IrFuncBlock(IrToplevel& toplevel,
			 const Function* func,
			 const vector<const Var*>& arg_list,
			 const vector<IrNode*>& arg_init_list) :
  mToplevel(toplevel),
  mFunction(func),
  mArgList(arg_list),
  mArgInitList(arg_init_list),
  mIndex(0)
{
}

// @brief デストラクタ
IrFuncBlock::~IrFuncBlock()
{
}

// @brief トップレベルのブロックを返す．
IrToplevel&
IrFuncBlock::toplevel()
{
  return mToplevel;
}

// @brief 関数本体を返す．
const Function*
IrFuncBlock::function() const
{
  return mFunction;
}

// @brief インデックスを返す．
ymuint
IrFuncBlock::index() const
{
  return mIndex;
}

// @brief インデックスを設定する．
void
IrFuncBlock::set_index(ymuint index)
{
  mIndex = index;
}

END_NAMESPACE_YM_YMSL

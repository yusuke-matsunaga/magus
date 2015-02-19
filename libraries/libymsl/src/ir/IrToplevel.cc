
/// @file IrToplevel.cc
/// @brief IrToplevel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrToplevel.h"
#include "Var.h"
#include "IrFuncBlock.h"
#include "Function.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrToplevel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrToplevel::IrToplevel()
{
}

// @brief デストラクタ
IrToplevel::~IrToplevel()
{
}

// @brief トップレベルのブロックを返す．
IrToplevel&
IrToplevel::toplevel()
{
  return *this;
}

// @brief 変数を追加する．
void
IrToplevel::add_var(Var* var)
{
  if ( var->is_global() ) {
    ymuint index = mGlobalVarList.size();
    var->set_index(index);
    mGlobalVarList.push_back(var);
  }
  else {
    IrCodeBlock::add_var(var);
  }
}

// @brief 関数を追加する．
// @param[in] func 関数
void
IrToplevel::add_function(IrFuncBlock* func)
{
  ymuint index = mFuncList.size();
  func->set_index(index);
  mFuncList.push_back(func);
}

// @brief グローバル変数のリストを返す．
const vector<const Var*>&
IrToplevel::global_var_list() const
{
  return mGlobalVarList;
}

// @brief 関数のリストを返す．
const vector<IrFuncBlock*>&
IrToplevel::func_list() const
{
  return mFuncList;
}

END_NAMESPACE_YM_YMSL


/// @file IrToplevel.cc
/// @brief IrToplevel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrToplevel.h"
#include "IrHandle.h"
#include "IrFuncBlock.h"


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

// @brief import しているモジュールを追加する．
// @param[in] module モジュール
// @return モジュール番号を返す．
void
IrToplevel::add_imported_module(VsmModule* module)
{
  mModuleList.push_back(module);
}

// @brief 次のグローバル変数インデックスを得る．
ymuint
IrToplevel::next_global_index() const
{
  return mGlobalVarList.size();
}

// @brief グローバル変数を追加する．
void
IrToplevel::add_global_var(IrHandle* var)
{
  ASSERT_COND ( var->handle_type() == IrHandle::kGlobalVar );
  mGlobalVarList.push_back(var);
}

// @brief 次の関数インデックスを得る．
ymuint
IrToplevel::next_func_index() const
{
  return mFuncList.size();
}

// @brief 関数を追加する．
// @param[in] func 関数
void
IrToplevel::add_function(IrFuncBlock* func)
{
  mFuncList.push_back(func);
}

// @brief import しているモジュールのリストを返す．
const vector<VsmModule*>&
IrToplevel::imported_module_list() const
{
  return mModuleList;
}

// @brief グローバル変数のリストを返す．
const vector<IrHandle*>&
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

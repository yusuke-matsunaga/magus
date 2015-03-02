
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
ymuint
IrToplevel::add_imported_module(VsmModule* module)
{
  ymuint id = mModuleList.size();
  mModuleList.push_back(module);
  return id;
}

// @brief 変数を追加する．
void
IrToplevel::add_var(IrHandle* var)
{
  ASSERT_COND( var->handle_type() == IrHandle::kVar );
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
  mFuncList.push_back(func);
}

// @brief 関数テーブルに登録する．
// @param[in] func_handle 関数ハンドル
void
IrToplevel::reg_function(IrHandle* func_handle)
{
  if ( !func_handle->has_index() ) {
    ymuint index = mFuncTable.size();
    func_handle->set_index(index);
    mFuncTable.push_back(func_handle);
  }
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

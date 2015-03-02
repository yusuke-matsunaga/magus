
/// @file VsmModule.cc
/// @brief VsmModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmModule.h"
#include "VsmFunction.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス VsmModule::Builder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name モジュール名
VsmModule::Builder::Builder(ShString name) :
  mName(name)
{
}

// @brief デストラクタ
VsmModule::Builder::~Builder()
{
}

// @brief import しているモジュールを追加する．
// @param[in] module import しているモジュール
// @return モジュール番号を返す．
ymuint
VsmModule::Builder::add_module(VsmModule* module)
{
  ymuint id = mModuleList.size();
  mModuleList.push_back(module);
  return id;
}

// @brief 関数を追加する．
// @param[in] func 追加する関数
// @return 関数番号を返す．
ymuint
VsmModule::Builder::add_function(VsmFunction* func)
{
  ymuint id = mFuncList.size();
  mFuncList.push_back(func);
  return id;
}

// @brief export している変数を追加する．
// @param[in] var export している変数
// @return 変数番号を返す．
ymuint
VsmModule::Builder::add_exported_var(VsmVar* var)
{
  ymuint id = mVarList.size();
  mVarList.push_back(var);
  return id;
}

// @brief 名前を返す．
ShString
VsmModule::Builder::name() const
{
  return mName;
}

// @brief import しているモジュールのリストを返す．
const vector<VsmModule*>&
VsmModule::Builder::imported_module_list() const
{
  return mModuleList;
}

// @brief export している関数のリストを返す．
const vector<VsmFunction*>&
VsmModule::Builder::exported_function_list() const
{
  return mFuncList;
}

// @brief export している変数のリストを返す．
const vector<VsmVar*>&
VsmModule::Builder::exported_var_list() const
{
  return mVarList;
}


//////////////////////////////////////////////////////////////////////
// クラス VsmModule
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] builder ビルダー
VsmModule::VsmModule(Builder& builder) :
  mName(builder.name())
{
  const vector<VsmModule*>& module_list = builder.imported_module_list();
  mImportedModuleNum = module_list.size();
  mImportedModuleList = new VsmModule*[mImportedModuleNum];
  for (ymuint i = 0; i < mImportedModuleNum; ++ i) {
    mImportedModuleList[i] = module_list[i];
  }

  const vector<VsmFunction*>& func_list = builder.exported_function_list();
  mExportedFuncNum = func_list.size();
  mFuncTable = new VsmFunction*[mExportedFuncNum];
  for (ymuint i = 0; i < mExportedFuncNum; ++ i) {
    mFuncTable[i] = func_list[i];
  }

  const vector<VsmVar*>& var_list = builder.exported_var_list();
  mExportedVarNum = var_list.size();
  mExportedVarList = new VsmVar*[mExportedVarNum];
  for (ymuint i = 0; i < mExportedVarNum; ++ i) {
    mExportedVarList[i] = var_list[i];
  }
}

// @brief デストラクタ
VsmModule::~VsmModule()
{
  for (ymuint i = 0; i < mExportedFuncNum; ++ i) {
    delete mFuncTable;
  }
  for (ymuint i = 0; i < mExportedVarNum; ++ i) {
    delete mExportedVarList[i];
  }
  delete [] mImportedModuleList;
  delete [] mFuncTable;
  delete [] mExportedVarList;
}

// @brief モジュール名を返す．
ShString
VsmModule::name() const
{
  return mName;
}

// @brief このモジュールが import しているモジュールの数を返す．
ymuint
VsmModule::imported_module_num() const
{
  return mImportedModuleNum;
}

// @brief このモジュールが import しているモジュールを返す．
// @param[in] pos 位置 ( 0 <= pos < imported_module_num() )
VsmModule*
VsmModule::imported_module(ymuint pos) const
{
  ASSERT_COND( pos < imported_module_num() );
  return mImportedModuleList[pos];
}

// @brief このモジュールが export している関数の数を返す．
ymuint
VsmModule::exported_function_num() const
{
  return mExportedFuncNum;
}

// @brief このモジュールが export している関数を返す．
// @param[in] pos 位置 ( 0 <= pos < exported_function_num() )
VsmFunction*
VsmModule::exported_function(ymuint pos) const
{
  ASSERT_COND( pos < exported_function_num() );
  return mFuncTable[pos];
}

// @brief このモジュールが export している変数の数を返す．
ymuint
VsmModule::exported_variable_num() const
{
  return mExportedVarNum;
}

// @brief このモジュールが export している変数を返す．
// @param[in] pos 位置 ( 0 <= pos < exported_variable_num() )
VsmVar*
VsmModule::exported_variable(ymuint pos) const
{
  ASSERT_COND( pos < exported_variable_num() );
  return mExportedVarList[pos];
}

END_NAMESPACE_YM_YMSL

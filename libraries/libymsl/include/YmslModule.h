#ifndef YMSLMODULE_H
#define YMSLMODULE_H

/// @file YmslModule.h
/// @brief YmslModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslModule YmslModule.h "YmslModule.h"
/// @brief YMSL のモジュールを表すクラス
//////////////////////////////////////////////////////////////////////
class YmslModule
{
public:

  /// @brief コンストラクタ
  /// @param[in] name モジュール名
  YmslModule(ShString name);

  /// @brief デストラクタ
  virtual
  ~YmslModule();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュール名を返す．
  ShString
  name() const;

  /// @brief このモジュールが import しているモジュールの数を返す．
  ymuint
  imported_module_num() const;

  /// @brief このモジュールが import しているモジュールを返す．
  /// @param[in] pos 位置 ( 0 <= pos < imported_module_num() )
  YmslModule*
  imported_module(ymuint pos) const;

  /// @brief このモジュールが export している関数の数を返す．
  ymuint
  exported_function_num() const;

  /// @brief このモジュールが export している関数を返す．
  /// @param[in] pos 位置 ( 0 <= pos < exported_function_num() )
  YmslFunction*
  exported_function(ymuint pos) const;

  /// @brief このモジュールが export している変数の数を返す．
  ymuint
  exported_variable_num() const;

  /// @brief このモジュールが export している変数を返す．
  /// @param[in] pos 位置 ( 0 <= pos < exported_variable_num() )
  Var*
  exported_variable(ymuint pos) const;

  /// @brief トップレベルの実行を行う．
  /// @param[in] vsm 仮想マシン
  virtual
  void
  execute_toplevel(YmslVSM& vsm) const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュール名
  ShString mName;

  // import されたモジュールの数
  ymuint mImportedModuleNum;

  // import されたモジュールの配列
  YmslModule** mImportedModuleList;

  // export している関数の数
  ymuint mExportedFuncNum;

  // export している関数の配列
  YmslFunction* mExportedFuncList;

  // export している変数の数
  ymuint mExportedVarNum;

  // export している変数の配列
  Var* mExportedVarList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLMODULE_H

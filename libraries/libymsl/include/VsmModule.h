#ifndef VSMMODULE_H
#define VSMMODULE_H

/// @file VsmModule.h
/// @brief VsmModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class VsmModule VsmModule.h "VsmModule.h"
/// @brief YMSL のモジュールを表すクラス
//////////////////////////////////////////////////////////////////////
class VsmModule
{
public:
  //////////////////////////////////////////////////////////////////////
  // ビルダークラス
  //////////////////////////////////////////////////////////////////////

  class Builder
  {
  public:

    /// @brief コンストラクタ
    /// @param[in] name モジュール名
    Builder(ShString name);

    /// @brief デストラクタ
    ~Builder();


  public:
    //////////////////////////////////////////////////////////////////////
    // 外部インターフェイス
    //////////////////////////////////////////////////////////////////////

    /// @brief import しているモジュールを追加する．
    /// @param[in] module import しているモジュール
    void
    add_imported_module(VsmModule* module);

    /// @brief export している関数を追加する．
    /// @param[in] func export している関数
    void
    add_exported_function(VsmFunction* func);

    /// @brief export している変数を追加する．
    /// @param[in] var export している変数
    void
    add_exported_var(VsmVar* var);

    /// @brief 名前を返す．
    ShString
    name() const;

    /// @brief import しているモジュールのリストを返す．
    const vector<VsmModule*>&
    imported_module_list() const;

    /// @brief export している関数のリストを返す．
    const vector<VsmFunction*>&
    exported_function_list() const;

    /// @brief export している変数のリストを返す．
    const vector<VsmVar*>&
    exported_var_list() const;


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 名前
    ShString mName;

    // import しているモジュールのリスト
    vector<VsmModule*> mModuleList;

    // export している関数のリスト
    vector<VsmFunction*> mFuncList;

    // export している変数のリスト
    vector<VsmVar*> mVarList;

  };


public:

  /// @brief コンストラクタ
  /// @param[in] builder ビルダー
  VsmModule(Builder& builder);

  /// @brief デストラクタ
  virtual
  ~VsmModule();


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
  VsmModule*
  imported_module(ymuint pos) const;

  /// @brief このモジュールが export している関数の数を返す．
  ymuint
  exported_function_num() const;

  /// @brief このモジュールが export している関数を返す．
  /// @param[in] pos 位置 ( 0 <= pos < exported_function_num() )
  VsmFunction*
  exported_function(ymuint pos) const;

  /// @brief このモジュールが export している変数の数を返す．
  ymuint
  exported_variable_num() const;

  /// @brief このモジュールが export している変数を返す．
  /// @param[in] pos 位置 ( 0 <= pos < exported_variable_num() )
  VsmVar*
  exported_variable(ymuint pos) const;

  /// @brief トップレベルの実行を行う．
  /// @param[in] vsm 仮想マシン
  virtual
  void
  execute_toplevel(Vsm& vsm) const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュール名
  ShString mName;

  // import されたモジュールの数
  ymuint mImportedModuleNum;

  // import されたモジュールの配列
  VsmModule** mImportedModuleList;

  // export している関数の数
  ymuint mExportedFuncNum;

  // export している関数の配列
  VsmFunction** mExportedFuncList;

  // export している変数の数
  ymuint mExportedVarNum;

  // export している変数の配列
  VsmVar** mExportedVarList;

};

END_NAMESPACE_YM_YMSL

#endif // VSMMODULE_H

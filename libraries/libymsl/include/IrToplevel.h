#ifndef IRTOPLEVEL_H
#define IRTOPLEVEL_H

/// @file IrToplevel.h
/// @brief IrToplevel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrCodeBlock.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrToplevel IrToplevel.h "IrToplevel.h"
/// @brief トップレベルのコードブロック
//////////////////////////////////////////////////////////////////////
class IrToplevel :
  public IrCodeBlock
{
public:

  /// @brief コンストラクタ
  IrToplevel();

  /// @brief デストラクタ
  ~IrToplevel();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief import しているモジュールを追加する．
  /// @param[in] module モジュール
  /// @return モジュール番号を返す．
  ymuint
  add_imported_module(VsmModule* module);

  /// @brief 変数を追加する．
  /// @param[in] var 変数のハンドル
  virtual
  void
  add_var(IrHandle* var);

  /// @brief 関数を追加する．
  /// @param[in] func 関数
  void
  add_function(IrFuncBlock* func);

  /// @brief 関数テーブルに登録する．
  /// @param[in] func_handle 関数ハンドル
  void
  reg_function(IrHandle* func_handle);

  /// @brief import しているモジュールのリストを返す．
  const vector<VsmModule*>&
  imported_module_list() const;

  /// @brief グローバル変数のリストを返す．
  const vector<IrHandle*>&
  global_var_list() const;

  /// @brief 関数のリストを返す．
  const vector<IrFuncBlock*>&
  func_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // import しているモジュールのリスト
  vector<VsmModule*> mModuleList;

  // グローバル変数のリスト
  vector<IrHandle*> mGlobalVarList;

  // 関数のリスト
  vector<IrFuncBlock*> mFuncList;

  // 関数テーブル
  vector<IrHandle*> mFuncTable;

};

END_NAMESPACE_YM_YMSL

#endif // IRTOPLEVEL_H

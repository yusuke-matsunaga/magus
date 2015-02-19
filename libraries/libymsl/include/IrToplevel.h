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

  /// @brief トップレベルのブロックを返す．
  virtual
  IrToplevel&
  toplevel();

  /// @brief 変数を追加する．
  virtual
  void
  add_var(Var* var);

  /// @brief 関数を追加する．
  /// @param[in] func 関数
  void
  add_function(IrFuncBlock* func);

  /// @brief グローバル変数のリストを返す．
  const vector<const Var*>&
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

  // グローバル変数のリスト
  vector<const Var*> mGlobalVarList;

  // 関数のリスト
  vector<IrFuncBlock*> mFuncList;

};

END_NAMESPACE_YM_YMSL

#endif // IRTOPLEVEL_H

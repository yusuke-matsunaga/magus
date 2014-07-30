#ifndef YMYMLOGIC_SMTVAR_H
#define YMYMLOGIC_SMTVAR_H

/// @file YmLogic/SmtVar.h
/// @brief SmtVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/smt_nsdef.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtVar SmtVar.h "YmLogic/SmtVar.h"
/// @brief SMT ソルバ用の変数を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtVar
{
public:

  /// @brief デストラクタ
  virtual
  ~SmtVar() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型番号を返す．
  virtual
  tSmtSortId
  sort() const = 0;

  /// @brief 変数の種類を返す．
  virtual
  tSmtVarType
  type() const = 0;

  /// @brief global 変数の場合に true を返す．
  virtual
  bool
  is_global() const = 0;

  /// @brief local 変数(関数の引数)の場合に true を返す．
  virtual
  bool
  is_fun_arg() const = 0;

  /// @brief forall 変数の場合に true を返す．
  virtual
  bool
  is_forall() const = 0;

  /// @brief exists 変数の場合に true を返す．
  virtual
  bool
  is_exists() const = 0;

};

END_NAMESPACE_YM_SMT

#endif // YMYMLOGIC_SMTVAR_H

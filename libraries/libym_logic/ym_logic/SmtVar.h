#ifndef YM_LOGIC_SMTVAR_H
#define YM_LOGIC_SMTVAR_H

/// @file ym_logic/SmtVar.h
/// @brief SmtVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/smt_nsdef.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtVar SmtVar.h "ym_logic/SmtVar.h"
/// @brief SMT ソルバ用の変数を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtVar
{
public:

  /// @brief 変数の種類を表す列挙型
  enum tType {
    kGlobal,
    kForall,
    kExists
  };


public:

  /// @brief デストラクタ
  virtual
  ~SmtVar() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  const SmtSort*
  sort() const = 0;

  /// @brief 変数の種類を返す．
  virtual
  tType
  type() const = 0;

  /// @brief global 変数の場合に true を返す．
  virtual
  bool
  is_global() const = 0;

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

#endif // YM_LOGIC_SMTVAR_H

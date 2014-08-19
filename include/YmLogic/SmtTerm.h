#ifndef YMYMLOGIC_SMTTERM_H
#define YMYMLOGIC_SMTTERM_H

/// @file YmLogic/SmtTerm.h
/// @brief SmtTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/smt_nsdef.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtTerm SmtTerm.h "YmLogic/SmtTerm.h"
/// @brief term (項)を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtTerm
{
public:
  //////////////////////////////////////////////////////////////////////
  // 列挙型の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を表す型
  enum tType {
    /// @brief \<numeral\>
    kNumConst,
    /// @brief \<decimal\>
    kDecConst,
    /// @brief \<hexadecimal\>
    kHexConst,
    /// @brief \<binary\>
    kBinConst,
    /// @brief \<string\>
    kStrConst,
    /// @brief variable term
    kVarTerm,
    /// @brief function term
    kFunTerm,
    /// @brief forall term
    kForall,
    /// @brief exists term
    kExists
  };


public:

  /// @brief デストラクタ
  virtual
  ~SmtTerm() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const = 0;

  /// @brief 関連付けられている型を返す．
  virtual
  tSmtSortId
  sort() const = 0;

  /// @brief kNumConst 型の場合に整数値を返す．
  virtual
  ymuint32
  int_value() const = 0;

  /// @brief kDecConst/kHexConst/kBinConst/kStrConst 型の場合に文字列を返す．
  virtual
  ShString
  str_value() const = 0;

  /// @brief kVarTerm 型の場合に関数を返す．
  virtual
  const SmtVar*
  var() const = 0;

  /// @brief kFunTerm 型の場合に関数の型を返す．
  virtual
  tSmtFunType
  function_type() const = 0;

  /// @brief kFunTerm 型の場合に関数を返す．
  /// @note 組み込み関数の場合には NULL を返す．
  virtual
  const SmtFun*
  function() const = 0;

  /// @brief kFunTerm 型の場合に入力数を返す．
  virtual
  ymuint
  input_num() const = 0;

  /// @brief kFunTerm 型の場合に入力を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtTerm*
  input(ymuint pos) const = 0;

  /// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
  virtual
  ymuint
  var_num() const = 0;

  /// @brief kForall/kExists 型の場合に変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < var_num() )
  virtual
  const SmtVar*
  bound_var(ymuint pos) const = 0;

  /// @brief kForall/kExists 型の場合に本体の項を返す．
  virtual
  const SmtTerm*
  body() const = 0;

};

END_NAMESPACE_YM_SMT

#endif // YMYMLOGIC_SMTTERM_H

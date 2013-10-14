#ifndef YM_SMTLIBV2_SMTTERM_H
#define YM_SMTLIBV2_SMTTERM_H

/// @file ym_smtlibv2/SmtTerm.h
/// @brief SmtTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_smtlibv2/SmtSortedVar.h"
#include "ym_smtlibv2/SmtVarBinding.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtTerm SmtTerm.h "ym_smtlibv2/SmtTerm.h"
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
    /// @brief <numeric>
    kNumConst,
    /// @brief <decimal>
    kDecConst,
    /// @brief <hexadecimal>
    kHexConst,
    /// @brief <binary>
    kBinConst,
    /// @brief <string>
    kStrConst,
    /// @brief <keyword>
    kKeyword,
    /// @brief <symbol>
    kSymbol,
    /// @brief <identifier>
    kIdentifier,
    /// @brief <qual_identifier>
    kQualIdentifier,
    /// @brief function
    kFunTerm,
    /// @brief let
    kLet,
    /// @brief forall
    kForall,
    /// @brief exists
    kExists,
    /// @brief !
    kAttr,
    /// @brief list
    kList
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

  /// @brief kNumConst 型の場合に整数値を返す．
  virtual
  ymuint32
  int_value() const = 0;

  /// @brief kDecConst/kHexConst/kBinConst/kStrConst/kKeyword/kSymbol 型の場合に文字列を返す．
  virtual
  ShString
  str_value() const = 0;

  /// @brief kIdentifier/kQualIdentifier 型の場合に識別子を返す．
  virtual
  const SmtId*
  identifier() const = 0;

  /// @brief kQualIdentifier 型の場合に型を返す．
  /// @note 型がついていない場合には NULL を返す．
  virtual
  const SmtSort*
  identifier_sort() const = 0;

  /// @brief kFunTerm 型の場合に関数を返す．
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

  /// @brief kLet 型の場合に変数バインディングリストの要素数を返す．
  virtual
  ymuint
  let_binding_num() const = 0;

  /// @brief kLet 型の場合に変数バインディングリストの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < let_binding_num() )
  virtual
  SmtVarBinding
  let_binding(ymuint pos) const = 0;

  /// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
  virtual
  ymuint
  var_num() const = 0;

  /// @brief kForall/kExists 型の場合に変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < forall_var_num() )
  virtual
  SmtSortedVar
  sorted_var(ymuint pos) const = 0;

  /// @brief attr 型の場合に属性リストの要素数を返す．
  virtual
  ymuint
  attr_num() const = 0;

  /// @brief attr 型の場合に属性キーを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
  virtual
  ShString
  attr_keyword(ymuint pos) const = 0;

  /// @brief attr 型の場合に属性値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
  virtual
  const SmtTerm*
  attr_value(ymuint pos) const = 0;

  /// @brief kLet/kForall/kExists/kAttr 型の場合に本体の項を返す．
  virtual
  const SmtTerm*
  body() const = 0;

  /// @brief kList 型の場合に要素数を返す．
  virtual
  ymuint
  elem_num() const = 0;

  /// @brief kList 型の場合に要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  virtual
  const SmtTerm*
  elem(ymuint pos) const = 0;

};


/// @relates SmtTerm
/// @brief 内容を表す文字列を返す．
extern
string
term_str(const SmtTerm* term);

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTTERM_H

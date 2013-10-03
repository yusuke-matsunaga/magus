#ifndef YM_SMTLIBV2_SMTTERM_H
#define YM_SMTLIBV2_SMTTERM_H

/// @file ym_smtlibv2/SmtTerm.h
/// @brief SmtTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
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
    /// @brief <spec_constant>
    kConstant,
    /// @brief <qual_identifier>
    kQualIdentifier,
    /// @brief let
    kLet,
    /// @brief forall
    kForall,
    /// @brief exists
    kExists,
    /// @brief !
    kAttr
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

  /// @brief kConstant 型の場合に SmtConst を返す．
  virtual
  const SmtConst*
  const_val() const;

  /// @brief kQualIdentifier 型の場合に識別子を返す．
  virtual
  const SmtIdentifier*
  identifier() const;

  /// @brief kQualIdentifier 型の場合に型を返す．
  /// @note 型がついていない場合には NULL を返す．
  virtual
  const SmtSort*
  identifier_sort() const;

  /// @brief kQualIdentifier 型の場合にパラメータの項数を返す．
  virtual
  ymuint
  identifier_term_num() const;

  /// @brief kQualIdentifier 型の場合にパラメータを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < identifier_term_num() )
  virtual
  const SmtTerm*
  identifier_term(ymuint pos) const;

  /// @brief kLet 型の場合に変数バインディングリストの要素数を返す．
  virtual
  ymuint
  let_binding_num() const;

  /// @brief kLet 型の場合に変数バインディングリストの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < let_binding_num() )
  virtual
  pair<ShString, const SmtTerm*>
  let_binding(ymuint pos) const;

  /// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
  virtual
  ymuint
  var_num() const;

  /// @brief kForall/kExists 型の場合に変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < forall_var_num() )
  virtual
  pair<ShString, const SmtSort*>
  sorted_var(ymuint pos) const;

  /// @brief attr 型の場合に属性リストの要素数を返す．
  virtual
  ymuint
  attr_num() const;

  /// @brief attr 型の場合に属性を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
  virtual
  const SmtAttr*
  attr(ymuint pos) const;

  /// @brief kLet/kForall/kExists/kAttr 型の場合に本体の項を返す．
  virtual
  const SmtTerm*
  body() const;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTTERM_H

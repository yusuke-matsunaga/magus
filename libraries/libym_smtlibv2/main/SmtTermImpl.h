#ifndef SMTTERMIMPL_H
#define SMTTERMIMPL_H

/// @file SmtTermImpl.h
/// @brief SmtTermImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtTermImpl SmtTermImpl.h "SmtTermImpl.h"
/// @brief SmtTerm の実装クラス
//////////////////////////////////////////////////////////////////////
class SmtTermImpl :
  public SmtTerm
{
protected:

  /// @brief コンストラクタ
  SmtTermImpl();

  /// @brief デストラクタ
  ~SmtTermImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief kNumConst 型の場合に整数値を返す．
  virtual
  ymuint32
  int_value() const;

  /// @brief kDecConst, kHexConst, kBinConst, kStrConst 型の場合に文字列を返す．
  virtual
  ShString
  str_value() const;

  /// @brief kQualIdentifier 型の場合に識別子を返す．
  virtual
  const SmtId*
  identifier() const;

  /// @brief kQualIdentifier 型の場合に型を返す．
  /// @note 型がついていない場合には NULL を返す．
  virtual
  const SmtSort*
  identifier_sort() const;

  /// @brief kFunTerm 型の場合に関数を返す．
  virtual
  const SmtFun*
  function() const;

  /// @brief kFunTerm 型の場合に入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief kFunTerm 型の場合に入力を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtTerm*
  input(ymuint pos) const;

  /// @brief kLet 型の場合に変数バインディングリストの要素数を返す．
  virtual
  ymuint
  let_binding_num() const;

  /// @brief kLet 型の場合に変数バインディングリストの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < let_binding_num() )
  virtual
  SmtVarBinding
  let_binding(ymuint pos) const;

  /// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
  virtual
  ymuint
  var_num() const;

  /// @brief kForall/kExists 型の場合に変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < forall_var_num() )
  virtual
  SmtSortedVar
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

  /// @brief kList 型の場合に要素数を返す．
  virtual
  ymuint
  elem_num() const;

  /// @brief kList 型の場合に要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  virtual
  const SmtTerm*
  elem(ymuint pos) const;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTTERMIMPL_H

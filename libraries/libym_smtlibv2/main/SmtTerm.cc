
/// @file SmtTerm.cc
/// @brief SmtTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtTerm
//////////////////////////////////////////////////////////////////////

// @brief kConstant 型の場合に SmtConst を返す．
const SmtConst*
SmtTerm::const_val() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kQualIdentifier 型の場合に識別子を返す．
const SmtIdentifier*
SmtTerm::identifier() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kQualIdentifier 型の場合に型を返す．
// @note 型がついていない場合には NULL を返す．
const SmtSort*
SmtTerm::identifier_sort() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kQualIdentifier 型の場合にパラメータの項数を返す．
ymuint
SmtTerm::identifier_term_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kQualIdentifier 型の場合にパラメータを返す．
// @param[in] pos 位置番号 ( 0 <= pos < identifier_term_num() )
const SmtTerm*
SmtTerm::identifier_term(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kLet 型の場合に変数バインディングリストの要素数を返す．
ymuint
SmtTerm::let_binding_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kLet 型の場合に変数バインディングリストの要素を返す．
// @param[in] pos 位置番号 ( 0 <= pos < let_binding_num() )
pair<ShString, const SmtTerm*>
SmtTerm::let_binding(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return make_pair(ShString(), static_cast<const SmtTerm*>(NULL));
}

// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
ymuint
SmtTerm::var_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kForall/kExists 型の場合に変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < forall_var_num() )
pair<ShString, const SmtSort*>
SmtTerm::sorted_var(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return make_pair(ShString(), static_cast<const SmtSort*>(NULL));
}

// @brief attr 型の場合に属性リストの要素数を返す．
ymuint
SmtTerm::attr_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief attr 型の場合に属性を返す．
// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
const SmtAttr*
SmtTerm::attr(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kLet/kForall/kExists/kAttr 型の場合に本体の項を返す．
const SmtTerm*
SmtTerm::body() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_SMTLIBV2

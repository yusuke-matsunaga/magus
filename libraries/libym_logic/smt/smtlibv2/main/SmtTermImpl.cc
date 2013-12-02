
/// @file SmtTermImpl.cc
/// @brief SmtTermImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtTermImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtTermImpl::SmtTermImpl()
{
}

// @brief デストラクタ
SmtTermImpl::~SmtTermImpl()
{
}

// @brief kNumConst 型の場合に整数値を返す．
ymuint32
SmtTermImpl::int_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kDecConst, kHexConst, kBinConst, kStrConst 型の場合に文字列を返す．
ShString
SmtTermImpl::str_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

#if 0
// @brief kQualIdentifier 型の場合に識別子を返す．
const SmtId*
SmtTermImpl::identifier() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kQualIdentifier 型の場合に型を返す．
// @note 型がついていない場合には NULL を返す．
const SmtSort*
SmtTermImpl::identifier_sort() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}
#else
// @brief kVarTerm 型の場合に関数を返す．
const SmtVarFun*
SmtTermImpl::var() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}
#endif

// @brief kFunTerm 型の場合に関数を返す．
const SmtVarFun*
SmtTermImpl::function() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kFunTerm 型の場合に入力数を返す．
ymuint
SmtTermImpl::input_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kFunTerm 型の場合に入力を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtTerm*
SmtTermImpl::input(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
ymuint
SmtTermImpl::var_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kForall/kExists 型の場合に変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < var_num() )
const SmtVarFun*
SmtTermImpl::bound_var(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief attr 型の場合に属性リストの要素数を返す．
ymuint
SmtTermImpl::attr_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief attr 型の場合に属性キーを返す．
// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
ShString
SmtTermImpl::attr_keyword(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief attr 型の場合に属性値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
const SmtTerm*
SmtTermImpl::attr_value(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kLet/kForall/kExists/kAttr 型の場合に本体の項を返す．
const SmtTerm*
SmtTermImpl::body() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kList 型の場合に要素数を返す．
ymuint
SmtTermImpl::elem_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kList 型の場合に要素を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
const SmtTerm*
SmtTermImpl::elem(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_SMTLIBV2

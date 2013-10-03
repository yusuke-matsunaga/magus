
/// @file SmtIdTerm.cc
/// @brief SmtIdTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtIdTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtIdTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id 識別子
// @param[in] term_list 項のリスト
SmtIdTerm::SmtIdTerm(const SmtIdentifier* id,
		     const vector<const SmtTerm*>& term_list) :
  mId(id),
  mSort(NULL),
  mTermList(term_list)
{
}

// @brief コンストラクタ
// @param[in] id 識別子
// @param[in] sort 型
// @param[in] term_list 項のリスト
SmtIdTerm::SmtIdTerm(const SmtIdentifier* id,
		     const SmtSort* sort,
		     const vector<const SmtTerm*>& term_list) :
  mId(id),
  mSort(sort),
  mTermList(term_list)
{
}

// @brief デストラクタ
SmtIdTerm::~SmtIdTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtIdTerm::type() const
{
  return kQualIdentifier;
}

// @brief kQualIdentifier 型の場合に識別子を返す．
const SmtIdentifier*
SmtIdTerm::identifier() const
{
  return mId;
}

// @brief kQualIdentifier 型の場合に型を返す．
// @note 型がついていない場合には NULL を返す．
const SmtSort*
SmtIdTerm::identifier_sort() const
{
  return mSort;
}

// @brief kQualIdentifier 型の場合にパラメータの項数を返す．
ymuint
SmtIdTerm::identifier_term_num() const
{
  return mTermList.size();
}

// @brief kQualIdentifier 型の場合にパラメータを返す．
// @param[in] pos 位置番号 ( 0 <= pos < identifier_term_num() )
const SmtTerm*
SmtIdTerm::identifier_term(ymuint pos) const
{
  assert_cond( pos < identifier_term_num(), __FILE__, __LINE__);
  return mTermList[pos];
}

END_NAMESPACE_YM_SMTLIBV2

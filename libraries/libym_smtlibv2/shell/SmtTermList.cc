
/// @file SmtTermList.cc
/// @brief SmtTermList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermList.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtTermList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] term_list SmtTerm のリスト
SmtTermList::SmtTermList(const vector<const SmtTerm*>& term_list) :
  mTermNum(term_list.size())
{
  for (ymuint i = 0; i< mTermNum; ++ i) {
    mTermList[i] = term_list[i];
  }
}

// @brief デストラクタ
SmtTermList::~SmtTermList()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtTermList::type() const
{
  return kTermList;
}

// @brief kTermList 型の場合に項数を返す．
ymuint
SmtTermList::term_num() const
{
  return mTermNum;
}

// @brief kTermList 型の場合に項を返す．
// @param[in] pos 位置番号 ( 0 <= pos < identifier_term_num() )
const SmtTerm*
SmtTermList::term(ymuint pos) const
{
  assert_cond( pos < term_num(), __FILE__, __LINE__);
  return mTermList[pos];
}

END_NAMESPACE_YM_SMTLIBV2


/// @file SmtListTerm.cc
/// @brief SmtListTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtListTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtListTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elem_list 要素のリスト
SmtListTerm::SmtListTerm(const vector<const SmtTerm*>& elem_list) :
  mElemNum(elem_list.size())
{
  for (ymuint i = 0; i< mElemNum; ++ i) {
    mElemList[i] = elem_list[i];
  }
}

// @brief デストラクタ
SmtListTerm::~SmtListTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtListTerm::type() const
{
  return kList;
}

// @brief kList 型の場合に要素数を返す．
ymuint
SmtListTerm::elem_num() const
{
  return mElemNum;
}

// @brief kList 型の場合に要素を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
const SmtTerm*
SmtListTerm::elem(ymuint pos) const
{
  ASSERT_COND( pos < elem_num() );
  return mElemList[pos];
}

END_NAMESPACE_YM_SMTLIBV2

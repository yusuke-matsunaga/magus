
/// @file SmtListExpr.cc
/// @brief SmtListExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtListExpr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtListExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elem_list 要素のリスト
SmtListExpr::SmtListExpr(const vector<const SmtExpr*>& elem_list) :
  mElemList(elem_list)
{
}

// @brief デストラクタ
SmtListExpr::~SmtListExpr()
{
}

// @brief 型を返す．
SmtExpr::tType
SmtListExpr::type() const
{
  return kList;
}

// @brief kList 型の場合に要素数を返す．
ymuint
SmtListExpr::list_num() const
{
  return mElemList.size();
}

// @brief kList 型の場合に要素を返す．
// @param[in] pos 位置番号 ( 0 <= pos < lit_num() )
const SmtExpr*
SmtListExpr::list_elem(ymuint pos) const
{
  assert_cond( pos < list_num(), __FILE__, __LINE__);
  return mElemList[pos];
}

END_NAMESPACE_YM_SMTLIBV2


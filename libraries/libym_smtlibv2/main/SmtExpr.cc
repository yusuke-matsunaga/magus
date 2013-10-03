
/// @file SmtExpr.cc
/// @brief SmtExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtExpr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtExpr
//////////////////////////////////////////////////////////////////////

// @brief 定数型の場合に値を表す文字列を返す．
const SmtConst*
SmtExpr::const_val() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief kSymbol 型の場合にシンボル名を表す文字列を返す．
ShString
SmtExpr::symbol_val() const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief kKeyword 型の場合にキーワードを表す文字列を返す．
ShString
SmtExpr::keyword_val() const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief kList 型の場合に要素数を返す．
ymuint
SmtExpr::list_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief kList 型の場合に要素を返す．
// @param[in] pos 位置番号 ( 0 <= pos < lit_num() )
const SmtExpr*
SmtExpr::list_elem(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_SMTLIBV2

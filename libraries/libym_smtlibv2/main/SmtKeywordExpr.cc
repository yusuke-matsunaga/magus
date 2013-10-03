
/// @file SmtKeywordExpr.h
/// @brief SmtKeywordExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtKeywordExpr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtKeywordExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val キーワード
SmtKeywordExpr::SmtKeywordExpr(const ShString& val) :
  mVal(val)
{
}

// @brief デストラクタ
SmtKeywordExpr::~SmtKeywordExpr()
{
}

// @brief 型を返す．
SmtExpr::tType
SmtKeywordExpr::type() const
{
  return kKeyword;
}

// @brief kKeyword 型の場合にキーワードを表す文字列を返す．
ShString
SmtKeywordExpr::keyword_val() const
{
  return mVal;
}

END_NAMESPACE_YM_SMTLIBV2

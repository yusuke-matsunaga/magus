
/// @file SmtSymbolExpr.h
/// @brief SmtSymbolExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtSymbolExpr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtSymbolExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val シンボル名
SmtSymbolExpr::SmtSymbolExpr(const ShString& val) :
  mVal(val)
{
}

// @brief デストラクタ
SmtSymbolExpr::~SmtSymbolExpr()
{
}

// @brief 型を返す．
SmtExpr::tType
SmtSymbolExpr::type() const
{
  return kSymbol;
}

// @brief kSymbol 型の場合にシンボル名を表す文字列を返す．
ShString
SmtSymbolExpr::symbol_val() const
{
  return mVal;
}

END_NAMESPACE_YM_SMTLIBV2

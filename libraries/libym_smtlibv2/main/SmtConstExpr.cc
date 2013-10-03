
/// @file SmtConstExpr.cc
/// @brief SmtConstExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtConstExpr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtConstExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値を表すインスタンス
SmtConstExpr::SmtConstExpr(const SmtConst* val) :
  mVal(val)
{
}

// @brief デストラクタ
SmtConstExpr::~SmtConstExpr()
{
}

// @brief 型を返す．
SmtExpr::tType
SmtConstExpr::type() const
{
  return kConst;
}

// @brief 定数型の場合に SmtConst を返す．
const SmtConst*
SmtConstExpr::const_val() const
{
  return mVal;
}

END_NAMESPACE_YM_SMTLIBV2


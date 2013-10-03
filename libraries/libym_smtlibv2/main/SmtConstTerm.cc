
/// @file SmtConstTerm.cc
/// @brief SmtConstTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtConstTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtConstTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 定数を表すインスタンス
SmtConstTerm::SmtConstTerm(const SmtConst* val) :
  mVal(val)
{
}

// @brief デストラクタ
SmtConstTerm::~SmtConstTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtConstTerm::type() const
{
  return kConstant;
}

// @brief kConstant 型の場合に SmtConst を返す．
const SmtConst*
SmtConstTerm::const_val() const
{
  return mVal;
}

END_NAMESPACE_YM_SMTLIBV2

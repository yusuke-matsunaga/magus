
/// @file SmtVarTerm.cc
/// @brief SmtVarTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtVarTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtVarTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var 変数
SmtVarTerm::SmtVarTerm(const SmtVarFun* var) :
  mVar(var)
{
}

// @brief デストラクタ
SmtVarTerm::~SmtVarTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtVarTerm::type() const
{
  return kVarTerm;
}

// @brief kVar 型の場合に変数を返す．
const SmtVarFun*
SmtVarTerm::var() const
{
  return mVar;
}

END_NAMESPACE_YM_SMTLIBV2

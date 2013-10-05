
/// @file SmtLet.cc
/// @brief SmtLet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLet.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtLet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_binding 変数割り当てのリスト
// @param[in] body 本体
SmtLet::SmtLet(const vector<SmtVarBinding>& var_binding,
	       const SmtTerm* body) :
  mBody(body),
  mBindingNum(var_binding.size())
{
  for (ymuint i = 0; i < mBindingNum; ++ i) {
    mBindingList[i] = var_binding[i];
  }
}

// @brief デストラクタ
SmtLet::~SmtLet()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtLet::type() const
{
  return kLet;
}

// @brief kLet 型の場合に変数バインディングリストの要素数を返す．
ymuint
SmtLet::let_binding_num() const
{
  return mBindingNum;
}

// @brief kLet 型の場合に変数バインディングリストの要素を返す．
// @param[in] pos 位置番号 ( 0 <= pos < let_binding_num() )
const SmtVarBinding*
SmtLet::let_binding(ymuint pos) const
{
  assert_cond( pos < let_binding_num(), __FILE__, __LINE__);
  return mBindingList[pos];
}

END_NAMESPACE_YM_SMTLIBV2

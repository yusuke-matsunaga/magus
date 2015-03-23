
/// @file SmtFunTerm.cc
/// @brief SmtFunTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtFunTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtFunTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fun 関数
// @param[in] input_list 入力のリスト
SmtFunTerm::SmtFunTerm(const SmtVarFun* fun,
		       const vector<const SmtTerm*>& input_list) :
  mFunction(fun),
  mInputNum(input_list.size())
{
  for (ymuint i = 0; i< mInputNum; ++ i) {
    mInputList[i] = input_list[i];
  }
}

// @brief デストラクタ
SmtFunTerm::~SmtFunTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtFunTerm::type() const
{
  return kFunTerm;
}

// @brief kFunTerm 型の場合に関数を返す．
const SmtVarFun*
SmtFunTerm::function() const
{
  return mFunction;
}

// @brief kFunTerm 型の場合に入力数を返す．
ymuint
SmtFunTerm::input_num() const
{
  return mInputNum;
}

// @brief kFunTerm 型の場合に入力を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtTerm*
SmtFunTerm::input(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputList[pos];
}

END_NAMESPACE_YM_SMTLIBV2

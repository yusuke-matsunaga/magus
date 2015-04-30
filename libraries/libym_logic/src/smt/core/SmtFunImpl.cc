
/// @file SmtFunImpl.cc
/// @brief SmtFunImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SmtFunImpl.h"
#include "YmLogic/SmtVar.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
// クラス SmtFunImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
SmtFunImpl::SmtFunImpl(tSmtSortId output_sort):
  mOutputSort(output_sort)
{
}

// @brief デストラクタ
SmtFunImpl::~SmtFunImpl()
{
}

// @brief 入力数を返す．
ymuint
SmtFunImpl::input_num() const
{
  return 0;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
tSmtSortId
SmtFunImpl::input_sort(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return kSmtSort_None;
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtVar*
SmtFunImpl::input_var(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 出力の型を返す．
tSmtSortId
SmtFunImpl::output_sort() const
{
  return mOutputSort;
}

// @brief 本体の式を返す．
// @note uninterpreted function の場合は NULL を返す．
const SmtTerm*
SmtFunImpl::body() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDeclFun1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
SmtDeclFun1::SmtDeclFun1(tSmtSortId output_sort) :
  SmtFunImpl(output_sort)
{
}

// @brief デストラクタ
SmtDeclFun1::~SmtDeclFun1()
{
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDeclFun2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
SmtDeclFun2::SmtDeclFun2(const vector<tSmtSortId>& input_sort_list,
			 tSmtSortId output_sort) :
  SmtDeclFun1(output_sort),
  mInputNum(input_sort_list.size())
{
  for (ymuint i = 0; i < mInputNum; ++ i) {
    mInputList[i] = input_sort_list[i];
  }
}

// @brief デストラクタ
SmtDeclFun2::~SmtDeclFun2()
{
}

// @brief 入力数を返す．
ymuint
SmtDeclFun2::input_num() const
{
  return mInputNum;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
tSmtSortId
SmtDeclFun2::input_sort(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputList[pos];
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtVar*
SmtDeclFun2::input_var(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDefFun1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
// @param[in] body 本体
SmtDefFun1::SmtDefFun1(tSmtSortId output_sort,
		       const SmtTerm* body) :
  SmtFunImpl(output_sort),
  mBody(body)
{
}

// @brief デストラクタ
SmtDefFun1::~SmtDefFun1()
{
}

// @brief 本体の式を返す．
// @note uninterpreted function の場合は NULL を返す．
const SmtTerm*
SmtDefFun1::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDefFun2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_var_list 入力変数のリスト
// @param[in] output_sort 出力の型
// @param[in] body 本体
SmtDefFun2::SmtDefFun2(const vector<const SmtVar*>& input_var_list,
		       tSmtSortId output_sort,
		       const SmtTerm* body) :
  SmtDefFun1(output_sort, body),
  mInputNum(input_var_list.size())
{
  for (ymuint i = 0; i < mInputNum; ++ i) {
    mInputList[i] = input_var_list[i];
  }
}

// @brief デストラクタ
SmtDefFun2::~SmtDefFun2()
{
}

// @brief 入力数を返す．
ymuint
SmtDefFun2::input_num() const
{
  return mInputNum;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
tSmtSortId
SmtDefFun2::input_sort(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputList[pos]->sort();
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtVar*
SmtDefFun2::input_var(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputList[pos];
}

END_NAMESPACE_YM_SMT

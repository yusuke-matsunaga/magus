
/// @file SmtFunImpl.cc
/// @brief SmtFunImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtFunImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtFunImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] sort 出力の型
// @param[in] input_num 入力数
SmtFunImpl::SmtFunImpl(const SmtId* name,
		       const SmtSort* sort,
		       ymuint input_num) :
  mName(name),
  mSort(sort),
  mInputNum(input_num)
{
  mLink = NULL;
};

// @brief デストラクタ
SmtFunImpl::~SmtFunImpl()
{
}

// @brief ID番号を返す．
ymuint
SmtFunImpl::id() const
{
  return mId;
}

// @brief 名前を返す．
const SmtId*
SmtFunImpl::name() const
{
  return mName;
}

// @brief 出力の型を返す．
const SmtSort*
SmtFunImpl::sort() const
{
  return mSort;
}

// @brief 入力数を返す．
ymuint
SmtFunImpl::input_num() const
{
  return mInputNum;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtFun1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] sort 出力の型
// @param[in] input_num 入力数
SmtFun1::SmtFun1(const SmtId* name,
		 const SmtSort* sort,
		 ymuint input_num) :
  SmtFunImpl(name, sort, input_num)
{
}

// @brief デストラクタ
SmtFun1::~SmtFun1()
{
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtSort*
SmtFun1::input_sort(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos];
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtId*
SmtFun1::input_var(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return NULL;
}

// @brief 本体の式を返す．
// @note uninterpreted function の場合は NULL を返す．
const SmtTerm*
SmtFun1::body() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtFun2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] sort 出力の型
// @param[in] input_num 入力数
// @param[in] body 本体
SmtFun2::SmtFun2(const SmtId* name,
		 const SmtSort* sort,
		 ymuint input_num,
		 const SmtTerm* body) :
  SmtFunImpl(name, sort, input_num),
  mBody(body)
{
}

// @brief デストラクタ
SmtFun2::~SmtFun2()
{
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtSort*
SmtFun2::input_sort(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos].mSort;
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtId*
SmtFun2::input_var(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos].mVar;
}

// @brief 本体の式を返す．
// @note uninterpreted function の場合は NULL を返す．
const SmtTerm*
SmtFun2::body() const
{
  return mBody;
}

END_NAMESPACE_YM_SMTLIBV2


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

#if 0
// @brief ID番号を返す．
ymuint
SmtFunImpl::id() const
{
  return mId;
}
#endif

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

// @brief 属性を返す．
SmtFun::tAttr
SmtFunImpl::attr() const
{
  return kNone;
}

// @brief パラメータの数を返す．
ymuint
SmtFunImpl::param_num() const
{
  return 0;
}

// @brief ハッシュ値を返す．
ymuint
SmtFunImpl::hash() const
{
  return mId + mLevel * 2047;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtFun1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] sort 出力の型
// @param[in] input_num 入力数
// @param[in] attr 属性
// @param[in] param_num パラメータの数
SmtFun1::SmtFun1(const SmtId* name,
		 const SmtSort* sort,
		 ymuint input_num,
		 tAttr attr,
		 ymuint param_num) :
  SmtFunImpl(name, sort, input_num),
  mAttr(attr),
  mParamNum(param_num)
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

// @brief 属性を返す．
SmtFun::tAttr
SmtFun1::attr() const
{
  return mAttr;
}

// @brief パラメータの数を返す．
ymuint
SmtFun1::param_num() const
{
  return mParamNum;
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

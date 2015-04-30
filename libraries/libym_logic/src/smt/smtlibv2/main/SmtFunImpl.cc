
/// @file SmtFunImpl.cc
/// @brief SmtFunImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtFunImpl.h"
#include "ym_smtlibv2/SmtId.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtunImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] output_sort 出力の型
SmtFunImpl::SmtFunImpl(const SmtId* name,
		       const SmtSort* output_sort) :
  mName(name),
  mOutputSort(output_sort)
{
  mLink = NULL;
};

// @brief デストラクタ
SmtFunImpl::~SmtFunImpl()
{
}

// @brief 名前を返す．
const SmtId*
SmtFunImpl::name() const
{
  return mName;
}

// @brief 出力の型を返す．
const SmtSort*
SmtFunImpl::output_sort() const
{
  return mOutputSort;
}

// @brief 入力数を返す．
ymuint
SmtFunImpl::input_num() const
{
  return 0;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtSort*
SmtFunImpl::input_sort(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtId*
SmtFunImpl::input_var(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 本体の式を返す．
// @note uninterpreted function の場合は NULL を返す．
const SmtTerm*
SmtFunImpl::body() const
{
  return NULL;
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


//////////////////////////////////////////////////////////////////////
// クラス SmtDeclFun1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] output_sort 出力の型
SmtDeclFun1::SmtDeclFun1(const SmtId* name,
			 const SmtSort* output_sort) :
  SmtFunImpl(name, output_sort)
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
// @param[in] name 名前
// @param[in] input_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @param[in] attr 属性
// @param[in] param_num パラメータの数
SmtDeclFun2::SmtDeclFun2(const SmtId* name,
			 const vector<const SmtSort*>& input_list,
			 const SmtSort* output_sort,
			 tAttr attr,
			 ymuint param_num) :
  SmtDeclFun1(name, output_sort),
  mAttr(attr),
  mParamNum(param_num),
  mInputNum(input_list.size())
{
  for (ymuint i = 0; i < mInputNum; ++ i) {
    mInputList[i] = input_list[i];
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
const SmtSort*
SmtDeclFun2::input_sort(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputList[pos];
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtId*
SmtDeclFun2::input_var(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return NULL;
}

// @brief 属性を返す．
SmtFun::tAttr
SmtDeclFun2::attr() const
{
  return mAttr;
}

// @brief パラメータの数を返す．
ymuint
SmtDeclFun2::param_num() const
{
  return mParamNum;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDefFun1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] output_sort 出力の型
// @param[in] body 本体
SmtDefFun1::SmtDefFun1(const SmtId* name,
		       const SmtSort* output_sort,
		       const SmtTerm* body) :
  SmtFunImpl(name, output_sort),
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
// @param[in] name 名前
// @param[in] input_var_list 型つき入力変数のリスト
// @param[in] output_sort 出力の型
// @param[in] body 本体
SmtDefFun2::SmtDefFun2(const SmtId* name,
		       const vector<SmtSortedVar>& input_var_list,
		       const SmtSort* output_sort,
		       const SmtTerm* body) :
  SmtDefFun1(name, output_sort, body),
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
const SmtSort*
SmtDefFun2::input_sort(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputList[pos].mSort;
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtId*
SmtDefFun2::input_var(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputList[pos].mVar;
}

END_NAMESPACE_YM_SMTLIBV2

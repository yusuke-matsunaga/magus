
/// @file SmtVarFunImpl.cc
/// @brief SmtVarFunImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtVarFunImpl.h"
#include "ym_smtlibv2/SmtId.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtVarFunImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
SmtVarFunImpl::SmtVarFunImpl(const SmtId* name) :
  mName(name),
  mLink(NULL)
{
}

// @brief デストラクタ
SmtVarFunImpl::~SmtVarFunImpl()
{
}

// @brief 名前を返す．
const SmtId*
SmtVarFunImpl::name() const
{
  return mName;
}

// @brief 変数の場合に true を返す．
bool
SmtVarFunImpl::is_var() const
{
  return false;
}

// @brief 変数の型を返す．
// @note 型がついていない場合は NULL を返す．
// @note is_var() == false の場合にはエラーとなる．
const SmtSort*
SmtVarFunImpl::var_sort() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief global 変数の場合に true を返す．
// @note is_var() == false の場合にはエラーとなる．
bool
SmtVarFunImpl::is_global() const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief forall 変数の場合に true を返す．
// @note is_var() == false の場合にはエラーとなる．
bool
SmtVarFunImpl::is_forall() const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief exists 変数の場合に true を返す．
// @note is_var() == false の場合にはエラーとなる．
bool
SmtVarFunImpl::is_exists() const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 関数の場合に true を返す．
bool
SmtVarFunImpl::is_fun() const
{
  return false;
}

// @brief 入力数を返す．
// @note is_fun() == false の場合にはエラーとなる．
ymuint
SmtVarFunImpl::input_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note is_fun() == false の場合にはエラーとなる．
const SmtSort*
SmtVarFunImpl::input_sort(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
// @note is_fun() == false の場合にはエラーとなる．
const SmtVarFun*
SmtVarFunImpl::input_var(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 出力の型を返す．
// @note is_fun() == false の場合にはエラーとなる．
const SmtSort*
SmtVarFunImpl::output_sort() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 本体の式を返す．
// @note uninterpreted function の場合は NULL を返す．
// @note is_fun() == false の場合にはエラーとなる．
const SmtTerm*
SmtVarFunImpl::fun_body() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 関数の属性を返す．
// @note is_fun() == false の場合にはエラーとなる．
SmtVarFun::tFunAttr
SmtVarFunImpl::fun_attr() const
{
  assert_not_reached(__FILE__, __LINE__);
  return kNone;
}

// @brief パラメータの数を返す．
// @note is_fun() == false の場合にはエラーとなる．
ymuint
SmtVarFunImpl::param_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtVarBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前を表す識別子
// @param[in] sort 型
SmtVarBase::SmtVarBase(const SmtId* name,
		       const SmtSort* sort) :
  SmtVarFunImpl(name),
  mSort(sort)
{
}

// @brief デストラクタ
SmtVarBase::~SmtVarBase()
{
}

// @brief 変数の場合に true を返す．
bool
SmtVarBase::is_var() const
{
  return true;
}

// @brief 変数の型を返す．
// @note 型がついていない場合は NULL を返す．
// @note is_var() == false の場合にはエラーとなる．
const SmtSort*
SmtVarBase::var_sort() const
{
  return mSort;
}

// @brief global 変数の場合に true を返す．
// @note is_var() == false の場合にはエラーとなる．
bool
SmtVarBase::is_global() const
{
  return false;
}

// @brief forall 変数の場合に true を返す．
// @note is_var() == false の場合にはエラーとなる．
bool
SmtVarBase::is_forall() const
{
  return false;
}

// @brief exists 変数の場合に true を返す．
// @note is_var() == false の場合にはエラーとなる．
bool
SmtVarBase::is_exists() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtGlobalVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前を表す識別子
// @param[in] sort 型
SmtGlobalVar::SmtGlobalVar(const SmtId* name,
			   const SmtSort* sort) :
  SmtVarBase(name, sort)
{
}

// @brief デストラクタ
SmtGlobalVar::~SmtGlobalVar()
{
}

// @brief global 変数の場合に true を返す．
// @note is_var() == false の場合にはエラーとなる．
bool
SmtGlobalVar::is_global() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtForallVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前を表す識別子
// @param[in] sort 型
SmtForallVar::SmtForallVar(const SmtId* name,
			   const SmtSort* sort) :
  SmtVarBase(name, sort)
{
}

// @brief デストラクタ
SmtForallVar::~SmtForallVar()
{
}

// @brief forall 変数の場合に true を返す．
// @note is_var() == false の場合にはエラーとなる．
bool
SmtForallVar::is_forall() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtExistsVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前を表す識別子
// @param[in] sort 型
SmtExistsVar::SmtExistsVar(const SmtId* name,
			const SmtSort* sort) :
  SmtVarBase(name, sort)
{
}

// @brief デストラクタ
SmtExistsVar::~SmtExistsVar()
{
}

// @brief exists 変数の場合に true を返す．
// @note is_var() == false の場合にはエラーとなる．
bool
SmtExistsVar::is_exists() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtFunBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] output_sort 出力の型
SmtFunBase::SmtFunBase(const SmtId* name,
		       const SmtSort* output_sort) :
  SmtVarFunImpl(name),
  mOutputSort(output_sort)
{
}

// @brief デストラクタ
SmtFunBase::~SmtFunBase()
{
}

// @brief 関数の場合に true を返す．
bool
SmtFunBase::is_fun() const
{
  return true;
}

// @brief 入力数を返す．
// @note is_fun() == false の場合にはエラーとなる．
ymuint
SmtFunBase::input_num() const
{
  return 0;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note is_fun() == false の場合にはエラーとなる．
const SmtSort*
SmtFunBase::input_sort(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
// @note is_fun() == false の場合にはエラーとなる．
const SmtVarFun*
SmtFunBase::input_var(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 出力の型を返す．
// @note is_fun() == false の場合にはエラーとなる．
const SmtSort*
SmtFunBase::output_sort() const
{
  return mOutputSort;
}

// @brief 本体の式を返す．
// @note uninterpreted function の場合は NULL を返す．
// @note is_fun() == false の場合にはエラーとなる．
const SmtTerm*
SmtFunBase::fun_body() const
{
  return NULL;
}

// @brief 関数の属性を返す．
// @note is_fun() == false の場合にはエラーとなる．
SmtVarFun::tFunAttr
SmtFunBase::fun_attr() const
{
  return kNone;
}

// @brief パラメータの数を返す．
// @note is_fun() == false の場合にはエラーとなる．
ymuint
SmtFunBase::param_num() const
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
  SmtFunBase(name, output_sort)
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
// @param[in] fun_attr 属性
// @param[in] param_num パラメータの数
SmtDeclFun2::SmtDeclFun2(const SmtId* name,
			 const vector<const SmtSort*>& input_list,
			 const SmtSort* output_sort,
			 tFunAttr fun_attr,
			 ymuint param_num) :
  SmtDeclFun1(name, output_sort),
  mFunAttr(fun_attr),
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
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos];
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtVarFun*
SmtDeclFun2::input_var(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return NULL;
}

// @brief 属性を返す．
SmtVarFun::tFunAttr
SmtDeclFun2::fun_attr() const
{
  return mFunAttr;
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
  SmtFunBase(name, output_sort),
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
SmtDefFun1::fun_body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDefFun2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] input_var_list 入力変数のリスト
// @param[in] output_sort 出力の型
// @param[in] body 本体
SmtDefFun2::SmtDefFun2(const SmtId* name,
		       const vector<const SmtVarFun*>& input_var_list,
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
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos]->var_sort();
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtVarFun*
SmtDefFun2::input_var(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos];
}

END_NAMESPACE_YM_SMTLIBV2

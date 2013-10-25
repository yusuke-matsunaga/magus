
/// @file SmtFunImpl.cc
/// @brief SmtFunImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtFunImpl.h"
#include "ym_logic/SmtSort.h"
#include "ym_logic/SmtVar.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
// クラス SmtFunImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
SmtFunImpl::SmtFunImpl(const SmtSort* output_sort):
  mOutputSort(output_sort)
{
}

// @brief デストラクタ
SmtFunImpl::~SmtFunImpl()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtFunImpl::type() const
{
  return kSmtFun_UserDef;
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
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtVar*
SmtFunImpl::input_var(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 出力の型を返す．
const SmtSort*
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

// @brief 属性を返す．
tSmtFunAttr
SmtFunImpl::attr() const
{
  return kSmtFunAttr_None;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtTrueFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
// @note output_sort はブール型でなければならない．
SmtTrueFun::SmtTrueFun(const SmtSort* output_sort) :
  SmtFunImpl(output_sort)
{
  assert_cond( output_sort->type() == kSmtSort_Bool, __FILE__, __LINE__);
}

// @brief デストラクタ
SmtTrueFun::~SmtTrueFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtTrueFun::type() const
{
  return kSmtFun_True;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtFalseFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
// @note output_sort はブール型でなければならない．
SmtFalseFun::SmtFalseFun(const SmtSort* output_sort) :
  SmtFunImpl(output_sort)
{
  assert_cond( output_sort->type() == kSmtSort_Bool, __FILE__, __LINE__);
}

// @brief デストラクタ
SmtFalseFun::~SmtFalseFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtFalseFun::type() const
{
  return kSmtFun_False;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtNotFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] bool_sort 出力の型
// @note bool_sort はブール型でなければならない．
SmtNotFun::SmtNotFun(const SmtSort* bool_sort) :
  SmtFunImpl(bool_sort)
{
  assert_cond( bool_sort->type() == kSmtSort_Bool, __FILE__, __LINE__);
}

// @brief デストラクタ
SmtNotFun::~SmtNotFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtNotFun::type() const
{
  return kSmtFun_Not;
}

// @brief 入力数を返す．
ymuint
SmtNotFun::input_num() const
{
  return 1;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtSort*
SmtNotFun::input_sort(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return output_sort();
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLogBinFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] bool_sort 出力の型
// @note bool_sort はブール型でなければならない．
SmtLogBinFun::SmtLogBinFun(const SmtSort* bool_sort) :
  SmtFunImpl(bool_sort)
{
  assert_cond( bool_sort->type() == kSmtSort_Bool, __FILE__, __LINE__);
}

// @brief デストラクタ
SmtLogBinFun::~SmtLogBinFun()
{
}

// @brief 入力数を返す．
ymuint
SmtLogBinFun::input_num() const
{
  return 2;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtSort*
SmtLogBinFun::input_sort(ymuint pos) const
{
  assert_cond( pos < 2, __FILE__, __LINE__);
  return output_sort();
}

// @brief 属性を返す．
tSmtFunAttr
SmtLogBinFun::attr() const
{
  return kSmtFunAttr_RightAssoc;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtAndFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] bool_sort 出力の型
// @note bool_sort はブール型でなければならない．
SmtAndFun::SmtAndFun(const SmtSort* bool_sort) :
  SmtLogBinFun(bool_sort)
{
}

// @brief デストラクタ
SmtAndFun::~SmtAndFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtAndFun::type() const
{
  return kSmtFun_And;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtOrFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] bool_sort 出力の型
// @note bool_sort はブール型でなければならない．
SmtOrFun::SmtOrFun(const SmtSort* bool_sort) :
  SmtLogBinFun(bool_sort)
{
}

// @brief デストラクタ
SmtOrFun::~SmtOrFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtOrFun::type() const
{
  return kSmtFun_Or;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtXorFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] bool_sort 出力の型
// @note bool_sort はブール型でなければならない．
SmtXorFun::SmtXorFun(const SmtSort* bool_sort) :
  SmtLogBinFun(bool_sort)
{
}

// @brief デストラクタ
SmtXorFun::~SmtXorFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtXorFun::type() const
{
  return kSmtFun_Xor;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtImpFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] bool_sort 出力の型
// @note bool_sort はブール型でなければならない．
SmtImpFun::SmtImpFun(const SmtSort* bool_sort) :
  SmtLogBinFun(bool_sort)
{
}

// @brief デストラクタ
SmtImpFun::~SmtImpFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtImpFun::type() const
{
  return kSmtFun_Imp;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtCompFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_sort 入力の型
// @param[in] bool_sort ブール型
SmtCompFun::SmtCompFun(const SmtSort* input_sort,
		       const SmtSort* bool_sort) :
  SmtFunImpl(bool_sort),
  mInputSort(input_sort)
{
  assert_cond( bool_sort->type() == kSmtSort_Bool, __FILE__, __LINE__);
}

// @brief デストラクタ
SmtCompFun::~SmtCompFun()
{
}

// @brief 入力数を返す．
ymuint
SmtCompFun::input_num() const
{
  return 2;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtSort*
SmtCompFun::input_sort(ymuint pos) const
{
  assert_cond( pos < 2, __FILE__, __LINE__);
  return mInputSort;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtEqFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_sort 入力の型
// @param[in] bool_sort ブール型
SmtEqFun::SmtEqFun(const SmtSort* input_sort,
		   const SmtSort* bool_sort) :
  SmtCompFun(input_sort, bool_sort)
{
}

// @brief デストラクタ
SmtEqFun::~SmtEqFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtEqFun::type() const
{
  return kSmtFun_Eq;
}

// @brief 属性を返す．
tSmtFunAttr
SmtEqFun::attr() const
{
  return kSmtFunAttr_Chainable;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDiseqFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_sort 入力の型
// @param[in] bool_sort ブール型
SmtDiseqFun::SmtDiseqFun(const SmtSort* input_sort,
			 const SmtSort* bool_sort) :
  SmtCompFun(input_sort, bool_sort)
{
}

// @brief デストラクタ
SmtDiseqFun::~SmtDiseqFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtDiseqFun::type() const
{
  return kSmtFun_Diseq;
}

// @brief 属性を返す．
tSmtFunAttr
SmtDiseqFun::attr() const
{
  return kSmtFunAttr_Pairwise;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtIteFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] data_sort データの型
// @param[in] bool_sort ブール型
SmtIteFun::SmtIteFun(const SmtSort* data_sort,
		     const SmtSort* bool_sort) :
  SmtFunImpl(data_sort),
  mBoolSort(bool_sort)
{
  assert_cond( bool_sort->type() == kSmtSort_Bool, __FILE__, __LINE__);
}

// @brief デストラクタ
SmtIteFun::~SmtIteFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtIteFun::type() const
{
  return kSmtFun_Ite;
}

// @brief 入力数を返す．
ymuint
SmtIteFun::input_num() const
{
  return 3;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtSort*
SmtIteFun::input_sort(ymuint pos) const
{
  switch ( pos ) {
  case 0: return mBoolSort;
  case 1:
  case 2: return output_sort();
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtUminusFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
// @note 入力の型も出力と同じ(IntかReal)
SmtUminusFun::SmtUminusFun(const SmtSort* output_sort) :
  SmtFunImpl(output_sort)
{
  assert_cond( output_sort->type() == kSmtSort_Int || output_sort->type() == kSmtSort_Real, __FILE__, __LINE__);
}

// @brief デストラクタ
SmtUminusFun::~SmtUminusFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtUminusFun::type() const
{
  return kSmtFun_Uminus;
}

// @brief 入力数を返す．
ymuint
SmtUminusFun::input_num() const
{
  return 1;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtSort*
SmtUminusFun::input_sort(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return output_sort();
}


//////////////////////////////////////////////////////////////////////
// クラス SmtArithBinFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
// @note 入力の型も出力と同じ(IntかReal)
SmtArithBinFun::SmtArithBinFun(const SmtSort* output_sort) :
  SmtFunImpl(output_sort)
{
  assert_cond( output_sort->type() == kSmtSort_Int || output_sort->type() == kSmtSort_Real, __FILE__, __LINE__);
}

// @brief デストラクタ
SmtArithBinFun::~SmtArithBinFun()
{
}

// @brief 入力数を返す．
ymuint
SmtArithBinFun::input_num() const
{
  return 2;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtSort*
SmtArithBinFun::input_sort(ymuint pos) const
{
  assert_cond( pos < 2, __FILE__, __LINE__);
  return output_sort();
}

// @brief 属性を返す．
tSmtFunAttr
SmtArithBinFun::attr() const
{
  return kSmtFunAttr_LeftAssoc;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtAddFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
// @note 入力の型も出力と同じ(IntかReal)
SmtAddFun::SmtAddFun(const SmtSort* output_sort) :
  SmtArithBinFun(output_sort)
{
}

// @brief デストラクタ
SmtAddFun::~SmtAddFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtAddFun::type() const
{
  return kSmtFun_Add;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtSubFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
// @note 入力の型も出力と同じ(IntかReal)
SmtSubFun::SmtSubFun(const SmtSort* output_sort) :
  SmtArithBinFun(output_sort)
{
}

// @brief デストラクタ
SmtSubFun::~SmtSubFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtSubFun::type() const
{
  return kSmtFun_Sub;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtMulFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
// @note 入力の型も出力と同じ(IntかReal)
SmtMulFun::SmtMulFun(const SmtSort* output_sort) :
  SmtArithBinFun(output_sort)
{
}

// @brief デストラクタ
SmtMulFun::~SmtMulFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtMulFun::type() const
{
  return kSmtFun_Mul;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDivFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
// @note 入力の型も出力と同じ(Real)
SmtDivFun::SmtDivFun(const SmtSort* output_sort) :
  SmtArithBinFun(output_sort)
{
  assert_cond( output_sort->type() == kSmtSort_Real, __FILE__, __LINE__);
}

// @brief デストラクタ
SmtDivFun::~SmtDivFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtDivFun::type() const
{
  return kSmtFun_Div;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtArithCompFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_sort 入力の型
// @param[in] output_sort 出力の型
// @note output_sort はブール型でなければならない．
SmtArithCompFun::SmtArithCompFun(const SmtSort* input_sort,
				 const SmtSort* output_sort) :
  SmtFunImpl(output_sort),
  mInputSort(input_sort)
{
  assert_cond( output_sort->type() == kSmtSort_Bool, __FILE__, __LINE__);
  assert_cond( input_sort->type() == kSmtSort_Int || input_sort->type() == kSmtSort_Real, __FILE__, __LINE__);
}

// @brief デストラクタ
SmtArithCompFun::~SmtArithCompFun()
{
}

// @brief 入力数を返す．
ymuint
SmtArithCompFun::input_num() const
{
  return 2;
}

// @brief 入力の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const SmtSort*
SmtArithCompFun::input_sort(ymuint pos) const
{
  assert_cond( pos < 2, __FILE__, __LINE__);
  return mInputSort;
}

// @brief 属性を返す．
tSmtFunAttr
SmtArithCompFun::attr() const
{
  return kSmtFunAttr_Chainable;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLeFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_sort 入力の型
// @param[in] output_sort 出力の型
// @note output_sort はブール型でなければならない．
SmtLeFun::SmtLeFun(const SmtSort* input_sort,
		   const SmtSort* output_sort) :
  SmtArithCompFun(input_sort, output_sort)
{
}

// @brief デストラクタ
SmtLeFun::~SmtLeFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtLeFun::type() const
{
  return kSmtFun_Le;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtLtFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_sort 入力の型
// @param[in] output_sort 出力の型
// @note output_sort はブール型でなければならない．
SmtLtFun::SmtLtFun(const SmtSort* input_sort,
		   const SmtSort* output_sort) :
  SmtArithCompFun(input_sort, output_sort)
{
}

// @brief デストラクタ
SmtLtFun::~SmtLtFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtLtFun::type() const
{
  return kSmtFun_Lt;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtGeFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_sort 入力の型
// @param[in] output_sort 出力の型
// @note output_sort はブール型でなければならない．
SmtGeFun::SmtGeFun(const SmtSort* input_sort,
		   const SmtSort* output_sort) :
  SmtArithCompFun(input_sort, output_sort)
{
}

// @brief デストラクタ
SmtGeFun::~SmtGeFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtGeFun::type() const
{
  return kSmtFun_Ge;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtGtFun
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_sort 入力の型
// @param[in] output_sort 出力の型
// @note output_sort はブール型でなければならない．
SmtGtFun::SmtGtFun(const SmtSort* input_sort,
		   const SmtSort* output_sort) :
  SmtArithCompFun(input_sort, output_sort)
{
}

// @brief デストラクタ
SmtGtFun::~SmtGtFun()
{
}

// @brief 組み込み関数の場合に型を返す．
// @note 普通の関数は kSmtFun_UserDef を返す．
tSmtFun
SmtGtFun::type() const
{
  return kSmtFun_Gt;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDeclFun1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
SmtDeclFun1::SmtDeclFun1(const SmtSort* output_sort) :
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
SmtDeclFun2::SmtDeclFun2(const vector<const SmtSort*>& input_sort_list,
			 const SmtSort* output_sort) :
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
const SmtSort*
SmtDeclFun2::input_sort(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos];
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtVar*
SmtDeclFun2::input_var(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtDefFun1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] output_sort 出力の型
// @param[in] body 本体
SmtDefFun1::SmtDefFun1(const SmtSort* output_sort,
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
		       const SmtSort* output_sort,
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
const SmtSort*
SmtDefFun2::input_sort(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos]->sort();
}

// @brief 入力変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
// @note uninterpreted function の場合は NULL を返す．
const SmtVar*
SmtDefFun2::input_var(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos];
}

END_NAMESPACE_YM_SMT


/// @file SmtSolverImpl.cc
/// @brief SmtSolverImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtSolverImpl.h"

#include "SmtSortImpl.h"
#include "SmtVarImpl.h"
#include "SmtFunImpl.h"
#include "SmtTermImpl.h"


BEGIN_NAMESPACE_YM_SMT


//////////////////////////////////////////////////////////////////////
// クラス SmtSolver
//////////////////////////////////////////////////////////////////////

// @brief インスタンスを生成する静的関数
SmtSolver*
SmtSolver::new_solver()
{
  return new SmtSolverImpl();
}


//////////////////////////////////////////////////////////////////////
// クラス SmtSolverImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtSolverImpl::SmtSolverImpl() :
  mAlloc(4096)
{
  mLogic = kSmtLogic_NONE;

  mSortId = 0;
  mVarId = 0;

  mBoolSort = NULL;
  mIntSort = NULL;
  mRealSort = NULL;

  mTrueFun = NULL;
  mFalseFun = NULL;
  mNotFun = NULL;
  mAndFun = NULL;
  mOrFun = NULL;
  mXorFun = NULL;
  mImpFun = NULL;

  mIntUminusFun = NULL;
  mIntAddFun = NULL;
  mIntSubFun = NULL;
  mIntMulFun = NULL;
  mIntLeFun = NULL;
  mIntLtFun = NULL;
  mIntGeFun = NULL;
  mIntGtFun = NULL;
}

// @brief デストラクタ
SmtSolverImpl::~SmtSolverImpl()
{
}

// @brief 使用するロジックを設定する
// @param[in] logic 設定するロジック
bool
SmtSolverImpl::set_logic(tSmtLogic logic)
{
  // このコマンドは1度しか使えない．
  if ( mLogic != kSmtLogic_NONE ) {
    return false;
  }

  switch ( logic ) {
  case kSmtLogic_AUFLIA:
    Core_init();
    Ints_init();
    break;

  case kSmtLogic_AUFLIRA:
    Core_init();
    Ints_init();
    break;

  case kSmtLogic_AUFNIRA:
    Core_init();
    Ints_init();
    break;

  case kSmtLogic_LRA:
    Core_init();
    break;

  case kSmtLogic_QF_ABV:
    Core_init();
    break;

  case kSmtLogic_QF_AUFBV:
    Core_init();
    break;

  case kSmtLogic_QF_AUFLIA:
    Core_init();
    Ints_init();
    break;

  case kSmtLogic_QF_AX:
    Core_init();
    break;

  case kSmtLogic_QF_BV:
    Core_init();
    break;

  case kSmtLogic_QF_IDL:
    Core_init();
    Ints_init();
    break;

  case kSmtLogic_QF_LIA:
    Core_init();
    Ints_init();
    break;

  case kSmtLogic_QF_LRA:
    Core_init();
    break;

  case kSmtLogic_QF_NIA:
    Core_init();
    Ints_init();
    break;

  case kSmtLogic_QF_NRA:
    Core_init();
    break;

  case kSmtLogic_QF_RDL:
    Core_init();
    break;

  case kSmtLogic_QF_UF:
    Core_init();
    break;

  case kSmtLogic_QF_UFBV:
    Core_init();
    break;

  case kSmtLogic_QF_UFIDL:
    Core_init();
    Ints_init();
    break;

  case kSmtLogic_QF_UFLIA:
    Core_init();
    Ints_init();
    break;

  case kSmtLogic_QF_UFLRA:
    Core_init();
    break;

  case kSmtLogic_QF_UFNRA:
    Core_init();
    break;

  case kSmtLogic_UFLRA:
    Core_init();
    break;

  case kSmtLogic_UFNIA:
    Core_init();
    Ints_init();
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  return true;
}

// @brief CORE logic の初期化を行う．
void
SmtSolverImpl::Core_init()
{
}

// @brief INTS logic の初期化を行う．
void
SmtSolverImpl::Ints_init()
{
}

// @brief 型を作る．
// @param[in] elem_list 要素の型のリスト
// @return 作成した型を返す．
const SmtSort*
SmtSolverImpl::make_sort(const vector<const SmtSort*>& elem_list)
{
  ymuint n = elem_list.size();
  const SmtSort* sort;
  if ( n == 0 ) {
    void* p = mAlloc.get_memory(sizeof(SmtSimpleSort));
    sort = new (p) SmtSimpleSort(mSortId);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtComplexSort) + sizeof(const SmtSort*) * (n - 1));
    sort = new (p) SmtComplexSort(mSortId, elem_list);
  }
  ++ mSortId;

  return sort;
}

// @brief 組み込み型を作る．
const SmtSort*
SmtSolverImpl::make_builtin_sort(tSmtSort type)
{
  switch ( type ) {
  case kSmtSort_Bool:
    if ( mBoolSort == NULL ) {
      void* p = mAlloc.get_memory(sizeof(SmtBoolSort));
      mBoolSort = new (p) SmtBoolSort(mSortId);
      ++ mSortId;
    }
    return mBoolSort;

  case kSmtSort_Int:
    if ( mIntSort == NULL ) {
      void* p = mAlloc.get_memory(sizeof(SmtIntSort));
      mIntSort = new (p) SmtIntSort(mSortId);
      ++ mSortId;
    }
    return mIntSort;

  case kSmtSort_Real:
    if ( mRealSort == NULL ) {
      void* p = mAlloc.get_memory(sizeof(SmtRealSort));
      mRealSort = new (p) SmtRealSort(mSortId);
      ++ mSortId;
    }
    return mRealSort;

  default:
    break;
  }
  assert_not_reached(__FILE__, __LINE__);

  return NULL;
}

// @brief 変数を作る．
// @param[in] sort 変数の型
// @param[in] type 変数の種類
// @return 作成した変数を返す．
const SmtVar*
SmtSolverImpl::make_var(const SmtSort* sort,
			tSmtVar type)
{
  void* p;
  const SmtVar* var;
  switch ( type ) {
  case kSmtVar_Global:
    p = mAlloc.get_memory(sizeof(SmtGlobalVar));
    var = new (p) SmtGlobalVar(mVarId, sort);
    break;

  case kSmtVar_Forall:
    p = mAlloc.get_memory(sizeof(SmtForallVar));
    var = new (p) SmtForallVar(mVarId, sort);
    break;

  case kSmtVar_Exists:
    p = mAlloc.get_memory(sizeof(SmtExistsVar));
    var = new (p) SmtExistsVar(mVarId, sort);
    break;
  }
  ++ mVarId;

  return var;
}

// @brief 関数を作る．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @return 作成した関数を返す．
const SmtFun*
SmtSolverImpl::make_fun(const vector<const SmtSort*>& input_sort_list,
			const SmtSort* output_sort)
{
  ymuint n = input_sort_list.size();
  const SmtFun* fun;
  if ( n == 0 ) {
    void* p = mAlloc.get_memory(sizeof(SmtDeclFun1));
    fun = new (p) SmtDeclFun1(output_sort);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtDeclFun2) + sizeof(const SmtSort*) * (n - 1));
    fun = new (p) SmtDeclFun2(input_sort_list, output_sort);
  }

  return fun;
}

// @brief 内容を持った関数を作る．
// @param[in] input_var_list 入力の変数のリスト
// @param[in] output_sort 出力の型
// @param[in] body 本体を式
// @return 作成した関数を返す．
const SmtFun*
SmtSolverImpl::make_fun(const vector<const SmtVar*>& input_var_list,
			const SmtSort* output_sort,
			const SmtTerm* body)
{
  ymuint n = input_var_list.size();
  const SmtFun* fun;
  if ( n == 0 ) {
    void* p = mAlloc.get_memory(sizeof(SmtDefFun1));
    fun = new (p) SmtDefFun1(output_sort, body);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtDefFun2) + sizeof(const SmtVar*) * (n - 1));
    fun = new (p) SmtDefFun2(input_var_list, output_sort, body);
  }

  return fun;
}

BEGIN_NONAMESPACE

/// @brief Eq/Diseq 用の input_sort_list のチェックを行う．
/// @param[in] input_sort_list 入力の型のリスト
const SmtSort*
check1(const vector<const SmtSort*>& input_sort_list)
{
  if ( input_sort_list.size() != 2 ) {
    return NULL;
  }

  const SmtSort* sort0 = input_sort_list[0];
  const SmtSort* sort1 = input_sort_list[1];

  if ( sort0 != sort1 ) {
    return NULL;
  }

  return sort0;
}

/// @brief Ite 用の input_sort_list のチェックを行う．
/// @param[in] input_sort_list 入力の型のリスト
const SmtSort*
check2(const vector<const SmtSort*>& input_sort_list)
{
  if ( input_sort_list.size() != 3 ) {
    return NULL;
  }

  const SmtSort* sort0 = input_sort_list[0];
  const SmtSort* sort1 = input_sort_list[1];
  const SmtSort* sort2 = input_sort_list[2];

  if ( sort0->type() != kSmtSort_Bool ) {
    return NULL;
  }
  if ( sort1 != sort2 ) {
    return NULL;
  }

  return sort1;
}

/// @brief Uminus 用の input_sort_list のチェックを行う．
/// @param[in] input_sort_list 入力の型のリスト
const SmtSort*
check3(const vector<const SmtSort*>& input_sort_list)
{
  if ( input_sort_list.size() != 1 ) {
    return NULL;
  }

  return input_sort_list[0];
}

/// @brief 算術二項演算子用の input_sort_list のチェックを行う．
/// @param[in] input_sort_list 入力の型のリスト
const SmtSort*
check4(const vector<const SmtSort*>& input_sort_list)
{
  ymuint n = input_sort_list.size();

  if ( n < 2 ) {
    return NULL;
  }

  const SmtSort* sort0 = input_sort_list[0];
  for (ymuint i = 1; i < n; ++ i) {
    const SmtSort* sort1 = input_sort_list[i];
    if ( sort0 != sort1 ) {
      return NULL;
    }
  }

  return sort0;
}

/// @brief 比較演算子用の input_sort_list のチェックを行う．
/// @param[in] input_sort_list 入力の型のリスト
const SmtSort*
check5(const vector<const SmtSort*>& input_sort_list)
{
  if ( input_sort_list.size() != 2 ) {
    return NULL;
  }

  const SmtSort* sort0 = input_sort_list[0];
  const SmtSort* sort1 = input_sort_list[1];
  if ( sort0 != sort1 ) {
    return NULL;
  }

  return sort0;
}

END_NONAMESPACE

// @brief 組み込み関数を作る．
// @param[in] fun_type 関数の種類
// @param[in] input_sort_list 入力の型のリスト
// @note 関数によっては input_sort_list が必要ない場合もある．
const SmtFun*
SmtSolverImpl::make_builtin_fun(tSmtFun fun_type,
				const vector<const SmtSort*>& input_sort_list)
{
  switch ( fun_type ) {
  case kSmtFun_True:
    if ( mTrueFun == NULL ) {
      void* p = mAlloc.get_memory(sizeof(SmtTrueFun));
      mTrueFun = new (p) SmtTrueFun(make_builtin_sort(kSmtSort_Bool));
    }
    return mTrueFun;

  case kSmtFun_False:
    if ( mFalseFun == NULL ) {
      void* p = mAlloc.get_memory(sizeof(SmtTrueFun));
      mFalseFun = new (p) SmtTrueFun(make_builtin_sort(kSmtSort_Bool));
    }
    return mFalseFun;

  case kSmtFun_Not:
    if ( mNotFun == NULL ) {
      void* p = mAlloc.get_memory(sizeof(SmtNotFun));
      mNotFun = new (p) SmtNotFun(make_builtin_sort(kSmtSort_Bool));
    }
    return mNotFun;

  case kSmtFun_And:
    if ( mAndFun == NULL ) {
      void* p = mAlloc.get_memory(sizeof(SmtAndFun));
      mAndFun = new (p) SmtAndFun(make_builtin_sort(kSmtSort_Bool));
    }
    return mAndFun;

  case kSmtFun_Or:
    if ( mOrFun == NULL ) {
      void* p = mAlloc.get_memory(sizeof(SmtOrFun));
      mOrFun = new (p) SmtOrFun(make_builtin_sort(kSmtSort_Bool));
    }
    return mOrFun;

  case kSmtFun_Xor:
    if ( mXorFun == NULL ) {
      void* p = mAlloc.get_memory(sizeof(SmtXorFun));
      mXorFun = new (p) SmtXorFun(make_builtin_sort(kSmtSort_Bool));
    }
    return mXorFun;

  case kSmtFun_Imp:
    if ( mImpFun == NULL ) {
      void* p = mAlloc.get_memory(sizeof(SmtImpFun));
      mImpFun = new (p) SmtImpFun(make_builtin_sort(kSmtSort_Bool));
    }
    return mImpFun;

  case kSmtFun_Eq:
    {
      const SmtSort* sort = check1(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( mEqFunMap.count(sort->id()) == 0 ) {
	void* p = mAlloc.get_memory(sizeof(SmtEqFun));
	const SmtFun* fun = new (p) SmtEqFun(sort, make_builtin_sort(kSmtSort_Bool));
	mEqFunMap.insert(make_pair(sort->id(), fun));
      }
      return mEqFunMap[sort->id()];
    }

  case kSmtFun_Diseq:
    {
      const SmtSort* sort = check1(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( mDiseqFunMap.count(sort->id()) == 0 ) {
	void* p = mAlloc.get_memory(sizeof(SmtDiseqFun));
	const SmtFun* fun = new (p) SmtDiseqFun(sort, make_builtin_sort(kSmtSort_Bool));
	mDiseqFunMap.insert(make_pair(sort->id(), fun));
      }
      return mDiseqFunMap[sort->id()];
    }

  case kSmtFun_Ite:
    {
      const SmtSort* sort = check2(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( mIteFunMap.count(sort->id()) == 0 ) {
	void* p = mAlloc.get_memory(sizeof(SmtIteFun));
	const SmtFun* fun = new (p) SmtIteFun(sort, make_builtin_sort(kSmtSort_Bool));
	mIteFunMap.insert(make_pair(sort->id(), fun));
      }
      return mIteFunMap[sort->id()];
    }

  case kSmtFun_Uminus:
    {
      const SmtSort* sort = check3(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( sort->type() == kSmtSort_Int ) {
	if ( mIntUminusFun == NULL ) {
	  void* p = mAlloc.get_memory(sizeof(SmtUminusFun));
	  mIntUminusFun = new (p) SmtUminusFun(sort);
	}
	return mIntUminusFun;
      }
    }
    break;

  case kSmtFun_Add:
    {
      const SmtSort* sort = check4(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( sort->type() == kSmtSort_Int ) {
	if ( mIntAddFun == NULL ) {
	  void* p = mAlloc.get_memory(sizeof(SmtAddFun));
	  mIntAddFun = new (p) SmtAddFun(sort);
	}
	return mIntAddFun;
      }
    }
    break;

  case kSmtFun_Sub:
    {
      const SmtSort* sort = check4(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( sort->type() == kSmtSort_Int ) {
	if ( mIntSubFun == NULL ) {
	  void* p = mAlloc.get_memory(sizeof(SmtSubFun));
	  mIntSubFun = new (p) SmtSubFun(sort);
	}
	return mIntSubFun;
      }
    }
    break;

  case kSmtFun_Mul:
    {
      const SmtSort* sort = check4(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( sort->type() == kSmtSort_Int ) {
	if ( mIntMulFun == NULL ) {
	  void* p = mAlloc.get_memory(sizeof(SmtMulFun));
	  mIntMulFun = new (p) SmtMulFun(sort);
	}
	return mIntMulFun;
      }
    }
    break;

  case kSmtFun_Le:
    {
      const SmtSort* sort = check5(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( sort->type() == kSmtSort_Int ) {
	if ( mIntLeFun == NULL ) {
	  void* p = mAlloc.get_memory(sizeof(SmtLeFun));
	  mIntLeFun = new (p) SmtLeFun(sort, make_builtin_sort(kSmtSort_Bool));
	}
	return mIntLeFun;
      }
    }
    break;

  case kSmtFun_Lt:
    {
      const SmtSort* sort = check5(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( sort->type() == kSmtSort_Int ) {
	if ( mIntLtFun == NULL ) {
	  void* p = mAlloc.get_memory(sizeof(SmtLtFun));
	  mIntLtFun = new (p) SmtLtFun(sort, make_builtin_sort(kSmtSort_Bool));
	}
	return mIntLtFun;
      }
    }
    break;

  case kSmtFun_Ge:
    {
      const SmtSort* sort = check5(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( sort->type() == kSmtSort_Int ) {
	if ( mIntGeFun == NULL ) {
	  void* p = mAlloc.get_memory(sizeof(SmtGeFun));
	  mIntGeFun = new (p) SmtGeFun(sort, make_builtin_sort(kSmtSort_Bool));
	}
	return mIntGeFun;
      }
    }
    break;

  case kSmtFun_Gt:
    {
      const SmtSort* sort = check5(input_sort_list);
      if ( sort == NULL ) {
	return NULL;
      }
      if ( sort->type() == kSmtSort_Int ) {
	if ( mIntGtFun == NULL ) {
	  void* p = mAlloc.get_memory(sizeof(SmtGtFun));
	  mIntGtFun = new (p) SmtGtFun(sort, make_builtin_sort(kSmtSort_Bool));
	}
	return mIntGtFun;
      }
    }
    break;

  default:
    break;
  }

  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief <numeric> 型の term を作る．
// @param[in] val 値
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_numeric_term(ymuint32 val)
{
  void* p = mAlloc.get_memory(sizeof(SmtNumTerm));
  const SmtTerm* term = new (p) SmtNumTerm(val);

  return term;
}

// @brief <decimal> 型の term を作る．
// @param[in] val 値
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_decimal_term(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtDecTerm));
  const SmtTerm* term = new (p) SmtDecTerm(val);

  return term;
}

// @brief <hexadecimal> 型の term を作る．
// @param[in] val 値
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_hexadecimal_term(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtHexTerm));
  const SmtTerm* term = new (p) SmtHexTerm(val);

  return term;
}

// @brief <binary> 型の term を作る．
// @param[in] val 値
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_binary_term(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtBinTerm));
  const SmtTerm* term = new (p) SmtBinTerm(val);

  return term;
}

// @brief <string> 型の term を作る．
// @param[in] val 値
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_string_term(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtStrTerm));
  const SmtTerm* term = new (p) SmtStrTerm(val);

  return term;
}

// @brief 変数型の term を作る．
// @param[in] var 変数
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_var_term(const SmtVar* var)
{
  void* p = mAlloc.get_memory(sizeof(SmtVarTerm));
  const SmtTerm* term = new (p) SmtVarTerm(var);

  return term;
}

// @brief 関数呼び出しの term を作る．
// @param[in] fun 関数
// @param[in] arg_list 引数のリスト
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_fun_term(const SmtFun* fun,
			     const vector<const SmtTerm*>& arg_list)
{
  ymuint n = arg_list.size();
  const SmtTerm* term;
  if ( n == 0 ) {
    void* p = mAlloc.get_memory(sizeof(SmtFunTerm1));
    term = new (p) SmtFunTerm1(fun);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtFunTerm2) + sizeof(const SmtTerm*) * (n - 1));
    term = new (p) SmtFunTerm2(fun, arg_list);
  }

  return term;
}

// @brief forall の term を作る．
// @param[in] var_list 束縛変数のリスト
// @param[in] body 本体の式
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_forall_term(const vector<const SmtVar*>& var_list,
				const SmtTerm* body)
{
  ymuint n = var_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtForallTerm) + sizeof(const SmtVar*) * (n - 1));
  const SmtTerm* term = new (p) SmtForallTerm(var_list, body);

  return term;
}

// @brief exists の term を作る．
// @param[in] var_list 束縛変数のリスト
// @param[in] body 本体の式
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_exists_term(const vector<const SmtVar*>& var_list,
				const SmtTerm* body)
{
  ymuint n = var_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtExistsTerm) + sizeof(const SmtVar*) * (n - 1));
  const SmtTerm* term = new (p) SmtExistsTerm(var_list, body);

  return term;
}

// @brief 属性付きの term を作る．
// @param[in] body 本体の式
// @param[in] attr_list 属性リスト
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_annotated_term(const SmtTerm* body,
				   const vector<SmtAttr>& attr_list)
{
  ymuint n = attr_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtAnnotatedTerm) + sizeof(SmtAttr) * (n - 1));
  const SmtTerm* term = new (p) SmtAnnotatedTerm(body, attr_list);

  return term;
}

// @brief 充足可能性を調べる．
// @param[in] assert_list 仮定する式のリスト
tSmtLibResponse
SmtSolverImpl::check_sat(const vector<const SmtTerm*>& assert_list)
{
}

// @brief 直前の check_sat() の証明を得る．
// @note check_sat() の結果が UNSAT の時のみ意味を持つ．
tSmtLibResponse
SmtSolverImpl::get_proof()
{
}

// @brief 直前の check_sat() の unsat-core を得る．
// @param[out] term_list unsat-core の式を格納するリスト
// @note check_sat() の結果が UNSAT の時のみ意味を持つ．
tSmtLibResponse
SmtSolverImpl::get_unsat_core(vector<const SmtTerm*>& term_list)
{
}

// @brief 直前の check_sat() の値割り当てを得る．
// @param[in] expr_list expr 式のリスト
// @param[out] value_list 値のリスト
// @note expr_list[i] に対応する値が value_list[i] に格納される．
tSmtLibResponse
SmtSolverImpl::get_value(const vector<const SmtTerm*>& expr_list)
{
}

// @brief 直前の check_sat() の値割り当てを得る．
tSmtLibResponse
SmtSolverImpl::get_assignment()
{
}

END_NAMESPACE_YM_SMT

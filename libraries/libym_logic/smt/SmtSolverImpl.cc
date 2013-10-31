
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

// @brief 関数呼び出しの term を作る．(組み込み関数)
// @param[in] fun_type 関数の型
// @param[in] arg_list 引数のリスト
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_fun_term(tSmtFun fun_type,
			     const vector<const SmtTerm*>& arg_list)
{
  ymuint n = arg_list.size();
  const SmtTerm* term;
  if ( n == 0 ) {
    void* p = mAlloc.get_memory(sizeof(SmtFunTerm3));
    term = new (p) SmtFunTerm3(fun_type);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtFunTerm4) + sizeof(const SmtTerm*) * (n - 1));
    term = new (p) SmtFunTerm4(fun_type, arg_list);
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

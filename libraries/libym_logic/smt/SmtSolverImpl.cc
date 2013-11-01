
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
  if ( check_logic() ) {
    return false;
  }

  mLogic = logic;

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
  {
    void* p = mAlloc.get_memory(sizeof(SmtBoolSort));
    mBoolSort = new (p) SmtBoolSort(new_sort_id());
  }
}

// @brief INTS logic の初期化を行う．
void
SmtSolverImpl::Ints_init()
{
  {
    void* p = mAlloc.get_memory(sizeof(SmtIntSort));
    mIntSort = new (p) SmtIntSort(new_sort_id());
  }
}

// @brief REALS logic の初期化を行う．
void
SmtSolverImpl::Reals_init()
{
  {
    void* p = mAlloc.get_memory(sizeof(SmtRealSort));
    mRealSort = new (p) SmtRealSort(new_sort_id());
  }
 }

// @brief 型を作る．(単純型)
// @param[in] elem_list 要素の型のリスト
// @return 作成した型を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
const SmtSort*
SmtSolverImpl::make_sort()
{
  if ( !check_logic() ) {
    return NULL;
  }

  void* p = mAlloc.get_memory(sizeof(SmtSimpleSort));
  const SmtSort* sort = new (p) SmtSimpleSort(new_sort_id());

  return sort;
}

// @brief 型を作る．(複合型)
// @param[in] elem_list 要素の型のリスト
// @return 作成した型を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
const SmtSort*
SmtSolverImpl::make_sort(const vector<const SmtSort*>& elem_list)
{
  if ( !check_logic() ) {
    return NULL;
  }

  ymuint n = elem_list.size();
  if ( n == 0 ) {
    return make_sort();
  }

  void* p = mAlloc.get_memory(sizeof(SmtComplexSort) + sizeof(const SmtSort*) * (n - 1));
  const SmtSort* sort = new (p) SmtComplexSort(new_sort_id(), elem_list);

  return sort;
}

// @brief 組み込み型を作る．
// @param[in] type 型の種類
// @return 作成した型を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
//  - set_logic() で使える組み込み型ではない．
const SmtSort*
SmtSolverImpl::make_builtin_sort(tSmtSort type)
{
  // set_logic() によってセットされているはず．
  switch ( type ) {
  case kSmtSort_Bool: return mBoolSort;
  case kSmtSort_Int:  return mIntSort;
  case kSmtSort_Real: return mRealSort;
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
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
const SmtVar*
SmtSolverImpl::make_var(const SmtSort* sort,
			tSmtVar type)
{
  if ( !check_logic() ) {
    return NULL;
  }

  void* p;
  const SmtVar* var;
  switch ( type ) {
  case kSmtVar_Global:
    p = mAlloc.get_memory(sizeof(SmtGlobalVar));
    var = new (p) SmtGlobalVar(new_var_id(), sort);
    break;

  case kSmtVar_FunArg:
    p = mAlloc.get_memory(sizeof(SmtFunArgVar));
    var = new (p) SmtFunArgVar(new_var_id(), sort);
    break;

  case kSmtVar_Forall:
    p = mAlloc.get_memory(sizeof(SmtForallVar));
    var = new (p) SmtForallVar(new_var_id(), sort);
    break;

  case kSmtVar_Exists:
    p = mAlloc.get_memory(sizeof(SmtExistsVar));
    var = new (p) SmtExistsVar(new_var_id(), sort);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  return var;
}

// @brief 関数を作る．(引数なし)
// @param[in] output_sort 出力の型
// @return 作成した関数を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
const SmtFun*
SmtSolverImpl::make_fun(const SmtSort* output_sort)
{
  if ( !check_logic() ) {
    return NULL;
  }

  void* p = mAlloc.get_memory(sizeof(SmtDeclFun1));
  const SmtFun* fun = new (p) SmtDeclFun1(output_sort);

  return fun;
}

// @brief 関数を作る．
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @return 作成した関数を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
const SmtFun*
SmtSolverImpl::make_fun(const vector<const SmtSort*>& input_sort_list,
			const SmtSort* output_sort)
{
  if ( !check_logic() ) {
    return NULL;
  }

  ymuint n = input_sort_list.size();
  if ( n == 0 ) {
    return make_fun(output_sort);
  }

  void* p = mAlloc.get_memory(sizeof(SmtDeclFun2) + sizeof(const SmtSort*) * (n - 1));
  const SmtFun* fun = new (p) SmtDeclFun2(input_sort_list, output_sort);

  return fun;
}

// @brief 内容を持った関数を作る．(引数なし)
// @param[in] output_sort 出力の型
// @param[in] body 本体を式
// @return 作成した関数を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
const SmtFun*
SmtSolverImpl::make_fun(const SmtSort* output_sort,
			const SmtTerm* body)
{
  if ( !check_logic() ) {
    return NULL;
  }

  void* p = mAlloc.get_memory(sizeof(SmtDefFun1));
  const SmtFun* fun = new (p) SmtDefFun1(output_sort, body);

  return fun;
}

// @brief 内容を持った関数を作る．
// @param[in] input_var_list 入力の変数のリスト
// @param[in] output_sort 出力の型
// @param[in] body 本体を式
// @return 作成した関数を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
const SmtFun*
SmtSolverImpl::make_fun(const vector<const SmtVar*>& input_var_list,
			const SmtSort* output_sort,
			const SmtTerm* body)
{
  if ( !check_logic() ) {
    return NULL;
  }

  ymuint n = input_var_list.size();
  if ( n == 0 ) {
    return make_fun(output_sort, body);
  }

  void* p = mAlloc.get_memory(sizeof(SmtDefFun2) + sizeof(const SmtVar*) * (n - 1));
  const SmtFun* fun = new (p) SmtDefFun2(input_var_list, output_sort, body);

  return fun;
}

// @brief <numeral> 型の term を作る．
// @param[in] val 値
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_numeral_term(ymuint32 val)
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

// @brief 関数呼び出しの term を作る．(引数なし)
// @param[in] fun 関数
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_fun_term(const SmtFun* fun)
{
  void* p = mAlloc.get_memory(sizeof(SmtFunTerm1));
  const SmtTerm* term = new (p) SmtFunTerm1(fun);

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
  if ( n == 0 ) {
    return make_fun_term(fun);
  }

  void* p = mAlloc.get_memory(sizeof(SmtFunTerm2) + sizeof(const SmtTerm*) * (n - 1));
  const SmtTerm* term = new (p) SmtFunTerm2(fun, arg_list);

  return term;
}

// @brief 関数呼び出しの term を作る．(組み込み関数，引数なし)
// @param[in] fun_type 関数の型
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_fun_term(tSmtFun fun_type)
{
  void* p = mAlloc.get_memory(sizeof(SmtFunTerm3));
  const SmtTerm* term = new (p) SmtFunTerm3(fun_type);

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
  if ( n == 0 ) {
    return make_fun_term(fun_type);
  }

  void* p = mAlloc.get_memory(sizeof(SmtFunTerm4) + sizeof(const SmtTerm*) * (n - 1));
  const SmtTerm* term = new (p) SmtFunTerm4(fun_type, arg_list);

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

// @brief set_logic() のチェックを行う．
bool
SmtSolverImpl::check_logic()
{
  if ( mLogic != kSmtLogic_NONE ) {
    return true;
  }
  return false;
}

// @brief SmtSort のID番号を得る．
ymuint32
SmtSolverImpl::new_sort_id()
{
  ymuint32 id = mSortId;
  ++ mSortId;

  return id;
}

// @brief SmtVar のID番号を得る．
ymuint32
SmtSolverImpl::new_var_id()
{
  ymuint32 id = mVarId;
  ++ mVarId;

  return id;
}

END_NAMESPACE_YM_SMT


/// @file SmtSolverImpl.cc
/// @brief SmtSolverImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SmtSolverImpl.h"

#include "SmtSortImpl.h"
#include "SmtVarImpl.h"
#include "SmtFunImpl.h"
#include "SmtFunValidatorImpl.h"
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

  mSortId = kSmtSort_User1;
  mSortArraySize = 32;
  mSortArray = new const SmtSortInfo*[mSortArraySize];
  for (ymuint i = 0; i < mSortArraySize; ++ i) {
    mSortArray[i] = NULL;
  }

  mVarId = 0;
}

// @brief デストラクタ
SmtSolverImpl::~SmtSolverImpl()
{
  delete [] mSortArray;
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
    Reals_init();
    break;

  case kSmtLogic_AUFNIRA:
    Core_init();
    Ints_init();
    Reals_init();
    break;

  case kSmtLogic_LRA:
    Core_init();
    Reals_init();
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
    Reals_init();
    break;

  case kSmtLogic_QF_NIA:
    Core_init();
    Ints_init();
    break;

  case kSmtLogic_QF_NRA:
    Core_init();
    Reals_init();
    break;

  case kSmtLogic_QF_RDL:
    Core_init();
    Reals_init();
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
    Reals_init();
    break;

  case kSmtLogic_QF_UFNRA:
    Core_init();
    Reals_init();
    break;

  case kSmtLogic_UFLRA:
    Core_init();
    Reals_init();
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
  assert_cond( kSmtSort_Bool < mSortArraySize, __FILE__, __LINE__);

  // Bool 型の生成
  void* p = mAlloc.get_memory(sizeof(SmtSimpleSort));
  SmtSortImpl* sort = new (p) SmtSimpleSort();

  mSortArray[kSmtSort_Bool] = sort;
  sort->mId = kSmtSort_Bool;

  // true/false 関数用 validator の生成
  void* q = mAlloc.get_memory(sizeof(SmtBool0Validator));
  SmtFunValidator* fv0 = new (q) SmtBool0Validator();

  mFunValidatorMap.insert(make_pair(kSmtFun_True, fv0));
  mFunValidatorMap.insert(make_pair(kSmtFun_False, fv0));

  // not 関数用の validator の生成
  void* r = mAlloc.get_memory(sizeof(SmtBool1Validator));
  SmtFunValidator* fv1 = new (r) SmtBool1Validator();

  mFunValidatorMap.insert(make_pair(kSmtFun_Not, fv1));

  // and/or/xor/=> 用の validator の生成
  void* s = mAlloc.get_memory(sizeof(SmtBool2Validator));
  SmtFunValidator* fv2 = new (s) SmtBool2Validator();

  mFunValidatorMap.insert(make_pair(kSmtFun_And, fv2));
  mFunValidatorMap.insert(make_pair(kSmtFun_Or, fv2));
  mFunValidatorMap.insert(make_pair(kSmtFun_Xor, fv2));
  mFunValidatorMap.insert(make_pair(kSmtFun_Imp, fv2));

  // ite 用の validator の生成
  void* t = mAlloc.get_memory(sizeof(SmtIteValidator));
  SmtFunValidator* fv3 = new (t) SmtIteValidator();

  mFunValidatorMap.insert(make_pair(kSmtFun_Ite, fv3));

  // eq/distinct 用の validator の生成
  void* u = mAlloc.get_memory(sizeof(SmtEqValidator));
  SmtFunValidator* fv4 = new (u) SmtEqValidator();

  mFunValidatorMap.insert(make_pair(kSmtFun_Eq, fv4));
  mFunValidatorMap.insert(make_pair(kSmtFun_Diseq, fv4));
}

// @brief INTS logic の初期化を行う．
void
SmtSolverImpl::Ints_init()
{
  assert_cond( kSmtSort_Int < mSortArraySize, __FILE__, __LINE__);

  // Int 型の生成
  void* p = mAlloc.get_memory(sizeof(SmtSimpleSort));
  SmtSortImpl* sort = new (p) SmtSimpleSort();

  mSortArray[kSmtSort_Int] = sort;
  sort->mId = kSmtSort_Int;

  // 単項マイナス用の validator の生成
  void* q = mAlloc.get_memory(sizeof(SmtUminusValidator));
  SmtFunValidator* fv1 = new (q) SmtUminusValidator();

  mFunValidatorMap.insert(make_pair(kSmtFun_Uminus, fv1));

  // 2項演算用の validator の生成
  void* r = mAlloc.get_memory(sizeof(SmtArithValidator));
  SmtFunValidator* fv2 = new (r) SmtArithValidator();

  mFunValidatorMap.insert(make_pair(kSmtFun_Add, fv2));
  mFunValidatorMap.insert(make_pair(kSmtFun_Sub, fv2));
  mFunValidatorMap.insert(make_pair(kSmtFun_Mul, fv2));

  // 比較演算用の validator の生成
  void* s = mAlloc.get_memory(sizeof(SmtCompValidator));
  SmtFunValidator* fv3 = new (s) SmtCompValidator();

  mFunValidatorMap.insert(make_pair(kSmtFun_Le, fv3));
  mFunValidatorMap.insert(make_pair(kSmtFun_Lt, fv3));
  mFunValidatorMap.insert(make_pair(kSmtFun_Ge, fv3));
  mFunValidatorMap.insert(make_pair(kSmtFun_Gt, fv3));
}

// @brief REALS logic の初期化を行う．
void
SmtSolverImpl::Reals_init()
{
  assert_cond( kSmtSort_Real < mSortArraySize, __FILE__, __LINE__);

  // Real 型の生成
  void* p = mAlloc.get_memory(sizeof(SmtSimpleSort));
  SmtSortImpl* sort = new (p) SmtSimpleSort();

  mSortArray[kSmtSort_Real] = sort;
  sort->mId = kSmtSort_Real;

  // 除算用の validator の生成
  void* q = mAlloc.get_memory(sizeof(SmtDivValidator));
  SmtFunValidator* fv1 = new (q) SmtDivValidator();

  mFunValidatorMap.insert(make_pair(kSmtFun_Div, fv1));
}

// @brief 型を作る．(単純型)
// @return 作成した型を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
tSmtSortId
SmtSolverImpl::make_sort()
{
  if ( !check_logic() ) {
    return kSmtSort_None;
  }

  void* p = mAlloc.get_memory(sizeof(SmtSimpleSort));
  SmtSortImpl* sort = new (p) SmtSimpleSort();

  return reg_sort(sort);
}

// @brief 型を作る．(複合型)
// @param[in] elem_list 要素の型のリスト
// @return 作成した型を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
tSmtSortId
SmtSolverImpl::make_sort(const vector<tSmtSortId>& elem_list)
{
  if ( !check_logic() ) {
    return kSmtSort_None;
  }

  ymuint n = elem_list.size();
  if ( n == 0 ) {
    return make_sort();
  }

  void* p = mAlloc.get_memory(sizeof(SmtComplexSort) + sizeof(tSmtSortId) * (n - 1));
  SmtSortImpl* sort = new (p) SmtComplexSort(elem_list);

  return reg_sort(sort);
}

// @brief 型番号から SmtSort を得る．
// @param[in] id 型番号
// @return SmtSort を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
//  - set_logic() で使える組み込み型ではない．
//  - 存在しない型番号だった．
const SmtSortInfo*
SmtSolverImpl::get_sort(tSmtSortId id)
{
  if ( id < mSortId ) {
    return mSortArray[id];
  }
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
SmtSolverImpl::make_var(tSmtSortId sort,
			tSmtVarType type)
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
SmtSolverImpl::make_fun(tSmtSortId output_sort)
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
SmtSolverImpl::make_fun(const vector<tSmtSortId>& input_sort_list,
			tSmtSortId output_sort)
{
  if ( !check_logic() ) {
    return NULL;
  }

  ymuint n = input_sort_list.size();
  if ( n == 0 ) {
    return make_fun(output_sort);
  }

  void* p = mAlloc.get_memory(sizeof(SmtDeclFun2) + sizeof(tSmtSortId) * (n - 1));
  const SmtFun* fun = new (p) SmtDeclFun2(input_sort_list, output_sort);

  return fun;
}

// @brief 内容を持った関数を作る．(引数なし)
// @param[in] body 本体を式
// @return 作成した関数を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
const SmtFun*
SmtSolverImpl::make_fun(const SmtTerm* body)
{
  if ( !check_logic() ) {
    return NULL;
  }

  void* p = mAlloc.get_memory(sizeof(SmtDefFun1));
  const SmtFun* fun = new (p) SmtDefFun1(body->sort(), body);

  return fun;
}

// @brief 内容を持った関数を作る．
// @param[in] input_var_list 入力の変数のリスト
// @param[in] body 本体を式
// @return 作成した関数を返す．
//
// エラーが起きた場合には NULL を返す．
// エラーとなる原因は以下のとおり
//  - set_logic() が呼ばれていない．
const SmtFun*
SmtSolverImpl::make_fun(const vector<const SmtVar*>& input_var_list,
			const SmtTerm* body)
{
  if ( !check_logic() ) {
    return NULL;
  }

  ymuint n = input_var_list.size();
  if ( n == 0 ) {
    return make_fun(body);
  }

  void* p = mAlloc.get_memory(sizeof(SmtDefFun2) + sizeof(const SmtVar*) * (n - 1));
  const SmtFun* fun = new (p) SmtDefFun2(input_var_list, body->sort(), body);

  return fun;
}

// @brief 組み込み関数用のチェッカーを得る．
// @param[in] fun_type 関数の型
const SmtFunValidator*
SmtSolverImpl::get_fun_validator(tSmtFunType fun_type)
{
  FunValidatorMap::const_iterator p = mFunValidatorMap.find(fun_type);
  if ( p == mFunValidatorMap.end() ) {
    return NULL;
  }
  return p->second;
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
//
// エラーが起きたら NULL を返す．
// エラーとなる原因は以下のとおり
//  - fun の引数の数か型が arg_list と合わない．
const SmtTerm*
SmtSolverImpl::make_fun_term(const SmtFun* fun)
{
  if ( fun->input_num() != 0 ) {
    // fun は引数をとる関数だった．
    return NULL;
  }

  void* p = mAlloc.get_memory(sizeof(SmtFunTerm1));
  const SmtTerm* term = new (p) SmtFunTerm1(fun);

  return term;
}

// @brief 関数呼び出しの term を作る．
// @param[in] fun 関数
// @param[in] arg_list 引数のリスト
// @return 作成した式を返す．
//
// エラーが起きたら NULL を返す．
// エラーとなる原因は以下のとおり
//  - fun の引数の数か型が arg_list と合わない．
const SmtTerm*
SmtSolverImpl::make_fun_term(const SmtFun* fun,
			     const vector<const SmtTerm*>& arg_list)
{
  ymuint n = arg_list.size();
  if ( n == 0 ) {
    return make_fun_term(fun);
  }

  if ( fun->input_num() != n ) {
    // fun の引数の数と arg_list のサイズが異なる．
    return NULL;
  }
  for (ymuint i = 0; i < n; ++ i) {
    if ( fun->input_sort(i) != arg_list[i]->sort() ) {
      // i 番めの入力の型が i 番目の引数の型と異なる．
      return NULL;
    }
  }

  void* p = mAlloc.get_memory(sizeof(SmtFunTerm2) + sizeof(const SmtTerm*) * (n - 1));
  const SmtTerm* term = new (p) SmtFunTerm2(fun, arg_list);

  return term;
}

// @brief 関数呼び出しの term を作る．(組み込み関数，引数なし)
// @param[in] fun_type 関数の型
// @return 作成した式を返す．
//
// エラーが起きたら NULL を返す．
// エラーとなる原因は以下のとおり
//  - fun の引数の数か型が arg_list と合わない．
const SmtTerm*
SmtSolverImpl::make_fun_term(tSmtFunType fun_type)
{
  const SmtFunValidator* fv = get_fun_validator(fun_type);
  if ( fv == NULL ) {
    // ?? 関数が登録されていない．
    return NULL;
  }

  if ( !fv->check_inputs(vector<tSmtSortId>(0)) ) {
    // 引数の数が合わない．
    return NULL;
  }

  void* p = mAlloc.get_memory(sizeof(SmtFunTerm3));
  const SmtTerm* term = new (p) SmtFunTerm3(fun_type);

  return term;
}

// @brief 関数呼び出しの term を作る．(組み込み関数)
// @param[in] fun_type 関数の型
// @param[in] arg_list 引数のリスト
// @return 作成した式を返す．
//
// エラーが起きたら NULL を返す．
// エラーとなる原因は以下のとおり
//  - fun の引数の数か型が arg_list と合わない．
const SmtTerm*
SmtSolverImpl::make_fun_term(tSmtFunType fun_type,
			     const vector<const SmtTerm*>& arg_list)
{
  ymuint n = arg_list.size();
  if ( n == 0 ) {
    return make_fun_term(fun_type);
  }

  const SmtFunValidator* fv1 = get_fun_validator(fun_type);
  if ( fv1 == NULL ) {
    // ?? 関数が登録されていない．
    return NULL;
  }
  { // チェックのための引数の型のリストを作る．
    vector<tSmtSortId> arg_sort_list(n);
    for (ymuint i = 0; i < n; ++ i) {
      arg_sort_list[i] = arg_list[i]->sort();
    }
    if ( !fv1->check_inputs(arg_sort_list) ) {
      // 引数の数か型が合わない．
      return NULL;
    }
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

// @brief SmtSort を登録する．
// @param[in] sort 登録するオブジェクト
// @return 割り当てたID番号を返す．
tSmtSortId
SmtSolverImpl::reg_sort(SmtSortImpl* sort)
{
  if ( mSortArraySize <= mSortId ) {
    // mSortArray の大きさを2倍にする．
    ymuint32 old_size = mSortArraySize;
    const SmtSortInfo** old_array = mSortArray;
    mSortArraySize = old_size * 2;
    mSortArray = new const SmtSortInfo*[mSortArraySize];
    for (ymuint i = 0; i < old_size; ++ i) {
      mSortArray[i] = old_array[i];
    }
    delete [] old_array;
  }
  mSortArray[mSortId] = sort;
  sort->mId = mSortId;
  ++ mSortId;
  return sort->mId;
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

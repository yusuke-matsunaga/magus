
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
  mSortId = 0;
  mVarId = 0;
}

// @brief デストラクタ
SmtSolverImpl::~SmtSolverImpl()
{
}

// @brief 使用するロジックを設定する
// @param[in] logic 設定するロジック
tSmtLibResponse
SmtSolverImpl::set_logic(tSmtLogic logic)
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

// @brief 変数を作る．
// @param[in] sort 変数の型
// @param[in] type 変数の種類
// @return 作成した変数を返す．
const SmtVar*
SmtSolverImpl::make_var(const SmtSort* sort,
			SmtVar::tType type)
{
  void* p;
  const SmtVar* var;
  switch ( type ) {
  case SmtVar::kGlobal:
    p = mAlloc.get_memory(sizeof(SmtGlobalVar));
    var = new (p) SmtGlobalVar(mVarId, sort);
    break;

  case SmtVar::kForall:
    p = mAlloc.get_memory(sizeof(SmtForallVar));
    var = new (p) SmtForallVar(mVarId, sort);
    break;

  case SmtVar::kExists:
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
    fun = new (p) SmtDeclFun2(input_sort_list, output_sort, SmtFun::kNone, 0);
  }

  return fun;
}

// @brief 内容を持った関数を作る．
// @param[in] input_var_list 入力の変数のリスト
// @param[in] body 本体を式
// @return 作成した関数を返す．
const SmtFun*
SmtSolverImpl::make_fun(const vector<const SmtVar*>& input_var_list,
			const SmtTerm* body)
{
  const SmtSort* output_sort = NULL; // 実際には body から得る．
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
}

// @brief <decimal> 型の term を作る．
// @param[in] val 値
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_decimal_term(const ShString& val)
{
}

// @brief <hexadecimal> 型の term を作る．
// @param[in] val 値
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_hexadecimal_term(const ShString& val)
{
}

// @brief <binary> 型の term を作る．
// @param[in] val 値
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_binary_term(const ShString& val)
{
}

// @brief <string> 型の term を作る．
// @param[in] val 値
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_string_term(const ShString& val)
{
}

// @brief 変数型の term を作る．
// @param[in] var 変数
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_var_term(const SmtVar* var)
{
}

// @brief 関数呼び出しの term を作る．
// @param[in] fun 関数
// @param[in] arg_list 引数のリスト
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_fun_term(const SmtFun* fun,
			     const vector<const SmtTerm*>& arg_list)
{
}

// @brief forall の term を作る．
// @param[in] var_list 束縛変数のリスト
// @param[in] body 本体の式
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_forall_term(const vector<const SmtVar*>& var_list,
				const SmtTerm* body)
{
}

// @brief exists の term を作る．
// @param[in] var_list 束縛変数のリスト
// @param[in] body 本体の式
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_exists_term(const vector<const SmtVar*>& var_list,
				const SmtTerm* body)
{
}

// @brief 属性付きの term を作る．
// @param[in] body 本体の式
// @param[in] attr_list 属性リスト
// @return 作成した式を返す．
const SmtTerm*
SmtSolverImpl::make_annotated_term(const SmtTerm* body,
				   const vector<SmtAttr>& attr_list)
{
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

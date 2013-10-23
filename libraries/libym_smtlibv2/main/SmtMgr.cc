
/// @file SmtMgr.cc
/// @brief SmtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtMgr.h"

#include "ym_smtlibv2/SmtId.h"
#include "ym_smtlibv2/SmtSort.h"
#include "ym_smtlibv2/SmtVarFun.h"
#include "SmtIdMgr.h"
#include "SmtTermMgr.h"
#include "StackPage.h"
#if 0
#include "SmtConstTerm.h"
#include "SmtIdTerm.h"
#include "SmtCompTerm.h"
#include "SmtFunTerm.h"
#include "SmtListTerm.h"
#endif

BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtMgr::SmtMgr() :
  mLogic(kSmtLogic_NONE)
{
  // スタックの初期化
  StackPage* page = new StackPage();
  mStack.push_back(page);

  mIdMgr = new SmtIdMgr(alloc());
  mTermMgr = new SmtTermMgr(alloc());
}

// @brief デストラクタ
SmtMgr::~SmtMgr()
{
  // スタックの削除
  // 順序は問題ないよね？
  for (vector<StackPage*>::iterator p = mStack.begin();
       p != mStack.end(); ++ p) {
    delete *p;
  }
  delete mIdMgr;
  delete mTermMgr;
}

// @brief set-logic の処理を行う．
// @param[in] logic tSmtLogic ロジック
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// エラーの原因は以下のとおり
//  - set_logic が実行されていた．
bool
SmtMgr::set_logic(tSmtLogic logic)
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

// @brief Core theory の初期化を行う．
void
SmtMgr::Core_init()
{
  // :sorts
  // (Bool 0)
  const SmtId* bool_id = mIdMgr->make_id(ShString("Bool"));
  assert_cond( bool_id != NULL, __FILE__, __LINE__);
  sort_mgr().reg_sort(bool_id, 0);

  const SmtSort* bool_sort = sort_mgr().make_sort(bool_id, vector<const SmtSort*>(0));
  assert_cond( bool_sort != NULL, __FILE__, __LINE__);

  // :funs
  // (true Bool)
  const SmtId* true_id = mIdMgr->make_id(ShString("true"));
  assert_cond( true_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(true_id, vector<const SmtSort*>(0), bool_sort);

  // (false Bool)
  const SmtId* false_id = mIdMgr->make_id(ShString("false"));
  assert_cond( false_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(false_id, vector<const SmtSort*>(0), bool_sort);

  vector<const SmtSort*> b1_list(1);
  b1_list[0] = bool_sort;

  // (not Bool Bool)
  const SmtId* not_id = mIdMgr->make_id(ShString("not"));
  assert_cond( not_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(not_id, b1_list, bool_sort);

  vector<const SmtSort*> b2_list(2);
  b2_list[0] = bool_sort;
  b2_list[1] = bool_sort;

  // (and Bool Bool Bool :right-assoc)
  const SmtId* and_id = mIdMgr->make_id(ShString("and"));
  assert_cond( and_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(and_id, b2_list, bool_sort, SmtVarFun::kRightAssoc);

  // (or Bool Bool Bool :right-assoc)
  const SmtId* or_id = mIdMgr->make_id(ShString("or"));
  assert_cond( or_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(or_id, b2_list, bool_sort, SmtVarFun::kRightAssoc);

  // (xor Bool Bool Bool :right-assoc)
  const SmtId* xor_id = mIdMgr->make_id(ShString("xor"));
  assert_cond( xor_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(xor_id, b2_list, bool_sort, SmtVarFun::kRightAssoc);

  // (=> Bool Bool Bool :right-assoc)
  const SmtId* imp_id = mIdMgr->make_id(ShString("=>"));
  assert_cond( imp_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(imp_id, b2_list, bool_sort, SmtVarFun::kRightAssoc);

  const SmtSort* A_sort = sort_mgr().make_param_sort(0);
  vector<const SmtSort*> a2_list(2);
  a2_list[0] = A_sort;
  a2_list[1] = A_sort;

  // (par (A) (= A A Bool :chainable))
  const SmtId* eq_id = mIdMgr->make_id(ShString("="));
  assert_cond( eq_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(eq_id, a2_list, bool_sort, SmtVarFun::kChainable);

  // (par (A) (distinct A A Bool :pairwise))
  const SmtId* dis_id = mIdMgr->make_id(ShString("distinct"));
  assert_cond( dis_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(dis_id, a2_list, bool_sort, SmtVarFun::kPairwise);

  vector<const SmtSort*> ite_list(3);
  ite_list[0] = bool_sort;
  ite_list[1] = A_sort;
  ite_list[2] = A_sort;

  // (par (A) (ite Bool A A A)
  const SmtId* ite_id = mIdMgr->make_id(ShString("ite"));
  assert_cond( ite_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(ite_id, ite_list, A_sort, SmtVarFun::kNone);
}

// @brief Ints theory の初期化を行う．
void
SmtMgr::Ints_init()
{
  // :sorts
  // (Int 0)
  const SmtId* int_id = mIdMgr->make_id(ShString("Int"));
  assert_cond( int_id != NULL, __FILE__, __LINE__);
  sort_mgr().reg_sort(int_id, 0);

  const SmtSort* int_sort = sort_mgr().make_sort(int_id, vector<const SmtSort*>(0));
  assert_cond( int_sort != NULL, __FILE__, __LINE__);

  // :funs
  // (- Int Int)
  const SmtId* minus_id = mIdMgr->make_id(ShString("-"));
  assert_cond( minus_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(minus_id, vector<const SmtSort*>(1, int_sort), int_sort);

  vector<const SmtSort*> i2_list(2);
  i2_list[0] = int_sort;
  i2_list[1] = int_sort;

  // (- Int Int Int :left-assoc)
  name_mgr().reg_fun(minus_id, i2_list, int_sort, SmtVarFun::kLeftAssoc);

  // (+ Int Int Int :left-assoc)
  const SmtId* plus_id = mIdMgr->make_id(ShString("+"));
  assert_cond( plus_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(plus_id, i2_list, int_sort, SmtVarFun::kLeftAssoc);

  // (* Int Int Int :left-assoc)
  const SmtId* star_id = mIdMgr->make_id(ShString("*"));
  assert_cond( star_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(star_id, i2_list, int_sort, SmtVarFun::kLeftAssoc);

  const SmtId* bool_id = mIdMgr->make_id(ShString("Bool"));
  assert_cond( bool_id != NULL, __FILE__, __LINE__);
  const SmtSort* bool_sort = sort_mgr().make_sort(bool_id, vector<const SmtSort*>(0));
  assert_cond( bool_sort != NULL, __FILE__, __LINE__);

  // (<= Int Int Bool :chainable)
  const SmtId* le_id = mIdMgr->make_id(ShString("<="));
  assert_cond( le_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(le_id, i2_list, bool_sort, SmtVarFun::kChainable);

  // (< Int Int Bool :chainable)
  const SmtId* lt_id = mIdMgr->make_id(ShString("<"));
  assert_cond( lt_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(lt_id, i2_list, bool_sort, SmtVarFun::kChainable);

  // (>= Int Int Bool :chainable)
  const SmtId* ge_id = mIdMgr->make_id(ShString(">="));
  assert_cond( ge_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(ge_id, i2_list, bool_sort, SmtVarFun::kChainable);

  // (> Int Int Bool :chainable)
  const SmtId* gt_id = mIdMgr->make_id(ShString(">"));
  assert_cond( gt_id != NULL, __FILE__, __LINE__);
  name_mgr().reg_fun(gt_id, i2_list, bool_sort, SmtVarFun::kChainable);
}

// @brief set-info の処理を行う．
// @param[in] attr 属性
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
bool
SmtMgr::set_info(const ShString& keyword,
		 const SmtTerm* value)
{
  // 未完
  return true;
}

// @brief sort の宣言を行う．
// @param[in] name_id 型名を表す識別子
// @param[in] param_num パラメータの数
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// エラーの原因は以下のとおり
//  - 同名の型が別に宣言されている．
bool
SmtMgr::declare_sort(const SmtId* name_id,
		     ymuint param_num)
{
  // 型を登録する．
  bool stat = sort_mgr().reg_sort(name_id, param_num);

  return stat;
}

// @brief sort の alias を定義する．
// @param[in] name_id 型名を表す識別子
// @param[in] sort_tmpl 型テンプレート
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// エラーの原因は以下のとおり
//  - 同名の型が既に宣言されている．
//  - sort_tmpl のパラメータに不備がある．
//
// 型テンプレートは見かけは SmtSort だが，
// 要素の型として `パラメータ型' を一つ以上
// 含んでいる．
// sort_tmpl 中に現れるパラメータ型の番号
// に空きがある場合，例えば，2つの要素型を
// 持つ複合型の定義で2つの要素型が，
// それぞれパラメータ番号0 とパラメータ番号2
// だった場合，パラメータ番号1が抜けているので
// エラーとなる．
bool
SmtMgr::define_sort(const SmtId* name_id,
		    const SmtSort* sort_tmpl)
{
  // 型テンプレートを登録する．
  bool stat = sort_mgr().reg_alias(name_id, sort_tmpl);

  return stat;
}

// @brief 関数の宣言を行う．
// @param[in] name_id 関数名を表す識別子
// @param[in] input_sort_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// エラーの原因は以下のとおり
//  - 同名の関数が既に宣言されている．
bool
SmtMgr::declare_fun(const SmtId* name_id,
		    const vector<const SmtSort*>& input_sort_list,
		    const SmtSort* output_sort)
{
  // 関数を登録する．
  bool stat = name_mgr().reg_fun(name_id, input_sort_list, output_sort);

  return stat;
}

// @brief 関数の定義を行う．
// @param[in] name_id 関数名を表す識別子
// @param[in] input_var_list 型つきの入力変数のリスト
// @param[in] output_sort 出力の型
// @param[in] body 本体の式
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// エラーの原因は以下のとおり
//  - 同名の関数が既に定義されている．
bool
SmtMgr::define_fun(const SmtId* name_id,
		   const vector<const SmtVarFun*>& input_var_list,
		   const SmtSort* output_sort,
		   const SmtTerm* body)
{
  // 関数を登録する．
  bool stat = name_mgr().reg_fun(name_id, input_var_list, output_sort, body);

  return stat;
}

// @brief assertion を追加する．
// @param[in] term 式
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// この関数は通常は成功するはず．
bool
SmtMgr::assert(const SmtTerm* term)
{
  assertion_list().push_back(term);

  return true;
}

// @brief assertion スタックにプッシュする．
// @param[in] num プッシュするレベル
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// この関数は通常は成功するはず．
bool
SmtMgr::push(ymuint num)
{
  for (ymuint i = 0; i < num; ++ i) {
    StackPage* prev = mStack.back();
    StackPage* page = new StackPage(mStack.size(), prev);
    mStack.push_back(page);
  }

  return true;
}

// @brief assertion スタックからポップする．
// @param[in] num ポップするレベル．
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// エラーの原因は以下のとおり
//  - num がスタックのサイズと等しいか大きかった．
bool
SmtMgr::pop(ymuint num)
{
  if ( mStack.size() <= num ) {
    // スタックのサイズが小さすぎる．
    return false;
  }

  for (ymuint i = 0; i < num; ++ i) {
    StackPage* page = mStack.back();
    delete page;
    mStack.pop_back();
  }

  return true;
}

// @brief 識別子を返す．
// @param[in] name 名前
// @param[in] index_list インデックスのリスト
// @return 同じ識別子があればそれを返す．なければ作る．
//
// この関数は通常は成功するはず．
const SmtId*
SmtMgr::make_id(const ShString& name,
		const vector<ymuint32>& index_list)
{
  return mIdMgr->make_id(name, index_list);
}

// @brief 型を返す．
// @param[in] name_id 名前を表す識別子
// @param[in] elem_list 要素のリスト
// @return 同じ型があればそれを返す．なければ作る．
// @note エラーの場合には NULL を返す．
//
// エラーの原因は以下のとおり
//  - name_id という名の型が登録されていなかった．
//  - 登録されている型と elem_list のサイズが異なった．
const SmtSort*
SmtMgr::make_sort(const SmtId* name_id,
		  const vector<const SmtSort*>& elem_list)
{
  return sort_mgr().make_sort(name_id, elem_list);
}

// @brief 型パラメータを作る．
// @param[in] pid パラメータ番号
const SmtSort*
SmtMgr::make_param_sort(ymuint pid)
{
  return sort_mgr().make_param_sort(pid);
}

// @brief 変数を返す．
const SmtVarFun*
SmtMgr::make_var(const SmtId* name_id,
		 const SmtSort* sort)
{
  const SmtVarFun* obj = find_obj(name_id);
  if ( obj != NULL ) {
    if ( !obj->is_var() ) {
      // 同じ名前の関数があったらエラー
      return NULL;
    }
    if ( obj->var_sort() != sort &&
	 obj->var_sort() != NULL &&
	 sort != NULL ) {
      // 型が異なっていたらエラー
      return NULL;
    }
    return obj;
  }
  return name_mgr().reg_var(name_id, sort);
}

// @brief 関数を返す．
// @param[in] name_id 関数名
// @return 指定された名前の関数を返す．
// @note エラーの場合には NULL を返す．
//
// エラーの原因は以下のとおり
//  - name_id という名の関数が登録されていなかった．
const SmtVarFun*
SmtMgr::find_obj(const SmtId* name_id)
{
  return name_mgr().find_obj(name_id);
}

// @brief <numeric> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtMgr::make_numeric_term(ymuint32 val)
{
  return mTermMgr->make_numeric(val);
}

// @brief <decimal> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtMgr::make_decimal_term(const ShString& val)
{
  return mTermMgr->make_decimal(val);
}

// @brief <hexadecimal> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtMgr::make_hexadecimal_term(const ShString& val)
{
  return mTermMgr->make_hexadecimal(val);
}

// @brief <binary> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtMgr::make_binary_term(const ShString& val)
{
  return mTermMgr->make_binary(val);
}

// @brief <string> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtMgr::make_string_term(const ShString& val)
{
  return mTermMgr->make_string(val);
}

// @brief <symbol> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtMgr::make_symbol_term(const ShString& val)
{
  return mTermMgr->make_symbol(val);
}

// @brief <keyword> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtMgr::make_keyword_term(const ShString& val)
{
  return mTermMgr->make_keyword(val);
}

#if 0
// @brief <identifier> 型の term を作る．
// @param[in] id 識別子
const SmtTerm*
SmtMgr::make_identifier_term(const SmtId* id)
{
  return mTermMgr->make_identifier(id);
}

// @brief <qualified identifier> 型の term を作る．
// @param[in] id 識別子
// @param[in] sort 型
const SmtTerm*
SmtMgr::make_qual_identifier_term(const SmtId* id,
				  const SmtSort* sort)
{
  return mTermMgr->make_qual_identifier(id, sort);
}
#else
// @brief 変数型の term を作る．
// @param[in] var 変数
// @note 実は引数なしの関数もこの形
const SmtTerm*
SmtMgr::make_var_term(const SmtVarFun* var)
{
  return mTermMgr->make_var(var);
}
#endif

// @brief function term を作る．
// @param[in] function 関数
// @param[in] input_list 入力のリスト
const SmtTerm*
SmtMgr::make_fun_term(const SmtVarFun* function,
		      const vector<const SmtTerm*>& input_list)
{
  return mTermMgr->make_fun(function, input_list);
}

#if 0
// @brief let 文を作る．
// @param[in] var_binding 変数割り当てのリスト
// @param[in] body 本体
const SmtTerm*
SmtMgr::make_let_term(const vector<SmtVarBinding>& var_binding,
		      const SmtTerm* body)
{
  return mTermMgr->make_let(var_binding, body);
}
#endif

// @brief forall 文を作る．
// @param[in] var_list 変数リスト
// @param[in] body 本体
const SmtTerm*
SmtMgr::make_forall_term(const vector<const SmtVarFun*>& var_list,
			 const SmtTerm* body)
{
  return mTermMgr->make_forall(var_list, body);
}

// @brief exists 文を作る．
// @param[in] var_list 変数リスト
// @param[in] body 本体
const SmtTerm*
SmtMgr::make_exists_term(const vector<const SmtVarFun*>& var_list,
			 const SmtTerm* body)
{
  return mTermMgr->make_exists(var_list, body);
}

// @brief attr 文を作る．
// @param[in] body 本体
// @param[in] attr_list 属性リスト
const SmtTerm*
SmtMgr::make_attr_term(const SmtTerm* body,
		       const vector<SmtAttr>& attr_list)
{
  return mTermMgr->make_attr(body, attr_list);
}

// @brief list term を作る．
// @param[in] term_list 要素のリスト
const SmtTerm*
SmtMgr::make_list_term(const vector<const SmtTerm*>& term_list)
{
  return mTermMgr->make_list(term_list);
}

// @brief 現在の SortMgr を返す．
SmtSortMgr&
SmtMgr::sort_mgr()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mSortMgr;
}

// @brief 現在の NameMgr を返す．
SmtNameMgr&
SmtMgr::name_mgr()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mNameMgr;
}

// @brief 現在のアロケータを返す．
Alloc&
SmtMgr::alloc()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mAlloc;
}

// @brief 現在の assertion リストを返す．
vector<const SmtTerm*>&
SmtMgr::assertion_list()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mTermList;
}

END_NAMESPACE_YM_SMTLIBV2

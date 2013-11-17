
/// @file SmtLibMgr.cc
/// @brief SmtLibMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibMgr.h"
#include "SmtLibNode.h"

#include "ym_smtlibv2/SmtId.h"
#include "ym_smtlibv2/SmtSort.h"
#include "ym_smtlibv2/SmtFun.h"
#include "SmtConstTerm.h"
#include "SmtIdTerm.h"
#include "SmtCompTerm.h"
#include "SmtFunTerm.h"
#include "SmtListTerm.h"
#include "SmtAttrImpl.h"


BEGIN_NONAMESPACE

const bool debug = true;

inline
const char*
bool_str(bool val)
{
  if ( val ) {
    return "true";
  }
  else {
    return "false";
  }
}

inline
const char*
stat_str(bool val)
{
  if ( val ) {
    return "success";
  }
  else {
    return "error";
  }
}

END_NONAMESPACE

BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtLibMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtLibMgr::SmtLibMgr() :
  mAlloc(4096),
  mIdMgr(mAlloc),
  mLogic(kSmtLogic_NONE)
{
  StackPage* page = new StackPage();
  mStack.push_back(page);
}

// @brief デストラクタ
SmtLibMgr::~SmtLibMgr()
{
  for (vector<StackPage*>::iterator p = mStack.begin();
       p != mStack.end(); ++ p) {
    delete *p;
  }
}

// @brief set-logic の処理を行う．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::set_logic(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "set-logic ";
    print(cerr, arg_top);
    cerr << endl;
  }

  ostringstream ebuf;

  // このコマンドは1度しか使えない．
  if ( mLogic != kSmtLogic_NONE ) {
    ebuf << "already set";
    goto syntax_error;
  }

  // このコマンドは1つの引数をとる．
  if ( arg_top == NULL || arg_top->sibling() != NULL ||
       arg_top->type() != kSymbolToken ) {
    ebuf << "syntax_error";
    goto syntax_error;
  }
  else {
    const char* str = arg_top->str_value();
    if ( strcmp(str, "AUFLIA") == 0 ) {
      mLogic = kSmtLogic_AUFLIA;

      Core_init();
      Ints_init();
    }
    else if ( strcmp(str, "AUFLIRA") == 0 ) {
      mLogic = kSmtLogic_AUFLIRA;

      Core_init();
      Ints_init();
    }
    else if ( strcmp(str, "AUFNIRA") == 0 ) {
      mLogic = kSmtLogic_AUFNIRA;

      Core_init();
      Ints_init();
    }
    else if ( strcmp(str, "LRA") == 0 ) {
      mLogic = kSmtLogic_LRA;

      Core_init();
    }
    else if ( strcmp(str, "QF_ABV") == 0 ) {
      mLogic = kSmtLogic_QF_ABV;

      Core_init();
    }
    else if ( strcmp(str, "QF_AUFBV") == 0 ) {
      mLogic = kSmtLogic_QF_AUFBV;

      Core_init();
    }
    else if ( strcmp(str, "QF_AUFLIA") == 0 ) {
      mLogic = kSmtLogic_QF_AUFLIA;

      Core_init();
      Ints_init();
    }
    else if ( strcmp(str, "QF_AX") == 0 ) {
      mLogic = kSmtLogic_QF_AX;

      Core_init();
    }
    else if ( strcmp(str, "QF_BV") == 0 ) {
      mLogic = kSmtLogic_QF_BV;

      Core_init();
    }
    else if ( strcmp(str, "QF_IDL") == 0 ) {
      mLogic = kSmtLogic_QF_IDL;

      Core_init();
      Ints_init();
    }
    else if ( strcmp(str, "QF_LIA") == 0 ) {
      mLogic = kSmtLogic_QF_LIA;

      Core_init();
      Ints_init();
    }
    else if ( strcmp(str, "QF_LRA") == 0 ) {
      mLogic = kSmtLogic_QF_LRA;

      Core_init();
    }
    else if ( strcmp(str, "QF_NIA") == 0 ) {
      mLogic = kSmtLogic_QF_NIA;

      Core_init();
      Ints_init();
    }
    else if ( strcmp(str, "QF_NRA") == 0 ) {
      mLogic = kSmtLogic_QF_NRA;

      Core_init();
    }
    else if ( strcmp(str, "QF_RDL") == 0 ) {
      mLogic = kSmtLogic_QF_RDL;

      Core_init();
    }
    else if ( strcmp(str, "QF_UF") == 0 ) {
      mLogic = kSmtLogic_QF_UF;

      Core_init();
    }
    else if ( strcmp(str, "QF_UFBF") == 0 ) {
      mLogic = kSmtLogic_QF_UFBV;

      Core_init();
    }
    else if ( strcmp(str, "QF_UFIDL") == 0 ) {
      mLogic = kSmtLogic_QF_UFIDL;

      Core_init();
      Ints_init();
    }
    else if ( strcmp(str, "QF_UFLIA") == 0 ) {
      mLogic = kSmtLogic_QF_UFLIA;

      Core_init();
      Ints_init();
    }
    else if ( strcmp(str, "QF_UFLRA") == 0 ) {
      mLogic = kSmtLogic_QF_UFLRA;

      Core_init();
    }
    else if ( strcmp(str, "QF_UFNRA") == 0 ) {
      mLogic = kSmtLogic_QF_UFNRA;

      Core_init();
    }
    else if ( strcmp(str, "UFLRA") == 0 ) {
      mLogic = kSmtLogic_UFLRA;

      Core_init();
    }
    else if ( strcmp(str, "UFNIA") == 0 ) {
      mLogic = kSmtLogic_UFNIA;

      Core_init();
      Ints_init();
    }
    else {
      ebuf << str << ": unknown logic";
      goto syntax_error;
    }
  }

  if ( debug ) {
    cerr << "  ==> success" << endl;
  }
  return true;

 syntax_error:
  if ( debug ) {
    cerr << "  ==> error: " << ebuf.str() << endl;
  }
  return false;
}

// @brief Core theory の初期化を行う．
void
SmtLibMgr::Core_init()
{
  // :sorts
  // (Bool 0)
  const SmtId* bool_id = mIdMgr.make_id(ShString("Bool"));
  assert_cond( bool_id != NULL, __FILE__, __LINE__);
  sort_mgr().reg_sort(bool_id, 0);

  const SmtSort* bool_sort = sort_mgr().make_sort(bool_id);
  assert_cond( bool_sort != NULL, __FILE__, __LINE__);

  // :funs
  // (true Bool)
  const SmtId* true_id = mIdMgr.make_id(ShString("true"));
  assert_cond( true_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(true_id, vector<const SmtSort*>(0), bool_sort);

  // (false Bool)
  const SmtId* false_id = mIdMgr.make_id(ShString("false"));
  assert_cond( false_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(false_id, vector<const SmtSort*>(0), bool_sort);

  vector<const SmtSort*> b1_list(1);
  b1_list[0] = bool_sort;

  // (not Bool Bool)
  const SmtId* not_id = mIdMgr.make_id(ShString("not"));
  assert_cond( not_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(not_id, b1_list, bool_sort);

  vector<const SmtSort*> b2_list(2);
  b2_list[0] = bool_sort;
  b2_list[1] = bool_sort;

  // (and Bool Bool Bool :right-assoc)
  const SmtId* and_id = mIdMgr.make_id(ShString("and"));
  assert_cond( and_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(and_id, b2_list, bool_sort, SmtFun::kRightAssoc);

  // (or Bool Bool Bool :right-assoc)
  const SmtId* or_id = mIdMgr.make_id(ShString("or"));
  assert_cond( or_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(or_id, b2_list, bool_sort, SmtFun::kRightAssoc);

  // (xor Bool Bool Bool :right-assoc)
  const SmtId* xor_id = mIdMgr.make_id(ShString("xor"));
  assert_cond( xor_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(xor_id, b2_list, bool_sort, SmtFun::kRightAssoc);

  // (=> Bool Bool Bool :right-assoc)
  const SmtId* imp_id = mIdMgr.make_id(ShString("=>"));
  assert_cond( imp_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(imp_id, b2_list, bool_sort, SmtFun::kRightAssoc);

  const SmtSort* A_sort = sort_mgr().make_param_sort(0);
  vector<const SmtSort*> a2_list(2);
  a2_list[0] = A_sort;
  a2_list[1] = A_sort;

  // (par (A) (= A A Bool :chainable))
  const SmtId* eq_id = mIdMgr.make_id(ShString("="));
  assert_cond( eq_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(eq_id, a2_list, bool_sort, SmtFun::kChainable, 1);

  // (par (A) (distinct A A Bool :pairwise))
  const SmtId* dis_id = mIdMgr.make_id(ShString("distinct"));
  assert_cond( dis_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(dis_id, a2_list, bool_sort, SmtFun::kPairwise, 1);

  vector<const SmtSort*> ite_list(3);
  ite_list[0] = bool_sort;
  ite_list[1] = A_sort;
  ite_list[2] = A_sort;

  // (par (A) (ite Bool A A A)
  const SmtId* ite_id = mIdMgr.make_id(ShString("ite"));
  assert_cond( ite_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(ite_id, ite_list, A_sort, SmtFun::kNone, 1);
}

// @brief Ints theory の初期化を行う．
void
SmtLibMgr::Ints_init()
{
  // :sorts
  // (Int 0)
  const SmtId* int_id = mIdMgr.make_id(ShString("Int"));
  assert_cond( int_id != NULL, __FILE__, __LINE__);
  sort_mgr().reg_sort(int_id, 0);

  const SmtSort* int_sort = sort_mgr().make_sort(int_id, vector<const SmtSort*>(0));
  assert_cond( int_sort != NULL, __FILE__, __LINE__);

  // :funs
  // (- Int Int)
  const SmtId* minus_id = mIdMgr.make_id(ShString("-"));
  assert_cond( minus_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(minus_id, vector<const SmtSort*>(1, int_sort), int_sort);

  vector<const SmtSort*> i2_list(2);
  i2_list[0] = int_sort;
  i2_list[1] = int_sort;

  // (- Int Int Int :left-assoc)
  fun_mgr().reg_fun(minus_id, i2_list, int_sort, SmtFun::kLeftAssoc);

  // (+ Int Int Int :left-assoc)
  const SmtId* plus_id = mIdMgr.make_id(ShString("+"));
  assert_cond( plus_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(plus_id, i2_list, int_sort, SmtFun::kLeftAssoc);

  // (* Int Int Int :left-assoc)
  const SmtId* star_id = mIdMgr.make_id(ShString("*"));
  assert_cond( star_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(star_id, i2_list, int_sort, SmtFun::kLeftAssoc);

  const SmtId* bool_id = mIdMgr.make_id(ShString("Bool"));
  assert_cond( bool_id != NULL, __FILE__, __LINE__);
  const SmtSort* bool_sort = sort_mgr().make_sort(bool_id, vector<const SmtSort*>(0));
  assert_cond( bool_sort != NULL, __FILE__, __LINE__);

  // (<= Int Int Bool :chainable)
  const SmtId* le_id = mIdMgr.make_id(ShString("<="));
  assert_cond( le_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(le_id, i2_list, bool_sort, SmtFun::kChainable);

  // (< Int Int Bool :chainable)
  const SmtId* lt_id = mIdMgr.make_id(ShString("<"));
  assert_cond( lt_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(lt_id, i2_list, bool_sort, SmtFun::kChainable);

  // (>= Int Int Bool :chainable)
  const SmtId* ge_id = mIdMgr.make_id(ShString(">="));
  assert_cond( ge_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(ge_id, i2_list, bool_sort, SmtFun::kChainable);

  // (> Int Int Bool :chainable)
  const SmtId* gt_id = mIdMgr.make_id(ShString(">"));
  assert_cond( gt_id != NULL, __FILE__, __LINE__);
  fun_mgr().reg_fun(gt_id, i2_list, bool_sort, SmtFun::kChainable);
}

// @brief set-info の処理を行う．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::set_info(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "set-info ";
    print(cerr, arg_top);
    cerr << endl;
  }

  vector<const SmtAttr*> attr_list;
  const char* emsg = "";

  if ( !eval_to_attr(arg_top, attr_list) ) {
    emsg = "syntax_error";
    goto syntax_error;
  }
  if ( attr_list.empty() || attr_list.size() > 1 ) {
    emsg = "syntax_error";
    goto syntax_error;
  }

  if ( debug ) {
    cerr << "  ==> success" << endl;
  }

  return true;

 syntax_error:
  if ( debug ) {
    cerr << "  ==> error: " << emsg << endl;
  }

  return false;
}

// @brief sort の宣言を行う．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::declare_sort(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "declare-sort ";
    print(cerr, arg_top);
    cerr << endl;
  }

  // このコマンドは2つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  const SmtId* name = NULL;
  ymuint num = 0;
  const char* emsg = "";

  if ( !parse_args(arg_top, 2, arg_list) ) {
    emsg = "syntax error";
    goto syntax_error;
  }

  // 1つめは型名
  name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    emsg = "first argument is not an indentifier";
    goto syntax_error;
  }

  // 2つめは引数の数
  if ( arg_list[1]->type() != kNumToken ) {
    emsg = "second argument is not a numeral";
    goto syntax_error;
  }
  num = arg_list[1]->int_value();

  // 型を登録する．
  if ( !sort_mgr().reg_sort(name, num) ) {
    emsg = "already declared";
    goto syntax_error;
  }

  if ( debug ) {
    cerr << "  ==> success" << endl;
  }

  return true;

 syntax_error:
  if ( debug ) {
    cerr << "  ==> error: " << emsg << endl;
  }

  return false;
}

// @brief sort の alias を定義する．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::define_sort(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "define-sort ";
    print(cerr, arg_top);
    cerr << endl;
  }

  // このコマンドは3つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  const SmtId* name = NULL;
  vector<const SmtId*> param_list;
  const SmtSort* sort = NULL;
  const char* emsg = "";

  if ( !parse_args(arg_top, 3, arg_list) ) {
    emsg = "syntax error";
    goto syntax_error;
  }

  // 1つめは型名
  name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    emsg = "first argument is not an identifier";
    goto syntax_error;
  }

  // 2つめはパラメータリスト
  if ( arg_list[1]->type() != kListToken ) {
    emsg = "second argument is not 'list' type";
    goto syntax_error;
  }
  param_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node1 = arg_list[1]->child();
       node1 != NULL; node1 = node1->sibling()) {
    const SmtId* id = eval_to_id(node1);
    if ( id == NULL ) {
      emsg = "second argument is not a list of identifiers";
      goto syntax_error;
    }
    param_list.push_back(id);
  }

  // 3つめは型テンプレート
  sort = eval_to_sort_template(arg_list[2], param_list);
  if ( sort == NULL ) {
    emsg = "third argument is not a valid sort";
    goto syntax_error;
  }

  // 型テンプレートを登録する．
  if ( !sort_mgr().reg_alias(name, param_list.size(), sort) ) {
    emsg = "already defined";
    goto syntax_error;
  }

  if ( debug ) {
    cerr << "  ==> success" << endl;
  }

  return true;

 syntax_error:
  if ( debug ) {
    cerr << "  ==> error: " << emsg << endl;
  }

  return false;
}

// @brief 関数の宣言を行う．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::declare_fun(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "declare-fun ";
    print(cerr, arg_top);
    cerr << endl;
  }

  // このコマンドは3つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  const SmtId* name = NULL;
  vector<const SmtSort*> input_sort_list;
  const SmtSort* output_sort = NULL;
  const char* emsg = "";

  if ( !parse_args(arg_top, 3, arg_list) ) {
    emsg = "syntax error";
    goto syntax_error;
  }

  // 1つめは名前
  name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    emsg = "first argument is not an identifier";
    goto syntax_error;
  }

  // 2つめは入力の型のリスト
  if ( arg_list[1]->type() != kListToken ) {
    emsg = "second argument is not 'list' type";
    goto syntax_error;
  }
  input_sort_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node = arg_list[1]->child();
       node != NULL; node = node->sibling()) {
    const SmtSort* sort = eval_to_sort(node);
    if ( sort == NULL ) {
      ostringstream buf;
      print(buf, node);
      buf << " is not a registered sort";
      emsg = buf.str().c_str();
      goto syntax_error;
    }
    input_sort_list.push_back(sort);
  }

  // 3つめは出力の型のリスト
  output_sort = eval_to_sort(arg_list[2]);
  if ( output_sort == NULL ) {
    emsg = "third argument is not a valid sort";
    goto syntax_error;
  }

  // 関数を登録する．
  if ( !fun_mgr().reg_fun(name, input_sort_list, output_sort) ) {
    emsg = "already declared";
    goto syntax_error;
  }

  if ( debug ) {
    cerr << "  ==> success" << endl;
  }

  return true;

 syntax_error:
  if ( debug ) {
    cerr << "  ==> error: " << emsg << endl;
  }

  return false;
}

// @brief 関数の定義を行う．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::define_fun(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "define-fun ";
    print(cerr, arg_top);
    cerr << endl;
  }

  // このコマンドは4つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  const SmtId* name = NULL;
  vector<SmtSortedVar> var_list;
  const SmtSort* output_sort = NULL;
  const SmtTerm* body = NULL;

  const char* emsg = "";

  if ( !parse_args(arg_top, 4, arg_list) ) {
    emsg = "syntax error";
    goto syntax_error;
  }

  // 1つめは名前
  name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    emsg = "first argument is not an identifier";
    goto syntax_error;
  }

  // 2つめは変数のリスト
  if ( arg_list[1]->type() != kListToken ) {
    emsg = "second argument is not 'list' type";
    goto syntax_error;
  }
  var_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node = arg_list[1]->child();
       node != NULL; node = node->sibling()) {
    SmtSortedVar sorted_var;
    if ( !eval_to_sorted_var(node, sorted_var) ) {
      emsg = "sorted var is expected.";
      goto syntax_error;
    }
    var_list.push_back(sorted_var);
  }

  // 3つめは出力の型
  output_sort = eval_to_sort(arg_list[2]);
  if ( output_sort == NULL ) {
    emsg = "third argument is not a valid sort";
    goto syntax_error;
  }

  // 4つめは本体の式
  body = eval_to_term(arg_list[3]);
  if ( body == NULL ) {
    emsg = "forth argument is not a valid term";
    goto syntax_error;
  }

  // 関数を登録する．
  if ( !fun_mgr().reg_fun(name, var_list, output_sort, body) ) {
    emsg = "already defined";
    goto syntax_error;
  }

  if ( debug ) {
    cerr << "  ==> success" << endl;
  }

  return true;

 syntax_error:
  if ( debug ) {
    cerr << "  ==> error: " << emsg << endl;
  }

  return false;
}

// @brief assertion を追加する．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::assert(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "assert ";
    print(cerr, arg_top);
    cerr << endl;
  }

  const char* emsg = "";

  // このコマンドは1つの引数をとる．
  if ( arg_top == NULL || arg_top->sibling() != NULL ) {
    emsg = "syntax error";
    goto syntax_error;
  }
  else {
    const SmtTerm* term = eval_to_term(arg_top);
    if ( term == NULL ) {
      // エラーメッセージは出力されている．
      goto syntax_error;
    }
    assertion_list().push_back(term);
 }


  if ( debug ) {
    cerr << "  ==> success" << endl;
  }

  return true;

 syntax_error:
  if ( debug ) {
    cerr << "  ==> error: " << emsg << endl;
  }

  return false;
}

// @brief assertion スタックにプッシュする．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::push(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "push ";
    print(cerr, arg_top);
    cerr << endl;
  }

  // このコマンドは1つの整数値を引数にとる．
  if ( arg_top == NULL ||
       arg_top->sibling() != NULL ||
       arg_top->type() != kNumToken ) {
    if ( debug ) {
      cerr << "  ==> error: syntax error" << endl;
      return false;
    }
  }

  ymuint num = arg_top->int_value();
  for (ymuint i = 0; i < num; ++ i) {
    StackPage* prev = mStack.back();
    StackPage* page = new StackPage(mStack.size(), prev);
    mStack.push_back(page);
  }

  if ( debug ) {
    cerr << "  ==> success" << endl;
  }
  return true;
}

// @brief assertion スタックからポップする．
// @param[in] arg_top 引数の先頭ノード
// @return ポップが成功したら true を返す．
bool
SmtLibMgr::pop(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "pop ";
    print(cerr, arg_top);
    cerr << endl;
  }

  // このコマンドは1つの整数値を引数にとる．
  if ( arg_top == NULL ||
       arg_top->sibling() != NULL ||
       arg_top->type() != kNumToken ) {
    if ( debug ) {
      cerr << "  ==> error: syntax error" << endl;
      return false;
    }
  }

  ymuint num = arg_top->int_value();
  if ( mStack.size() <= num ) {
    // スタックのサイズが小さすぎる．
    if ( debug ) {
      cerr << "  ==> error: arg is too large" << endl;
    }
    return false;
  }

  for (ymuint i = 0; i < num; ++ i) {
    StackPage* page = mStack.back();
    delete page;
    mStack.pop_back();
  }

  if ( debug ) {
    cerr << "  ==> success" << endl;
  }

  return true;
}

// @brief S式を識別子に変換する．
// @param[in] node S式を表すノード
const SmtId*
SmtLibMgr::eval_to_id(const SmtLibNode* node)
{
  ShString name;
  vector<ymuint32> index_list;
  if ( node->type() == kSymbolToken ) {
    // 単純な形式の場合
    name = node->str_value();
  }
  else if ( node->type() == kListToken ) {
    // 最初は '_'
    const SmtLibNode* node1 = node->child();
    if ( node1->type() != kSymbolToken ||
	 strcmp("_", static_cast<const char*>(node1->str_value())) != 0 ) {
      // syntax error
      goto syntax_error;
    }

    // 2番めが名前
    const SmtLibNode* node2 = node1->sibling();
    if ( node2 == NULL ||
	 node2->type() != kSymbolToken ) {
      // syntax error
      goto syntax_error;
    }
    name = node2->str_value();

    // 3番め以降にインデックス
    index_list.reserve(node->child_num() - 2);
    for (const SmtLibNode* node3 = node2->sibling();
	 node3 != NULL; node3 = node3->sibling()) {
      if ( node3->type() != kNumToken ) {
	// syntax error
	goto syntax_error;
      }
      index_list.push_back(node3->int_value());
    }
    if ( index_list.empty() ) {
      // syntax error
      goto syntax_error;
    }
  }

  return mIdMgr.make_id(name, index_list);

 syntax_error:
  return NULL;
}

// @brief S式を sort に変換する．
// @param[in] node S式を表すノード
const SmtSort*
SmtLibMgr::eval_to_sort(const SmtLibNode* node)
{
  const SmtId* id = eval_to_id(node);
  vector<const SmtSort*> elem_list;
  const SmtSort* sort = NULL;
  ostringstream ebuf;

  if ( id == NULL ) {
    if ( node->type() != kListToken ) {
      ebuf << "syntax error : " << node->loc();
      goto syntax_error;
    }

    elem_list.reserve(node->child_num() - 1);

    const SmtLibNode* node1 = node->child();
    id = eval_to_id(node1);
    if ( id == NULL ) {
      ebuf << "not an identifier: " << node1->loc();
      goto syntax_error;
    }

    for (const SmtLibNode* node2 = node1->sibling();
	 node2 != NULL; node2 = node2->sibling()) {
      const SmtSort* sort1 = eval_to_sort(node2);
      if ( sort1 == NULL ) {
	ebuf << "not a registered sort: " << node2->loc();
	goto syntax_error;
      }
      elem_list.push_back(sort1);
    }
    if ( elem_list.empty() ) {
      ebuf << "at least one sort required: " << node1->loc();
      goto syntax_error;
    }
  }

  sort = sort_mgr().make_sort(id, elem_list);
  if ( sort == NULL ) {
    ebuf << id->name() << ": not registered";
    goto syntax_error;
  }
  return sort;

 syntax_error:
  if ( debug ) {
    cerr << "eval_to_sort failed: " << ebuf.str() << endl;
  }

  return NULL;
}

// @brief S式を sort に変換する．
// @param[in] node S式を表すノード
const SmtSort*
SmtLibMgr::eval_to_sort_template(const SmtLibNode* node,
				 const vector<const SmtId*>& param_list)
{
  const SmtId* id = eval_to_id(node);
  vector<const SmtSort*> elem_list;
  if ( id != NULL ) {
    ymuint n = param_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      if ( id == param_list[i] ) {
	return sort_mgr().make_param_sort(i);
      }
    }
  }
  else {
    if ( node->type() != kListToken ) {
      // syntax error
      goto syntax_error;
    }

    elem_list.reserve(node->child_num() - 1);

    const SmtLibNode* node1 = node->child();
    id = eval_to_id(node1);
    if ( id == NULL ) {
      // syntax error
      goto syntax_error;
    }

    for (const SmtLibNode* node2 = node1->sibling();
	 node2 != NULL; node2 = node2->sibling()) {
      const SmtSort* sort1 = eval_to_sort_template(node2, param_list);
      if ( sort1 == NULL ) {
	// syntax error
	goto syntax_error;
      }
      elem_list.push_back(sort1);
    }
    if ( elem_list.empty() ) {
      // syntax error
      goto syntax_error;
    }
  }

  return sort_mgr().make_sort(id, elem_list);

 syntax_error:
  return NULL;
}

// @brief S式を term に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
SmtLibMgr::eval_to_term(const SmtLibNode* node)
{
  // 定数の場合
  switch ( node->type() ) {
  case kNumToken:    return new_numeral(node->int_value());
  case kDecToken:    return new_decimal(node->str_value());
  case kHexToken:    return new_hexadecimal(node->str_value());
  case kBinToken:    return new_binary(node->str_value());
  case kStringToken: return new_string(node->str_value());
  default: ;
  }

  // qual_identifier の場合
  const SmtTerm* term = eval_to_qid(node);
  if ( term != NULL ) {
    return term;
  }

  ostringstream ebuf;

  // 残りは ( identifier term+ ) の形
  if ( node->type() != kListToken ) {
    ebuf << "syntax error at " << node->loc();
    goto syntax_error;
  }
  else {
    ymuint n = node->child_num();
    const SmtLibNode* node1 = node->child();
    const SmtId* fid = eval_to_id(node1);
    if ( fid == NULL ) {
      ebuf << "not an identifier: " << node1->loc();
      goto syntax_error;
    }
    const SmtFun* fun = find_fun(fid);
    if ( fun != NULL ) {
      if ( fun->attr() == SmtFun::kNone && fun->input_num() != n - 1) {
	// 引数の数が合わない．
	ebuf << "# of args mismatch: " << node->loc();
	goto syntax_error;
      }
      vector<const SmtTerm*> input_list;
      input_list.reserve(n - 1);
      for (const SmtLibNode* node2 = node1->sibling();
	   node2 != NULL; node2 = node2->sibling()) {
	const SmtTerm* term1 = eval_to_term(node2);
	if ( term1 == NULL ) {
	  // エラーメッセージは下位の呼び出しで出力されているはず
	  return NULL;
	}
	input_list.push_back(term1);
      }
      return new_fun_term(fun, input_list);
    }
    else if ( node1->type() != kSymbolToken ||
	      strcmp("let", static_cast<const char*>(node1->str_value())) == 0 ) {
      if ( node->child_num() != 3 ) {
	ebuf << "# of args mismatch: " << node->loc();
	goto syntax_error;
      }
      const SmtLibNode* node2 = node1->sibling();
      if ( node2->type() != kListToken ) {
	ebuf << "'list' type expected: " << node2->loc();
	goto syntax_error;
      }

      vector<SmtVarBinding> var_binding_list;
      var_binding_list.reserve(node2->child_num());
      for (const SmtLibNode* node21 = node2->child();
	   node21 != NULL; node21 = node21->sibling()) {
	SmtVarBinding var_binding;
	if ( !eval_to_var_binding(node21, var_binding) ) {
	  // エラーメッセージは eval_to_var_binding() 中で出力されているはず
	  return NULL;
	}
	var_binding_list.push_back(var_binding);
      }

      const SmtLibNode* node3 = node2->sibling();
      const SmtTerm* term3 = eval_to_term(node3);
      if ( term3 == NULL ) {
	// エラーメッセージは下位の呼び出しで出力されているはず
	return NULL;
      }
      return new_let(var_binding_list, term3);
    }
    else if ( node1->type() != kSymbolToken ||
	      strcmp("forall", static_cast<const char*>(node1->str_value())) == 0 ) {
      if ( node->child_num() != 3 ) {
	ebuf << "# of args mismatch: " << node->loc();
	goto syntax_error;
      }
      const SmtLibNode* node2 = node1->sibling();
      if ( node2->type() != kListToken ) {
	ebuf << "'list' type expected: " << node2->loc();
	goto syntax_error;
      }

      vector<SmtSortedVar> sorted_var_list;
      sorted_var_list.reserve(node2->child_num());
      for (const SmtLibNode* node21 = node2->child();
	   node21 != NULL; node21 = node21->sibling()) {
	SmtSortedVar sorted_var;
	if ( !eval_to_sorted_var(node21, sorted_var) ) {
	  // エラーメッセージは eval_to_sorted_var() 中で出力されているはず
	  return NULL;
	}
	sorted_var_list.push_back(sorted_var);
      }

      const SmtLibNode* node3 = node2->sibling();
      const SmtTerm* term3 = eval_to_term(node3);
      if ( term3 == NULL ) {
	// エラーメッセージは下位の呼び出しで出力されているはず
	return NULL;
      }
      return new_forall(sorted_var_list, term3);
    }
    else if ( node1->type() != kSymbolToken ||
	      strcmp("exists", static_cast<const char*>(node1->str_value())) == 0 ) {
      if ( node->child_num() != 3 ) {
	ebuf << "# of args mismatch: " << node->loc();
	goto syntax_error;
      }
      const SmtLibNode* node2 = node1->sibling();
      if ( node2->type() != kListToken ) {
	ebuf << "'llist' type expected: " << node2->loc();
	goto syntax_error;
      }

      vector<SmtSortedVar> sorted_var_list;
      sorted_var_list.reserve(node2->child_num());
      for (const SmtLibNode* node21 = node2->child();
	   node21 != NULL; node21 = node21->sibling()) {
	SmtSortedVar sorted_var;
	if ( !eval_to_sorted_var(node21, sorted_var) ) {
	  // エラーメッセージは eval_to_sorted_var() 中で出力されているはず
	  return NULL;
	}
	sorted_var_list.push_back(sorted_var);
      }

      const SmtLibNode* node3 = node2->sibling();
      const SmtTerm* term3 = eval_to_term(node3);
      if ( term3 == NULL ) {
	// エラーメッセージは下位の呼び出しで出力されているはず
	return NULL;
      }
      return new_exists(sorted_var_list, term3);
    }
    else if ( node1->type() != kSymbolToken ||
	      strcmp("!", static_cast<const char*>(node1->str_value())) == 0 ) {
      if ( node->child_num() < 3 ) {
	ebuf << "# of args mismatch: " << node->loc();
	goto syntax_error;
      }
      const SmtLibNode* node2 = node1->sibling();
      const SmtTerm* term = eval_to_term(node2);
      if ( term == NULL ) {
	// エラーメッセージは下位の呼び出しで出力されているはず
	return NULL;
      }

      vector<const SmtAttr*> attr_list;
      attr_list.reserve(node->child_num() - 2);
      if ( !eval_to_attr(node2->sibling(), attr_list) ) {
	// エラーメッセージは eval_to_attr() で出力されているはず
	return NULL;
      }
      return new_attr_term(term, attr_list);
    }
    else {
      ebuf << fid->name() << ": undefined: " << node1->loc();
      goto syntax_error;
    }
  }

 syntax_error:
  if ( debug ) {
    cerr << ebuf.str() << endl;
  }
  return NULL;
}

// @brief S式を s-expr に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
SmtLibMgr::eval_to_expr(const SmtLibNode* node)
{
  switch ( node->type() ) {
  case kNumToken:
    return new_numeral(node->int_value());

  case kDecToken:
    return new_decimal(node->str_value());

  case kHexToken:
    return new_hexadecimal(node->str_value());

  case kBinToken:
    return new_binary(node->str_value());

  case kStringToken:
    return new_string(node->str_value());

  case kSymbolToken:
    return new_symbol(node->str_value());

  case kKeywordToken:
    return new_keyword(node->str_value());

  case kListToken:
    {
      vector<const SmtTerm*> expr_list;
      expr_list.reserve(node->child_num());
      for (const SmtLibNode* node1 = node->child();
	   node1 != NULL; node1 = node1->sibling()) {
	const SmtTerm* expr1 = eval_to_expr(node1);
	if ( expr1 == NULL ) {
	  return NULL;
	}
	expr_list.push_back(expr1);
      }
      return new_list_term(expr_list);
    }

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return NULL;
}

// @brief S式を qual_identifier に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
SmtLibMgr::eval_to_qid(const SmtLibNode* node)
{
  // ただの identifier の場合もある．
  const SmtId* id = eval_to_id(node);
  if ( id != NULL ) {
    return new_identifier(id);
  }

  // それ以外は ( 'as' <identifier> <sort> ) の形
  vector<const SmtLibNode*> arg_list;
  if ( !parse_args(node, 3, arg_list) ) {
    return NULL;
  }

  if ( arg_list[0]->type() != kSymbolToken ) {
    return NULL;
  }
  if ( strcmp("as", static_cast<const char*>(arg_list[0]->str_value())) != 0 ) {
    return NULL;
  }

  id = eval_to_id(arg_list[1]);
  if ( id == NULL ) {
    return NULL;
  }

  const SmtSort* sort = eval_to_sort(arg_list[2]);
  if ( sort == NULL ) {
    return NULL;
  }

  return new_qual_identifier(id, sort);
}

// @brief S式を sorted_var に変換する．
// @param[in] node S式を表すノード
// @param[out] sorted_var 結果を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．
bool
SmtLibMgr::eval_to_sorted_var(const SmtLibNode* node,
			      SmtSortedVar& sorted_var)
{
  // ( <identifier> <sort> ) の形
  vector<const SmtLibNode*> arg_list;
  const SmtId* id = NULL;
  const SmtSort* sort = NULL;
  ostringstream ebuf;
  if ( !parse_args(node->child(), 2, arg_list) ) {
    ebuf << "syntax error: " << node->loc();
    goto syntax_error;
  }

  id = eval_to_id(arg_list[0]);
  if ( id == NULL ) {
    ebuf << "not a valid identifier: " << arg_list[0]->loc();
    goto syntax_error;
  }

  sort = eval_to_sort(arg_list[1]);
  if ( sort == NULL ) {
    return false;
  }

  sorted_var.mVar = id;
  sorted_var.mSort = sort;

  return true;

 syntax_error:
  if ( debug ) {
    cerr << "eval_to_sorted_var() failed: " << ebuf.str() << endl;
  }

  return false;
}

// @brief S式を var_binding に変換する．
// @param[in] node S式を表すノード
// @param[out] var_binding 結果を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．
bool
SmtLibMgr::eval_to_var_binding(const SmtLibNode* node,
			       SmtVarBinding& var_binding)
{
  // ( <identifier> <term> ) の形
  vector<const SmtLibNode*> arg_list;
  const SmtId* id = NULL;
  const SmtTerm* term = NULL;
  ostringstream ebuf;
  if ( !parse_args(node->child(), 2, arg_list) ) {
    ebuf << "syntax error: " << node->loc();
    goto syntax_error;
  }

  id = eval_to_id(arg_list[0]);
  if ( id == NULL ) {
    ebuf << "not a valid identifier: " << arg_list[0]->loc();
    goto syntax_error;
  }

  term = eval_to_term(arg_list[1]);
  if ( term == NULL ) {
    // エラーメッセージは eval_to_term() 中で出力されている．
    return false;
  }

  var_binding.mVar = id;
  var_binding.mExpr = term;

  return true;

 syntax_error:
  if ( debug ) {
    cerr << "eval_to_var_binding() failed: " << ebuf.str() << endl;
  }

  return false;
}

// @brief S式を attribute に変換する．
// @param[in] node S式を表すノード
// @param[out] attr_list 結果の attribute のリストを格納する変数
bool
SmtLibMgr::eval_to_attr(const SmtLibNode* node,
			vector<const SmtAttr*>& attr_list)
{
  attr_list.clear();
  for (const SmtLibNode* node1 = node; node1 != NULL; node1 = node1->sibling()) {
    if ( node1->type() != kKeywordToken ) {
      return false;
    }
    const SmtLibNode* node2 = node1->sibling();
    if ( node2 != NULL && node2->type() != kKeywordToken ) {
      const SmtTerm* expr = eval_to_expr(node2);
      if ( expr == NULL ) {
	return false;
      }
      const SmtAttr* attr = new_attr(node1->str_value(), expr);
      attr_list.push_back(attr);

      node1 = node2;
    }
    else {
      const SmtAttr* attr = new_attr(node1->str_value());
      attr_list.push_back(attr);
    }
  }
  return true;
}

// @brief 引数のリストをパーズする．
// @param[in] arg_top 引数の先頭のノード
// @param[in] arg_num 引数の数
// @param[out] arg_list 引数を格納するリスト
// @retval true 引数の数が arg_num と一致した．
// @retval false 引数の数が arg_num と一致しなかった．
bool
SmtLibMgr::parse_args(const SmtLibNode*  arg_top,
		      ymuint arg_num,
		      vector<const SmtLibNode*>& arg_list)
{
  arg_list.clear();
  arg_list.reserve(arg_num);
  const SmtLibNode* node = arg_top;
  for (ymuint i = 0; i < arg_num; ++ i) {
    if ( node == NULL ) {
      return false;
    }
    arg_list.push_back(node);
    node = node->sibling();
  }
  if ( node != NULL ) {
    return false;
  }
  return true;
}

// @brief <numeral> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_numeral(ymuint32 val)
{
  void* p = mAlloc.get_memory(sizeof(SmtNumTerm));
  return new (p) SmtNumTerm(val);
}

// @brief <decimal> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_decimal(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtDecTerm));
  return new (p) SmtDecTerm(val);
}

// @brief <hexadecimal> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_hexadecimal(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtHexTerm));
  return new (p) SmtHexTerm(val);
}

// @brief <binary> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_binary(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtBinTerm));
  return new (p) SmtBinTerm(val);
}

// @brief <string> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_string(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtStrTerm));
  return new (p) SmtStrTerm(val);
}

// @brief <symbol> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_symbol(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtSymbolTerm));
  return new (p) SmtSymbolTerm(val);
}

// @brief <keyword> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_keyword(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtKeywordTerm));
  return new (p) SmtKeywordTerm(val);
}

// @brief identifier を作る．
// @param[in] id 識別子
const SmtTerm*
SmtLibMgr::new_identifier(const SmtId* id)
{
  void* p = mAlloc.get_memory(sizeof(SmtIdTerm));
  return new (p) SmtIdTerm(id);
}

// @brief qualified identifier を作る．
// @param[in] id 識別子
// @param[in] sort 型
const SmtTerm*
SmtLibMgr::new_qual_identifier(const SmtId* id,
			       const SmtSort* sort)
{
  void* p = mAlloc.get_memory(sizeof(SmtQualIdTerm));
  return new (p) SmtQualIdTerm(id, sort);
}

// @brief function term を作る．
// @param[in] function 関数
// @param[in] input_list 入力のリスト
const SmtTerm*
SmtLibMgr::new_fun_term(const SmtFun* function,
			const vector<const SmtTerm*>& input_list)
{
  ymuint n = input_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtFunTerm) + sizeof(const SmtTerm*) * ( n - 1));
  return new (p) SmtFunTerm(function, input_list);
}

// @brief let 文を作る．
// @param[in] var_binding 変数割り当てのリスト
// @param[in] body 本体
const SmtTerm*
SmtLibMgr::new_let(const vector<SmtVarBinding>& var_binding,
		   const SmtTerm* body)
{
  ymuint n = var_binding.size();
  void* p = mAlloc.get_memory(sizeof(SmtLet) + sizeof(SmtVarBinding) * (n - 1));
  return new (p) SmtLet(var_binding, body);
}

// @brief forall 文を作る．
// @param[in] var_list 変数リスト
// @param[in] body 本体
const SmtTerm*
SmtLibMgr::new_forall(const vector<SmtSortedVar>& var_list,
		      const SmtTerm* body)
{
  ymuint n = var_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtForall) + sizeof(SmtSortedVar) * (n - 1));
  return new (p) SmtForall(var_list, body);
}

// @brief exists 文を作る．
// @param[in] var_list 変数リスト
// @param[in] body 本体
const SmtTerm*
SmtLibMgr::new_exists(const vector<SmtSortedVar>& var_list,
		      const SmtTerm* body)
{
  ymuint n = var_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtExists) + sizeof(SmtSortedVar) * (n - 1));
  return new (p) SmtExists(var_list, body);
}

// @brief attr 文を作る．
// @param[in] body 本体
// @param[in] attr_list 属性リスト
const SmtTerm*
SmtLibMgr::new_attr_term(const SmtTerm* body,
			 const vector<const SmtAttr*>& attr_list)
{
  ymuint n = attr_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtAttrTerm) + sizeof(const SmtAttr*) * (n - 1));
  return new (p) SmtAttrTerm(body, attr_list);
}

// @brief term list を作る．
// @param[in] term_list 要素のリスト
const SmtTerm*
SmtLibMgr::new_list_term(const vector<const SmtTerm*>& term_list)
{
  ymuint n = term_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtListTerm) + sizeof(const SmtTerm*) * (n - 1));
  return new (p) SmtListTerm(term_list);
}

// @brief attribute を作る．
// @param[in] keyword キーワード
// @param[in] expr 値
const SmtAttr*
SmtLibMgr::new_attr(const ShString& keyword,
		    const SmtTerm* expr)
{
  SmtAttr* attr = NULL;
  if ( expr == NULL ) {
    void* p = mAlloc.get_memory(sizeof(SmtAttrImpl));
    attr = new (p) SmtAttrImpl(keyword);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtAttrImpl2));
    attr = new (p) SmtAttrImpl2(keyword, expr);
  }
  return attr;
}

// @brief 識別子から関数に変換する．
// @param[in] name 関数名
const SmtFun*
SmtLibMgr::find_fun(const SmtId* name)
{
  return fun_mgr().find_fun(name);
}

// @brief 現在の SortMgr を返す．
SmtSortMgr&
SmtLibMgr::sort_mgr()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mSortMgr;
}

// @brief 現在の FunMgr を返す．
SmtFunMgr&
SmtLibMgr::fun_mgr()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mFunMgr;
}

// @brief 現在の assertion リストを返す．
vector<const SmtTerm*>&
SmtLibMgr::assertion_list()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mTermList;
}

END_NAMESPACE_YM_SMTLIBV2

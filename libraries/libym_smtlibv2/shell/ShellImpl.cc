
/// @file ShellImpl.cc
/// @brief ShellImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ShellImpl.h"
#include "SmtLibParser.h"
#include "SmtLibNode.h"

#include "StackPage.h"
#include "IdMgr.h"
#include "NameObj.h"

#include "ym_logic/SmtAttr.h"
#include "ym_logic/SmtSort.h"
#include "ym_logic/SmtTerm.h"
#include "ym_logic/SmtVar.h"

#include "ym_smtlibv2/SmtId.h"

#include <readline/readline.h>
#include <readline/history.h>

#include "ym_utils/StrListIDO.h"


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

// line の先頭と末尾の空白を取り除く．
// line 中の文字が書き換えられる可能性がある．
char*
chop(char* line)
{
  char* s;
  for (s = line; *s && isspace(*s); ++ s) ;
  if ( *s == 0 ) {
    return s;
  }

  char* t = line + strlen(line) - 1;
  for ( ; t > s && isspace(*t); -- t) ;
  ++ t;
  *t = '\0';
  return s;
}

END_NONAMESPACE

BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス ShellImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ShellImpl::ShellImpl()
{
  mParser = new SmtLibParser();
  mPrompt1 = ">";
  mPrompt2 = "...>";
  mAllowCtrlDExit = false;

  mSolver = SmtSolver::new_solver();

  // スタックの初期化
  StackPage* page0 = new StackPage(*mSolver);
  mStack.push_back(page0);

  mIdMgr = new IdMgr(alloc());
}

// @brief デストラクタ
ShellImpl::~ShellImpl()
{
  delete mParser;

  delete mSolver;

  // スタックの削除
  // 順序は問題ないよね？
  for (vector<StackPage*>::iterator p = mStack.begin();
       p != mStack.end(); ++ p) {
    delete *p;
  }
  delete mIdMgr;
}

// @brief 実行する．
void
ShellImpl::run()
{
  const char* prompt = mPrompt1.c_str();
  for (mLoop = true; mLoop; ) {
    char* line = readline(prompt);
    if ( line == NULL ) {
      // EOF が入力された．
      if ( mAllowCtrlDExit ) {
	mLoop = false;
      }
      printf("\n");
      continue;
    }

    // 前後の空白を取り除く
    char* str = chop(line);
    if ( *str == '\0' ) {
      // 空行ならスキップする．
      free(line);
      continue;
    }

    // ヒストリに追加する．
    add_history(str);
    ++ mHistoryNum;

    if ( mHistoryNum >= mMaxHistory ) {
      // あふれたヒストリを削除する．
      HIST_ENTRY* hist = remove_history(0);
      free(hist);
      -- mHistoryNum;
    }

    mStrList.push_back(string(str));

    // 1行を評価する．
    int code = eval_line();
    switch ( code ) {
    case 0:
      // exit
      mLoop = false;
      break;

    case 1:
      // 通常の終了
      mStrList.clear();
      prompt = mPrompt1.c_str();
      break;

    case 2:
      // 継続行
      prompt = mPrompt2.c_str();
      break;
    }

    // line で確保された領域を開放する．
    free(line);
  }
}

// @brief 1行を評価する．
int
ShellImpl::eval_line()
{
  StrListIDO ido(mStrList);
  mParser->init(&ido);

  SmtLibNode* node = NULL;
  const SmtLibNode* node1 = NULL;
  const SmtLibNode* node2 = NULL;
  const char* str1 = NULL;
  bool result = false;

  SmtLibParser::tResult res = mParser->read(node);
  switch ( res ) {
  case SmtLibParser::kOK:
    break;

  case SmtLibParser::kError:
    goto syntax_error;

  case SmtLibParser::kEOF:
    assert_not_reached(__FILE__, __LINE__);
    break;

  case SmtLibParser::kOpen:
    // 継続行
    return 2;
  }

  if ( debug ) {
    print(cerr, node);
    cerr << endl;
  }
  mErrBuf.str("");
  mErrBuf.clear();

  if ( node->type() != kListToken ) {
    goto syntax_error;
  }

  node1 = node->child();
  if ( node1->type() != kSymbolToken ) {
    goto syntax_error;
  }
  str1 = static_cast<const char*>(node1->str_value());
  node2 = node1->sibling();
  if ( strcmp("set-logic", str1) == 0 ) {
    result = set_logic(node2);
  }
  else if ( strcmp("set-option", str1) == 0 ) {
    result = set_option(node2);
  }
  else if ( strcmp("set-info", str1) == 0 ) {
    result = set_info(node2);
  }
  else if ( strcmp("declare-sort", str1) == 0 ) {
    result = declare_sort(node2);
  }
  else if ( strcmp("define-sort", str1) == 0 ) {
    result = define_sort(node2);
  }
  else if ( strcmp("declare-fun", str1) == 0 ) {
    result = declare_fun(node2);
  }
  else if ( strcmp("define-fun", str1) == 0 ) {
    result = define_fun(node2);
  }
  else if ( strcmp("push", str1) == 0 ) {
    result = push(node2);
  }
  else if ( strcmp("pop", str1) == 0 ) {
    result = pop(node2);
  }
  else if ( strcmp("assert", str1) == 0 ) {
    result = assert(node2);
  }
  else if ( strcmp("check-sat", str1) == 0 ) {
    result = check_sat(node2);
  }
  else if ( strcmp("get-assertions", str1) == 0 ) {
    result = get_assertions(node2);
  }
  else if ( strcmp("get-proof", str1) == 0 ) {
    result = get_proof(node2);
  }
  else if ( strcmp("get-unsat-core", str1) == 0 ) {
    result = get_unsat_core(node2);
  }
  else if ( strcmp("get-value", str1) == 0 ) {
    result = get_value(node2);
  }
  else if ( strcmp("get-assignment", str1) == 0 ) {
    result = get_assignment(node2);
  }
  else if ( strcmp("get-option", str1) == 0 ) {
    result = get_option(node2);
  }
  else if ( strcmp("get-info", str1) == 0 ) {
    result = get_info(node2);
  }
  else if ( strcmp("exit", str1) == 0 ) {
    return 0;
  }
  else {
    goto syntax_error;
  }
  if ( result ) {
    if ( debug ) {
      cerr << "  ==> success" << endl;
    }
    return 1;
  }
  // result == false の時は syntax_error に続く

 syntax_error:
  if ( debug ) {
    cerr << "  ==> error: " << mErrBuf.str() << endl;
  }
  return 1;
}

// @brief set-logic の処理を行う．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::set_logic(const SmtLibNode* arg_top)
{
  // このコマンドは1つの引数をとる．
  if ( arg_top == NULL || arg_top->sibling() != NULL ||
       arg_top->type() != kSymbolToken ) {
    mErrBuf << "syntax_error";
    return false;
  }

  const char* str = arg_top->str_value();
  tSmtLogic logic = kSmtLogic_NONE;
  if ( strcmp(str, "AUFLIA") == 0 ) {
    logic = kSmtLogic_AUFLIA;
    CORE_init();
  }
  else if ( strcmp(str, "AUFLIRA") == 0 ) {
    logic = kSmtLogic_AUFLIRA;
    CORE_init();
  }
  else if ( strcmp(str, "AUFNIRA") == 0 ) {
    logic = kSmtLogic_AUFNIRA;
    CORE_init();
  }
  else if ( strcmp(str, "LRA") == 0 ) {
    logic = kSmtLogic_LRA;
    CORE_init();
  }
  else if ( strcmp(str, "QF_ABV") == 0 ) {
    logic = kSmtLogic_QF_ABV;
    CORE_init();
  }
  else if ( strcmp(str, "QF_AUFBV") == 0 ) {
    logic = kSmtLogic_QF_AUFBV;
    CORE_init();
  }
  else if ( strcmp(str, "QF_AUFLIA") == 0 ) {
    logic = kSmtLogic_QF_AUFLIA;
    CORE_init();
  }
  else if ( strcmp(str, "QF_AX") == 0 ) {
    logic = kSmtLogic_QF_AX;
    CORE_init();
  }
  else if ( strcmp(str, "QF_BV") == 0 ) {
    logic = kSmtLogic_QF_BV;
    CORE_init();
  }
  else if ( strcmp(str, "QF_IDL") == 0 ) {
    logic = kSmtLogic_QF_IDL;
    CORE_init();
  }
  else if ( strcmp(str, "QF_LIA") == 0 ) {
    logic = kSmtLogic_QF_LIA;
    CORE_init();
  }
  else if ( strcmp(str, "QF_LRA") == 0 ) {
    logic = kSmtLogic_QF_LRA;
    CORE_init();
  }
  else if ( strcmp(str, "QF_NIA") == 0 ) {
    logic = kSmtLogic_QF_NIA;
    CORE_init();
  }
  else if ( strcmp(str, "QF_NRA") == 0 ) {
    logic = kSmtLogic_QF_NRA;
    CORE_init();
  }
  else if ( strcmp(str, "QF_RDL") == 0 ) {
    logic = kSmtLogic_QF_RDL;
    CORE_init();
  }
  else if ( strcmp(str, "QF_UF") == 0 ) {
    logic = kSmtLogic_QF_UF;
    CORE_init();
  }
  else if ( strcmp(str, "QF_UFBF") == 0 ) {
    logic = kSmtLogic_QF_UFBV;
    CORE_init();
  }
  else if ( strcmp(str, "QF_UFIDL") == 0 ) {
    logic = kSmtLogic_QF_UFIDL;
    CORE_init();
  }
  else if ( strcmp(str, "QF_UFLIA") == 0 ) {
    logic = kSmtLogic_QF_UFLIA;
    CORE_init();
  }
  else if ( strcmp(str, "QF_UFLRA") == 0 ) {
    logic = kSmtLogic_QF_UFLRA;
    CORE_init();
  }
  else if ( strcmp(str, "QF_UFNRA") == 0 ) {
    logic = kSmtLogic_QF_UFNRA;
    CORE_init();
  }
  else if ( strcmp(str, "UFLRA") == 0 ) {
    logic = kSmtLogic_UFLRA;
    CORE_init();
  }
  else if ( strcmp(str, "UFNIA") == 0 ) {
    logic = kSmtLogic_UFNIA;
    CORE_init();
  }
  else {
    mErrBuf << str << ": unknown logic";
    return false;
  }

  if ( !mSolver->set_logic(logic) ) {
    mErrBuf << "already set";
    return false;
  }

  return true;
}

// @brief CORE theory の初期化を行う．
void
ShellImpl::CORE_init()
{
  // :sorts
  // (Bool 0)
  const SmtId* bool_id = mIdMgr->make_id(ShString("Bool"));
  assert_cond( bool_id != NULL, __FILE__, __LINE__);
  sort_mgr().declare_sort(bool_id, 0);

  const SmtSort* bool_sort = sort_mgr().make_sort(bool_id);
  assert_cond( bool_sort != NULL, __FILE__, __LINE__);

  // :funs
  vector<const SmtSort*> b0_list(0);

  // (true Bool)
  const SmtFun* true_fun = mSolver->make_fun(b0_list, bool_sort);
  assert_cond( true_fun != NULL, __FILE__, __LINE__);

  const SmtId* true_id = mIdMgr->make_id(ShString("true"));
  assert_cond( true_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(true_id, true_fun);

  // (false Bool)
  const SmtFun* false_fun = mSolver->make_fun(b0_list, bool_sort);
  assert_cond( false_fun != NULL, __FILE__, __LINE__);

  const SmtId* false_id = mIdMgr->make_id(ShString("false"));
  assert_cond( false_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(false_id, false_fun);

  vector<const SmtSort*> b1_list(1);
  b1_list[0] = bool_sort;

  // (not Bool Bool)
  const SmtFun* not_fun = mSolver->make_fun(b1_list, bool_sort);
  assert_cond( not_fun, __FILE__, __LINE__);

  const SmtId* not_id = mIdMgr->make_id(ShString("not"));
  assert_cond( not_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(not_id, not_fun);

  vector<const SmtSort*> b2_list(2);
  b2_list[0] = bool_sort;
  b2_list[1] = bool_sort;

  // (and Bool Bool Bool :right-assoc)
  const SmtFun* and_fun = mSolver->make_fun(b2_list, bool_sort);
  assert_cond( and_fun, __FILE__, __LINE__);

  const SmtId* and_id = mIdMgr->make_id(ShString("and"));
  assert_cond( and_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(and_id, and_fun);

  // (or Bool Bool Bool :right-assoc)
  const SmtFun* or_fun = mSolver->make_fun(b2_list, bool_sort);
  assert_cond( or_fun, __FILE__, __LINE__);

  const SmtId* or_id = mIdMgr->make_id(ShString("or"));
  assert_cond( or_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(or_id, or_fun);

  // (xor Bool Bool Bool :right-assoc)
  const SmtFun* xor_fun = mSolver->make_fun(b2_list, bool_sort);
  assert_cond( xor_fun, __FILE__, __LINE__);

  const SmtId* xor_id = mIdMgr->make_id(ShString("xor"));
  assert_cond( xor_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(xor_id, xor_fun);

  // (=> Bool Bool Bool :right-assoc)
  const SmtFun* imp_fun = mSolver->make_fun(b2_list, bool_sort);
  assert_cond( imp_fun, __FILE__, __LINE__);

  const SmtId* imp_id = mIdMgr->make_id(ShString("=>"));
  assert_cond( imp_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(imp_id, imp_fun);

#if 0
  const SmtSort* A_sort = sort_mgr().make_param_sort_templ(0);
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
#endif
}

// @brief INTS theory の初期化を行う．
void
ShellImpl::INTS_init()
{
  // :sorts
  // (Int 0)
  const SmtId* int_id = mIdMgr->make_id(ShString("Int"));
  assert_cond( int_id != NULL, __FILE__, __LINE__);

  sort_mgr().declare_sort(int_id, 0);

  const SmtSort* int_sort = sort_mgr().make_sort(int_id);
  assert_cond( int_sort != NULL, __FILE__, __LINE__);

  // :funs

  vector<const SmtSort*> i1_list(1);
  i1_list[0] = int_sort;

  // (- Int Int)
  const SmtFun* uminus_fun = mSolver->make_fun(i1_list, int_sort);
  assert_cond( uminus_fun, __FILE__, __LINE__);

  const SmtId* minus_id = mIdMgr->make_id(ShString("-"));
  assert_cond( minus_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(minus_id, uminus_fun);

  vector<const SmtSort*> i2_list(2);
  i2_list[0] = int_sort;
  i2_list[1] = int_sort;

  // (- Int Int Int :left-assoc)
  const SmtFun* minus_fun = mSolver->make_fun(i2_list, int_sort);
  assert_cond( minus_fun, __FILE__, __LINE__);

  name_mgr().reg_fun(minus_id, minus_fun);

  // (+ Int Int Int :left-assoc)
  const SmtFun* plus_fun = mSolver->make_fun(i2_list, int_sort);
  assert_cond( plus_fun, __FILE__, __LINE__);

  const SmtId* plus_id = mIdMgr->make_id(ShString("+"));
  assert_cond( plus_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(plus_id, plus_fun);

  // (* Int Int Int :left-assoc)
  const SmtFun* star_fun = mSolver->make_fun(i2_list, int_sort);
  assert_cond( star_fun, __FILE__, __LINE__);

  const SmtId* star_id = mIdMgr->make_id(ShString("*"));
  assert_cond( star_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(star_id, star_fun);

  const SmtId* bool_id = mIdMgr->make_id(ShString("Bool"));
  assert_cond( bool_id != NULL, __FILE__, __LINE__);

  const SmtSort* bool_sort = sort_mgr().make_sort(bool_id, vector<const SmtSort*>(0));
  assert_cond( bool_sort != NULL, __FILE__, __LINE__);

  // (<= Int Int Bool :chainable)
  const SmtFun* le_fun = mSolver->make_fun(i2_list, bool_sort);
  assert_cond( le_fun, __FILE__, __LINE__);

  const SmtId* le_id = mIdMgr->make_id(ShString("<="));
  assert_cond( le_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(le_id, le_fun);

  // (< Int Int Bool :chainable)
  const SmtFun* lt_fun = mSolver->make_fun(i2_list, bool_sort);
  assert_cond( lt_fun, __FILE__, __LINE__);

  const SmtId* lt_id = mIdMgr->make_id(ShString("<"));
  assert_cond( lt_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(lt_id, lt_fun);

  // (>= Int Int Bool :chainable)
  const SmtFun* ge_fun = mSolver->make_fun(i2_list, bool_sort);
  assert_cond( ge_fun, __FILE__, __LINE__);

  const SmtId* ge_id = mIdMgr->make_id(ShString(">="));
  assert_cond( ge_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(ge_id, ge_fun);

  // (> Int Int Bool :chainable)
  const SmtFun* gt_fun = mSolver->make_fun(i2_list, bool_sort);
  assert_cond( gt_fun, __FILE__, __LINE__);

  const SmtId* gt_id = mIdMgr->make_id(ShString(">"));
  assert_cond( gt_id != NULL, __FILE__, __LINE__);

  name_mgr().reg_fun(gt_id, gt_fun);
}


// @brief オプションを設定する．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::set_option(const SmtLibNode* arg_top)
{
  // 未完
  return true;
}

// @brief 情報を設定する．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::set_info(const SmtLibNode* arg_top)
{
  if ( arg_top->type() != kKeywordToken ) {
    mErrBuf << "syntx error: keyword expected at " << arg_top->loc();
    return false;
  }

  const SmtTerm* expr = NULL;
  const SmtLibNode* node2 = arg_top->sibling();
  if ( node2 != NULL ) {
    if ( node2->sibling() != NULL ) {
      mErrBuf << "syntax error: too many tokens at " << node2->sibling()->loc();
      return false;
    }
    expr = eval_to_expr(node2);
    if ( expr == NULL ) {
      return false;
    }
  }

#if 0
  if ( !mMgr.set_info(arg_top->str_value(), expr) ) {
    return false;
  }
#endif

  return true;
}

// @brief sort の宣言を行う．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::declare_sort(const SmtLibNode* arg_top)
{
  // このコマンドは2つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  if ( !parse_args(arg_top, 2, arg_list) ) {
    mErrBuf << "syntax error";
    return false;
  }

  // 1つめは型名
  const SmtId* name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    mErrBuf << "first argument is not an indentifier";
    return false;
  }

  // 2つめは引数の数
  if ( arg_list[1]->type() != kNumToken ) {
    mErrBuf << "second argument is not a numeric";
    return false;
  }
  ymuint num = arg_list[1]->int_value();

  // 型を登録する．
  if ( !sort_mgr().declare_sort(name, num) ) {
    mErrBuf << name->name() << "already declared";
    return false;
  }

  return true;
}

// @brief sort の alias を定義する．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::define_sort(const SmtLibNode* arg_top)
{
  // このコマンドは3つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  if ( !parse_args(arg_top, 3, arg_list) ) {
    mErrBuf << "syntax error";
    return false;
  }

  // 1つめは型名
  const SmtId* name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    mErrBuf << "first argument is not an identifier";
    return false;
  }

  // 2つめはパラメータリスト
  if ( arg_list[1]->type() != kListToken ) {
    mErrBuf << "second argument is not 'list' type";
    return false;
  }
  vector<const SmtId*> param_list;
  param_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node1 = arg_list[1]->child();
       node1 != NULL; node1 = node1->sibling()) {
    const SmtId* id = eval_to_id(node1);
    if ( id == NULL ) {
      mErrBuf << "second argument is not a list of identifiers";
      return false;
    }
    param_list.push_back(id);
  }

  // 3つめは型テンプレート
  const SortElem* sort = eval_to_sort_template(arg_list[2], param_list);
  if ( sort == NULL ) {
    mErrBuf << "third argument is not a valid sort";
    return false;
  }

  // 型テンプレートを登録する．
  if ( !sort_mgr().define_sort(name, sort) ) {
    mErrBuf << name->name() << "already defined";
    return false;
  }

  return true;
}

// @brief 関数の宣言を行う．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::declare_fun(const SmtLibNode* arg_top)
{
  // このコマンドは3つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  if ( !parse_args(arg_top, 3, arg_list) ) {
    mErrBuf << "syntax error";
    return false;
  }

  // 1つめは名前
  const SmtId* name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    mErrBuf << "first argument is not an identifier";
    return false;
  }

  // 2つめは入力の型のリスト
  if ( arg_list[1]->type() != kListToken ) {
    mErrBuf << "second argument is not 'list' type";
    return false;
  }

  vector<const SmtSort*> input_sort_list;
  input_sort_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node = arg_list[1]->child();
       node != NULL; node = node->sibling()) {
    const SmtSort* sort = eval_to_sort(node);
    if ( sort == NULL ) {
      print(mErrBuf, node);
      mErrBuf << " is not a registered sort";
      return false;
    }
    input_sort_list.push_back(sort);
  }

  // 3つめは出力の型のリスト
  const SmtSort* output_sort = eval_to_sort(arg_list[2]);
  if ( output_sort == NULL ) {
    mErrBuf << "third argument is not a valid sort";
    return false;
  }

  // 同名のオブジェクトが登録されていないか調べる．
  const NameObj* obj = name_mgr().find_obj(name);
  if ( obj != NULL ) {
    mErrBuf << name->name() << "already declared";
    return false;
  }

  // 関数を作る．
  const SmtFun* fun = mSolver->make_fun(input_sort_list, output_sort);
  assert_cond( fun != NULL, __FILE__, __LINE__);

  // 関数を登録する．
  bool stat = name_mgr().reg_fun(name, fun);
  assert_cond( stat, __FILE__, __LINE__);

  return true;
}

// @brief 関数の定義を行う．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::define_fun(const SmtLibNode* arg_top)
{
  // このコマンドは4つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  if ( !parse_args(arg_top, 4, arg_list) ) {
    mErrBuf << "syntax error";
    return false;
  }

  // 1つめは名前
  const SmtId* name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    mErrBuf << "first argument is not an identifier";
    return false;
  }

  // 2つめは変数のリスト
  if ( arg_list[1]->type() != kListToken ) {
    mErrBuf <<  "second argument is not 'list' type";
    return false;
  }
  vector<const SmtVar*> var_list;
  var_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node = arg_list[1]->child();
       node != NULL; node = node->sibling()) {
    const SmtVar* var = eval_to_sorted_var(node);
    if ( var == NULL ) {
      mErrBuf << "sorted var is expected.";
      return false;
    }
    var_list.push_back(var);
  }

  // 3つめは出力の型
  const SmtSort* output_sort = eval_to_sort(arg_list[2]);
  if ( output_sort == NULL ) {
    mErrBuf << "third argument is not a valid sort";
    return false;
  }

  // 4つめは本体の式
  const SmtTerm* body = eval_to_term(arg_list[3]);
  if ( body == NULL ) {
    mErrBuf << "forth argument is not a valid term";
    return false;
  }

  // 同名のオブジェクトが存在しないか確かめる．
  const NameObj* obj = name_mgr().find_obj(name);
  if ( obj != NULL ) {
    mErrBuf << name->name() << ": already exists";
    return false;
  }

  // 関数を作る．
  const SmtFun* fun = mSolver->make_fun(var_list, output_sort, body);
  assert_cond( fun != NULL, __FILE__, __LINE__);

  // 関数を登録する．
  bool stat = name_mgr().reg_fun(name, fun);
  assert_cond( stat, __FILE__, __LINE__);

  return true;
}

// @brief assertion を追加する．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::assert(const SmtLibNode* arg_top)
{
  // このコマンドは1つの引数をとる．
  if ( arg_top == NULL || arg_top->sibling() != NULL ) {
    mErrBuf <<  "syntax error";
    return false;
  }

  const SmtTerm* term = eval_to_term(arg_top);
  if ( term == NULL ) {
    // エラーメッセージは出力されている．
    return false;
  }
#if 0
  if ( !mMgr.assert(term) ) {
    return false;
  }
#endif

  return true;
}

// @brief assertion スタックにプッシュする．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::push(const SmtLibNode* arg_top)
{
  // このコマンドは1つの整数値を引数にとる．
  if ( arg_top == NULL ||
       arg_top->sibling() != NULL ||
       arg_top->type() != kNumToken ) {
    mErrBuf <<  "syntax error";
    return false;
  }

  ymuint num = arg_top->int_value();
  for (ymuint i = 0; i < num; ++ i) {
    StackPage* prev = mStack.back();
    StackPage* page = new StackPage(*mSolver, mStack.size(), prev);
    mStack.push_back(page);
  }

  return true;
}

// @brief assertion スタックからポップする．
// @param[in] arg_top 引数の先頭ノード
// @return ポップが成功したら true を返す．
bool
ShellImpl::pop(const SmtLibNode* arg_top)
{
  // このコマンドは1つの整数値を引数にとる．
  if ( arg_top == NULL ||
       arg_top->sibling() != NULL ||
       arg_top->type() != kNumToken ) {
    mErrBuf <<  "syntax error";
    return false;
  }

  ymuint num = arg_top->int_value();
  if ( mStack.size() <= num ) {
    // スタックのサイズが小さすぎる．
    mErrBuf <<  "arg is too large";
    return false;
  }

  for (ymuint i = 0; i < num; ++ i) {
    StackPage* page = mStack.back();
    delete page;
    mStack.pop_back();
  }

  return true;
}

// @brief 充足可能性判定を行なう．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::check_sat(const SmtLibNode* arg_top)
{
  // 未完
  return true;
}

// @brief assertion を得る．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::get_assertions(const SmtLibNode* arg_top)
{
  // 未完
  return true;
}

// @brief 証明を得る．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::get_proof(const SmtLibNode* arg_top)
{
  // 未完
  return true;
}

// @brief unsat core を得る．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::get_unsat_core(const SmtLibNode* arg_top)
{
  // 未完
  return true;
}

// @brief 値を得る．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::get_value(const SmtLibNode* arg_top)
{
  // 未完
  return true;
}

// @brief 割り当てを得る．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::get_assignment(const SmtLibNode* arg_top)
{
  // 未完
  return true;
}

// @brief オプションを得る．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::get_option(const SmtLibNode* arg_top)
{
  // 未完
  return true;
}

// @brief 情報を得る．
// @param[in] arg_top 引数の先頭ノード
bool
ShellImpl::get_info(const SmtLibNode* arg_top)
{
  // 未完
  return true;
}

// @brief 識別子を返す．
// @param[in] name 名前
// @param[in] index_list インデックスのリスト
// @return 同じ識別子があればそれを返す．なければ作る．
//
// この関数は通常は成功するはず．
const SmtId*
ShellImpl::make_id(const ShString& name,
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
ShellImpl::make_sort(const SmtId* name_id,
		     const vector<const SmtSort*>& elem_list)
{
  return sort_mgr().make_sort(name_id, elem_list);
}

// @brief S式を識別子に変換する．
// @param[in] node S式を表すノード
const SmtId*
ShellImpl::eval_to_id(const SmtLibNode* node)
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

  return mIdMgr->make_id(name, index_list);

 syntax_error:
  return NULL;
}

// @brief S式を sort に変換する．
// @param[in] node S式を表すノード
const SmtSort*
ShellImpl::eval_to_sort(const SmtLibNode* node)
{
  const SmtId* id = eval_to_id(node);
  vector<const SmtSort*> elem_list;
  const SmtSort* sort = NULL;

  if ( id == NULL ) {
    if ( node->type() != kListToken ) {
      mErrBuf << "syntax error : " << node->loc();
      goto syntax_error;
    }

    elem_list.reserve(node->child_num() - 1);

    const SmtLibNode* node1 = node->child();
    id = eval_to_id(node1);
    if ( id == NULL ) {
      mErrBuf << "not an identifier: " << node1->loc();
      goto syntax_error;
    }

    for (const SmtLibNode* node2 = node1->sibling();
	 node2 != NULL; node2 = node2->sibling()) {
      const SmtSort* sort1 = eval_to_sort(node2);
      if ( sort1 == NULL ) {
	mErrBuf << "not a registered sort: " << node2->loc();
	goto syntax_error;
      }
      elem_list.push_back(sort1);
    }
    if ( elem_list.empty() ) {
      mErrBuf << "at least one sort required: " << node1->loc();
      goto syntax_error;
    }
  }

  sort = make_sort(id, elem_list);
  if ( sort == NULL ) {
    mErrBuf << id->name() << ": not registered";
    goto syntax_error;
  }
  return sort;

 syntax_error:
  if ( debug ) {
    cerr << "eval_to_sort failed: " << mErrBuf.str() << endl;
  }

  return NULL;
}

// @brief S式を sort に変換する．
// @param[in] node S式を表すノード
const SortElem*
ShellImpl::eval_to_sort_template(const SmtLibNode* node,
				 const vector<const SmtId*>& param_list)
{
  const SmtId* id = eval_to_id(node);
  vector<const SortElem*> elem_list;
  if ( id != NULL ) {
    ymuint n = param_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      if ( id == param_list[i] ) {
	return sort_mgr().make_param_sort_templ(i);
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
      const SortElem* sort1 = eval_to_sort_template(node2, param_list);
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

  return sort_mgr().make_complex_sort_templ(id, elem_list);

 syntax_error:

  return NULL;
}

// @brief 名前から変数か関数を探す
// @param[in] name_id 名前を表す識別子
// @return 指定された名前の変数または関数を返す．
// @note エラーの場合には NULL を返す．
//
// エラーの原因は以下のとおり
//  - name_id という名の関数が登録されていなかった．
const NameObj*
ShellImpl::find_obj(const SmtId* name_id)
{
  return name_mgr().find_obj(name_id);
}

// @brief S式を term に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
ShellImpl::eval_to_term(const SmtLibNode* node)
{
  // <term> ::= <spec_constant> |
  //            <qual_identifier> |
  //            '(' <qual_identifier> <term>+ ')' |
  //            '(' 'let' '(' <var_binding>+ ')' <term> ')' |
  //            '(' 'forall' '(' <sorted_var>+ ')' <term> ')' |
  //            '(' 'exists' '(' <sorted_var>+ ')' <term> ')' |
  //            '(' '!' <term> <attribute>+ ')'
  //
  // <spec_constant> ::= <numeral> | <decimal> | <hexadecimal> | <binary> | <string>
  //
  // <qual_identifier> ::= <identifier> | '(' 'as' <identifier> <sort> ')'
  //
  // <identifier> ::= <symbol> | '(' '_' <symbol> <numeral>+ ')'
  //
  // <var_binding> ::= '(' <symbol> <term> ')'
  //
  // <sorted_var> ::= '(' <symbol> <sort> ')'

  switch ( node->type() ) {
  case kNumToken:
    // numeric
    return mSolver->make_numeric_term(node->int_value());

  case kDecToken:
    // decimal
    return mSolver->make_decimal_term(node->str_value());

  case kHexToken:
    // hexadecimal
    return mSolver->make_hexadecimal_term(node->str_value());

  case kBinToken:
    // binary
    return mSolver->make_binary_term(node->str_value());

  case kStringToken:
    // string
    return mSolver->make_string_term(node->str_value());

  case kSymbolToken:
    {
      ShString name = node->str_value();
      const SmtId* id = mIdMgr->make_id(name);
      // id が置き換え対象ならそれを返す．

      const NameObj* obj = find_obj(id);
      if ( obj != NULL ) {
	if ( obj->is_var() ) {
	  return mSolver->make_var_term(obj->var());
	}
	else {
	  return mSolver->make_fun_term(obj->fun(), vector<const SmtTerm*>(0));
	}
      }
      else {
	const SmtVar* var = mSolver->make_var(NULL);
	name_mgr().reg_var(id, var);
	return mSolver->make_var_term(var);
      }
    }

  case kListToken:
    break;

  default:
    mErrBuf << "syntax error at " << node->loc();
    goto syntax_error;
  }

  // ここに来ているということは node->type() == kListToken
  {
    const SmtLibNode* node1 = node->child();
    assert_cond( node1 != NULL, __FILE__, __LINE__);

    // qual_identifier の場合
    const SmtTerm* term = eval_to_qid(node);
    if ( term != NULL ) {
      return term;
    }
  }

  // 残りは ( identifier term+ ) の形
  if ( node->type() != kListToken ) {
    mErrBuf << "syntax error at " << node->loc();
    goto syntax_error;
  }
  else {
    ymuint n = node->child_num();
    const SmtLibNode* node1 = node->child();
    if ( node1->type() == kSymbolToken ) {
      const char* name = static_cast<const char*>(node1->str_value());
      if ( strcmp("let", name) == 0 ) {
	return eval_to_let(node1->sibling());
      }
      else if ( strcmp("forall", name) == 0 ) {
	return eval_to_forall(node1->sibling());
      }
      else if ( strcmp("exists", name) == 0 ) {
	return eval_to_exists(node1->sibling());
      }
      else if ( strcmp("!", name) == 0 ) {
	return eval_to_attr_term(node1->sibling());
      }
    }
    const SmtId* fid = eval_to_id(node1);
    if ( fid == NULL ) {
      mErrBuf << "not an identifier: " << node1->loc();
      goto syntax_error;
    }
    else {
      const NameObj* obj = find_obj(fid);
      if ( obj != NULL ) {
	if ( !obj->is_fun() ) {
	  mErrBuf << obj->name()->name() << " is not a function";
	  goto syntax_error;
	}
	const SmtFun* fun = obj->fun();
	if ( fun->attr() == SmtFun::kNone && fun->input_num() != n - 1) {
	  // 引数の数が合わない．
	  mErrBuf << "# of args for function '" << obj->name()->name()
		  << "' mismatch: " << node->loc();
	  mErrBuf << " fun->input_num() = " << fun->input_num()
		  << ", n = " << n;
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
	return mSolver->make_fun_term(fun, input_list);
      }
      else {
	mErrBuf << fid->name() << ": undefined: " << node1->loc();
	goto syntax_error;
      }
    }
  }

 syntax_error:
  if ( debug ) {
    cerr << mErrBuf.str() << endl;
  }
  return NULL;
}

// @brief let 文の処理を行なう．
// @param[in] node 引数の先頭ノード
const SmtTerm*
ShellImpl::eval_to_let(const SmtLibNode* node)
{
  vector<const SmtLibNode*> arg_list(2);
  if ( !parse_args(node, 2, arg_list) ) {
    mErrBuf << "# of args for 'let' mismatch: " << node->loc();
    return NULL;
  }

  const SmtLibNode* node2 = arg_list[0];
  if ( node2->type() != kListToken ) {
    mErrBuf << "'list' type expected: " << node2->loc();
    return NULL;
  }

  hash_map<ymuint32, const SmtTerm*> var_binding_map;
  for (const SmtLibNode* node21 = node2->child();
       node21 != NULL; node21 = node21->sibling()) {
    ymuint32 id;
    const SmtTerm* term;
    if ( !eval_to_var_binding(node21, id, term) ) {
      // エラーメッセージは eval_to_var_binding() 中で出力されているはず
      return NULL;
    }
    var_binding_map.insert(make_pair(id, term));
  }

  const SmtLibNode* node3 = arg_list[1];
  const SmtTerm* body = eval_to_term(node3);
  if ( body == NULL ) {
    // エラーメッセージは下位の呼び出しで出力されているはず
    return NULL;
  }
  return body;
}

// @brief forall 文の処理を行なう．
// @param[in] node 引数の先頭ノード
const SmtTerm*
ShellImpl::eval_to_forall(const SmtLibNode* node)
{
  vector<const SmtLibNode*> arg_list(2);
  if ( !parse_args(node, 2, arg_list) ) {
    mErrBuf << "# of args for 'forall' mismatch: " << node->loc();
    return NULL;
  }

  const SmtLibNode* node2 = arg_list[0];
  if ( node2->type() != kListToken ) {
    mErrBuf << "'list' type expected: " << node2->loc();
    return NULL;
  }

  vector<const SmtVar*> var_list;
  var_list.reserve(node2->child_num());
  for (const SmtLibNode* node21 = node2->child();
       node21 != NULL; node21 = node21->sibling()) {
    const SmtVar* var = eval_to_sorted_var(node21);
    if ( var == NULL ) {
      // エラーメッセージは eval_to_sorted_var() 中で出力されているはず
      return NULL;
    }
    var_list.push_back(var);
  }

  const SmtLibNode* node3 = arg_list[1];
  const SmtTerm* body = eval_to_term(node3);
  if ( body == NULL ) {
    // エラーメッセージは下位の呼び出しで出力されているはず
    return NULL;
  }
  return mSolver->make_forall_term(var_list, body);
}

// @brief exists 文の処理を行なう．
// @param[in] node 引数の先頭ノード
const SmtTerm*
ShellImpl::eval_to_exists(const SmtLibNode* node)
{
  vector<const SmtLibNode*> arg_list(2);
  if ( !parse_args(node, 2, arg_list) ) {
    mErrBuf << "# of args for 'exists' mismatch: " << node->loc();
    return NULL;
  }

  const SmtLibNode* node2 = arg_list[0];
  if ( node2->type() != kListToken ) {
    mErrBuf << "'list' type expected: " << node2->loc();
    return NULL;
  }

  vector<const SmtVar*> var_list;
  var_list.reserve(node2->child_num());
  for (const SmtLibNode* node21 = node2->child();
       node21 != NULL; node21 = node21->sibling()) {
    const SmtVar* var = eval_to_sorted_var(node21);
    if ( var == NULL ) {
      // エラーメッセージは eval_to_sorted_var() 中で出力されているはず
      return NULL;
    }
    var_list.push_back(var);
  }

  const SmtLibNode* node3 = arg_list[1];
  const SmtTerm* body = eval_to_term(node3);
  if ( body == NULL ) {
    // エラーメッセージは下位の呼び出しで出力されているはず
    return NULL;
  }
  return mSolver->make_exists_term(var_list, body);
}

// @brief attr 文の処理を行なう．
// @param[in] node 引数の先頭ノード
const SmtTerm*
ShellImpl::eval_to_attr_term(const SmtLibNode* node)
{
  const SmtTerm* body = eval_to_term(node);
  if ( body == NULL ) {
    // エラーメッセージは下位の呼び出しで出力されているはず
    return NULL;
  }

  vector<SmtAttr> attr_list;
  for (const SmtLibNode* node1 = node->sibling();
       node1 != NULL; node1 = node1->sibling()) {
    if ( node1->type() != kKeywordToken ) {
      mErrBuf << "syntax error: keyword expected at " << node1->loc();
      return NULL;
    }
    const SmtLibNode* node2 = node1->sibling();
    if ( node2 != NULL && node2->type() != kKeywordToken ) {
      const SmtTerm* expr = eval_to_expr(node2);
      if ( expr == NULL ) {
	mErrBuf << "syntax error: s-expression expected at " << node2->loc();
	return NULL;
      }
#if 0
      ShString tmp_str(term_str(expr));
      attr_list.push_back(SmtAttr(node1->str_value(), tmp_str));
#endif
      node1 = node2;
    }
    else {
      attr_list.push_back(SmtAttr(node1->str_value()));
    }
  }
  return mSolver->make_annotated_term(body, attr_list);
}

// @brief S式を s-expr に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
ShellImpl::eval_to_expr(const SmtLibNode* node)
{
  switch ( node->type() ) {
  case kNumToken:
    return mSolver->make_numeric_term(node->int_value());

  case kDecToken:
    return mSolver->make_decimal_term(node->str_value());

  case kHexToken:
    return mSolver->make_hexadecimal_term(node->str_value());

  case kBinToken:
    return mSolver->make_binary_term(node->str_value());

  case kStringToken:
    return mSolver->make_string_term(node->str_value());

#if 0
  case kSymbolToken:
    return mSolver->make_symbol_term(node->str_value());

  case kKeywordToken:
    return mSolver->make_keyword_term(node->str_value());

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
      return mSolver->make_list_term(expr_list);
    }
#endif

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return NULL;
}

// @brief S式を qual_identifier に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
ShellImpl::eval_to_qid(const SmtLibNode* node)
{
  // ( 'as' <identifier> <sort> ) の形かどうか調べる．
  vector<const SmtLibNode*> arg_list;
  if ( !parse_args(node, 3, arg_list) ) {
    return NULL;
  }

  if ( arg_list[0]->type() != kSymbolToken ||
       strcmp("as", static_cast<const char*>(arg_list[0]->str_value())) != 0 ) {
    return NULL;
  }

  const SmtId* id = eval_to_id(arg_list[1]);
  if ( id == NULL ) {
    return NULL;
  }

  const SmtSort* sort = eval_to_sort(arg_list[2]);
  if ( sort == NULL ) {
    return NULL;
  }

  const SmtVar* obj = mSolver->make_var(sort);
  if ( obj == NULL ) {
    return NULL;
  }
  return mSolver->make_var_term(obj);
}

// @brief S式を sorted_var に変換する．
// @param[in] node S式を表すノード
const SmtVar*
ShellImpl::eval_to_sorted_var(const SmtLibNode* node)
{
  // ( <identifier> <sort> ) の形
  vector<const SmtLibNode*> arg_list;
  const SmtId* id = NULL;
  const SmtSort* sort = NULL;

  if ( !parse_args(node->child(), 2, arg_list) ) {
    mErrBuf << "syntax error: " << node->loc();
    goto syntax_error;
  }

  id = eval_to_id(arg_list[0]);
  if ( id == NULL ) {
    mErrBuf << "not a valid identifier: " << arg_list[0]->loc();
    goto syntax_error;
  }

  sort = eval_to_sort(arg_list[1]);
  if ( sort == NULL ) {
    return false;
  }

  return mSolver->make_var(sort);

 syntax_error:
  if ( debug ) {
    cerr << "eval_to_sorted_var() failed: " << mErrBuf.str() << endl;
  }

  return false;
}

// @brief S式を var_binding に変換する．
// @param[in] node S式を表すノード
// @param[out] id 変数のID番号
// @param[out] term 置き換える式
// @retval true 変換が成功した．
// @retval false 変換が失敗した．
bool
ShellImpl::eval_to_var_binding(const SmtLibNode* node,
			       ymuint32& id,
			       const SmtTerm*& term)

{
  // ( <identifier> <term> ) の形
  vector<const SmtLibNode*> arg_list;
  if ( node->type() != kListToken ) {
    mErrBuf << "syntax error: " << node->loc();
    goto syntax_error;
  }
  if ( !parse_args(node->child(), 2, arg_list) ) {
    mErrBuf << "syntax error: " << node->loc();
    goto syntax_error;
  }

  {
    const SmtId* id_obj = eval_to_id(arg_list[0]);
    if ( id_obj == NULL ) {
      mErrBuf << "not a valid identifier: " << arg_list[0]->loc();
      goto syntax_error;
    }
    id = id_obj->id();
  }

  term = eval_to_term(arg_list[1]);
  if ( term == NULL ) {
    // エラーメッセージは eval_to_term() 中で出力されている．
    return false;
  }

  return true;

 syntax_error:
  if ( debug ) {
    cerr << "eval_to_var_binding() failed: " << mErrBuf.str() << endl;
  }

  return false;
}

// @brief 引数のリストをパーズする．
// @param[in] arg_top 引数の先頭のノード
// @param[in] arg_num 引数の数
// @param[out] arg_list 引数を格納するリスト
// @retval true 引数の数が arg_num と一致した．
// @retval false 引数の数が arg_num と一致しなかった．
bool
ShellImpl::parse_args(const SmtLibNode*  arg_top,
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

// @brief 現在の SortMgr を返す．
SortMgr&
ShellImpl::sort_mgr()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mSortMgr;
}

// @brief 現在の NameMgr を返す．
NameMgr&
ShellImpl::name_mgr()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mNameMgr;
}

// @brief 現在のアロケータを返す．
Alloc&
ShellImpl::alloc()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mAlloc;
}

// @brief 現在の assertion リストを返す．
vector<const SmtTerm*>&
ShellImpl::assertion_list()
{
  assert_cond( !mStack.empty(), __FILE__, __LINE__);
  return mStack.back()->mTermList;
}

END_NAMESPACE_YM_SMTLIBV2

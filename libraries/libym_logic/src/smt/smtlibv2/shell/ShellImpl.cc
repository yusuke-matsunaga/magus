
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

#include "YmLogic/SmtTerm.h"
#include "YmLogic/SmtVar.h"

#include "SmtId.h"

#if defined(READLINE_FOUND)
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "YmUtils/StrListIDO.h"


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

  mCurNameMgr = &page0->mNameMgr;

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
  mParser->init(ido);

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
    INTS_init();
  }
  else if ( strcmp(str, "AUFLIRA") == 0 ) {
    logic = kSmtLogic_AUFLIRA;
    CORE_init();
    INTS_init();
  }
  else if ( strcmp(str, "AUFNIRA") == 0 ) {
    logic = kSmtLogic_AUFNIRA;
    CORE_init();
    INTS_init();
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
    INTS_init();
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
    INTS_init();
  }
  else if ( strcmp(str, "QF_LRA") == 0 ) {
    logic = kSmtLogic_QF_LRA;
    CORE_init();
  }
  else if ( strcmp(str, "QF_NIA") == 0 ) {
    logic = kSmtLogic_QF_NIA;
    CORE_init();
    INTS_init();
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
    INTS_init();
  }
  else if ( strcmp(str, "QF_UFLIA") == 0 ) {
    logic = kSmtLogic_QF_UFLIA;
    CORE_init();
    INTS_init();
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
    INTS_init();
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
  bind_builtin_sort("Bool", kSmtSort_Bool);

  // :funs
  // (true Bool)
  bind_builtin_fun("true", kSmtFun_True);

  // (false Bool)
  bind_builtin_fun("false", kSmtFun_False);

  // (not Bool Bool)
  bind_builtin_fun("not", kSmtFun_Not);

  // (and Bool Bool Bool :right-assoc)
  bind_builtin_fun("and", kSmtFun_And);

  // (or Bool Bool Bool :right-assoc)
  bind_builtin_fun("or", kSmtFun_Or);

  // (xor Bool Bool Bool :right-assoc)
  bind_builtin_fun("xor", kSmtFun_Or);

  // (=> Bool Bool Bool :right-assoc)
  bind_builtin_fun("=>", kSmtFun_Or);

  // (par (A) (= A A Bool :chainable))
  bind_builtin_fun("=", kSmtFun_Eq);

  // (par (A) (distinct A A Bool :pairwise))
  bind_builtin_fun("distinct", kSmtFun_Diseq);

  // (par (A) (ite Bool A A A)
  bind_builtin_fun("ite", kSmtFun_Ite);
}

// @brief INTS theory の初期化を行う．
void
ShellImpl::INTS_init()
{
  // :sorts
  // (Int 0)
  bind_builtin_sort("Int", kSmtSort_Int);

  // :funs

  // (- Int Int)
  //bind_builtin_fun("-", kSmtFun_Uminus, i1_list);

  // (- Int Int Int :left-assoc)
  bind_builtin_fun("-", kSmtFun_Sub);

  // (+ Int Int Int :left-assoc)
  bind_builtin_fun("+", kSmtFun_Add);

  // (* Int Int Int :left-assoc)
  bind_builtin_fun("*", kSmtFun_Mul);

  // (<= Int Int Bool :chainable)
  bind_builtin_fun("<=", kSmtFun_Le);

  // (< Int Int Bool :chainable)
  bind_builtin_fun("<", kSmtFun_Lt);

  // (>= Int Int Bool :chainable)
  bind_builtin_fun(">=", kSmtFun_Ge);

  // (> Int Int Bool :chainable)
  bind_builtin_fun(">", kSmtFun_Gt);
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

  const SmtLibNode* node2 = arg_top->sibling();
  if ( node2 != NULL ) {
    if ( node2->sibling() != NULL ) {
      mErrBuf << "syntax error: too many tokens at " << node2->sibling()->loc();
      return false;
    }
  }

  const char* keyword = arg_top->str_value();
  if ( strcmp(keyword, "") == 0 ) {
  }
  else if ( strcmp(keyword, "") == 0 ) {
  }

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
  const SmtId* name = eval_as_id(arg_list[0]);
  if ( name == NULL ) {
    mErrBuf << "first argument is not an indentifier";
    return false;
  }

  // 2つめは引数の数
  if ( arg_list[1]->type() != kNumToken ) {
    mErrBuf << "second argument is not a numeral";
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
  const SmtId* name = eval_as_id(arg_list[0]);
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
    const SmtId* id = eval_as_id(node1);
    if ( id == NULL ) {
      mErrBuf << "second argument is not a list of identifiers";
      return false;
    }
    param_list.push_back(id);
  }

  // 3つめは型テンプレート
  const SortElem* sort = eval_as_sort_template(arg_list[2], param_list);
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
  const SmtId* name = eval_as_id(arg_list[0]);
  if ( name == NULL ) {
    mErrBuf << "first argument is not an identifier";
    return false;
  }

  // 2つめは入力の型のリスト
  if ( arg_list[1]->type() != kListToken ) {
    mErrBuf << "second argument is not 'list' type";
    return false;
  }

  vector<tSmtSortId> input_sort_list;
  input_sort_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node = arg_list[1]->child();
       node != NULL; node = node->sibling()) {
    tSmtSortId sort = eval_as_sort(node);
    if ( sort == kSmtSort_None ) {
      print(mErrBuf, node);
      mErrBuf << " is not a registered sort";
      return false;
    }
    input_sort_list.push_back(sort);
  }

  // 3つめは出力の型のリスト
  tSmtSortId output_sort = eval_as_sort(arg_list[2]);
  if ( output_sort == kSmtSort_None ) {
    mErrBuf << "third argument is not a valid sort";
    return false;
  }

  // 同名のオブジェクトが登録されていないか調べる．
  if ( check_obj(name) ) {
    mErrBuf << name->name() << ": already declared";
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
  const SmtId* name = eval_as_id(arg_list[0]);
  if ( name == NULL ) {
    mErrBuf << "first argument is not an identifier";
    return false;
  }

  // 2つめは変数のリスト
  if ( arg_list[1]->type() != kListToken ) {
    mErrBuf <<  "second argument is not 'list' type";
    return false;
  }

  // 関数本体の式中に現れる関数の引数を置き換えるための辞書
  SimpleAlloc alloc(1024);
  NameMgr temp_name_mgr(alloc, 0, mCurNameMgr);

  vector<const SmtVar*> var_list;
  var_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node = arg_list[1]->child();
       node != NULL; node = node->sibling()) {
    const SmtId* id;
    tSmtSortId sort;
    if ( !eval_as_sorted_var(node, id, sort) ) {
      return false;
    }
    const SmtVar* var = mSolver->make_var(sort, kSmtVar_FunArg);

    temp_name_mgr.reg_var(id, var);
    var_list.push_back(var);
  }

  // 3つめは出力の型
  tSmtSortId output_sort = eval_as_sort(arg_list[2]);
  if ( output_sort == kSmtSort_None ) {
    mErrBuf << "third argument is not a valid sort";
    return false;
  }

  // 新しい辞書をスタックに積む．
  NameMgr* old_name_mgr = mCurNameMgr;
  mCurNameMgr = &temp_name_mgr;

  // 4つめは本体の式
  const SmtTerm* body = eval_as_term(arg_list[3], output_sort);

  // 新しい辞書をスタックから取り除く
  // temp_name_mgr は自動変数なので自動的に開放される．
  mCurNameMgr = old_name_mgr;

  if ( body == NULL ) {
    mErrBuf << "forth argument is not a valid term";
    return false;
  }

  // 同名のオブジェクトが存在しないか確かめる．
  if ( check_obj(name) ) {
    mErrBuf << name->name() << ": already exists";
    return false;
  }

  // 関数を作る．
  const SmtFun* fun = mSolver->make_fun(var_list, body);
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

  const SmtTerm* term = eval_as_term(arg_top, kSmtSort_Bool);
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
  mCurNameMgr = &mStack.back()->mNameMgr;

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
  mCurNameMgr = &mStack.back()->mNameMgr;

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
tSmtSortId
ShellImpl::make_sort(const SmtId* name_id,
		     const vector<tSmtSortId>& elem_list)
{
  tSmtSortId ans;
  if ( mBuiltinSortMap.find(name_id->id(), ans) ) {
    return ans;
  }
  else {
    return sort_mgr().make_sort(name_id, elem_list);
  }
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

// @brief name_id という名前が登録されているか調べる．
bool
ShellImpl::check_obj(const SmtId* name_id)
{
  if ( find_obj(name_id) != NULL ) {
    return true;
  }

  return false;
}

// @brief S式を識別子に変換する．
// @param[in] node S式を表すノード
const SmtId*
ShellImpl::eval_as_id(const SmtLibNode* node)
{
  // - <symbol>
  // - ( '_' <symbol> <numeral>+ )
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
      return NULL;
    }

    // 2番めが名前
    const SmtLibNode* node2 = node1->sibling();
    if ( node2 == NULL ||
	 node2->type() != kSymbolToken ) {
      // syntax error
      return NULL;
    }
    name = node2->str_value();

    // 3番め以降にインデックス
    index_list.reserve(node->child_num() - 2);
    for (const SmtLibNode* node3 = node2->sibling();
	 node3 != NULL; node3 = node3->sibling()) {
      if ( node3->type() != kNumToken ) {
	// syntax error
	return NULL;
      }
      index_list.push_back(node3->int_value());
    }
    if ( index_list.empty() ) {
      // syntax error
      return NULL;
    }
  }

  return mIdMgr->make_id(name, index_list);
}

// @brief S式を sort に変換する．
// @param[in] node S式を表すノード
// @note エラーが起こったら mErrBuf にエラーメッセージを出力して NULL を返す．
tSmtSortId
ShellImpl::eval_as_sort(const SmtLibNode* node)
{
  // - <identifier>
  // - ( <identifier> <sort>+ )
  const SmtId* id = eval_as_id(node);
  vector<tSmtSortId> elem_list;
  if ( id == NULL ) {
    if ( node->type() != kListToken ) {
      mErrBuf << "syntax error : " << node->loc();
      return kSmtSort_None;
    }

    elem_list.reserve(node->child_num() - 1);

    const SmtLibNode* node1 = node->child();
    id = eval_as_id(node1);
    if ( id == NULL ) {
      mErrBuf << "expected identifier at " << node1->loc();
      return kSmtSort_None;
    }

    for (const SmtLibNode* node2 = node1->sibling();
	 node2 != NULL; node2 = node2->sibling()) {
      tSmtSortId sort1 = eval_as_sort(node2);
      if ( sort1 == kSmtSort_None ) {
	mErrBuf << "not a registered sort: " << node2->loc();
	return kSmtSort_None;
      }
      elem_list.push_back(sort1);
    }
    if ( elem_list.empty() ) {
      mErrBuf << "at least one sort required: " << node1->loc();
      return kSmtSort_None;
    }
  }

  tSmtSortId sort = make_sort(id, elem_list);
  if ( sort == kSmtSort_None ) {
    mErrBuf << id->name() << ": not registered";
  }
  return sort;
}

// @brief S式を sort に変換する．
// @param[in] node S式を表すノード
// @param[in] param_list パラメータを表す ID のリスト
// @note param_list[i] に一致するIDは i 番めのパラメータに置き換えられる．
// @note エラーが起こったら mErrBuf にエラーメッセージを出力して NULL を返す．
const SortElem*
ShellImpl::eval_as_sort_template(const SmtLibNode* node,
				 const vector<const SmtId*>& param_list)
{
  const SmtId* id = eval_as_id(node);
  vector<const SortElem*> elem_list;
  if ( id != NULL ) {
    // 単純な線形探索を用いている．
    ymuint n = param_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      if ( id == param_list[i] ) {
	return sort_mgr().make_param_sort_templ(i);
      }
    }
    // 最後のmake_sort_templ() に続く
  }
  else {
    if ( node->type() != kListToken ) {
      // syntax error
      return NULL;
    }

    elem_list.reserve(node->child_num() - 1);

    const SmtLibNode* node1 = node->child();
    id = eval_as_id(node1);
    if ( id == NULL ) {
      // syntax error
      return NULL;
    }

    for (const SmtLibNode* node2 = node1->sibling();
	 node2 != NULL; node2 = node2->sibling()) {
      const SortElem* sort1 = eval_as_sort_template(node2, param_list);
      if ( sort1 == NULL ) {
	// syntax error
	return NULL;
      }
      elem_list.push_back(sort1);
    }
    if ( elem_list.empty() ) {
      // syntax error
      return NULL;
    }
  }

  return sort_mgr().make_sort_templ(id, elem_list);
}

BEGIN_NONAMESPACE

// 組み込み関数の入力の型を返す．
tSmtSortId
funtype_to_input_sort(tSmtFunType fun_type,
		      ymuint pos)
{
  switch ( fun_type ) {
  case kSmtFun_True:
  case kSmtFun_False:
    // 引数なしなのでエラー
    return kSmtSort_None;

  case kSmtFun_Not:
    // 引数は一つで Bool 型
    if ( pos == 0 ) {
      return kSmtSort_Bool;
    }
    return kSmtSort_None;

  case kSmtFun_And:
  case kSmtFun_Or:
  case kSmtFun_Xor:
  case kSmtFun_Imp:
    // 引数は2つ以上で Bool 型
    return kSmtSort_Bool;

  case kSmtFun_Eq:
  case kSmtFun_Diseq:
    // 引数は2つ以上で任意の型
    return kSmtSort_Any;

  case kSmtFun_Ite:
    // 引数は3つで，最初が Bool，残り2つは任意
    if ( pos == 0 ) {
      return kSmtSort_Bool;
    }
    if ( pos == 1 || pos == 2 ) {
      return kSmtSort_Any;
    }
    return kSmtSort_None;

  case kSmtFun_Uminus:
    // 引数は1つで数値型
    if ( pos == 0 ) {
      return kSmtSort_Num;
    }
    return kSmtSort_None;

  case kSmtFun_Add:
  case kSmtFun_Sub:
  case kSmtFun_Mul:
  case kSmtFun_Le:
  case kSmtFun_Lt:
  case kSmtFun_Ge:
  case kSmtFun_Gt:
    // 引数は2つ以上で数値型
    return kSmtSort_Num;

  case kSmtFun_Div:
    // 引数は2つ以上で実数型
    return kSmtSort_Real;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return kSmtSort_None;
}

// 2つの型が等しいかチェックする．
inline
bool
check_sort(tSmtSortId sort,
	   tSmtSortId req_sort)
{
  if ( req_sort == kSmtSort_Any ) {
    // 任意の型ならいつでもOK
    return true;
  }
  if ( req_sort == kSmtSort_Num ) {
    // 数値型は Int か Real
    return sort == kSmtSort_Int || sort == kSmtSort_Real;
  }
  // 残りは本当に等しいか調べる．
  return sort == req_sort;
}

END_NONAMESPACE

// @brief S式を term に変換する．
// @param[in] node S式を表すノード
// @param[in] req_sort 要求される型
// @note エラーが起こったら mErrBuf にエラーメッセージを出力して NULL を返す．
const SmtTerm*
ShellImpl::eval_as_term(const SmtLibNode* node,
			tSmtSortId req_sort)
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
    // numeral
    return mSolver->make_numeral_term(node->int_value());

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
      assert_cond( id != NULL, __FILE__, __LINE__);

      const NameObj* obj = find_obj(id);
      if ( obj != NULL ) {
	if ( obj->is_var() ) {
	  // id が変数だった．
	  const SmtVar* var = obj->var();
	  if ( var->sort() != kSmtSort_None ) {
	    if ( !check_sort(var->sort(), req_sort) ) {
	      // 型が合わない．
	      mErrBuf << "sort type mismatch at " << node->loc();
	      return NULL;
	    }
	  }
	  else {
	    if ( req_sort != kSmtSort_Any && req_sort != kSmtSort_Num ) {
	      // var の型を設定する．
	      //var->set_sort(req_sort);
	    }
	  }
	  return mSolver->make_var_term(obj->var());
	}
	else if ( obj->is_fun() ) {
	  // id が関数だった．
	  const SmtFun* fun = obj->fun();
	  if ( !check_sort(fun->output_sort(), req_sort) ) {
	    // 型が合わない．
	    mErrBuf << "sort type mismatch at " << node->loc();
	    return NULL;
	  }
	  return mSolver->make_fun_term(fun, vector<const SmtTerm*>(0));
	}
	else if ( obj->is_builtin_fun() ) {
	  // id が組み込み関数だった．(引数なし)
	  // 今のところ true/false しかないはず．
	  tSmtFunType fun_type = obj->fun_type();
	  if ( !check_sort(kSmtSort_Bool, req_sort) ) {
	    // 型が合わない．
	    mErrBuf << "sort type mismatch at " << node->loc();
	    return NULL;
	  }
	  return mSolver->make_fun_term(fun_type, vector<const SmtTerm*>(0));
	}
	else if ( obj->is_term() ) {
	  // id が式だった．(let 文の置き換え)
	  const SmtTerm* term = obj->term();
	  if ( !check_sort(term->sort(), req_sort) ) {
	    // 型が合わない．
	    mErrBuf << "sort type mismatch at " << node->loc();
	    return NULL;
	  }
	  return term;
	}
      }
      else {
	// 未定義だったので変数を作る．
	if ( req_sort == kSmtSort_Any ||
	     req_sort == kSmtSort_Num ) {
	  cerr << "Warning: " << name << "'s sort is undetermined" << endl;
	}
	const SmtVar* var = mSolver->make_var(req_sort);
	name_mgr().reg_var(id, var);
	return mSolver->make_var_term(var);
      }
    }

  case kListToken:
    // あとで処理する．
    break;

  default:
    mErrBuf << "syntax error at " << node->loc();
    return NULL;
  }

  // ここに来ているということは node->type() == kListToken
  // ありうるパタンは以下のとおり
  //  - ( 'as' <identifier> <sort> ) : <qual_identifier>
  //  - ( 'let' (<var_binding>+) <term> )
  //  - ( 'forall' (<sorted_var>+) <term> )
  //  - ( 'exists' (<sorted_var>+) <term> )
  //  - ( <identifier> <term>+ ) : <identifier> は関数名．なので <qual_identifier> ではないだろう．
  const SmtLibNode* node1 = node->child();
  if ( node1->type() == kSymbolToken ) {
    const char* name = static_cast<const char*>(node1->str_value());
    if ( strcmp("as", name) == 0 ) {
      return eval_as_qual_id(node1->sibling(), req_sort);
    }
    if ( strcmp("let", name) == 0 ) {
      return eval_as_let(node1->sibling(), req_sort);
    }
    else if ( strcmp("forall", name) == 0 ) {
      return eval_as_forall(node1->sibling(), req_sort);
    }
    else if ( strcmp("exists", name) == 0 ) {
      return eval_as_exists(node1->sibling(), req_sort);
    }
    else if ( strcmp("!", name) == 0 ) {
      return eval_as_attr_term(node1->sibling(), req_sort);
    }
  }

  const SmtId* fid = eval_as_id(node1);
  if ( fid == NULL ) {
    mErrBuf << "expected identifier at " << node1->loc();
    return NULL;
  }

  const NameObj* obj = find_obj(fid);
  if ( obj == NULL ) {
    mErrBuf << fid->name() << " : No such function";
    return NULL;
  }

  if ( !obj->is_builtin_fun() && !obj->is_fun() ) {
  }

  ymuint n = node->child_num();
  vector<const SmtTerm*> input_list;
  input_list.reserve(n - 1);

  if ( obj->is_fun() ) {
    const SmtFun* fun = obj->fun();
    ymuint input_num = fun->input_num();
    if ( input_num != n - 1 ) {
      // 引数の数が合わない．
      mErrBuf << "# of args for " << obj->name()->name() << " mismatch at " << node->loc();
      return NULL;
    }
    for (const SmtLibNode* node2 = node1->sibling();
	 node2 != NULL; node2 = node2->sibling()) {
      const SmtTerm* term1 = eval_as_term(node2, fun->input_sort(input_list.size()));
      if ( term1 == NULL ) {
	// エラーメッセージは下位の呼び出しで出力されているはず
	return NULL;
      }
      input_list.push_back(term1);
    }

    return mSolver->make_fun_term(fun, input_list);
  }

  if ( obj->is_builtin_fun() ) {
    tSmtFunType fun_type = obj->fun_type();
    for (const SmtLibNode* node2 = node1->sibling();
	 node2 != NULL; node2 = node2->sibling()) {
      tSmtSortId input_sort = funtype_to_input_sort(fun_type, input_list.size());
      const SmtTerm* term1 = eval_as_term(node2, input_sort);
      if ( term1 == NULL ) {
	// エラーメッセージは下位の呼び出しで出力されているはず
	return NULL;
      }
      input_list.push_back(term1);
    }
    return mSolver->make_fun_term(fun_type, input_list);
  }

  // obj はユーザー定義関数でも組み込み関数でもなかった．
  mErrBuf << obj->name()->name() << " is not a function";
  return NULL;
}

// @brief S式を qual_identifier に変換する．
// @param[in] node S式を表すノード
// @param[in] req_sort 要求される型
// @note エラーが起こったら mErrBuf にエラーメッセージを出力して NULL を返す．
const SmtTerm*
ShellImpl::eval_as_qual_id(const SmtLibNode* node,
			   tSmtSortId req_sort)
{
  vector<const SmtLibNode*> arg_list(2);
  if ( !parse_args(node, 2, arg_list) ) {
    mErrBuf << "# of args for 'as' mismatch: " << node->loc();
    return NULL;
  }

  const SmtId* id = eval_as_id(arg_list[0]);
  if ( id == NULL ) {
    // identifier の形式ではなかった．
    mErrBuf << "not a valid identifier: " << arg_list[0]->loc();
    return NULL;
  }

  tSmtSortId sort = eval_as_sort(arg_list[1]);
  if ( sort == kSmtSort_None ) {
    // 型名が未定義だった．
    return NULL;
  }
  if ( !check_sort(sort, req_sort) ) {
    // 要求される型と異なっていた．
    mErrBuf << "sort type mismatch at " << node->loc();
    return NULL;
  }

  const SmtVar* obj = mSolver->make_var(sort);
  if ( obj == NULL ) {
    return NULL;
  }

  return mSolver->make_var_term(obj);
}

// @brief let 文の処理を行なう．
// @param[in] node 引数の先頭ノード
// @param[in] req_sort 要求される型
// @note エラーが起こったら mErrBuf にエラーメッセージを出力して NULL を返す．
const SmtTerm*
ShellImpl::eval_as_let(const SmtLibNode* node,
		       tSmtSortId req_sort)
{
  assert_cond( node != NULL, __FILE__, __LINE__);

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

  // let 文の置き換え用の辞書を作る．
  SimpleAlloc alloc(1024);
  NameMgr temp_name_mgr(alloc, 0, mCurNameMgr);

  for (const SmtLibNode* node21 = node2->child();
       node21 != NULL; node21 = node21->sibling()) {
    const SmtId* id;
    const SmtTerm* term;
    if ( !eval_as_var_binding(node21, id, term) ) {
      // エラーメッセージは eval_as_var_binding() 中で出力されているはず
      return NULL;
    }
    // 置き換え用の辞書に登録しておく．
    temp_name_mgr.reg_term(id, term);
  }

  // temp_name_mgr をスタックに積む．
  NameMgr* old_name_mgr = mCurNameMgr;
  mCurNameMgr = &temp_name_mgr;

  const SmtLibNode* node3 = arg_list[1];
  const SmtTerm* body = eval_as_term(node3, req_sort);

  // temp_name_mgr をスタックから取り去る．
  // 自動変数なので自動的に開放される．
  mCurNameMgr = old_name_mgr;

  if ( body == NULL ) {
    // エラーメッセージは下位の呼び出しで出力されているはず
    return NULL;
  }
  return body;
}

// @brief forall 文の処理を行なう．
// @param[in] node 引数の先頭ノード
// @param[in] req_sort 要求される型
// @note エラーが起こったら mErrBuf にエラーメッセージを出力して NULL を返す．
const SmtTerm*
ShellImpl::eval_as_forall(const SmtLibNode* node,
			  tSmtSortId req_sort)
{
  assert_cond( node != NULL, __FILE__, __LINE__);

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

  // forall 文の束縛変数用のの辞書を作る．
  SimpleAlloc alloc(1024);
  NameMgr temp_name_mgr(alloc, 0, mCurNameMgr);

  // 束縛変数のリスト
  vector<const SmtVar*> var_list;
  var_list.reserve(node2->child_num());
  for (const SmtLibNode* node21 = node2->child();
       node21 != NULL; node21 = node21->sibling()) {
    const SmtId* id;
    tSmtSortId sort;
    if ( !eval_as_sorted_var(node21, id, sort) ) {
      // エラーメッセージは eval_as_sorted_var() 中で出力されているはず
      return NULL;
    }
    const SmtVar* var = mSolver->make_var(sort, kSmtVar_Forall);

    // 辞書に登録する．
    temp_name_mgr.reg_var(id, var);

    var_list.push_back(var);
  }

  // 新しい辞書をスタックに積む．
  NameMgr* old_name_mgr = mCurNameMgr;
  mCurNameMgr = &temp_name_mgr;

  const SmtLibNode* node3 = arg_list[1];
  const SmtTerm* body = eval_as_term(node3, req_sort);

  // 新しい辞書をスタックから取り除く
  // temp_name_mgr は自動変数なので自動的に開放される．
  mCurNameMgr = old_name_mgr;

  if ( body == NULL ) {
    // エラーメッセージは下位の呼び出しで出力されているはず
    return NULL;
  }

  return mSolver->make_forall_term(var_list, body);
}

// @brief exists 文の処理を行なう．
// @param[in] node 引数の先頭ノード
// @param[in] req_sort 要求される型
// @note エラーが起こったら mErrBuf にエラーメッセージを出力して NULL を返す．
const SmtTerm*
ShellImpl::eval_as_exists(const SmtLibNode* node,
			  tSmtSortId req_sort)
{
  assert_cond( node != NULL, __FILE__, __LINE__);

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

  // exists 文の束縛変数用のの辞書を作る．
  SimpleAlloc alloc(1024);
  NameMgr temp_name_mgr(alloc, 0, mCurNameMgr);

  // 束縛変数のリスト
  vector<const SmtVar*> var_list;
  var_list.reserve(node2->child_num());
  for (const SmtLibNode* node21 = node2->child();
       node21 != NULL; node21 = node21->sibling()) {
    const SmtId* id;
    tSmtSortId sort;
    if ( !eval_as_sorted_var(node21, id, sort) ) {
      // エラーメッセージは eval_as_sorted_var() 中で出力されているはず
      return NULL;
    }
    const SmtVar* var = mSolver->make_var(sort, kSmtVar_Exists);

    // 辞書に登録する．
    temp_name_mgr.reg_var(id, var);

    var_list.push_back(var);
  }

  // 新しい辞書をスタックに積む．
  NameMgr* old_name_mgr = mCurNameMgr;
  mCurNameMgr = &temp_name_mgr;

  const SmtLibNode* node3 = arg_list[1];
  const SmtTerm* body = eval_as_term(node3, req_sort);

  // 新しい辞書をスタックから取り除く
  // temp_name_mgr は自動変数なので自動的に開放される．
  mCurNameMgr = old_name_mgr;

  if ( body == NULL ) {
    // エラーメッセージは下位の呼び出しで出力されているはず
    return NULL;
  }

  return mSolver->make_exists_term(var_list, body);
}

// @brief attr 文の処理を行なう．
// @param[in] node 引数の先頭ノード
// @param[in] req_sort 要求される型
// @note エラーが起こったら mErrBuf にエラーメッセージを出力して NULL を返す．
const SmtTerm*
ShellImpl::eval_as_attr_term(const SmtLibNode* node,
			     tSmtSortId req_sort)
{
  assert_cond( node != NULL, __FILE__, __LINE__);

  const SmtLibNode* attr_top = node->sibling();
  if ( attr_top == NULL ) {
    mErrBuf << "syntax error at " << node->loc();
    return NULL;
  }

  const SmtTerm* body = eval_as_term(node, req_sort);
  if ( body == NULL ) {
    // エラーメッセージは下位の呼び出しで出力されているはず
    return NULL;
  }

  // 現時点では ( ! <term> :named <symbol> ) の形式だけが許された形式
  const SmtLibNode* node1 = node->sibling();
  if ( node1 == NULL ) {
    mErrBuf << "syntax error at " << node->loc();
    return NULL;
  }

  const SmtLibNode* node2 = node1->sibling();
  if ( node1->type() != kKeywordToken ||
       strcmp(":named", static_cast<const char*>(node1->str_value())) != 0 ||
       node2 == NULL ||
       node2->type() != kSymbolToken ) {
    mErrBuf << "syntax error at " << node->loc();
    return NULL;
  }

  // 関連付ける名前をとってくる．
  const SmtId* name = mIdMgr->make_id(node1->str_value());

  if ( check_obj(name) ) {
    mErrBuf << name->name() << ": already exists";
    return NULL;
  }

  // 関数を作る．
  const SmtFun* fun = mSolver->make_fun(vector<const SmtVar*>(0), body);
  assert_cond( fun != NULL, __FILE__, __LINE__);

  // 関数を登録する．
  bool stat = name_mgr().reg_fun(name, fun);
  assert_cond( stat, __FILE__, __LINE__);

  return body;
}

// @brief S式を var_binding に変換する．
// @param[in] node S式を表すノード
// @param[out] id 変数名を表す識別子
// @param[out] term 置き換える式
// @retval true 変換が成功した．
// @retval false 変換が失敗した．
// @note エラーが起こったら mErrBuf にエラーメッセージを出力して NULL を返す．
bool
ShellImpl::eval_as_var_binding(const SmtLibNode* node,
			       const SmtId*& id,
			       const SmtTerm*& term)

{
  // ( <identifier> <term> ) の形
  vector<const SmtLibNode*> arg_list;
  if ( node->type() != kListToken ) {
    mErrBuf << "syntax error: " << node->loc();
    return false;
  }
  if ( !parse_args(node->child(), 2, arg_list) ) {
    mErrBuf << "syntax error: " << node->loc();
    return false;
  }

  id = eval_as_id(arg_list[0]);
  if ( id == NULL ) {
    mErrBuf << "not a valid identifier: " << arg_list[0]->loc();
    return false;
  }

  term = eval_as_term(arg_list[1], kSmtSort_None);
  if ( term == NULL ) {
    // エラーメッセージは eval_as_term() 中で出力されている．
    return false;
  }

  return true;
}

// @brief S式を sorted_var に変換する．
// @param[out] id 変数名を表す識別子
// @param[out] sort 変数の型
// @note エラーが起こったら mErrBuf にエラーメッセージを出力して false を返す．
bool
ShellImpl::eval_as_sorted_var(const SmtLibNode* node,
			      const SmtId*& id,
			      tSmtSortId& sort)
{
  // ( <identifier> <sort> ) の形
  vector<const SmtLibNode*> arg_list;
  if ( !parse_args(node->child(), 2, arg_list) ) {
    mErrBuf << "syntax error: " << node->loc();
    return false;
  }

  id = eval_as_id(arg_list[0]);
  if ( id == NULL ) {
    mErrBuf << "not a valid identifier: " << arg_list[0]->loc();
    return false;
  }

  sort = eval_as_sort(arg_list[1]);
  if ( sort == kSmtSort_None ) {
    // エラーメッセージは eval_as_sort() 中で出力されている．
    return false;
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

// @brief 組み込み型と名前を結びつける．
// @param[in] name 名前
// @param[in] sort_type 組み込み型
void
ShellImpl::bind_builtin_sort(const char* name,
			     tSmtSortId sort_id)
{
  const SmtId* id = mIdMgr->make_id(ShString(name));
  assert_cond( id != NULL, __FILE__, __LINE__);

  mBuiltinSortMap.add(id->id(), sort_id);
}

// @brief 組み込み関数と名前を結びつける．
// @param[in] name 名前
// @param[in] fun_type 組み込み関数の型
void
ShellImpl::bind_builtin_fun(const char* name,
			    tSmtFunType fun_type)
{
  const SmtId* id = mIdMgr->make_id(ShString(name));
  assert_cond( id != NULL, __FILE__, __LINE__);

  bool stat = name_mgr().reg_builtin_fun(id, fun_type);
  assert_cond( stat, __FILE__, __LINE__);

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
  assert_cond( mCurNameMgr != NULL, __FILE__, __LINE__);
  return *mCurNameMgr;
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

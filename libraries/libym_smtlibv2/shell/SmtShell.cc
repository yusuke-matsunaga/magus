
/// @file SmtShell.cc
/// @brief SmtShell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtShell.h"
#include "SmtLibParser.h"
#include "SmtLibNode.h"

#include "ym_smtlibv2/SmtId.h"
#include "ym_smtlibv2/SmtSort.h"
#include "ym_smtlibv2/SmtFun.h"

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
// クラス SmtShell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtShell::SmtShell()
{
  mParser = new SmtLibParser();
  mPrompt1 = ">";
  mPrompt2 = "...>";
  mAllowCtrlDExit = false;
}

// @brief デストラクタ
SmtShell::~SmtShell()
{
  delete mParser;
}

// @brief 実行する．
void
SmtShell::run()
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
SmtShell::eval_line()
{
  StrListIDO ido(mStrList);
  mParser->init(&ido);

  for ( ; ; ) {
    SmtLibNode* node = NULL;
    const SmtLibNode* node1 = NULL;
    const SmtLibNode* node2 = NULL;
    const char* str1 = NULL;

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
      set_logic(node2);
    }
    else if ( strcmp("set-option", str1) == 0 ) {
      set_option(node2);
    }
    else if ( strcmp("set-info", str1) == 0 ) {
      set_info(node2);
    }
    else if ( strcmp("declare-sort", str1) == 0 ) {
      declare_sort(node2);
    }
    else if ( strcmp("define-sort", str1) == 0 ) {
      define_sort(node2);
    }
    else if ( strcmp("declare-fun", str1) == 0 ) {
      declare_fun(node2);
    }
    else if ( strcmp("define-fun", str1) == 0 ) {
      define_fun(node2);
    }
    else if ( strcmp("push", str1) == 0 ) {
      push(node2);
    }
    else if ( strcmp("pop", str1) == 0 ) {
      pop(node2);
    }
    else if ( strcmp("assert", str1) == 0 ) {
      assert(node2);
    }
    else if ( strcmp("check-sat", str1) == 0 ) {
      check_sat(node2);
    }
    else if ( strcmp("get-assertions", str1) == 0 ) {
      get_assertions(node2);
    }
    else if ( strcmp("get-proof", str1) == 0 ) {
      get_proof(node2);
    }
    else if ( strcmp("get-unsat-core", str1) == 0 ) {
      get_unsat_core(node2);
    }
    else if ( strcmp("get-value", str1) == 0 ) {
      get_value(node2);
    }
    else if ( strcmp("get-assignment", str1) == 0 ) {
      get_assignment(node2);
    }
    else if ( strcmp("get-option", str1) == 0 ) {
      get_option(node2);
    }
    else if ( strcmp("get-info", str1) == 0 ) {
      get_info(node2);
    }
    else if ( strcmp("exit", str1) == 0 ) {
      return 0;
    }
    else {
      goto syntax_error;
    }
  }

 syntax_error:
  return 1;
}

// @brief set-logic の処理を行う．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::set_logic(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "set-logic ";
    print(cerr, arg_top);
    cerr << endl;
  }

  ostringstream ebuf;

  // このコマンドは1つの引数をとる．
  if ( arg_top == NULL || arg_top->sibling() != NULL ||
       arg_top->type() != kSymbolToken ) {
    ebuf << "syntax_error";
    goto syntax_error;
  }
  else {
    const char* str = arg_top->str_value();
    tSmtLogic logic = kSmtLogic_NONE;
    if ( strcmp(str, "AUFLIA") == 0 ) {
      logic = kSmtLogic_AUFLIA;
    }
    else if ( strcmp(str, "AUFLIRA") == 0 ) {
      logic = kSmtLogic_AUFLIRA;
    }
    else if ( strcmp(str, "AUFNIRA") == 0 ) {
      logic = kSmtLogic_AUFNIRA;
    }
    else if ( strcmp(str, "LRA") == 0 ) {
      logic = kSmtLogic_LRA;
    }
    else if ( strcmp(str, "QF_ABV") == 0 ) {
      logic = kSmtLogic_QF_ABV;
    }
    else if ( strcmp(str, "QF_AUFBV") == 0 ) {
      logic = kSmtLogic_QF_AUFBV;
    }
    else if ( strcmp(str, "QF_AUFLIA") == 0 ) {
      logic = kSmtLogic_QF_AUFLIA;
    }
    else if ( strcmp(str, "QF_AX") == 0 ) {
      logic = kSmtLogic_QF_AX;
    }
    else if ( strcmp(str, "QF_BV") == 0 ) {
      logic = kSmtLogic_QF_BV;
    }
    else if ( strcmp(str, "QF_IDL") == 0 ) {
      logic = kSmtLogic_QF_IDL;
    }
    else if ( strcmp(str, "QF_LIA") == 0 ) {
      logic = kSmtLogic_QF_LIA;
    }
    else if ( strcmp(str, "QF_LRA") == 0 ) {
      logic = kSmtLogic_QF_LRA;
    }
    else if ( strcmp(str, "QF_NIA") == 0 ) {
      logic = kSmtLogic_QF_NIA;
    }
    else if ( strcmp(str, "QF_NRA") == 0 ) {
      logic = kSmtLogic_QF_NRA;
    }
    else if ( strcmp(str, "QF_RDL") == 0 ) {
      logic = kSmtLogic_QF_RDL;
    }
    else if ( strcmp(str, "QF_UF") == 0 ) {
      logic = kSmtLogic_QF_UF;
    }
    else if ( strcmp(str, "QF_UFBF") == 0 ) {
      logic = kSmtLogic_QF_UFBV;
    }
    else if ( strcmp(str, "QF_UFIDL") == 0 ) {
      logic = kSmtLogic_QF_UFIDL;
    }
    else if ( strcmp(str, "QF_UFLIA") == 0 ) {
      logic = kSmtLogic_QF_UFLIA;
    }
    else if ( strcmp(str, "QF_UFLRA") == 0 ) {
      logic = kSmtLogic_QF_UFLRA;
    }
    else if ( strcmp(str, "QF_UFNRA") == 0 ) {
      logic = kSmtLogic_QF_UFNRA;
    }
    else if ( strcmp(str, "UFLRA") == 0 ) {
      logic = kSmtLogic_UFLRA;
    }
    else if ( strcmp(str, "UFNIA") == 0 ) {
      logic = kSmtLogic_UFNIA;
    }
    else {
      ebuf << str << ": unknown logic";
      goto syntax_error;
    }

    if ( !mMgr.set_logic(logic) ) {
      ebuf << "already set";
      goto syntax_error;
    }

    if ( debug ) {
      cerr << "  ==> success" << endl;
    }

    return true;
  }

 syntax_error:
  if ( debug ) {
    cerr << "  ==> error: " << ebuf.str() << endl;
  }
  return false;
}

// @brief オプションを設定する．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::set_option(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "set-option ";
    print(cerr, arg_top);
    cerr << endl;
  }

  if ( debug ) {
    cerr << "  ==> OK" << endl;
  }
  return true;
}

// @brief 情報を設定する．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::set_info(const SmtLibNode* arg_top)
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

  if ( !mMgr.set_info(attr_list[0]) ) {
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
SmtShell::declare_sort(const SmtLibNode* arg_top)
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
    emsg = "second argument is not a numeric";
    goto syntax_error;
  }
  num = arg_list[1]->int_value();

  // 型を登録する．
  if ( !mMgr.declare_sort(name, num) ) {
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
SmtShell::define_sort(const SmtLibNode* arg_top)
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
  if ( !mMgr.define_sort(name, param_list.size(), sort) ) {
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
SmtShell::declare_fun(const SmtLibNode* arg_top)
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
  if ( !mMgr.declare_fun(name, input_sort_list, output_sort) ) {
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
SmtShell::define_fun(const SmtLibNode* arg_top)
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
  if ( !mMgr.define_fun(name, var_list, output_sort, body) ) {
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
SmtShell::assert(const SmtLibNode* arg_top)
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
    if ( !mMgr.assert(term) ) {
      goto syntax_error;
    }
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
SmtShell::push(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "push ";
    print(cerr, arg_top);
    cerr << endl;
  }

  const char* emsg = "";

  // このコマンドは1つの整数値を引数にとる．
  if ( arg_top == NULL ||
       arg_top->sibling() != NULL ||
       arg_top->type() != kNumToken ) {
    emsg = "syntax error";
    goto syntax_error;
  }

  {
    ymuint num = arg_top->int_value();
    if ( !mMgr.push(num) ) {
      goto syntax_error;
    }
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

// @brief assertion スタックからポップする．
// @param[in] arg_top 引数の先頭ノード
// @return ポップが成功したら true を返す．
bool
SmtShell::pop(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "pop ";
    print(cerr, arg_top);
    cerr << endl;
  }

  const char* emsg = "";

  // このコマンドは1つの整数値を引数にとる．
  if ( arg_top == NULL ||
       arg_top->sibling() != NULL ||
       arg_top->type() != kNumToken ) {
    emsg = "syntax error";
    goto syntax_error;
  }

  {
    ymuint num = arg_top->int_value();
    if ( !mMgr.pop(num) ) {
      emsg = "arg is too large";
      goto syntax_error;
    }
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

// @brief 充足可能性判定を行なう．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::check_sat(const SmtLibNode* arg_top)
{
  if ( debug ) {
    cerr << "check_sat ";
    print(cerr, arg_top);
    cerr << endl;
  }

  if ( debug ) {
    cerr << "  ==> success" << endl;
  }

  return true;
}

// @brief assertion を得る．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::get_assertions(const SmtLibNode* arg_top)
{
  return true;
}

// @brief 証明を得る．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::get_proof(const SmtLibNode* arg_top)
{
  return true;
}

// @brief unsat core を得る．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::get_unsat_core(const SmtLibNode* arg_top)
{
  return true;
}

// @brief 値を得る．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::get_value(const SmtLibNode* arg_top)
{
  return true;
}

// @brief 割り当てを得る．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::get_assignment(const SmtLibNode* arg_top)
{
  return true;
}

// @brief オプションを得る．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::get_option(const SmtLibNode* arg_top)
{
  return true;
}

// @brief 情報を得る．
// @param[in] arg_top 引数の先頭ノード
bool
SmtShell::get_info(const SmtLibNode* arg_top)
{
  return true;
}

// @brief S式を識別子に変換する．
// @param[in] node S式を表すノード
const SmtId*
SmtShell::eval_to_id(const SmtLibNode* node)
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

  return mMgr.make_id(name, index_list);

 syntax_error:
  return NULL;
}

// @brief S式を sort に変換する．
// @param[in] node S式を表すノード
const SmtSort*
SmtShell::eval_to_sort(const SmtLibNode* node)
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

  sort = mMgr.make_sort(id, elem_list);
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
SmtShell::eval_to_sort_template(const SmtLibNode* node,
				const vector<const SmtId*>& param_list)
{
  const SmtId* id = eval_to_id(node);
  vector<const SmtSort*> elem_list;
  if ( id != NULL ) {
    ymuint n = param_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      if ( id == param_list[i] ) {
	return mMgr.make_param_sort(i);
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

  return mMgr.make_sort(id, elem_list);

 syntax_error:

  return NULL;
}

// @brief S式を term に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
SmtShell::eval_to_term(const SmtLibNode* node)
{
  // 定数の場合
  switch ( node->type() ) {
  case kNumToken:    return mMgr.make_numeric_term(node->int_value());
  case kDecToken:    return mMgr.make_decimal_term(node->str_value());
  case kHexToken:    return mMgr.make_hexadecimal_term(node->str_value());
  case kBinToken:    return mMgr.make_binary_term(node->str_value());
  case kStringToken: return mMgr.make_string_term(node->str_value());
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
    const SmtFun* fun = mMgr.find_fun(fid);
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
      return mMgr.make_fun_term(fun, input_list);
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
      return mMgr.make_let_term(var_binding_list, term3);
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
      return mMgr.make_forall_term(sorted_var_list, term3);
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
      return mMgr.make_exists_term(sorted_var_list, term3);
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
      return mMgr.make_attr_term(term, attr_list);
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
SmtShell::eval_to_expr(const SmtLibNode* node)
{
  switch ( node->type() ) {
  case kNumToken:
    return mMgr.make_numeric_term(node->int_value());

  case kDecToken:
    return mMgr.make_decimal_term(node->str_value());

  case kHexToken:
    return mMgr.make_hexadecimal_term(node->str_value());

  case kBinToken:
    return mMgr.make_binary_term(node->str_value());

  case kStringToken:
    return mMgr.make_string_term(node->str_value());

  case kSymbolToken:
    return mMgr.make_symbol_term(node->str_value());

  case kKeywordToken:
    return mMgr.make_keyword_term(node->str_value());

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
      return mMgr.make_list_term(expr_list);
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
SmtShell::eval_to_qid(const SmtLibNode* node)
{
  // ただの identifier の場合もある．
  const SmtId* id = eval_to_id(node);
  if ( id != NULL ) {
    return mMgr.make_identifier_term(id);
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

  return mMgr.make_qual_identifier_term(id, sort);
}

// @brief S式を sorted_var に変換する．
// @param[in] node S式を表すノード
// @param[out] sorted_var 結果を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．
bool
SmtShell::eval_to_sorted_var(const SmtLibNode* node,
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
SmtShell::eval_to_var_binding(const SmtLibNode* node,
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
SmtShell::eval_to_attr(const SmtLibNode* node,
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
      const SmtAttr* attr = mMgr.make_attr(node1->str_value(), expr);
      attr_list.push_back(attr);

      node1 = node2;
    }
    else {
      const SmtAttr* attr = mMgr.make_attr(node1->str_value());
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
SmtShell::parse_args(const SmtLibNode*  arg_top,
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

END_NAMESPACE_YM_SMTLIBV2

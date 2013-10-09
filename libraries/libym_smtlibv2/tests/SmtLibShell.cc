
/// @file SmtLibShell.cc
/// @brief SmtLibMgr を使った簡単なシェル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibNode.h"
#include "SmtLibParser.h"
#include "SmtLibMgr.h"
#include "ym_utils/StrListIDO.h"

#include <readline/readline.h>
#include <readline/history.h>


BEGIN_NAMESPACE_YM_SMTLIBV2

int
read_line(const vector<string>& str_list,
	  SmtLibMgr& mgr)
{
  StrListIDO ido(str_list);

  SmtLibParser parser;
  parser.init(&ido);

  for ( ; ; ) {
    SmtLibNode* node;
    SmtLibParser::tResult res = parser.read(node);
    switch ( res ) {
    case SmtLibParser::kOK:
      break;

    case SmtLibParser::kError:
      cout << "Error" << endl;
      return -1;

    case SmtLibParser::kEOF:
      return 1;

    case SmtLibParser::kOpen:
      return 2;
    }

    if ( node->type() != kListToken ) {
      cerr << "syntax error" << endl;
      return -1;
    }
    const SmtLibNode* node1 = node->child();
    if ( node1->type() != kSymbolToken ) {
      cerr << "syntax error" << endl;
      return -1;
    }
    const char* str1 = static_cast<const char*>(node1->str_value());
    if ( strcmp("set-logic", str1) == 0 ) {
      mgr.set_logic(node1->sibling());
    }
    else if ( strcmp("set-option", str1) == 0 ) {
      cout << "SET_OPTION" << endl;
    }
    else if ( strcmp("set-info", str1) == 0 ) {
      mgr.set_info(node1->sibling());
    }
    else if ( strcmp("declare-sort", str1) == 0 ) {
      mgr.declare_sort(node1->sibling());
    }
    else if ( strcmp("define-sort", str1) == 0 ) {
      mgr.define_sort(node1->sibling());
    }
    else if ( strcmp("declare-fun", str1) == 0 ) {
      mgr.declare_fun(node1->sibling());
    }
    else if ( strcmp("define-fun", str1) == 0 ) {
      mgr.define_fun(node1->sibling());
    }
    else if ( strcmp("push", str1) == 0 ) {
      cout << "PUSH " << node1->sibling()->int_value() << endl;
    }
    else if ( strcmp("pop", str1) == 0 ) {
      cout << "POP " << node1->sibling()->int_value() << endl;
    }
    else if ( strcmp("assert", str1) == 0 ) {
      mgr.assert(node1->sibling());
    }
    else if ( strcmp("check-sat", str1) == 0 ) {
      cout << "CHECK_SAT" << endl;
    }
    else if ( strcmp("get-assertions", str1) == 0 ) {
      cout << "GET_ASSERTIONS" << endl;
    }
    else if ( strcmp("get-proof", str1) == 0 ) {
      cout << "GET_PROOF" << endl;
    }
    else if ( strcmp("get-unsat-core", str1) == 0 ) {
      cout << "GET_UNSAT_CORE" << endl;
    }
    else if ( strcmp("get-value", str1) == 0 ) {
      cout << "GET_VALUE" << endl;
    }
    else if ( strcmp("get-assignment", str1) == 0 ) {
      cout << "GET_ASSIGNMENT" << endl;
    }
    else if ( strcmp("get-option", str1) == 0 ) {
      cout << "GET_OPTION" << endl;
    }
    else if ( strcmp("get-info", str1) == 0 ) {
      cout << "GET_INFO" << endl;
    }
    else if ( strcmp("exit", str1) == 0 ) {
      cout << "EXIT" << endl;
      return 0;
    }
    else {
      cout << "undefined command " << str1 << endl;
      return -1;
    }
  }
}

BEGIN_NONAMESPACE

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

void
shell()
{
  const char* PROMPT1 = ">";
  const char* PROMPT2 = "...>";

  ymuint hist_num = 0;
  ymuint max_history = 1000;

  SmtLibMgr mgr;

  vector<string> str_list;
  const char* prompt = PROMPT1;
  for (bool run = true; run; ) {
    char* line = readline(prompt);
    if ( line == NULL ) {
      // EOF
      run = false;
      printf("\n");
      continue;
    }

    // 無駄な空白を取り除く
    char* str = chop(line);
    if ( *str == '\0' ) {
      continue;
    }

    // ヒストリに追加する．
    add_history(str);
    ++ hist_num;
    if ( hist_num > max_history ) {
      // あふれたヒストリを削除する．
      HIST_ENTRY* hist = remove_history(0);
      free(hist);
      -- hist_num;
    }

    str_list.push_back(string(str));

    int code = read_line(str_list, mgr);
    if ( code == 2 ) {
      prompt = PROMPT2;
      continue;
    }
    else if ( code == 0 ) {
      run = false;
      break;
    }
    else if ( code == 1 ) {
      str_list.clear();
      prompt = PROMPT1;
      continue;
    }
    else if ( code == -1 ) {
      str_list.clear();
      prompt = PROMPT1;
      continue;
    }
  }
}

END_NAMESPACE_YM_SMTLIBV2

int
main(int argc,
     const char** argv)
{
  using namespace nsYm::nsSmtLibV2;

  shell();

  return 0;
}

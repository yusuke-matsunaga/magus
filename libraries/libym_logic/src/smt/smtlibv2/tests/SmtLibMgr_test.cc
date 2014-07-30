
/// @file libym_smtlibv2/test/parsertest.cc
/// @brief SmtLibParser のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibNode.h"
#include "SmtLibParser.h"
#include "SmtLibMgr.h"
#include "ym_YmUtils/FileIDO.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

void
SmtLibMgr_test(const string& filename)
{
  FileIDO ido(filename);
  if ( !ido ) {
    cout << filename << ": No such file" << endl;
    return;
  }

  SmtLibParser parser;
  SmtLibMgr mgr;

  parser.init(&ido);

  for ( ; ; ) {
    SmtLibNode* node;
    SmtLibParser::tResult res = parser.read(node);
    switch ( res ) {
    case SmtLibParser::kOK:
      break;

    case SmtLibParser::kError:
      cout << "Error" << endl;
      return;

    case SmtLibParser::kEOF:
      return;

    case SmtLibParser::kOpen:
      cout << "unexpected EOF" << endl;
      return;
    }

    if ( node->type() != kListToken ) {
      cerr << "syntax error" << endl;
      return;
    }
    const SmtLibNode* node1 = node->child();
    if ( node1->type() != kSymbolToken ) {
      cerr << "syntax error" << endl;
      return;
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
      mgr.push(node1->sibling());
    }
    else if ( strcmp("pop", str1) == 0 ) {
      mgr.pop(node1->sibling());
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
      return;
    }
    else {
      cout << "undefined command " << str1 << endl;
    }
  }

}

END_NAMESPACE_YM_SMTLIBV2


int
main(int argc,
     char** argv)
{
  using namespace std;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 1;
  }

  string filename(argv[1]);

  nsYm::nsSmtLibV2::SmtLibMgr_test(filename);

  return 0;
}


/// @file parser_test.cc
/// @brief parser_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "RsrvWordDic.h"
#include "Ast.h"
#include "AstMgr.h"
#include "AstStatement.h"
#include "YmUtils/FileIDO.h"
#include "YmUtils/StreamIDO.h"
#include "YmUtils/StringIDO.h"
#include "YmUtils/MsgHandler.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_YMSL

int
parser_test1(IDO& ido)
{
  StreamMsgHandler handler(&cout);
  MsgMgr::reg_handler(&handler);

  AstMgr mgr;

  bool stat = mgr.read_source(ido);
  if ( stat ) {
    const vector<AstStatement*>& toplevel_list = mgr.toplevel_list();
    for (vector<AstStatement*>::const_iterator p = toplevel_list.begin();
	 p != toplevel_list.end(); ++ p) {
      AstStatement* stmt = *p;
      stmt->print(cout);
    }
  }

  return 0;
}

int
parser_test(int argc,
	    char** argv)
{
  if ( argc == 1 ) {
    const char* str =
      "import stdlib;\n"
      "import math as R;\n"
      "import a, b, c as D;\n"
      "function print(s:string):int {\n"
      "}\n"
      "var a:int;\n"
      "var b:int;\n"
      "var c:int;\n"
      "var d:int;\n"
      "var e:int;\n"
      "var f:int;\n"
      "var g:int;\n"
      "var h:int;\n"
      "var i:int;\n"
      "var k:int;\n"
      "var F:int;\n"
      "a = 1;\n"
      "if a == b {\n"
      "F = a + b;\n"
      "F = a\n;"
      "}\n"
      "elif a != c {\n"
      "F = c;"
      "}\n"
      "else {\n"
      "F = 0;\n"
      "}\n"
      "for (i = 0; i < 10; i = i + 1) {\n"
      "print(i);\n"
      "}\n"
      "F = a + b / c * d % e + (f > g) & (h <= k);\n"
      "//comment\n";

    StringIDO ido(str);
    return parser_test1(ido);
  }
  else {
    for (int i = 1; i < argc; ++ i) {
      FileIDO ido;
      if ( !ido.open(argv[i]) ) {
	cerr << argv[i] << ": no such file" << endl;
	return -1;
      }
      int stat = parser_test1(ido);
      if ( stat != 0 ) {
	return stat;
      }
    }
  }
  return 0;
}

END_NAMESPACE_YM_YMSL


int
main(int argc,
     char** argv)
{
  return nsYm::nsYmsl::parser_test(argc, argv);
}

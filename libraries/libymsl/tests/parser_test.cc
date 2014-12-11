
/// @file parser_test.cc
/// @brief parser_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslDriver.h"
#include "RsrvWordDic.h"
#include "Ast.h"
#include "AstBlock.h"
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

  YmslDriver driver;

  driver.read(ido);

  AstBlock* toplevel = driver.toplevel_block();
  toplevel->print(cout, 0);

  return 0;
}

int
parser_test(int argc,
	    char** argv)
{
  if ( argc == 1 ) {
    const char* str =
      "a:int;\n"
      "b:int;\n"
      "c:int;\n"
      "d:int;\n"
      "e:int;\n"
      "f:int;\n"
      "g:int;\n"
      "h:int;\n"
      "i:int;\n"
      "k:int;\n"
      "F:int;\n"
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

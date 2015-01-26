
/// @file scanner_test.cc
/// @brief scanner_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslScanner.h"

#include "YmUtils/FileIDO.h"
#include "YmUtils/StreamIDO.h"
#include "YmUtils/StringIDO.h"


BEGIN_NAMESPACE_YM_YMSL


int
scanner_test1(IDO& ido)
{
  YmslScanner scanner(ido);

  for ( ; ; ) {
    FileRegion loc;
    TokenType token = scanner.read_token(loc);
    cout << loc << ": ";
    scanner.print_token(token, cout);
    cout << endl;
    if ( token == EOF ) {
      break;
    }
  }
  return 0;
}

int
scanner_test(int argc,
	     char** argv)
{
  if ( argc == 1 ) {
    const char* str =
      ":\n"
      ";\n"
      ",\n"
      ".\n"
      "?\n"
      "++\n"
      "--\n"
      "=\n"
      "+=\n"
      "-=\n"
      "*=\n"
      "/=\n"
      "%=\n"
      "<<=\n"
      ">>=\n"
      "&=\n"
      "|=\n"
      "^=\n"
      "(\n"
      ")\n"
      "{\n"
      "}\n"
      "[\n"
      "]\n"
      "boolean\n"
      "break\n"
      "case\n"
      "continue\n"
      "default\n"
      "do\n"
      "elif\n"
      "else\n"
      "float\n"
      "for\n"
      "function\n"
      "global\n"
      "goto\n"
      "if\n"
      "import\n"
      "int\n"
      "return\n"
      "string\n"
      "switch\n"
      "var\n"
      "void\n"
      "while\n"
      "+\n"
      "-\n"
      "*\n"
      "/\n"
      "%\n"
      "not\n"
      "and\n"
      "or\n"
      "~\n"
      "&\n"
      "|\n"
      "^\n"
      "==\n"
      "!=\n"
      "<\n"
      ">\n"
      "<=\n"
      ">=\n"
      "abcd\n"
      "1\n"
      "1.23450\n"
      "//comment\n"
      "/*-----\n"
      "-----*/\n";

    StringIDO ido(str);
    return scanner_test1(ido);
  }
  else {
    for (int i = 1; i < argc; ++ i) {
      FileIDO ido;
      if ( !ido.open(argv[i]) ) {
	cerr << argv[i] << ": no such file" << endl;
	return -1;
      }
      int stat = scanner_test1(ido);
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
  return nsYm::nsYmsl::scanner_test(argc, argv);
}

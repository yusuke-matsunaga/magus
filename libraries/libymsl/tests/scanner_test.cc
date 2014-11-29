
/// @file scanner_test.cc
/// @brief scanner_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "../src/YmslScanner.h"
#include "../src/RsrvWordDic.h"
#include "YmUtils/FileIDO.h"
#include "YmUtils/StreamIDO.h"
#include "YmUtils/StringIDO.h"


BEGIN_NAMESPACE_YM_YMSL


int
scanner_test1(IDO& ido)
{
  YmslScanner scanner(ido);
  RsrvWordDic dic;

  for ( ; ; ) {
    FileRegion loc;
    TokenType token = scanner.read_token(loc);
    cout << loc << ": ";
    switch ( token ) {
    case SYMBOL:    cout << "SYMBOL[" << scanner.cur_string() << "]"; break;
    case INT_NUM:   cout << "INT[" << scanner.cur_int() << "]"; break;
    case FLOAT_NUM: cout << "FLOAT[" << scanner.cur_float() << "]"; break;
    case END:       cout << "END"; break;
    default:        cout << dic.str(token); break;
    }
    cout << endl;
    if ( token == END ) {
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
      "++\n"
      "--\n"
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
      "=\n"
      "(\n"
      ")\n"
      "{\n"
      "}\n"
      "[\n"
      "]\n"
      "if\n"
      "else\n"
      "elif\n"
      "for\n"
      "while\n"
      "do\n"
      "goto\n"
      "break\n"
      "continue\n"
      "switch\n"
      "case\n"
      "function\n"
      "var\n"
      "int\n"
      "float\n"
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

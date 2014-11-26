
/// @file scanner_test.cc
/// @brief scanner_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "../src/YmslScanner.h"
#include "YmUtils/FileIDO.h"
#include "YmUtils/StreamIDO.h"


BEGIN_NAMESPACE_YM_YMSL

int
scanner_test1(IDO& ido)
{
  YmslScanner scanner(ido);

  for ( ; ; ) {
    FileRegion loc;
    TokenType token = scanner.read_token(loc);
    cout << loc << ": " << token2str(token);
    switch ( token ) {
    case SYMBOL: cout << scanner.cur_string(); break;
    case INT_NUM: cout << scanner.cur_int(); break;
    case FLOAT_NUM: cout << scanner.cur_float(); break;
    default: break;
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
    StreamIDO ido(cin);
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

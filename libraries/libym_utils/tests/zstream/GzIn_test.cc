
/// @file GzIn_test.cc
/// @brief GzIn のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GzIn.h"


BEGIN_NAMESPACE_YM

int
GzIn_test(int argc,
	  const char** argv)
{
  GzIn gzin;

  if ( argc != 2 ) {
    return 1;
  }

  if ( !gzin.open(argv[1]) ) {
    cerr << argv[1] << ": No such file" << endl;
    return 2;
  }

  for ( ; ; ) {
    const ymuint BUFF_SIZE = 4096;
    ymuint8 buff[BUFF_SIZE];
    ssize_t n = gzin.decomp(buff, BUFF_SIZE);
    if ( n < 0 ) {
      cerr << "ERROR" << endl;
      return -1;
    }
    if ( n == 0 ) {
      break;
    }
    for (ymint i = 0; i < n; ++ i) {
      cout << static_cast<char>(buff[i]);
    }
  }

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  using namespace nsYm;

  return GzIn_test(argc, argv);
}

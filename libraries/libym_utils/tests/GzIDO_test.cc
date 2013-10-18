
/// @file GzIDO_test.cc
/// @brief GzIDO のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/GzIDO.h"


BEGIN_NAMESPACE_YM

int
GzIDO_test(int argc,
	   const char** argv)
{
  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 1;
  }

  GzIDO ido(argv[1]);
  if ( !ido ) {
    cerr << argv[1] << ": No such file" << endl;
    return 2;
  }

#if 1
  while ( ido ) {
    const ymuint BUFF_SIZE = 4096;
    ymuint8 buff[BUFF_SIZE];
    ssize_t n = ido.read(buff, BUFF_SIZE);
    if ( n < 0 ) {
      cerr << "ERROR in GzIDO::read(): n = " << n << endl;
      return -1;
    }
    if ( n == 0 ) {
      break;
    }
    for (ymint i = 0; i < n; ++ i) {
      cout << static_cast<char>(buff[i]);
    }
  }
#else
  while ( ido ) {
    ymuint8 c;
    ido >> c;
    cout << static_cast<char>(c);
  }
#endif

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  using namespace nsYm;

  return GzIDO_test(argc, argv);
}

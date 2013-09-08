
/// @file z_state.cc
/// @brief CompIDO のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/CompIDO.h"


BEGIN_NAMESPACE_YM

int
CompIDO_test(int argc,
	     const char** argv)
{
  CompIDO ido;

  if ( argc != 2 ) {
    return 1;
  }

  if ( !ido.open(argv[1]) ) {
    cerr << argv[1] << ": No such file" << endl;
    return -1;
  }

#if 1
  for ( ; ; ) {
    const ymuint BUFF_SIZE = 4096;
    ymuint8 buff[BUFF_SIZE];
    ssize_t n = ido.read(buff, BUFF_SIZE);
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
  using nsYm::CompIDO_test;

  return CompIDO_test(argc, argv);
}


/// @file z_state.cc
/// @brief ZState のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "../ZState.h"


BEGIN_NAMESPACE_YM

int
z_test(int argc,
       const char** argv)
{
  ZStateR zs;

  if ( !zs.open(argv[1], O_RDONLY) ) {
    cerr << argv[1] << ": No such file" << endl;
    return -1;
  }

  for ( ; ; ) {
    ymuint8 buff[4096];
    ssize_t n = zs.read(buff, sizeof(buff));
    if ( n < 0 ) {
      cerr << "ERROR" << endl;
      return -1;
    }
    if ( n == 0 ) {
      break;
    }
    for (ymuint i = 0; i < n; ++ i) {
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
  using nsYm::z_test;

  return z_test(argc, argv);
}

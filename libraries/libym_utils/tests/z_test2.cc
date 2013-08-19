
/// @file z_state2.cc
/// @brief ZState のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include <stdio.h>
#include "zopen.h"
#include "../ZState.h"


BEGIN_NAMESPACE_YM

int
z_test(int argc,
       const char** argv)
{
  FILE* fp = zopen(argv[1], "r", 0);

  for ( ; ; ) {
    ymuint8 buff[4096];
    char* tmp = fgets(reinterpret_cast<char*>(buff), sizeof(buff), fp);
    if ( tmp == NULL ) {
      break;
    }
    cout << tmp;
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

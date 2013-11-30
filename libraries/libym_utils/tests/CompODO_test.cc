
/// @file CompODO_test.cc
/// @brief CompODO のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/CompODO.h"
#include <fcntl.h>


BEGIN_NAMESPACE_YM

int
CompODO_test(int argc,
	     const char** argv)
{
  CompODO odo;

  if ( argc != 3 ) {
    cerr << "Usage: " << argv[0] << " <original filename> <compressed filename>" << endl;
    return 1;
  }

  int fd = open(argv[1], O_RDONLY);
  if ( fd < 0 ) {
    cerr << argv[1] << ": No such file" << endl;
    return -1;
  }

  if ( !odo.open(argv[2]) ) {
    cerr << argv[2] << ": Could not create" << endl;
    return -1;
  }

  for ( ; ; ) {
    const ymuint BUFF_SIZE = 4096;
    ymuint8 buff[BUFF_SIZE];
    ssize_t n = read(fd, buff, BUFF_SIZE);
    if ( n < 0 ) {
      cerr << "ERROR" << endl;
      return -1;
    }
    if ( n == 0 ) {
      break;
    }
    odo.write(buff, n);
  }
  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  using nsYm::CompODO_test;

  return CompODO_test(argc, argv);
}

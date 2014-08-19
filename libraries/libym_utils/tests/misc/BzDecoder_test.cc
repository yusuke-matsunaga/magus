
/// @file BzDecoder_test.cc
/// @brief BzDecoder のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "io/BzDecoder.h"


BEGIN_NAMESPACE_YM

int
BzDecoder_test(int argc,
	       const char** argv)
{
  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 1;
  }

  BzDecoder decoder;

  if ( !decoder.open(argv[1]) ) {
    cerr << argv[1] << ": No such file" << endl;
    return 2;
  }

  while ( decoder.is_ready() ) {
    const ymuint BUFF_SIZE = 4096;
    ymuint8 buff[BUFF_SIZE];
    ssize_t n = decoder.read(buff, BUFF_SIZE);
    if ( n < 0 ) {
      cerr << "ERROR in BzDecoder::read(): n = " << n << endl;
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

  return BzDecoder_test(argc, argv);
}

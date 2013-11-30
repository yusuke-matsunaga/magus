
/// @file gbm_test.cc
/// @brief Gbm のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gbm/Gbm.h"


BEGIN_NAMESPACE_YM_GBM

int
gbm_test(int argc,
	 const char** argv)
{
  Gbm gbm("", "", &cout);

  ifstream ifs;
  istream* isp = NULL;
  if ( argc == 1 ) {
    isp = &cin;
  }
  else if ( argc == 2 ) {
    const char* filename = argv[1];
    ifs.open(filename);
    if ( !ifs ) {
      cerr << "Could not open " << filename << endl;
      return 2;
    }
    isp = &ifs;
  }
  else {
    cerr << "USAGE: " << argv[0] << " [filename]" << endl;
    return 1;
  }

  string buf;
  while ( getline(*isp, buf) ) {
    cout << buf << endl;

  }

  return 0;
}

END_NAMESPACE_YM_GBM


int
main(int argc,
     const char** argv)
{
  return nsYm::nsGbm::gbm_test(argc, argv);
}

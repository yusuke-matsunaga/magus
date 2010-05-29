
/// @file libym_cell/tests/mislib_test.cc
/// @brief MisLib のテスト用プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: mislib_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/StopWatch.h"
#include "ym_cell/MisLib.h"


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using nsYm::nsCell::MisLib;
  using nsYm::nsCell::MisLibGate;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 255;
  }
  
  MisLib mislib;
  
  list<string> err_list;
  
  bool stat = mislib.read(argv[1], err_list);
  if ( !stat ) {
    for (list<string>::iterator p = err_list.begin();
	 p != err_list.end(); ++ p) {
      cout << *p << endl;
    }
    return 1;
  }

  size_t nc = mislib.num();
  for (size_t i = 0; i < nc; ++ i) {
    const MisLibGate* gate = mislib.cell(i);
    gate->dump(cout);
  }
  
  return 0;
}

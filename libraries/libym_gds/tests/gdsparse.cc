
/// @file gdsprint/gdsparse.cc
/// @brief GDS-II ファイルのパーサーテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"
#include "ym_gds/GdsParser.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm::nsGds;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <gds2 filename>" << endl;
    return 1;
  }

  GdsParser parser;

  bool stat = parser.parse(argv[1]);
  if ( !stat ) {
    cerr << "Error!" << endl;
    return 2;
  }

  return 0;
}

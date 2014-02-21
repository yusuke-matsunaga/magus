
/// @file lcpatmgr_test.cc
/// @brief LcPatMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp/LcPatMgr.h"
#include "logic/LogExpr.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

int
lcpatmgr_test(int argc,
	      const char** argv)
{
  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " logical expression" << endl;
    return -1;
  }

  LcPatMgr mgr;

  mgr.init();

  istringstream is(argv[1]);
  LogExpr expr;
  string error_msg;
  if ( !expr.read_from_stream(is, error_msg) ) {
    cerr << error_msg << endl;
    return -1;
  }

  mgr.reg_pat(expr, 0);

  mgr.display(cout);

  return 0;
}

END_NAMESPACE_YM_CELL_LIBCOMP


int
main(int argc,
     const char** argv)
{
  using namespace nsYm::nsCell::nsLibcomp;

  return lcpatmgr_test(argc, argv);
}

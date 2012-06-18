
/// @file npnnodemgr_test.cc
/// @brief NpnNodeMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "NpnNodeMgr.h"
#include "NpnNode.h"


BEGIN_NAMESPACE_YM

void
npnnodemgr_test()
{
  NpnNodeMgr mgr;

  NpnHandle c0 = mgr.make_const0();
  NpnHandle c1 = mgr.make_const1();

  NpnHandle x0 = mgr.make_input(0);
  NpnHandle x1 = mgr.make_input(1);
  NpnHandle x2 = mgr.make_input(2);
  NpnHandle x3 = mgr.make_input(3);

  NpnHandle x2n = ~x2;

  NpnHandle and01 = mgr.make_and(x0, x1);
  NpnHandle and13 = mgr.make_and(x1, x3);
  NpnHandle and0n1n = mgr.make_and(~x0, ~x1);
  NpnHandle or1 = mgr.make_or(and01, and13);
  NpnHandle and23 = mgr.make_and(x2, x3);
  NpnHandle orand0123 = mgr.make_or(and01, and23);
  NpnHandle and0n2 = mgr.make_and(~x0, x2);
  NpnHandle mpx1 = mgr.make_or(and01, and0n2);
  NpnHandle and0n1 = mgr.make_and(~x0, x1);
  NpnHandle and02 = mgr.make_and(x0, x2);
  NpnHandle mpx2 = mgr.make_or(and0n1, and02);
  NpnHandle and012n = mgr.make_and(and01, ~x2);
  NpnHandle orand011n = mgr.make_or(and01, ~x1);
  NpnHandle and01n = mgr.make_and(x0, ~x1);
  NpnHandle orand01n1 = mgr.make_or(and01n, x1);

  cout << "const0" << endl;
  mgr.dump_handle(cout, c0);

  cout << "const1" << endl;
  mgr.dump_handle(cout, c1);

  cout << "x0" << endl;
  mgr.dump_handle(cout, x0);

  cout << "x1" << endl;
  mgr.dump_handle(cout, x1);

  cout << "~x2" << endl;
  mgr.dump_handle(cout, x2n);

  cout << "x0 & x1" << endl;
  mgr.dump_handle(cout, and01);

  cout << "x1 & x3" << endl;
  mgr.dump_handle(cout, and13);

  cout << "~x0 & ~x1" << endl;
  mgr.dump_handle(cout, and0n1n);

  cout << "(x0 & x1) | (x1 & x3)" << endl;
  mgr.dump_handle(cout, or1);

  cout << "(x0 & x1) | (x2 & x3)" << endl;
  mgr.dump_handle(cout, orand0123);

  cout << "~x0 & x2" << endl;
  mgr.dump_handle(cout, and0n2);

  cout << "(x0 & x1) | (~x0 & x2)" << endl;
  mgr.dump_handle(cout, mpx1);

  cout << "(~x0 & x1) | (x0 & x2)" << endl;
  mgr.dump_handle(cout, mpx2);

  cout << "(x0 & x1) & ~x2)" << endl;
  mgr.dump_handle(cout, and012n);

  cout << "(x0 & x1) | ~x1" << endl;
  mgr.dump_handle(cout, orand011n);

  cout << "(x0 & ~x1) | x1" << endl;
  mgr.dump_handle(cout, orand01n1);
}

END_NAMESPACE_YM

int
main(int argc,
     const char** argv)
{
  nsYm::npnnodemgr_test();

  return 0;
}

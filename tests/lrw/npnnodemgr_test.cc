
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

  NpnHandle and01 = mgr.make_and(x0, x1);
  NpnHandle and13 = mgr.make_and(x1, x3);
  NpnHandle and0n1n = mgr.make_and(~x0, ~x1);
  NpnHandle or1 = ~mgr.make_and(~and01, ~and13);

  vector<NpnHandle> handle_list;
  handle_list.push_back(c0);
  handle_list.push_back(c1);
  handle_list.push_back(x0);
  handle_list.push_back(x1);
  handle_list.push_back(x2);
  handle_list.push_back(x3);
  handle_list.push_back(and01);
  handle_list.push_back(and13);
  handle_list.push_back(and0n1n);
  handle_list.push_back(or1);

  mgr.dump_handle(cout, handle_list);
}

END_NAMESPACE_YM

int
main(int argc,
     const char** argv)
{
  nsYm::npnnodemgr_test();

  return 0;
}

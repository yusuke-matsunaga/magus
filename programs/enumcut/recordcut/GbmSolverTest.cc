
/// @file GbmSolverTest.cc
/// @brief GbmSolver のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSolver.h"
#include "GbmMgr.h"

#include "GbmNaive.h"


BEGIN_NAMESPACE_YM

bool
and9_test()
{
  GbmMgr mgr;

  vector<GbmNodeHandle> inputs(9);
  for (ymuint i = 0; i < 9; ++ i) {
    inputs[i] = mgr.new_input();
  }

  vector<GbmNodeHandle> fanin1(4);
  for (ymuint i = 0; i < 4; ++ i) {
    fanin1[i] = inputs[i];
  }
  GbmNodeHandle lut1 = mgr.new_lut(fanin1);

  for (ymuint i = 0; i < 4; ++ i) {
    fanin1[i] = inputs[i + 4];
  }
  GbmNodeHandle lut2 = mgr.new_lut(fanin1);

  vector<GbmNodeHandle> fanin2(3);
  fanin2[0] = lut1;
  fanin2[1] = lut2;
  fanin2[2] = inputs[8];
  GbmNodeHandle lut3 = mgr.new_lut(fanin2);

  TvFunc and9 = TvFunc::const_one(9);
  for (ymuint i = 0; i < 9; ++ i) {
    and9 &= TvFunc::posi_literal(9, VarId(i));
  }

  cout << "AND9 test" << endl;
  GbmNaive solver;
  vector<bool> conf_bits;
  vector<ymuint> iorder;
  bool stat = solver.solve(mgr, lut3, and9, conf_bits, iorder);
  if ( stat ) {
    cout << "Match succeed" << endl;
    ymuint nc = mgr.conf_var_num();
    for (ymuint i = 0; i < nc; ++ i) {
      cout << " C#" << i << ": " << conf_bits[i] << endl;
    }
  }
  else {
    cout << "Match failed" << endl;
  }

  return stat;
}

int
GbmSolverTest(int argc,
	      const char** argv)
{
  GbmMgr mgr;

  GbmNodeHandle i1 = mgr.new_input();
  GbmNodeHandle i2 = mgr.new_input();
  GbmNodeHandle i3 = mgr.new_input();

  vector<GbmNodeHandle> inputs1(2);
  inputs1[0] = i1;
  inputs1[1] = i2;
  GbmNodeHandle lut1 = mgr.new_lut(inputs1);

  vector<GbmNodeHandle> inputs2(2);
  inputs2[0] = i3;
  inputs2[1] = lut1;
  GbmNodeHandle lut2 = mgr.new_lut(inputs2);

  vector<int> tv(8);
  tv[0] = 0;
  tv[1] = 0;
  tv[2] = 1;
  tv[3] = 0;
  tv[4] = 0;
  tv[5] = 0;
  tv[6] = 0;
  tv[7] = 1;
  TvFunc func(3, tv);

  GbmNaive solver;
  vector<bool> conf_bits;
  vector<ymuint> iorder;
  bool stat = solver.solve(mgr, lut2, func, conf_bits, iorder);
  if ( stat ) {
    cout << "Match succeed" << endl;
    ymuint nc = mgr.conf_var_num();
    for (ymuint i = 0; i < nc; ++ i) {
      cout << " C#" << i << ": " << conf_bits[i] << endl;
    }
    ymuint ni = mgr.input_num();
    for (ymuint i = 0; i < ni; ++ i) {
      cout << " I#" << i << ": " << iorder[i] << endl;
    }
  }
  else {
    cout << "Match failed" << endl;
  }

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  using nsYm::and9_test;
  using nsYm::GbmSolverTest;

  and9_test();

  return GbmSolverTest(argc, argv);
}

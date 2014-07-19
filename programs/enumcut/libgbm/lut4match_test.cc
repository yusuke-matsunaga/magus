
/// @file lut4match_test.cc
/// @brief lut4match_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSatNaiveBinary.h"
#include "GbmSatNaiveOneHot.h"
#include "RcfNetwork.h"
#include "utils/RandGen.h"


BEGIN_NAMESPACE_YM

void
lut4match_test(int argc,
	       const char** argv)
{
  RandGen rg;

  GbmSatNaiveBinary solver;

  solver.verify_on();
  solver.debug_on();

  for (ymuint i = 0; i < 10; ++ i) {
    vector<int> tv(16);
    for (ymuint b = 0; b < 16; ++ b) {
      tv[b] = rg.int32() & 1;
    }

    TvFunc f(4, tv);

    RcfNetwork network;

    vector<RcfNodeHandle> inputs(4);
    for (ymuint i = 0; i < 4; ++ i) {
      inputs[i] = network.new_input();
    }

    RcfNodeHandle lut = network.new_lut(inputs);

    network.set_output(lut);

    vector<bool> conf_bits;
    vector<ymuint> iorder;
    ymuint loop_count;
    bool stat = solver.solve(network, f, conf_bits, iorder, loop_count);
    if ( !stat ) {
      cerr << "Failed" << endl
	   << "f = " << f << endl;
    }
  }
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  nsYm::lut4match_test(argc, argv);

  return 0;
}

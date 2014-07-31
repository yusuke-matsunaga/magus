
/// @file eval.cc
/// @brief 複数の IGU 構成法の比較を行うプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/PoptMainApp.h"
#include "RvMgr.h"
#include "RegVect.h"
#include "FuncVect.h"
#include "PhfGen.h"
#include "RandHashGen.h"
#include "InputFunc.h"


BEGIN_NAMESPACE_YM_IGF

int
phf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // xor オプション
  PoptUint popt_xor("xor", 'x',
		    "specify XOR complexity", "<INT>");
  app.add_option(&popt_xor);

  // count オプション
  PoptUint popt_count("count", 'c',
		      "specify loop limit", "<INT>");
  app.add_option(&popt_count);

  app.set_other_option_help("<filename>");

  // オプション解析を行う．
  tPoptStat stat = app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  ymuint32 comp = 1;
  if ( popt_xor.is_specified() ) {
    comp = popt_xor.val();
  }

  ymuint32 count_limit = 1000;
  if ( popt_count.is_specified() ) {
    count_limit = popt_count.val();
  }

  vector<string> args;
  ymuint n_args = app.get_args(args);

  if ( n_args != 1 ) {
    app.usage(2);
  }

  string base = args[0];
  for (ymuint num = 4500; num <= 10000; num += 500) {
    ostringstream buf;
    ymuint i = 0;
    buf << base << "-" << num << "-" << i;
    const char* f_str = buf.str().c_str();
    cerr << "file = " << f_str << endl;
    ifstream ifs(f_str);
    if ( !ifs ) {
      cerr << f_str << ": No such file" << endl;
      return 2;
    }

    RvMgr rvmgr;

    bool rstat = rvmgr.read_data(ifs);
    if ( !rstat ) {
      cerr << "read error" << endl;
      return 3;
    }

    ymuint n = rvmgr.vect_size();
    const vector<const RegVect*>& vlist = rvmgr.vect_list();
    ymuint nv = vlist.size();
    ymuint p = 1;
    {
      ymuint x = (nv + 1) / 3;
      for ( ; ; ++ p) {
	ymuint exp_p = 1U << p;
	if ( exp_p >= x ) {
	  break;
	}
      }
    }
    ++ p;
    ymuint exp_p = 1U << p;
    ymuint q = rvmgr.index_size();
    ymuint mem = (exp_p * (q + n - p) * 4);
    ymuint exp_q = 1U << q;
    ymuint imem = exp_q * n;
    cout << num;
    cout << " " << p << " " << mem << " " << imem << endl;
  }

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::phf;

  return phf(argc, argv);
}

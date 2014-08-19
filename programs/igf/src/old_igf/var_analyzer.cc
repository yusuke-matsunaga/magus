
/// @file var_analyzer.cc
/// @brief var_analyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/PoptMainApp.h"
#include "RvMgr.h"
#include "RegVect.h"
#include "RandHashGen.h"
#include "InputFunc.h"
#include "FuncVect.h"


BEGIN_NAMESPACE_YM_IGF

ymuint
calc_ds(InputFunc* f,
	ymuint exp_p,
	const vector<const RegVect*>& vect_list)
{
  vector<ymuint> hist(exp_p, 0);
  ymuint nv = vect_list.size();
  for (ymuint i = 0; i < nv; ++ i) {
    const RegVect* rv = vect_list[i];
    ymuint pos = f->eval(rv);
    ++ hist[pos];
  }
  ymuint ds = 0;
  for (ymuint i = 0; i < exp_p; ++ i) {
    ymuint h = hist[i];
    ds += h * h;
  }
  return ds;
}

int
igf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // slack オプション
  PoptUint popt_slack("slack", 's',
		      "specify SLACK parameter", "<INT>");
  app.add_option(&popt_slack);

  // xor オプション
  PoptUint popt_xor("xor", 'x',
		    "specify XOR complexity", "<INT>");
  app.add_option(&popt_xor);

  app.set_other_option_help("<filename>");

  // オプション解析を行う．
  tPoptStat stat = app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  ymuint32 slack = 0;
  if ( popt_slack.is_specified() ) {
    slack = popt_slack.val();
  }

  ymuint32 comp = 1;
  if ( popt_xor.is_specified() ) {
    comp = popt_xor.val();
  }

  vector<string> args;
  ymuint n_args = app.get_args(args);

  if ( n_args != 1 ) {
    app.usage(2);
  }

  const char* f_str = args[0].c_str();
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

  const vector<const RegVect*>& vect_list = rvmgr.vect_list();

  ymuint n = rvmgr.vect_size();

  ymuint nlimit = 2000;
  ymuint p = rvmgr.index_size() - 1;
  ymuint exp_p = 1U << p;

  RandHashGen rhg;

  double ds_all = 0.0;
  ymuint ds_best = 0;
  for (ymuint c = 0; c < nlimit; ++ c) {
    InputFunc* f = rhg.gen_func(n, p, comp);

    ymuint ds = calc_ds(f, exp_p, vect_list);

    delete f;
    if ( ds_all == 0 ) {
      ds_best = ds;
    }
    else {
      if ( ds_best > ds ) {
	ds_best = ds;
      }
    }
    ds_all += ds;
  }
  double ds_ave = ds_all / nlimit;
  ymuint nv = vect_list.size();
  double base = nv * (nv - 1);
  cout << (ds_ave / base) << endl;
  cout << (ds_best / base) << endl;

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::igf;

  return igf(argc, argv);
}

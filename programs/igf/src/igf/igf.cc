
/// @file igf.cc
/// @brief igf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "ym_utils/PoptMainApp.h"
#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "IguGen.h"
#include "ym_utils/Generator.h"


BEGIN_NAMESPACE_YM_IGF

int
igf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // multi オプション
  PoptUint popt_multi("multi", 'm',
		      "specify multiplexity", "<INT>");
  // xor オプション
  PoptUint popt_xor("xor", 'x',
		    "specify XOR complexity", "<INT>");
  // branch-limit オプション
  PoptUint popt_blimit("branch-limit", 'b',
		       "specify branch limit", "<INT>");
  // ordering-mode オプション
  PoptUint popt_omode("ordering-mode", 'o',
		      "set ordering mode", "<INT>");
  // time-limit オプション
  PoptUint popt_tlimit("time-limit", 't',
		       "set time limit", "<INT>(min)");
  // debug-level オプション
  PoptUint popt_debug("debug-level", 'd',
		      "set debug level", "<INT>");

  app.add_option(&popt_multi);
  app.add_option(&popt_xor);
  app.add_option(&popt_blimit);
  app.add_option(&popt_omode);
  app.add_option(&popt_tlimit);
  app.add_option(&popt_debug);

  app.set_other_option_help("<filename>");

  // オプション解析を行う．
  tPoptStat stat = app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  ymuint32 multi = 1;
  ymuint32 comp = 1;
  ymuint32 blimit = 0;
  ymuint32 omode = 0;
  ymuint32 tlimit = 0;
  ymuint32 debug = 0;

  if ( popt_multi.is_specified() ) {
    multi = popt_multi.val();
  }
  if ( popt_xor.is_specified() ) {
    comp = popt_xor.val();
  }
  if ( popt_blimit.is_specified() ) {
    blimit = popt_blimit.val();
  }
  if ( popt_omode.is_specified() ) {
    omode = popt_omode.val();
  }
  if ( popt_tlimit.is_specified() ) {
    tlimit = popt_tlimit.val();
  }
  if ( popt_debug.is_specified() ) {
    debug = popt_debug.val();
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

  ymuint n = rvmgr.vect_size();

  // Variable を生成
  vector<Variable*> var_list;
  for (ymuint i = 0; i < n; ++ i) {
    Variable* var = new Variable(i);
    var_list.push_back(var);
  }
  for (ymuint comp_i = 2; comp_i <= comp; ++ comp_i) {
    CombiGen cg(n, comp_i);

    for (CombiGen::iterator p = cg.begin(); !p.is_end(); ++ p) {
      vector<ymuint> vid_list(comp_i);
      for (ymuint j = 0; j < comp_i; ++ j) {
	vid_list[j] = p(j);
      }
      Variable* var = new Variable(vid_list);
      var_list.push_back(var);
    }
  }

  IguGen igu_gen;
  vector<Variable*> solution;

  if ( blimit > 0 ) {
    igu_gen.set_branch_limit(blimit);
  }
  if ( omode > 0 ) {
    igu_gen.set_ordering_mode(omode);
  }
  if ( tlimit > 0 ) {
    igu_gen.set_time_limit(tlimit, 0);
  }
  if ( debug > 0 ) {
    igu_gen.set_debug_level(debug);
  }

  igu_gen.solve(rvmgr.vect_list(), multi, var_list, solution);

  cout << "Variables = " << endl;
  for (vector<Variable*>::iterator p = solution.begin();
       p != solution.end(); ++ p) {
    Variable* var = *p;
    const vector<ymuint>& vid_list = var->vid_list();
    for (vector<ymuint>::const_iterator q = vid_list.begin();
	 q != vid_list.end(); ++ q) {
      cout << " " << *q;
    }
    cout << endl;
  }
  cout << endl;

  {
    ymuint n = rvmgr.vect_size();
    ymuint p = solution.size();
    ymuint q = rvmgr.index_size();
    ymuint exp_p = (1 << p);
    ymuint exp_q = (1 << q);
    cout << "p = " << p << endl
	 << "k = " << rvmgr.vect_list().size() << endl
	 << "q = " << q << endl;
    cout << "Total memory size = "
	 << exp_p << " x (" << n << " - " << p << " + " << q << ") x " << multi
	 << " = " << (exp_p * (n - p + q) * multi) << endl;
    cout << "Sasao's IGU       = "
	 << exp_p << " x " << q << " + " << exp_q << " x ("
	 << n << " - " << p << ") x " << multi
	 << " = " << ((exp_p * q + exp_q * (n - p)) * multi) << endl;
  }
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

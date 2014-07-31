
/// @file igf.cc
/// @brief igf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/PoptMainApp.h"
#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "IguGen.h"
#include "YmUtils/CombiGen.h"
#include "YmUtils/RandGen.h"
#include "YmUtils/RandPermGen.h"


BEGIN_NAMESPACE_YM_IGF

int
igf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // xor オプション
  PoptUint popt_xor("xor", 'x',
		    "specify XOR complexity", "<INT>");

  // recur-limit オプション
  PoptUint popt_rlimit("recur-limit", 'r',
		       "specify recursive call limit", "<INT>");

  // ordering-mode オプション
  PoptUint popt_omode("ordering-mode", 'o',
		      "set ordering mode", "<INT>");

  // time-limit オプション
  PoptUint popt_tlimit("time-limit", 't',
		       "set time limit", "<INT>(min)");

  // debug-level オプション
  PoptUint popt_debug("debug-level", 'd',
		      "set debug level", "<INT>");

  app.add_option(&popt_xor);
  app.add_option(&popt_rlimit);
  app.add_option(&popt_omode);
  app.add_option(&popt_tlimit);
  app.add_option(&popt_debug);

  app.set_other_option_help("<filename>");

  // オプション解析を行う．
  tPoptStat stat = app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  ymuint32 comp = 1;
  ymuint32 rlimit = 0;
  ymuint32 omode = 0;
  ymuint32 tlimit = 0;
  ymuint32 debug = 0;

  if ( popt_xor.is_specified() ) {
    comp = popt_xor.val();
  }
  if ( popt_rlimit.is_specified() ) {
    rlimit = popt_rlimit.val();
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

  IguGen igu_gen;

  if ( rlimit > 0 ) {
    igu_gen.set_recur_limit(rlimit);
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

  const vector<const RegVect*>& vect_list = rvmgr.vect_list();

  igu_gen.set_vector_list(vect_list);

  ymuint n = rvmgr.vect_size();

  // Variable を生成
  vector<const Variable*> var_list;
  for (ymuint i = 0; i < n; ++ i) {
    const Variable* var = new Variable(i);
    var_list.push_back(var);
  }
  for (ymuint comp_i = 2; comp_i <= comp; ++ comp_i) {
    for (CombiGen cg(n, comp_i); !cg.is_end(); ++ cg) {
      vector<ymuint> vid_list(comp_i);
      for (ymuint j = 0; j < comp_i; ++ j) {
	vid_list[j] = cg(j);
      }
      const Variable* var = new Variable(vid_list);
      var_list.push_back(var);
    }
  }

  vector<const Variable*> solution;
  igu_gen.solve(1, var_list, n + 1, solution);

  cout << "Variables = " << endl;
  for (vector<const Variable*>::iterator p = solution.begin();
       p != solution.end(); ++ p) {
    const Variable* var = *p;
    var->dump(cout);
    cout << endl;
  }
  cout << endl;

  {
    ymuint n = rvmgr.vect_size();
    ymuint k = rvmgr.vect_list().size();
    ymuint p = solution.size();
    ymuint q = rvmgr.index_size();
    ymuint exp_p = (1 << p);
    ymuint exp_q = (1 << q);
    cout << "p = " << p << endl
	 << "k = " << rvmgr.vect_list().size() << endl
	 << "q = " << q << endl;
    cout << "Total memory size = "
	 << exp_p << " x (" << n << " - " << p << " + " << q << ")"
	 << " = " << (exp_p * (n - p + q)) << endl;
    cout << "Sasao's IGU       = "
	 << exp_p << " x " << q << " + " << exp_q << " x ("
	 << n << " - " << p << ")"
	 << " = " << ((exp_p * q + exp_q * (n - p))) << endl;
    cout << "Ideal memory size = "
	 << k << " x (" << n << " + " << q << ") = "
	 << (k * (n + q)) << endl;
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

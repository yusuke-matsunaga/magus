
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
#include "IguGen2.h"
#include "YmUtils/CombiGen.h"


BEGIN_NAMESPACE_YM_IGF

int
igf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // xor オプション
  PoptUint popt_xor("xor", 'x',
		    "specify XOR complexity", "<INT>");
  // branch-limit オプション
  PoptUint popt_blimit("branch-limit", 'b',
		       "specify branch limit", "<INT>");
  // time-limit オプション
  PoptUint popt_tlimit("time-limit", 't',
		       "set time limit", "<INT>(min)");
  // debug-level オプション
  PoptUint popt_debug("debug-level", 'd',
		      "set debug level", "<INT>");

  app.add_option(&popt_xor);
  app.add_option(&popt_blimit);
  app.add_option(&popt_tlimit);
  app.add_option(&popt_debug);

  app.set_other_option_help("<filename>");

  // オプション解析を行う．
  tPoptStat stat = app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  ymuint32 comp = 1;
  ymuint32 blimit = 0;
  ymuint32 tlimit = 0;
  ymuint32 debug = 0;

  if ( popt_xor.is_specified() ) {
    comp = popt_xor.val();
  }
  if ( popt_blimit.is_specified() ) {
    blimit = popt_blimit.val();
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

  IguGen2 igu_gen;

  if ( blimit > 0 ) {
    igu_gen.set_branch_limit(blimit);
  }
  if ( tlimit > 0 ) {
    igu_gen.set_time_limit(tlimit, 0);
  }
  if ( debug > 0 ) {
    igu_gen.set_debug_level(debug);
  }

  ymuint n = rvmgr.vect_size();

  igu_gen.set_vector_list(rvmgr.vect_list());

#if 0
  // Variable を生成
  vector<const Variable*> var_list;
  for (ymuint i = 0; i < n; ++ i) {
    const Variable* var = new Variable(i);
    var_list.push_back(var);
  }

  vector<const Variable*> solution;
  ymuint best_so_far = n + 1;
  {
    cerr << "Only Input Variables" << endl;
    igu_gen.solve(multi, var_list, best_so_far, solution);
    best_so_far = solution.size();
    cerr << " => " << best_so_far << endl;
  }
  for (ymuint comp_i = 2; comp_i <= comp; ++ comp_i) {
    cerr << "With Compound Variables of degree " << comp_i << endl;
    for (CombiGen cg(n, comp_i); !cg.is_end(); ++ cg) {
      vector<ymuint> vid_list(comp_i);
      for (ymuint j = 0; j < comp_i; ++ j) {
	vid_list[j] = cg(j);
      }
      const Variable* var = new Variable(vid_list);
      var_list.push_back(var);
    }
    vector<const Variable*> solution1;
    igu_gen.solve(multi, var_list, best_so_far, solution1);
    ymuint ans = solution1.size();
    cerr << " => " << ans << endl;
    if ( best_so_far > ans ) {
      best_so_far = ans;
      solution = solution1;
    }
  }
#else
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
  igu_gen.solve(var_list, n + 1, solution);
#endif

  cout << "Variables = " << endl;
  for (vector<const Variable*>::iterator p = solution.begin();
       p != solution.end(); ++ p) {
    const Variable* var = *p;
    var->dump(cout);
    cout << endl;
  }
  cout << endl;

  {
    ymuint nv = solution.size();
    ymuint exp_p = (1 << nv);
    vector<ymuint> multi_dist(exp_p, 0);
    const vector<const RegVect*>& vect_list = rvmgr.vect_list();
    for (vector<const RegVect*>::const_iterator p = vect_list.begin();
	 p != vect_list.end(); ++ p) {
      const RegVect* vect = *p;
      ymuint sig = 0;
      for (ymuint i = 0; i < nv; ++ i) {
	const Variable* var = solution[i];
	if ( var->classify(vect) ) {
	  sig |= (1 << i);
	}
      }
      ++ multi_dist[sig];
    }
    vector<ymuint> hist(2, 0);
    for (ymuint i = 0; i < exp_p; ++ i) {
      ymuint m = multi_dist[i];
      ASSERT_COND( m <= 1 );
      ++ hist[m];
    }
    cout << "Distributions" << endl;
    for (ymuint i = 0; i <= 1; ++ i) {
      cout << i << ": " << hist[i] << endl;
    }
    cout << endl;
  }
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
	 << " = " << (exp_p * q + exp_q * (n - p)) << endl;
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


/// @file phf_fixed.cc
/// @brief phf_fixed の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/PoptMainApp.h"
#include "RvMgr.h"
#include "RegVect.h"

#include "FuncVect.h"
#include "VarFunc.h"
#include "XorFunc.h"
#include "PhfGen.h"

#include "Variable.h"
#include "IguGen.h"
#include "YmUtils/CombiGen.h"
#include "YmUtils/RandGen.h"
#include "YmUtils/RandPermGen.h"
#include "YmUtils/RandCombiGen.h"


BEGIN_NAMESPACE_YM_IGF

int
phf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // d オプション
  PoptNone popt_d("display", 'd',
		  "display the result");

  // xor オプション
  PoptUint popt_xor("xor", 'x',
		    "specify XOR complexity", "<INT>");

  // recur-limit オプション
  PoptUint popt_rlimit("recur-limit", 'r',
		       "specify recursive call limit", "<INT>");

  app.add_option(&popt_d);
  app.add_option(&popt_xor);
  app.add_option(&popt_rlimit);

  app.set_other_option_help("<filename>");

  // オプション解析を行う．
  tPoptStat stat = app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  ymuint32 comp = 1;
  ymuint32 rlimit = 0;

  if ( popt_xor.is_specified() ) {
    comp = popt_xor.val();
  }
  if ( popt_rlimit.is_specified() ) {
    rlimit = popt_rlimit.val();
  }

  bool disp = popt_d.is_specified();

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

  vector<const Variable*> solution;
  { // distingushing signature を求める．
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

    IguGen igu_gen;

    if ( rlimit > 0 ) {
      igu_gen.set_recur_limit(rlimit);
    }

    igu_gen.set_vector_list(rvmgr.vect_list());
    igu_gen.solve(1, var_list, n + 1, solution);
  }
  ymuint ns = solution.size();

  ymuint p = rvmgr.index_size();

  cout << "ns = " << ns << ", p = " << p << endl;

  ymuint count_limit = 10000;
  RandGen rg;
  for ( ; ; ++ p) {
    bool found = false;
    for (ymuint count = 0; count < count_limit; ++ count) {
      const vector<const RegVect*>& vlist = rvmgr.vect_list();
      ymuint exp_p = 1U << p;
      vector<const FuncVect*> func_list(2);
      { // f1 と f2 を作る．
	vector<vector<ymuint32> > f1_vect(p);
	vector<vector<ymuint32> > f2_vect(p);
	if ( ns < p ) {
	  // ほとんど無意味だけど一応用意しておく．
	  for (ymuint k = 0; k < ns; ++ k) {
	    f1_vect[k] = solution[k]->vid_list();
	  }
	  for (ymuint k = ns; k < p; ++ k) {
	    // ダミー
	    f1_vect[k].push_back(0);
	  }
	}
	else {
	  ymuint nr = ns - p;
#if 1
	  RandCombiGen rcg1(ns, nr);
	  rcg1.generate(rg);
	  vector<bool> mark(ns, false);
	  for (ymuint k = 0; k < nr; ++ k) {
	    ymuint pos = rcg1.elem(k);
	    f1_vect[k] = solution[pos]->vid_list();
	    mark[pos] = true;
	  }
	  vector<ymuint> r_list;
	  r_list.reserve(p);
	  for (ymuint i = 0; i < ns; ++ i) {
	    if ( !mark[i] ) {
	      r_list.push_back(i);
	    }
	  }
	  assert_cond( r_list.size() == p, __FILE__, __LINE__);
	  RandCombiGen rcg2(p, nr);
	  rcg2.generate(rg);
	  for (ymuint i = 0; i < nr; ++ i) {
	    ymuint pos = r_list[rcg2.elem(i)];
	    f2_vect[i] = solution[pos]->vid_list();
	    mark[pos] = true;
	  }
	  vector<ymuint> r_list2;
	  r_list2.reserve(ns - nr - nr);
	  for (ymuint i = 0; i < ns; ++ i) {
	    if ( !mark[i] ) {
	      r_list2.push_back(i);
	    }
	  }
	  for (ymuint i = nr; i < p; ++ i) {
	    ymuint pos = r_list2[i - nr];
	    f1_vect[i] = solution[pos]->vid_list();
	    f2_vect[i] = solution[pos]->vid_list();
	  }
#else
	  RandPermGen rpg(ns);
	  rpg.generate(rg);
	  for (ymuint k = 0; k < p; ++ k) {
	    ymuint pos = rpg.elem(k);
	    f1_vect[k] = solution[pos]->vid_list();
	  }
	  for (ymuint k = 0; k < nr; ++ k) {
	    ymuint pos = rpg.elem(k + p);
	    f2_vect[k] = solution[pos]->vid_list();
	  }
	  RandPermGen rpg2(n);
	  rpg2.generate(rg);
	  for (ymuint k = nr; k < p; ++ k) {
	    ymuint pos = rpg2.elem(k - nr);
	    f2_vect[k].push_back(pos);
	  }
#endif
	}

	ymuint nv = vlist.size();

	XorFunc f1(f1_vect);
	FuncVect* fv1 = new FuncVect(nv, exp_p);
	func_list[0] = fv1;
	for (ymuint i = 0; i < nv; ++ i) {
	  const RegVect* rv = vlist[i];
	  fv1->set_val(i, f1.eval(rv));
	}

	XorFunc f2(f2_vect);
	FuncVect* fv2 = new FuncVect(nv, exp_p);
	func_list[1] = fv2;
	for (ymuint i = 0; i < nv; ++ i) {
	  const RegVect* rv = vlist[i];
	  fv2->set_val(i, f2.eval(rv));
	}
      }

      PhfGen phfgen;

      vector<vector<ymuint32> > g_list(2);
      bool stat1 = phfgen.mapping(func_list, g_list);

      {
	delete func_list[0];
	delete func_list[1];
      }

      if ( stat1 ) {
	cout << "p = " << p << endl;
	ymuint q = rvmgr.index_size();
	cout << "Total memory size = "
	     << (exp_p * q * 2 + vlist.size() * n) << endl;
	if ( disp ) {
	  ymuint nv = vlist.size();
	  for (ymuint i = 0; i < nv; ++ i) {
	    cout << "#" << i << ": ";
	    const char* comma = "";
	    ymuint32 val = 0;
	    for (ymuint j = 0; j < 2; ++ j) {
	      const FuncVect& f1 = *func_list[j];
	      vector<ymuint32>& g1 = g_list[j];
	      ymuint32 v1 = f1.val(i);
	      cout << comma << setw(6) << v1 << " = " << g1[v1];
	      val ^= g1[v1];
	      comma = ", ";
	    }
	    cout << ": " << val << endl;
	  }
	}
	found = true;
	break;
      }
    }
    if ( found ) {
      break;
    }
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

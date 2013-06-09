
/// @file migf.cc
/// @brief migf の実装ファイル
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
#include "ym_utils/CombiGen.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/RandPermGen.h"


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

  app.add_option(&popt_multi);
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

  ymuint32 multi = 1;
  ymuint32 comp = 1;
  ymuint32 rlimit = 0;
  ymuint32 omode = 0;
  ymuint32 tlimit = 0;
  ymuint32 debug = 0;

  if ( popt_multi.is_specified() ) {
    multi = popt_multi.val();
  }
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
  igu_gen.solve(multi, var_list, n + 1, solution);

  // 最終的な分割を作る．
  ymuint np = solution.size();
  ymuint np_exp = 1U << np;
  vector<const RegVect*> lut_array(np_exp * multi, NULL);
  for (vector<const RegVect*>::const_iterator p = vect_list.begin();
       p != vect_list.end(); ++ p) {
    const RegVect* vect = *p;
    ymuint id = 0U;
    for (ymuint i = 0; i < np; ++ i) {
      const Variable* var = solution[i];
      if ( var->classify(vect) ) {
	id |= (1U << i);
      }
    }
    ymuint base = id * multi;
    for (ymuint i = 0; i < multi; ++ i) {
      if ( lut_array[base + i] == NULL ) {
	lut_array[base + i] = vect;
	break;
      }
    }
  }

  vector<ymuint> hist_array(multi + 1, 0);
  {
    for (ymuint j = 0; j < np_exp; ++ j) {
      ymuint base = j * multi;
      ymuint nelem = 0;
      for ( ; nelem < multi; ++ nelem) {
	if ( lut_array[base + nelem] == NULL ) {
	  break;
	}
      }
      ++ hist_array[nelem];
    }
    cout << " Distribution" << endl;
    for (ymuint i = 0; i <= multi; ++ i) {
      cout << "  " << i << ": " << hist_array[i] << endl;
    }
    cout << endl;
  }

  RandGen rg;
  ymuint min_size = 0;
  vector<ymuint> p_array(multi);
  for (ymuint l = 0; l < 1; ++ l) {
    ymuint q = rvmgr.index_size();
    vector<const RegVect*> lut_array1(np_exp * multi, NULL);
    for (ymuint j = 0; j < np_exp; ++ j) {
      ymuint base = j * multi;
      ymuint nelem = 0;
      for ( ; nelem < multi; ++ nelem) {
	if ( lut_array[base + nelem] == NULL ) {
	  break;
	}
      }
      if ( nelem > 0 ) {
	RandPermGen rpg(nelem);
	rpg.generate(rg);
	for (ymuint i = 0; i < nelem; ++ i) {
	  ymuint pos = rpg.elem(i);
	  lut_array1[base + i] = lut_array[base + pos];
	}
      }
    }

    vector<ymuint> p_array1(multi);
    vector<ymuint> n_array(multi);
    for (ymuint i = 0; i < multi; ++ i) {
      vector<const RegVect*> vect_list1;
      vect_list1.reserve(np_exp);
      for (ymuint j = 0; j < np_exp; ++ j) {
	const RegVect* vect = lut_array1[j * multi + i];
	if ( vect != NULL ) {
	  vect_list1.push_back(vect);
	}
      }
      n_array[i] = vect_list1.size();
      if ( vect_list1.empty() ) {
	p_array1[i] = 0;
      }
      else {
	IguGen igu_gen1;

	if ( omode > 0 ) {
	  igu_gen1.set_ordering_mode(omode);
	}
	if ( tlimit > 0 ) {
	  igu_gen1.set_time_limit(tlimit, 0);
	}
	if ( debug > 0 ) {
	  igu_gen1.set_debug_level(debug);
	}

	igu_gen1.set_vector_list(vect_list1);

	vector<const Variable*> solution1;
	igu_gen1.solve(1, solution, np + 2, solution1);

	if ( rlimit > 0 ) {
	  igu_gen1.set_recur_limit(rlimit);
	  vector<const Variable*> solution2;
	  igu_gen1.solve(1, var_list, solution1.size() + 2, solution2);
	  if ( solution2.size() > 0 && solution2.size() < solution1.size() ) {
	    solution1 = solution2;
	  }
	}

	p_array1[i] = solution1.size();
      }
    }

    ymuint t = 0;
    for (ymuint i = 0; i < multi; ++ i) {
      ymuint p = p_array1[i];
      if ( p > 0 ) {
	ymuint exp_p = 1 << p;
	t += (exp_p * (n - p + q));
      }
    }
    {
      cout << "n = ";
      for (ymuint i = 0; i < multi; ++ i) {
	cout << " " << n_array[i];
      }
      cout << endl;

      cout << "p = ";
      for (ymuint i = 0; i < multi; ++ i) {
	cout << " " << p_array1[i];
      }
      cout << endl;

      cout << "Memory Size = " << t << endl
	   << endl;
    }
    if ( min_size == 0 || min_size > t ) {
      min_size = t;
      for (ymuint i = 0; i < multi; ++ i) {
	p_array[i] = p_array1[i];
      }
    }
  }

  {
    ymuint n = rvmgr.vect_size();
    ymuint k = rvmgr.vect_list().size();
    ymuint q = rvmgr.index_size();
    ymuint exp_q = (1 << q);
    ymuint t = 0;
    ymuint t1 = 0;
    for (ymuint i = 0; i < multi; ++ i) {
      ymuint p = p_array[i];
      if ( p > 0 ) {
	ymuint exp_p = 1 << p;
	t += (exp_p * (n - p + q));
	t1 += (exp_p * q) + exp_q * (n - q);
      }
    }
    cout << endl
	 << "page size = ";
    for (ymuint i = 0; i < multi; ++ i) {
      cout << " " << p_array[i];
    }
    cout << endl;
    cout << "Total memory size = "
	 << t << endl
	 << "Sasao's Parallel IGU = "
	 << t1 << endl
	 << "Ideal memory size = "
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


/// @file igugen.cc
/// @brief igugen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "VarHeap.h"
#include "RandHashGen.h"
#include "InputFunc.h"
#include "XorFunc.h"
#include "FuncVect.h"
#include "IguGen.h"
#include "YmUtils/PoptMainApp.h"
#include "YmUtils/RandCombiGen.h"


BEGIN_NAMESPACE_YM_IGF

struct Lt
{
  bool
  operator()(const pair<ymuint, Variable*>& left,
	     const pair<ymuint, Variable*>& right)
  {
    return left.first > right.first;
  }
};


int
igugen(int argc,
       const char** argv)
{
  PoptMainApp main_app;

  // m オプション
  PoptUint popt_m("mult", 'm',
		  "specify the number of hash functions", "<INT>");
  main_app.add_option(&popt_m);

  // n オプション
  PoptNone popt_n("naive", 'n',
		  "naive parallel sieve method");
  main_app.add_option(&popt_n);

  // x オプション
  PoptNone popt_x("xor", 'x', "linear transformation");
  main_app.add_option(&popt_x);

  // c オプション
  PoptUint popt_c("compose", 'c',
		  "compose mode", "max_degree");
  main_app.add_option(&popt_c);

  // l オプション
  PoptUint popt_l("count_limit", 'l',
		  "specify count limit", "<INT>");
  main_app.add_option(&popt_l);

  main_app.set_other_option_help("<filename>");

  tPoptStat stat = main_app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return 1;
  }

  vector<string> args;
  ymuint n_args = main_app.get_args(args);
  if ( n_args != 1 ) {
    main_app.usage(1);
  }

  RvMgr rv_mgr;

  ifstream ifs(args[0]);
  if ( !ifs ) {
    cerr << "Could not open " << args[0] << endl;
    return 1;
  }

  if ( !rv_mgr.read_data(ifs) ) {
    cerr << "Error in reading " << args[0] << endl;
    return 1;
  }

  cout << "# of inputs:     " << rv_mgr.vect_size() << endl
       << "# of vectors:    " << rv_mgr.vect_list().size() << endl
       << "# of index bits: " << rv_mgr.index_size() << endl;

  vector<Variable*> var_list;
  if ( popt_x.is_specified() ) {
    ymuint ni = rv_mgr.vect_size();
    VarHeap var_set(ni);
    const vector<const RegVect*>& v_list = rv_mgr.vect_list();
    for (ymuint i = 0; i < ni; ++ i) {
      Variable* var1 = new Variable(i);
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (vector<const RegVect*>::const_iterator p = v_list.begin();
	   p != v_list.end(); ++ p) {
	const RegVect* rv = *p;
	if ( var1->classify(rv) ) {
	  ++ n1;
	}
	else {
	  ++ n0;
	}
      }
      if ( n0 > 0 && n1 > 0 ) {
	ymuint n2 = n0 * n1;
	var_set.put(var1, n2);
      }
      else {
	delete var1;
      }
    }

    for ( ; ; ) {
      ymuint n_old = var_set.value(0);
      Variable* var_old = var_set.var(0);
      const vector<ymuint>& vid_list0 = var_old->vid_list();
      vector<bool> used(ni, false);
      for (vector<ymuint>::const_iterator p = vid_list0.begin();
	   p != vid_list0.end(); ++ p) {
	used[*p] = true;
      }
      ymuint max_n = n_old + 1;
      vector<Variable*> max_vars;
      for (ymuint i = 0; i < ni; ++ i) {
	if ( used[i] ) {
	  continue;
	}
	vector<ymuint> vid_list1(vid_list0);
	vid_list1.push_back(i);
	sort(vid_list1.begin(), vid_list1.end());
	bool found = false;
	for (ymuint j = 0; j < var_set.size(); ++ j) {
	  Variable* var1 = var_set.var(j);
	  if ( var1->vid_list() == vid_list1 ) {
	    found = true;
	    break;
	  }
	}
	if ( found ) {
	  continue;
	}
	Variable* var1 = new Variable(vid_list1);
	ymuint n0 = 0;
	ymuint n1 = 0;
	for (vector<const RegVect*>::const_iterator p = v_list.begin();
	     p != v_list.end(); ++ p) {
	  const RegVect* rv = *p;
	  if ( var1->classify(rv) ) {
	    ++ n1;
	  }
	  else {
	    ++ n0;
	  }
	}
	ymuint n2 = n0 * n1;
	if ( max_n < n2 ) {
	  max_n = n2;
	  for (vector<Variable*>::iterator p = max_vars.begin();
	       p != max_vars.end(); ++ p) {
	    delete *p;
	  }
	  max_vars.clear();
	  max_vars.push_back(var1);
	}
	else if ( max_n == n2 ) {
	  max_vars.push_back(var1);
	}
	else {
	  delete var1;
	}
      }
      if ( !max_vars.empty() ) {
	ymuint max_min_n = 0;
	Variable* max_var = NULL;
	for (vector<Variable*>::iterator p = max_vars.begin();
	     p != max_vars.end(); ++ p) {
	  Variable* var1 = *p;
	  ymuint n00 = 0;
	  ymuint n01 = 0;
	  ymuint n10 = 0;
	  ymuint n11 = 0;
	  for (ymuint j = 0; j < var_set.size(); ++ j) {
	    Variable* var2 = var_set.var(j);
	    for (vector<const RegVect*>::const_iterator p = v_list.begin();
		 p != v_list.end(); ++ p) {
	      const RegVect* rv = *p;
	      if ( var1->classify(rv) ) {
		if ( var2->classify(rv) ) {
		  ++ n11;
		}
		else {
		  ++ n10;
		}
	      }
	      else {
		if ( var2->classify(rv) ) {
		  ++ n01;
		}
		else {
		  ++ n00;
		}
	      }
	    }
	  }
	  ymuint min_n = n00;
	  if ( min_n > n01 ) {
	    min_n = n01;
	  }
	  if ( min_n > n10 ) {
	    min_n = n10;
	  }
	  if ( min_n > n11 ) {
	    min_n = n11;
	  }
	  if ( max_min_n < min_n ) {
	    max_min_n = min_n;
	    delete max_var;
	    max_var = var1;
	  }
	}
	var_set.get_min();
	var_set.put(max_var, max_n);
      }
      else {
	break;
      }
    }
    for (ymuint i = 0; i < var_set.size(); ++ i) {
      var_list.push_back(var_set.var(i));
    }
  }
  else {
    ymuint ni = rv_mgr.vect_size();
    const vector<const RegVect*>& v_list = rv_mgr.vect_list();
    for (ymuint i = 0; i < ni; ++ i) {
      Variable* var1 = new Variable(i);
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (vector<const RegVect*>::const_iterator p = v_list.begin();
	   p != v_list.end(); ++ p) {
	const RegVect* rv = *p;
	if ( var1->classify(rv) ) {
	  ++ n1;
	}
	else {
	  ++ n0;
	}
      }
      if ( n0 > 0 && n1 > 0 ) {
	var_list.push_back(var1);
      }
      else {
	delete var1;
      }
    }
  }

  ymuint comp = 1;
  if ( popt_c.is_specified() ) {
    comp = popt_c.val();
  }

  ymuint m = 1;
  if ( popt_m.is_specified() ) {
    m = popt_m.val();
  }

  bool naive = popt_n.is_specified();

  ymuint count_limit = 1000;
  if ( popt_l.is_specified() ) {
    count_limit = popt_l.val();
  }

  ymuint n = rv_mgr.vect_size();
  ymuint p = rv_mgr.index_size();

  ymuint p1 = p;
  {
    for (ymuint tmp_m = 1; tmp_m < m; ) {
      -- p1;
      tmp_m <<= 1;
    }
  }

  IguGen pg;

  RandHashGen rhg;
  RandGen rg;
  for ( ; ; ++ p1) {
    bool found = false;
    RandCombiGen rcg1(var_list.size(), p1);
    for (ymuint count = 0; count < count_limit; ++ count) {
      vector<const FuncVect*> fv_list(m);
      for (ymuint i = 0; i < m; ++ i) {
	InputFunc* f = NULL;
	if ( popt_c.is_specified() ) {
	  f = rhg.gen_func(n, p1, comp);
	}
	else {
	  rcg1.generate(rg);
	  vector<vector<ymuint32> > vars_list;
	  for (ymuint j = 0; j < p1; ++ j) {
	    ymuint idx = rcg1.elem(j);
	    Variable* var1 = var_list[idx];
	    const vector<ymuint>& vid_list = var1->vid_list();
	    vars_list.push_back(vector<ymuint32>(vid_list.size()));
	    for (ymuint k = 0; k < vid_list.size(); ++ k) {
	      vars_list[j][k] = vid_list[k];
	    }
	  }
	  f = new XorFunc(vars_list);
	}
	fv_list[i] = rv_mgr.gen_hash_vect(*f);
	delete f;
      }

      vector<ymuint> block_map;
      bool stat = false;
      if ( naive || m == 1 ) {
	stat = pg.naive_partition(fv_list, block_map);
      }
      else {
	stat = pg.cf_partition(fv_list, block_map);
      }
      for (ymuint i = 0; i < m; ++ i) {
	delete fv_list[i];
      }
      if ( stat ) {
	found = true;
#if 0
	ymuint exp_p = 1U << p1;
	for (ymuint i = 0; i < m; ++ i) {
	  cout << "Block#" << i << endl;
	  for (ymuint j = 0; j < exp_p; ++ j) {
	    cout << " " << map_list[i][j] << endl;
	  }
	}
#endif
	break;
      }
    }
    if ( found ) {
      break;
    }
  }

  ymuint exp_p = 1U << p1;
  ymuint q = rv_mgr.index_size();
  cout << " p = " << p1 << endl
       << "Total memory size = "
       << (exp_p * (q + n - p1) * m) << endl;

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  return nsYm::nsIgf::igugen(argc, argv);
}

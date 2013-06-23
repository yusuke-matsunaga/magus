
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
#include "FuncVect.h"
#include "PhfGen.h"
#include "VarFunc.h"

#include "Variable.h"
#include "IguGen.h"
#include "ym_utils/CombiGen.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/RandCombiGen.h"


BEGIN_NAMESPACE_YM_IGF

int
phf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // m オプション
  PoptUint popt_m("mult", 'm',
		  "specify the number of hash functions", "<INT>");
  app.add_option(&popt_m);

  // d オプション
  PoptNone popt_d("display", 'd',
		  "display the result");
  app.add_option(&popt_d);

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

  ymuint32 m = 2;
  if ( popt_m.is_specified() ) {
    m = popt_m.val();
  }

  bool disp = popt_d.is_specified();

  ymuint32 count_limit = 10000;
  if ( popt_count.is_specified() ) {
    count_limit = popt_count.val();
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

  ymuint p = rvmgr.index_size();

  RandGen rg;
  for (bool found = false; !found ; ++ p) {
    for (ymuint count = 0; count < count_limit; ++ count) {
      const vector<const RegVect*>& vlist = rvmgr.vect_list();
      ymuint nv = vlist.size();
      ymuint exp_p = 1U << p;
      RandCombiGen rpg2(n, p);
      vector<const FuncVect*> func_list(m);
      for (ymuint i = 0; i < m; ++ i) {
	rpg2.generate(rg);
	vector<ymuint> f1_vect(p);
	for (ymuint k = 0; k < p; ++ k) {
	  f1_vect[k] = rpg2.elem(k);
	}
	VarFunc f(f1_vect);

	FuncVect* fv = new FuncVect(exp_p, nv);
	func_list[i] = fv;
	for (ymuint v = 0; v < nv; ++ v) {
	  const RegVect* rv = vlist[v];
	  fv->set_val(v, f.eval(rv));
	}
      }

      PhfGen phfgen;

      vector<vector<ymuint32>* > g_list(m);
      for (ymuint i = 0; i < m; ++ i) {
	g_list[i] = new vector<ymuint32>(exp_p, 0U);
      }
      bool stat = phfgen.mapping(func_list, g_list);
      if ( stat ) {
	found = true;
	cout << "p = " << p << endl;
	ymuint q = rvmgr.index_size();
	cout << "Total memory size = "
	     << (exp_p * q * m + vlist.size() * n) << endl;
	if ( disp ) {
	  ymuint nv = vlist.size();
	  for (ymuint i = 0; i < nv; ++ i) {
	    cout << "#" << i << ": ";
	    const char* comma = "";
	    ymuint32 val = 0;
	    for (ymuint j = 0; j < m; ++ j) {
	      const FuncVect& f1 = *func_list[j];
	      vector<ymuint32>& g1 = *g_list[j];
	      ymuint32 v1 = f1.val(i);
	      cout << comma << setw(6) << v1 << " = " << g1[v1];
	      val ^= g1[v1];
	      comma = ", ";
	    }
	    cout << ": " << val << endl;
	  }
	}
      }
      for (ymuint i = 0; i < m; ++ i) {
	delete func_list[i];
	delete g_list[i];
      }
      if ( stat ) {
	break;
      }
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

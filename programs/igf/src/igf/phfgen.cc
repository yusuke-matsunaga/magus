
/// @file phfgen.cc
/// @brief phfgen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "RvMgr.h"
#include "RegVect.h"

#include "VarFunc.h"
#include "XorFunc.h"
#include "PhfGen.h"


BEGIN_NAMESPACE_YM_IGF

int
phf(int argc,
    const char** argv)
{

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

  RandGen rg;
  for (ymuint count = 0; count < count_limit; ++ count) {
    RandCombiGen rpg2(n, p);
    vector<const InputFunc*> func_list(m);
    for (ymuint i = 0; i < m; ++ i) {
      rpg2.generate(rg);
      vector<ymuint> f1_vect(p);
      for (ymuint k = 0; k < p; ++ k) {
	f1_vect[k] = rpg2.elem(k);
      }
      VarFunc* f = new VarFunc(f1_vect);
      func_list[i] = f;
    }

    PhfGen phfgen;

    const vector<RegVect*>& vlist = rvmgr.vect_list();
    ymuint np = 1U << p;
    vector<vector<ymuint32>* > g_list(m);
    for (ymuint i = 0; i < m; ++ i) {
      g_list[i] = new vector<ymuint32>(np, 0U);
    }
    bool stat = phfgen.mapping(vlist, func_list, g_list);
    if ( stat ) {
      ymuint nv = vlist.size();
      for (ymuint i = 0; i < nv; ++ i) {
	RegVect* rv = vlist[i];
	cout << "#" << i << ": ";
	const char* comma = "";
	ymuint32 val = 0;
	for (ymuint j = 0; j < m; ++ j) {
	  const InputFunc& f1 = *func_list[j];
	  vector<ymuint32>& g1 = *g_list[j];
	  ymuint32 v1 = f1.eval(rv);
	  cout << comma << setw(6) << v1 << " = " << g1[v1];
	  val ^= g1[v1];
	  comma = ", ";
	}
	cout << ": " << val << endl;
      }
      break;
    }
    for (ymuint i = 0; i < m; ++ i) {
      delete func_list[i];
      delete g_list[i];
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

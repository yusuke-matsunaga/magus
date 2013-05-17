
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

#include "VarFunc.h"
#include "PhfGen.h"

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

  // p オプション
  PoptUint popt_p(NULL, 'p',
		  "specify the number of inputs", "<INT>");
  app.add_option(&popt_p);

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

  ymuint32 p = 0;
  if ( popt_p.is_specified() ) {
    p = popt_p.val();
  }

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

#if 0
  // Variable を生成
  vector<Variable*> var_list;
  for (ymuint i = 0; i < n; ++ i) {
    Variable* var = new Variable(i);
    var_list.push_back(var);
  }


  vector<Variable*> solution;
  IguGen igu_gen;
  igu_gen.solve(rvmgr.vect_list(), 1, var_list, n + 1, solution);

  ymuint core_size = solution.size();

  cout << "IGU gen end: core_size = " << core_size << endl;

  if ( p * 2 < core_size ) {
    cerr << "p is too small" << endl;
    return 4;
  }
  if ( p > 31 ) {
    cerr << "p is too big" << endl;
    return 4;
  }

  vector<ymuint> core_list(core_size);
  vector<bool> core_map(n, false);
  for (ymuint i = 0; i < core_size; ++ i) {
    ymuint vid = solution[i]->vid();
    core_list[i] = vid;
    core_map[vid] = true;
  }
  vector<ymuint> rest_list;
  rest_list.reserve(n - core_size);
  for (ymuint i = 0; i < n; ++ i) {
    if ( core_map[i] == false ) {
      rest_list.push_back(i);
    }
  }
  assert_cond( rest_list.size() == n - core_size, __FILE__, __LINE__);
#endif

  RandGen rg;
  for (ymuint count = 0; count < count_limit; ++ count) {
#if 0
    ymuint nrest = p * 2 - core_size;
    cout << "nrest = " << nrest << endl
	 << "n - core_size = " << (n - core_size) << endl;
    // ランダムに nrest 個の変数を選ぶ．
    RandCombiGen rpg1(n - core_size, nrest);

    rpg1.generate(rg);

    vector<ymuint> tmp_list(p * 2);
    for (ymuint i = 0; i < core_size; ++ i) {
      tmp_list[i] = core_list[i];
    }
    for (ymuint i = 0; i < nrest; ++ i) {
      tmp_list[i + core_size] = rpg1.elem(i);
    }
#endif

    // tmp_list から f1, f2 をランダムに作る．
    RandCombiGen rpg2(n, p);

    rpg2.generate(rg);
    vector<ymuint> f1_vect(p);
    for (ymuint i = 0; i < p; ++ i) {
      f1_vect[i] = rpg2.elem(i);
    }

    rpg2.generate(rg);
    vector<ymuint> f2_vect(p);
    for (ymuint i = 0; i < p; ++ i) {
      f2_vect[i] = rpg2.elem(i);
    }

    PhfGen phfgen;

    const vector<RegVect*>& vlist = rvmgr.vect_list();
    VarFunc f1(f1_vect);
    VarFunc f2(f2_vect);
    ymuint np = 1U << p;
    vector<ymuint32> g1(np, 0U);
    vector<ymuint32> g2(np, 0U);
    bool stat = phfgen.mapping(vlist, f1, f2, g1, g2);
    if ( !stat ) {
      continue;
    }

    ymuint nv = vlist.size();
    for (ymuint i = 0; i < nv; ++ i) {
      RegVect* rv = vlist[i];
      ymuint32 v1 = f1.eval(rv);
      ymuint32 v2 = f2.eval(rv);
      cout << "#" << i << ": "
	   << setw(6) << v1 << " = " << g1[v1]
	   << ", "
	   << setw(6) << v2 << " = " << g2[v2]
	   << ": "
	   << (g1[v1] ^ g2[v2]) << endl;
    }
    break;
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

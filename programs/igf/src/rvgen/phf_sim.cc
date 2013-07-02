
/// @file phf_sim.cc
/// @brief phf_sim のソースファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "FuncVect.h"
#include "PhfGen.h"
#include "PhfGraph.h"
#include "ym_utils/CombiGen.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

void
set_random_func(FuncVect* fv,
		RandGen& rg)
{
  ymuint n = fv->input_size();
  ymuint k = fv->max_val();
  for (ymuint i = 0; i < n; ++ i) {
    ymuint val = rg.int32() % k;
    fv->set_val(i, val);
  }
}

ymuint
phf_sim1(const vector<const FuncVect*>& func_list)
{
  PhfGraph pg(func_list);

  ymuint ans = 0;
  if ( !pg.simple_check() ) {
    ans |= 1;
  }

  vector<PhfEdge*> edge_list;
  if ( !pg.acyclic_check(edge_list) ) {
    ans |= 2;
  }

  return ans;
}

bool
phf_sim2(const vector<const FuncVect*>& func_list)
{
  vector<ymuint> block_map;

  PhfGraph pg(func_list);
  return pg.split_check(block_map);
}

bool
phf_sim_recur(ymuint n,
	      ymuint k,
	      ymuint m,
	      const vector<const FuncVect*>& func_list,
	      RandGen& rg)
{
  ymuint nf = func_list.size();
  if ( nf == m ) {
    ymuint stat = phf_sim1(func_list);
    return (stat == 0);
  }

  if ( nf > 1 ) {
    PhfGraph pg(func_list);
    vector<PhfEdge*> edge_list;
    if ( !pg.acyclic_check(edge_list) ) {
      return false;
    }
  }

  vector<const FuncVect*> func_list1(nf + 1);
  for (ymuint i = 0; i < nf; ++ i) {
    func_list1[i] = func_list[i];
  }

  for (ymuint i = 0; i < 100; ++ i) {
    FuncVect* fv = new FuncVect(k, n);
    set_random_func(fv, rg);
    func_list1[nf] = fv;
    bool stat = phf_sim_recur(n, k, m, func_list1, rg);
    delete fv;
    if ( stat ) {
      return true;
    }
  }
  return false;
}

END_NONAMESPACE


int
phf_sim(int argc,
	const char** argv)
{
  if ( argc != 4 ) {
    cerr << "Usage: phf_sim <n> <k> <m>" << endl;
    return -1;
  }

  int tmp_n = atoi(argv[1]);
  if ( tmp_n <= 0 ) {
    cerr << "'n' must be positive integer" << endl;
    return -1;
  }

  ymuint n = static_cast<ymuint>(tmp_n);

  int tmp_k = atoi(argv[2]);
  if ( tmp_k <= 0 ) {
    cerr << "'k' must be positive integer" << endl;
    return -1;
  }

  ymuint k = static_cast<ymuint>(tmp_k);

  int tmp_m = atoi(argv[3]);
  if ( tmp_m <= 0 ) {
    cerr << "'m' must be positive integer" << endl;
    return -1;
  }
  ymuint m = static_cast<ymuint>(tmp_m);

  RandGen rg;

#if 0
  ymuint nt = 2000;
  ymuint c_ok = 0;
  ymuint c_simple = 0;
  ymuint c_cyclic = 0;
  for (ymuint i = 0; i < nt; ++ i) {
    FuncVect* fv = new FuncVect(k, n);
    set_random_func(fv, rg);
    vector<const FuncVect*> func_list(1, fv);
    if ( phf_sim_recur(n, k, m, func_list, rg) ) {
      ++ c_ok;
    }
    delete fv;
  }
#else
  ymuint nt = 2000;
#if 0
  ymuint c_ok = 0;
  ymuint c_simple = 0;
  ymuint c_cyclic = 0;
#else
  ymuint c_easy = 0;
  ymuint c_sat = 0;
  ymuint c_ng = 0;
#endif
  for (ymuint i = 0; i < nt; ++ i) {
    vector<const FuncVect*> func_list(m);
    // ランダムに関数を作る．
    for (ymuint j = 0; j < m; ++ j) {
      FuncVect* fv = new FuncVect(k, n);
      func_list[j] = fv;
      set_random_func(fv, rg);
    }
    bool stat = phf_sim2(func_list);
    for (ymuint j = 0; j < m; ++ j) {
      delete func_list[j];
    }
#if 0
    if ( stat == 0 ) {
      ++ c_ok;
    }
    if ( stat & 1U ){
      ++ c_simple;
    }
    if ( stat & 2U ) {
      ++ c_cyclic;
    }
#else
    if ( stat ) {
      ++ c_sat;
    }
    else {
      ++ c_ng;
    }
#endif
  }
#endif

  cout << "Total " << nt << " trials" << endl
#if 0
       << " # of Success:    " << c_ok << endl
       << " # of Not Simple: " << c_simple << endl
       << " # of Cyclic:     " << c_cyclic << endl;
#else
  << " # of easy case:  " << c_easy << endl
  << " # of SAT case:   " << c_sat << endl
  << " # of Failure:    " << c_ng << endl;
#endif

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::phf_sim;

  return phf_sim(argc, argv);
}

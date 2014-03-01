
/// @file phf_sim.cc
/// @brief phf_sim のソースファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "FuncVect.h"
#include "PhfGen.h"
#include "utils/CombiGen.h"
#include "utils/RandGen.h"


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

bool
phf_sim1(const vector<const FuncVect*>& func_list)
{
  PhfGen pg;

  vector<vector<ymuint32> > g_list;
  return pg.mapping(func_list, g_list);
}

bool
phf_sim2(const vector<const FuncVect*>& func_list)
{
  PhfGen pg;

  vector<ymuint> block_map;
  return pg.cf_partition(func_list, block_map);
}

bool
phf_sim3(const vector<const FuncVect*>& func_list)
{
  PhfGen pg;

  vector<ymuint> d_map;
  return pg.displace_decomposition(func_list[0], func_list[1], d_map, false);
}

bool
phf_sim4(const vector<const FuncVect*>& func_list)
{
  PhfGen pg;

  vector<ymuint> d_map;
  return pg.displace_decomposition(func_list[0], func_list[1], d_map, true);
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

  ymuint nt = 2000;
  ymuint c1 = 0;
  ymuint c2 = 0;
  ymuint c3 = 0;
  ymuint c4 = 0;
  for (ymuint i = 0; i < nt; ++ i) {
    vector<const FuncVect*> func_list(m);
    // ランダムに関数を作る．
    for (ymuint j = 0; j < m; ++ j) {
      FuncVect* fv = new FuncVect(n, k);
      func_list[j] = fv;
      set_random_func(fv, rg);
    }
    bool stat1 = phf_sim1(func_list);
    bool stat2 = phf_sim2(func_list);
    bool stat3 = false;
    bool stat4 = false;
    if ( m == 2 ) {
      stat3 = phf_sim3(func_list);
      stat4 = phf_sim4(func_list);
    }
    for (ymuint j = 0; j < m; ++ j) {
      delete func_list[j];
    }
    if ( stat1 ) {
      ++ c1;
    }
    if ( stat2 ) {
      ++ c2;
    }
    if ( stat3 ) {
      ++ c3;
    }
    if ( stat4 ) {
      ++ c4;
    }
  }
  cout << "Total " << nt << " trials" << endl
       << " # of C1 case:  " << c1 << endl
       << " # of C2 case:  " << c2 << endl
       << " # of C3 case:  " << c3 << endl
       << " # of C4 case:  " << c4 << endl;

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

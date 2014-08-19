
/// @file dist_sim.cc
/// @brief dist_sim のソースファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

void
dist_sim1(ymuint n,
	  ymuint m)
{
  vector<ymuint> slots(m, 0);

  RandGen rg;

  for (ymuint i = 0; i < n; ++ i) {
    ymuint pos = rg.int32() % m;
    ++ slots[pos];
  }

  vector<ymuint> hist_array(n, 0);
  ymuint max_d = 0;
  for (ymuint i = 0; i < m; ++ i) {
    ymuint d = slots[i];
    ++ hist_array[d];
    if ( max_d < d ) {
      max_d = d;
    }
  }

  cout << n << " : " << m << endl;
  for (ymuint i = 0; i <= max_d; ++ i) {
    cout << i << " : " << hist_array[i] << endl;
  }
  cout << endl;

  for (ymuint i = 1; i <= max_d; ++ i) {
    ymuint n = 0;
    for (ymuint j = i; j <= max_d; ++ j) {
      n += hist_array[j];
    }
    cout << "Page#" << i << ": " << n << endl;
  }
  cout << endl;
}

int
dist_sim(int argc,
	 const char** argv)
{
  if ( argc != 3 ) {
    cerr << "Usage: dist_sim <n> <m>" << endl;
    return -1;
  }

  int tmp_n = atoi(argv[1]);
  if ( tmp_n <= 0 ) {
    cerr << "'n' must be positive integer" << endl;
    return -1;
  }

  ymuint n = static_cast<ymuint>(tmp_n);

  int tmp_m = atoi(argv[2]);
  if ( tmp_m <= 0 ) {
    cerr << "'m' must be positive integer" << endl;
    return -1;
  }
  ymuint m = static_cast<ymuint>(tmp_m);

  dist_sim1(n, m);

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::dist_sim;

  return dist_sim(argc, argv);
}

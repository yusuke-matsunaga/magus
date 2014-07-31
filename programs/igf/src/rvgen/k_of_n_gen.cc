
/// @file k_of_n_gen.cc
/// @brief k_of_n_gen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/PoptMainApp.h"
#include "YmUtils/CombiGen.h"
#include <gmpxx.h>


BEGIN_NAMESPACE_YM_IGF

// n 次元で 1 が k 個のベクタを出力する．
void
gen_vectors(ymuint n,
	    ymuint k,
	    ostream& s)
{
  assert_cond( n > k, __FILE__, __LINE__);

  // ベクタの数を数える．
  mpz_class nv = 1;
  for (ymuint i = 0; i < k; ++ i) {
    nv *= (n - i);
    cout << "i = " << i << ", nv = " << nv << endl;
  }
  cout << endl;
  for (ymuint i = 2; i <= k; ++ i) {
    nv /= i;
    cout << "i = " << i << ", nv = " << nv << endl;
  }

  cout << n << " " << nv << endl;

  for (CombiGen cg(n, k); !cg.is_end(); ++ cg) {
    vector<bool> bits(n, false);
    for (ymuint i = 0; i < k; ++ i) {
      ymuint pos = cg(i);
      bits[pos] = true;
    }
    string pat;
    for (ymuint i = 0; i < n; ++ i) {
      if ( bits[i] ) {
	pat += '1';
      }
      else {
	pat += '0';
      }
    }
    s << pat << endl;
  }
}

int
k_of_n_gen(int argc,
	   const char** argv)
{
  PoptMainApp main_app;

  main_app.set_other_option_help("<n> <k>");

  // オプション解析を行なう．
  tPoptStat stat = main_app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  vector<string> args;
  ymuint n_args = main_app.get_args(args);

  if ( n_args != 2 ) {
    main_app.usage(2);
  }

  int tmp_n = atoi(args[0].c_str());
  if ( tmp_n <= 0 ) {
    cerr << "Argument value error: first argument should be >= 0" << endl;
    return 3;
  }

  int tmp_k = atoi(args[1].c_str());
  if ( tmp_k <= 0 ) {
    cerr << "Argument value error: second argument should be >= 0" << endl;
    return 3;
  }

  ymuint n = tmp_n;
  ymuint k = tmp_k;

  ymuint np = 1U << n;
  if ( n < 32 && k > np ) {
    cerr << "Argument value error: second argument should be < 2^n" << endl;
    return 3;
  }


  //  n と k を出力
  gen_vectors(n, k, cout);

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::k_of_n_gen;

  return k_of_n_gen(argc, argv);
}

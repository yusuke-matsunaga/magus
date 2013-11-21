
/// @file mscg_test.cc
/// @brief MultiSetCombiGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiSetCombiGen.h"


BEGIN_NAMESPACE_YM

int
mscg_test(int argc,
	  const char** argv)
{
  try {
    char buff[1024];
    while ( cin.getline(buff, 1024, '\n') ) {
      char* p = strtok(buff, " \t");
      if ( !p ) {
	cerr << "ERROR: ngrp must be specified" << endl;
	return 2;
      }
      ymuint ng = atoi(p);
      if ( ng == 0 ) {
	cerr << "ERROR: ngrp == 0" << endl;
	return 2;
      }
      vector<ymuint> num_array(ng);
      for (ymuint g = 0; g < ng; ++ g) {
	p = strtok(NULL, " \t");
	if ( !p ) {
	  cerr << "ERROR: n(" << g << ") must be specified" << endl;
	  return 2;
	}
	ymuint n = atoi(p);
	if ( n == 0 ) {
	  cerr << "ERROR: n(" << g << ") == 0" << endl;
	  return 2;
	}
	num_array[g] = n;
      }
      p = strtok(NULL, " \t");
      if ( !p ) {
	cerr << "ERROR: k must be specified" << endl;
	return 2;
      }
      ymuint k = atoi(p);
      ymuint nall = 0;
      for (ymuint g = 0; g < ng; ++ g) {
	nall += num_array[g];
      }
      if ( k > nall ) {
	cerr << "ERROR: k is too big" << endl;
	return 3;
      }
      for (MultiSetCombiGen mscg(num_array, k); !mscg.is_end(); ++ mscg) {
	for (ymuint i = 0; i < k; ++ i) {
	  cout << " " << mscg(i);
	}
	cout << endl;
      }
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return -1;
  }

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  using namespace nsYm;

  return mscg_test(argc, argv);
}

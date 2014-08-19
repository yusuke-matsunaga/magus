
/// @file mspg_test.cc
/// @brief MultiSetPermGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "utils/MultiSetPermGen.h"


BEGIN_NAMESPACE_YM

int
mspg_test(int argc,
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
      for (MultiSetPermGen mspg(num_array, k); !mspg.is_end(); ++ mspg) {
	for (ymuint i = 0; i < k; ++ i) {
	  cout << " " << mspg(i);
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

  return mspg_test(argc, argv);
}

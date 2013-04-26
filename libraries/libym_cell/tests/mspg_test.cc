
/// @file mspg_test.cc
/// @brief MultiSetPermGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "MultiSetPermGen.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

int
mspg_test(int argc,
	  const char** argv)
{
  vector<ymuint> num_array(2);
  num_array[0] = 3;
  num_array[1] = 2;

  MultiSetPermGen mspg(num_array, 5);

  for (MultiSetPermGen::iterator p = mspg.begin(); !p.is_end(); ++ p) {
    for (ymuint i = 0; i < 5; ++ i) {
      cout << " " << p(i);
    }
    cout << endl;
  }

  return 0;
}

END_NAMESPACE_YM_CELL_LIBCOMP


int
main(int argc,
     const char** argv)
{
  using namespace nsYm::nsCell::nsLibcomp;
  return mspg_test(argc, argv);
}

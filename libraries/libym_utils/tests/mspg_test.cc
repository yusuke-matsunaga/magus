
/// @file mspg_test.cc
/// @brief MultiSetPermGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiSetPermGen.h"


BEGIN_NAMESPACE_YM

int
mspg_test(int argc,
	  const char** argv)
{
  vector<ymuint> num_array(2);
  num_array[0] = 3;
  num_array[1] = 2;

  for (MultiSetPermGen mspg(num_array, 5); !mspg.is_end(); ++ mspg) {
    for (ymuint i = 0; i < 5; ++ i) {
      cout << " " << mspg(i);
    }
    cout << endl;
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

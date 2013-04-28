
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
  vector<ymuint> num_array(2);
  num_array[0] = 3;
  num_array[1] = 2;

  for (MultiSetCombiGen mscg(num_array, 3); !mscg.is_end(); ++ mscg) {
    for (ymuint i = 0; i < 3; ++ i) {
      cout << " " << mscg(i);
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

  return mscg_test(argc, argv);
}

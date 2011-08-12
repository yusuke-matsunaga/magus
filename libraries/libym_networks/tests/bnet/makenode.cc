
/// @file makenode.cc
/// @brief BNode を生成するテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BNetwork.h"
#include "ym_networks/BNetManip.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  try {
    BNetwork network;
    BNetManip manip(&network);

    for (size_t i = 0; i < 1000000; ++ i) {
      manip.new_logic();
    }
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}

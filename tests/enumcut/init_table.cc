
/// @file init_table.cc
/// @brief RwtMgr の初期化用テストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RwtPatGen.h"
#include "RwtMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS

void
set_abc_table(RwtPatGen& mgr);

END_NAMESPACE_YM_NETWORKS


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsNetworks;

  RwtPatGen patgen;
  set_abc_table(patgen);

  RwtMgr mgr;
  mgr.copy(patgen);

  mgr.print(cout);

  ostringstream buf;
  BinOStream bos(buf);
  mgr.dump(bos);

  string str = buf.str();
  ymuint n = str.size();
  cout << " size = " << n << endl;
  for (ymuint i = 0; i < n; ++ i) {
    char c = str[i];
    cout << hex << static_cast<ymuint>(c);
    if ( (i % 16) == 15 ) {
      cout << endl;
    }
  }
  cout << endl;

  return 0;
}

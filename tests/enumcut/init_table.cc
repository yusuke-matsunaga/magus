
/// @file init_table.cc
/// @brief RwtMgr の初期化用テストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RwtMgr.h"
#include "RwtPat.h"
#include "RwtNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

void
read_data(RwtMgr& mgr);

END_NAMESPACE_YM_NETWORKS


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsNetworks;

  RwtMgr rwt_mgr;
  read_data(rwt_mgr);

  return 0;
}

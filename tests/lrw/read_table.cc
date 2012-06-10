
/// @file read_table.cc
/// @brief read_table のメインプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "RwtMgr.h"


BEGIN_NAMESPACE_YM

int
read_table(int argc,
	   const char** argv)
{
  RwtMgr rwt_mgr;

  rwt_mgr.init();

  rwt_mgr.dump(cout);

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  return nsYm::read_table(argc, argv);
}

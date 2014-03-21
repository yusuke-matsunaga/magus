
/// @file SimpleAllocTest.cc
/// @brief SimpleAllocTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

int
test()
{
  for (ymuint i = 0; i < 1000; ++ i) {
    SimpleAlloc alloc;

    alloc.get_memory(10);
    alloc.get_memory(1024);
    alloc.get_memory(1024 * 1024);
  }
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  return nsYm::test();
}

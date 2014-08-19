
/// @file SimpleAllocTest.cc
/// @brief SimpleAlloc のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "utils/SimpleAlloc.h"


using namespace nsYm;

TEST_GROUP(libym_utils_SimpleAllocTestGroup)
{
  SimpleAlloc* mAlloc;

  TEST_SETUP()
  {
    mAlloc = new SimpleAlloc(4096);
  }

  TEST_TEARDOWN()
  {
    delete mAlloc;
  }
};

// setup(), teardown() でメモリリークが発生していないことを確認するためのテスト
TEST(libym_utils_SimpleAllocTestGroup, empty)
{
}

// メモリ領域の確保のテスト
TEST(libym_utils_SimpleAllocTestGroup, simple_alloc)
{
  void* p = mAlloc->get_memory(16);

  CHECK( p != NULL );

  // ちなみに mAlloc->put_memory() を呼ばなくてもメモリリークは発生しない．
}

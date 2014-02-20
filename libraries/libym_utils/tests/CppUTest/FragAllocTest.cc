
/// @file FragAllocTest.cc
/// @brief FragAlloc のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "utils/FragAlloc.h"


using namespace nsYm;

TEST_GROUP(libym_utils_FragAllocTestGroup)
{
  FragAlloc* mAlloc;

  TEST_SETUP()
  {
    mAlloc = new FragAlloc(4096);
  }

  TEST_TEARDOWN()
  {
    delete mAlloc;
  }
};

// setup(), teardown() でメモリリークが発生していないことを確認するためのテスト
TEST(libym_utils_FragAllocTestGroup, empty)
{
}

// メモリ領域の確保のテスト
TEST(libym_utils_FragAllocTestGroup, simple_alloc)
{
  void* p = mAlloc->get_memory(16);

  CHECK( p != NULL );

  // ちなみに mAlloc->put_memory() を呼ばなくてもメモリリークは発生しない．
}

// ちゃんと解放したメモリが再利用できるかのテスト
TEST(libym_utils_FragAllocTestGroup, simple_alloc2)
{
  const ymuint SIZE1 = 1001;
  const ymuint SIZE2 = 1024;

  for (ymuint i = 0; i < 100000; ++ i) {
    void* p = mAlloc->get_memory(SIZE1);

    CHECK( p != NULL );

    void* q = mAlloc->get_memory(SIZE2);

    CHECK( q != NULL );

    mAlloc->put_memory(SIZE1, p);

    mAlloc->put_memory(SIZE2, q);
  }
}

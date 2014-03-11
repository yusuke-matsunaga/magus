
/// @file SimpleAllocTest.cc
/// @brief SimpleAllocTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SimpleAllocTest.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス SimpleAllocTest
//////////////////////////////////////////////////////////////////////

// @brief setup 関数
void
SimpleAllocTest::setUp()
{
  mAllocPtr = new SimpleAlloc();
}

// @brief teardown 関数
void
SimpleAllocTest::tearDown()
{
  delete mAllocPtr;
}

// @brief メモリ確保のテスト
void
SimpleAllocTest::test_alloc()
{
  void* p = mAllocPtr->get_memory(1024);
  CPPUNIT_ASSERT( p != NULL );
}

// @brief destroy() のテスト
void
SimpleAllocTest::test_destroy()
{
  for (ymuint i = 0; i < 10000000; ++ i) {
    void* p = mAllocPtr->get_memory(1024);
    CPPUNIT_ASSERT( p != NULL );
    mAllocPtr->destroy();
  }
}

// @brief set_mem_limit() -> mem_limit() のテスト
void
SimpleAllocTest::test_set_mem_limit1()
{
  mAllocPtr->set_mem_limit(10);

  CPPUNIT_ASSERT_EQUAL( 10UL, mAllocPtr->mem_limit() );
}

// @brief set_mem_limit() が効いているかのテスト
void
SimpleAllocTest::test_set_mem_limit2()
{
  mAllocPtr->set_mem_limit(5);
  void* p = mAllocPtr->get_memory(10);
  CPPUNIT_ASSERT( p == NULL );
}

END_NAMESPACE_YM

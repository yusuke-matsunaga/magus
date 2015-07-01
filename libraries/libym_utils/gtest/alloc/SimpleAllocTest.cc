
/// @file SimpleAllocTest.cc
/// @brief SimpleAlloc のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

class SimpleAllocTest :
  public testing::Test
{
public:

  SimpleAllocTest();

  virtual
  ~SimpleAllocTest();


public:

  SimpleAlloc mAlloc;

};

SimpleAllocTest::SimpleAllocTest()
{
}

SimpleAllocTest::~SimpleAllocTest()
{
}

TEST_F( SimpleAllocTest, Empty )
{
  // なにもしない．
  // コンストラクタ/デストラクタでエラーがおきないことのテスト
}

TEST_F( SimpleAllocTest, Alloc1 )
{
  void* p = mAlloc.get_memory(16);

  EXPECT_NE( nullptr, p ) << "p = SimpleAlloc::get_memory(16)";
}

TEST_F( SimpleAllocTest, BigAlloc )
{
  void* p = mAlloc.get_memory(16 * 1024);

  EXPECT_NE( nullptr, p ) << "p = SimpleAlloc::get_memory(16 * 1024)";
}

TEST_F( SimpleAllocTest, Destroy1 )
{
  for (ymuint i = 0; i < 10000; ++ i) {
    void* p = mAlloc.get_memory(1024);
    EXPECT_NE( nullptr, p ) << "p = SimpleAlloc::get_memory(1024)";

    mAlloc.destroy();

    EXPECT_EQ( 0UL, mAlloc.used_size() ) << "mAlloc.used_size() != 0";
  }
}

TEST_F( SimpleAllocTest, Destroy2 )
{
  for (ymuint i = 0; i < 10000; ++ i) {
    void* p = mAlloc.get_memory(1024 * 1024);
    EXPECT_NE( nullptr, p ) << "p = SimpleAlloc::get_memory(1024 * 1024)";

    mAlloc.destroy();

    EXPECT_EQ( 0UL, mAlloc.used_size() ) << "mAlloc.used_size() != 0";
  }
}

TEST_F( SimpleAllocTest, SetMemLimit1 )
{
  mAlloc.set_mem_limit(10);

  EXPECT_EQ( 10UL, mAlloc.mem_limit() ) << "mAlloc.mem_limit() != 10";
}

TEST_F( SimpleAllocTest, SetMemLimit2 )
{
  mAlloc.set_mem_limit(10);

  void* p = mAlloc.get_memory(100);

  EXPECT_EQ( nullptr, p ) << "mAlloc.get_memory(100) != nullptr, under mem_limit(10)";
}

END_NAMESPACE_YM

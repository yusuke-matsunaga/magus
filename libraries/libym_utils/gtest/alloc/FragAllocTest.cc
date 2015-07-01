
/// @file FragAllocTest.cc
/// @brief FragAlloc のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "YmUtils/FragAlloc.h"


BEGIN_NAMESPACE_YM

class FragAllocTest :
  public testing::Test
{
public:

  FragAllocTest();

  virtual
  ~FragAllocTest();


public:

  FragAlloc mAlloc;

};

FragAllocTest::FragAllocTest()
{
}

FragAllocTest::~FragAllocTest()
{
}

TEST_F( FragAllocTest, Empty )
{
  // なにもしない．
  // コンストラクタ/デストラクタでエラーがおきないことのテスト
}

TEST_F( FragAllocTest, Alloc1 )
{
  void* p = mAlloc.get_memory(16);

  EXPECT_TRUE( p != NULL );
}

TEST_F( FragAllocTest, BigAlloc )
{
  void* p = mAlloc.get_memory(16 * 1024);

  EXPECT_TRUE( p != NULL );
}

TEST_F( FragAllocTest, RepeatAlloc )
{
  const ymuint SIZE1 = 1001;
  const ymuint SIZE2 = 1024;

  for (ymuint i = 0; i < 100000; ++ i) {
    void* p = mAlloc.get_memory(SIZE1);

    EXPECT_NE( nullptr, p ) << "p = mAlloc.get_memory(" << SIZE1 << ")";

    void* q = mAlloc.get_memory(SIZE2);

    EXPECT_NE( nullptr, q ) << "q = mAlloc.get_memory(" << SIZE2 << ")";

    mAlloc.put_memory(SIZE1, p);

    mAlloc.put_memory(SIZE2, q);

    EXPECT_EQ( 0UL, mAlloc.used_size() ) << "mAlloc.used_size() != 0";
  }
}

END_NAMESPACE_YM

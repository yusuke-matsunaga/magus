
/// @file UnitAllocTest.cc
/// @brief UnitAlloc のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "YmUtils/UnitAlloc.h"


BEGIN_NAMESPACE_YM

class UnitAllocTest :
  public testing::Test
{
public:

  UnitAllocTest();

  virtual
  ~UnitAllocTest();


public:

  ymuint mUnitSize;

  UnitAlloc mAlloc;

};

UnitAllocTest::UnitAllocTest() :
  mUnitSize(560),
  mAlloc(mUnitSize)
{
}

UnitAllocTest::~UnitAllocTest()
{
}

TEST_F( UnitAllocTest, Empty )
{
  // なにもしない．
  // コンストラクタ/デストラクタでエラーがおきないことのテスト
}

TEST_F( UnitAllocTest, Alloc1 )
{
  void* p = mAlloc.get_memory(mUnitSize);

  EXPECT_NE( nullptr, p ) << "p = mAlloc.get_memory(" << mUnitSize << ")";
}

TEST_F( UnitAllocTest, Alloc2 )
{
  void* p = mAlloc.get_memory(1024);

  EXPECT_NE( nullptr, p ) << "p = mAlloc.get_memory(1024)";
}

TEST_F( UnitAllocTest, BigAlloc )
{
  void* p = mAlloc.get_memory(16 * 1024);

  EXPECT_NE( nullptr, p ) << "p = mAlloc.get_memory(16 * 1024)";
}

TEST_F( UnitAllocTest, Destroy1 )
{
  for (ymuint i = 0; i < 10000; ++ i) {
    void* p = mAlloc.get_memory(mUnitSize);
    EXPECT_NE( nullptr, p ) << "p = mAlloc.get_memory(" << mUnitSize << ")";

    mAlloc.destroy();

    EXPECT_EQ( 0UL, mAlloc.used_size() ) << "mAlloc.used_size() != 0";
  }
}

TEST_F( UnitAllocTest, Destroy2 )
{
  for (ymuint i = 0; i < 10000; ++ i) {
    void* p = mAlloc.get_memory(1024 * 1024);
    EXPECT_NE( nullptr, p ) << "p = mAlloc.get_memory(1024 * 1024)";

    mAlloc.destroy();

    EXPECT_EQ( 0UL, mAlloc.used_size() ) << "mAlloc.used_size() != 0";
  }
}

TEST_F( UnitAllocTest, RepatAlloc )
{
  for (ymuint i = 0; i < 1000000; ++ i) {
    void* p = mAlloc.get_memory(mUnitSize);

    EXPECT_NE( nullptr, p ) << "p = mAlloc.get_memory(" << mUnitSize << ")";

    mAlloc.put_memory(mUnitSize, p);

    EXPECT_EQ( 0UL, mAlloc.used_size() ) << "mAlloc.used_size() != 0";
  }
}

END_NAMESPACE_YM

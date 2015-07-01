
/// @file PermGenTest.cc
/// @brief PermGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "YmUtils/PermGen.h"


BEGIN_NAMESPACE_YM

class PermGenTest :
  public testing::Test
{
public:

  PermGenTest();

  virtual
  ~PermGenTest();


public:

  ymuint* mExpData;

};

PermGenTest::PermGenTest()
{
  ymuint data_src[] = {
    0, 1, 2, 3, 4,
    0, 1, 2, 4, 3,
    0, 1, 3, 2, 4,
    0, 1, 3, 4, 2,
    0, 1, 4, 2, 3,
    0, 1, 4, 3, 2,
    0, 2, 1, 3, 4,
    0, 2, 1, 4, 3,
    0, 2, 3, 1, 4,
    0, 2, 3, 4, 1,
    0, 2, 4, 1, 3,
    0, 2, 4, 3, 1,
    0, 3, 1, 2, 4,
    0, 3, 1, 4, 2,
    0, 3, 2, 1, 4,
    0, 3, 2, 4, 1,
    0, 3, 4, 1, 2,
    0, 3, 4, 2, 1,
    0, 4, 1, 2, 3,
    0, 4, 1, 3, 2,
    0, 4, 2, 1, 3,
    0, 4, 2, 3, 1,
    0, 4, 3, 1, 2,
    0, 4, 3, 2, 1,
    1, 0, 2, 3, 4,
    1, 0, 2, 4, 3,
    1, 0, 3, 2, 4,
    1, 0, 3, 4, 2,
    1, 0, 4, 2, 3,
    1, 0, 4, 3, 2,
    1, 2, 0, 3, 4,
    1, 2, 0, 4, 3,
    1, 2, 3, 0, 4,
    1, 2, 3, 4, 0,
    1, 2, 4, 0, 3,
    1, 2, 4, 3, 0,
    1, 3, 0, 2, 4,
    1, 3, 0, 4, 2,
    1, 3, 2, 0, 4,
    1, 3, 2, 4, 0,
    1, 3, 4, 0, 2,
    1, 3, 4, 2, 0,
    1, 4, 0, 2, 3,
    1, 4, 0, 3, 2,
    1, 4, 2, 0, 3,
    1, 4, 2, 3, 0,
    1, 4, 3, 0, 2,
    1, 4, 3, 2, 0,
    2, 0, 1, 3, 4,
    2, 0, 1, 4, 3,
    2, 0, 3, 1, 4,
    2, 0, 3, 4, 1,
    2, 0, 4, 1, 3,
    2, 0, 4, 3, 1,
    2, 1, 0, 3, 4,
    2, 1, 0, 4, 3,
    2, 1, 3, 0, 4,
    2, 1, 3, 4, 0,
    2, 1, 4, 0, 3,
    2, 1, 4, 3, 0,
    2, 3, 0, 1, 4,
    2, 3, 0, 4, 1,
    2, 3, 1, 0, 4,
    2, 3, 1, 4, 0,
    2, 3, 4, 0, 1,
    2, 3, 4, 1, 0,
    2, 4, 0, 1, 3,
    2, 4, 0, 3, 1,
    2, 4, 1, 0, 3,
    2, 4, 1, 3, 0,
    2, 4, 3, 0, 1,
    2, 4, 3, 1, 0,
    3, 0, 1, 2, 4,
    3, 0, 1, 4, 2,
    3, 0, 2, 1, 4,
    3, 0, 2, 4, 1,
    3, 0, 4, 1, 2,
    3, 0, 4, 2, 1,
    3, 1, 0, 2, 4,
    3, 1, 0, 4, 2,
    3, 1, 2, 0, 4,
    3, 1, 2, 4, 0,
    3, 1, 4, 0, 2,
    3, 1, 4, 2, 0,
    3, 2, 0, 1, 4,
    3, 2, 0, 4, 1,
    3, 2, 1, 0, 4,
    3, 2, 1, 4, 0,
    3, 2, 4, 0, 1,
    3, 2, 4, 1, 0,
    3, 4, 0, 1, 2,
    3, 4, 0, 2, 1,
    3, 4, 1, 0, 2,
    3, 4, 1, 2, 0,
    3, 4, 2, 0, 1,
    3, 4, 2, 1, 0,
    4, 0, 1, 2, 3,
    4, 0, 1, 3, 2,
    4, 0, 2, 1, 3,
    4, 0, 2, 3, 1,
    4, 0, 3, 1, 2,
    4, 0, 3, 2, 1,
    4, 1, 0, 2, 3,
    4, 1, 0, 3, 2,
    4, 1, 2, 0, 3,
    4, 1, 2, 3, 0,
    4, 1, 3, 0, 2,
    4, 1, 3, 2, 0,
    4, 2, 0, 1, 3,
    4, 2, 0, 3, 1,
    4, 2, 1, 0, 3,
    4, 2, 1, 3, 0,
    4, 2, 3, 0, 1,
    4, 2, 3, 1, 0,
    4, 3, 0, 1, 2,
    4, 3, 0, 2, 1,
    4, 3, 1, 0, 2,
    4, 3, 1, 2, 0,
    4, 3, 2, 0, 1,
    4, 3, 2, 1, 0
  };

  ymuint n = sizeof(data_src) / sizeof(ymuint);
  mExpData = new ymuint[n];
  for (ymuint i = 0; i < n; ++ i) {
    mExpData[i] = data_src[i];
  }
}

PermGenTest::~PermGenTest()
{
  delete [] mExpData;
};

// コンストラクタ/デストラクタで問題が起きないことをチェックするためのテスト
TEST_F( PermGenTest, Empty )
{
}

// 組み合わせ列挙のテスト
TEST_F(PermGenTest, PermGen)
{
  PermGen pg(5, 5);

  EXPECT_EQ( 5, pg.n() );
  EXPECT_EQ( 5, pg.k() );

  ymuint base = 0;
  for ( ; !pg.is_end(); ++ pg) {
    for (ymuint i = 0; i < 5; ++ i) {
      EXPECT_EQ( mExpData[base + i], pg(i) );
    }
    base += 5;
  }

  // init() のテスト
  base = 0;
  for (pg.init(); !pg.is_end(); ++ pg) {
    for (ymuint i = 0; i < 5; ++ i) {
      EXPECT_EQ( mExpData[base + i], pg(i) );
    }
    base += 5;
  }
}

// コピーコンストラクタのテスト1
TEST_F(PermGenTest, Copy1)
{
  PermGen pg(5, 5);

  PermGen pg1(pg);

  EXPECT_EQ( 5, pg1.n() );
  EXPECT_EQ( 5, pg1.k() );

  ymuint base = 0;
  for ( ; !pg1.is_end(); ++ pg1) {
    for (ymuint i = 0; i < 5; ++ i) {
      EXPECT_EQ( mExpData[base + i], pg1(i) );
    }
    base += 5;
  }
}

// コピーコンストラクタのテスト2
TEST_F(PermGenTest, Copy2)
{
  PermGen pg(5, 5);

  ++ pg;
  ++ pg;
  ++ pg;

  PermGen pg1(pg);

  EXPECT_EQ( 5, pg1.n() );
  EXPECT_EQ( 5, pg1.k() );

  ymuint base = 5 * 3;
  for ( ; !pg1.is_end(); ++ pg1) {
    for (ymuint i = 0; i < 5; ++ i) {
      EXPECT_EQ( mExpData[base + i], pg1(i) );
    }
    base += 5;
  }
}

// 代入演算子のテスト1
TEST_F(PermGenTest, Assign1)
{
  PermGen pg(5, 5);

  PermGen pg1(2, 1);

  pg1 = pg;
  EXPECT_EQ( 5, pg1.n() );
  EXPECT_EQ( 5, pg1.k() );

  ymuint base = 0;
  for ( ; !pg1.is_end(); ++ pg1) {
    for (ymuint i = 0; i < 5; ++ i) {
      EXPECT_EQ( mExpData[base + i], pg1(i) );
    }
    base += 5;
  }
}

// 代入演算子のテスト2
TEST_F(PermGenTest, Assign2)
{
  PermGen pg(5, 5);

  ++ pg;
  ++ pg;
  ++ pg;

  PermGen pg1 = pg;

  EXPECT_EQ( 5, pg1.n() );
  EXPECT_EQ( 5, pg1.k() );

  ymuint base = 5 * 3;
  for ( ; !pg1.is_end(); ++ pg1) {
    for (ymuint i = 0; i < 5; ++ i) {
      EXPECT_EQ( mExpData[base + i], pg1(i) );
    }
    base += 5;
  }
}

END_NAMESPACE_YM

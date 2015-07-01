
/// @file CombiGenTest.cc
/// @brief CombiGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "YmUtils/CombiGen.h"


BEGIN_NAMESPACE_YM

class CombiGenTest :
  public testing::Test
{
public:

  CombiGenTest();

  virtual
  ~CombiGenTest();

public:


public:

  ymuint* mExpData;

};

CombiGenTest::CombiGenTest()
{
  ymuint data_src[] = {
    0, 1, 2,
    0, 1, 3,
    0, 1, 4,
    0, 2, 3,
    0, 2, 4,
    0, 3, 4,
    1, 2, 3,
    1, 2, 4,
    1, 3, 4,
    2, 3, 4
  };

  ymuint n = sizeof(data_src) / sizeof(ymuint);
  mExpData = new ymuint[n];
  for (ymuint i = 0; i < n; ++ i) {
    mExpData[i] = data_src[i];
  }
}

CombiGenTest::~CombiGenTest()
{
  delete [] mExpData;
}

TEST_F( CombiGenTest, Empty )
{
  // なにもしない．
  // コンストラクタ/デストラクタでエラーがおきないことのテスト
}

// 組み合わせ列挙のテスト
TEST_F( CombiGenTest, CombiGen )
{
  CombiGen cg(5, 3);

  EXPECT_EQ( 5, cg.n() );
  EXPECT_EQ( 3, cg.k() );

  ymuint base = 0;
  for ( ; !cg.is_end(); ++ cg) {
    for (ymuint i = 0; i < 3; ++ i) {
      EXPECT_EQ( mExpData[base + i], cg(i) );
    }
    base += 3;
  }

  // init() のテスト
  base = 0;
  for (cg.init(); !cg.is_end(); ++ cg) {
    for (ymuint i = 0; i < 3; ++ i) {
      EXPECT_EQ( mExpData[base + i], cg(i) );
    }
    base += 3;
  }
}

// コピーコンストラクタのテスト1
TEST_F( CombiGenTest, Copy1)
{
  CombiGen cg(5, 3);

  CombiGen cg1(cg);

  EXPECT_EQ( 5, cg1.n() );
  EXPECT_EQ( 3, cg1.k() );

  ymuint base = 0;
  for ( ; !cg1.is_end(); ++ cg1) {
    for (ymuint i = 0; i < 3; ++ i) {
      EXPECT_EQ( mExpData[base + i], cg1(i) );
    }
    base += 3;
  }
}

// コピーコンストラクタのテスト2
TEST_F( CombiGenTest, Copy2)
{
  CombiGen cg(5, 3);

  ++ cg;
  ++ cg;
  ++ cg;

  CombiGen cg1(cg);

  EXPECT_EQ( 5, cg1.n() );
  EXPECT_EQ( 3, cg1.k() );

  ymuint base = 3 * 3;
  for ( ; !cg1.is_end(); ++ cg1) {
    for (ymuint i = 0; i < 3; ++ i) {
      EXPECT_EQ( mExpData[base + i], cg1(i) );
    }
    base += 3;
  }
}

// 代入演算子のテスト1
TEST_F( CombiGenTest, Assign1)
{
  CombiGen cg(5, 3);

  CombiGen cg1(2, 1);

  cg1 = cg;
  EXPECT_EQ( 5, cg1.n() );
  EXPECT_EQ( 3, cg1.k() );

  ymuint base = 0;
  for ( ; !cg1.is_end(); ++ cg1) {
    for (ymuint i = 0; i < 3; ++ i) {
      EXPECT_EQ( mExpData[base + i], cg1(i) );
    }
    base += 3;
  }
}

// 代入演算子のテスト2
TEST_F( CombiGenTest, Assign2)
{
  CombiGen cg(5, 3);

  ++ cg;
  ++ cg;
  ++ cg;

  CombiGen cg1 = cg;

  EXPECT_EQ( 5, cg1.n() );
  EXPECT_EQ( 3, cg1.k() );

  ymuint base = 3 * 3;
  for ( ; !cg1.is_end(); ++ cg1) {
    for (ymuint i = 0; i < 3; ++ i) {
      EXPECT_EQ( mExpData[base + i], cg1(i) );
    }
    base += 3;
  }
}

END_NAMESPACE_YM

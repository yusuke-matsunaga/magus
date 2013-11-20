
/// @file PermGenTest.cc
/// @brief PermGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"
#include "ym_utils/PermGen.h"


using namespace nsYm;


TEST_GROUP(PermGenTestGroup)
{
  ymuint* exp_data;

  TEST_SETUP() {
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
    exp_data = new ymuint[n];
    for (ymuint i = 0; i < n; ++ i) {
      exp_data[i] = data_src[i];
    }
  }

  TEST_TEARDOWN() {
    delete [] exp_data;
  }
};

// setup(), teardown() で問題が起きないことをチェックするためのテスト
TEST(PermGenTestGroup, empty)
{
}

// 組み合わせ列挙のテスト
TEST(PermGenTestGroup, permgen)
{
  PermGen pg(5, 5);

  LONGS_EQUAL( 5, pg.n() );
  LONGS_EQUAL( 5, pg.k() );

  ymuint base = 0;
  for ( ; !pg.is_end(); ++ pg) {
    for (ymuint i = 0; i < 5; ++ i) {
      LONGS_EQUAL( exp_data[base + i], pg(i) );
    }
    base += 5;
  }

  // init() のテスト
  base = 0;
  for (pg.init(); !pg.is_end(); ++ pg) {
    for (ymuint i = 0; i < 5; ++ i) {
      LONGS_EQUAL( exp_data[base + i], pg(i) );
    }
    base += 5;
  }
}

// コピーコンストラクタのテスト1
TEST(PermGenTestGroup, copy1)
{
  PermGen pg(5, 5);

  PermGen pg1(pg);

  LONGS_EQUAL( 5, pg1.n() );
  LONGS_EQUAL( 5, pg1.k() );

  ymuint base = 0;
  for ( ; !pg1.is_end(); ++ pg1) {
    for (ymuint i = 0; i < 5; ++ i) {
      LONGS_EQUAL( exp_data[base + i], pg1(i) );
    }
    base += 5;
  }
}

// コピーコンストラクタのテスト2
TEST(PermGenTestGroup, copy2)
{
  PermGen pg(5, 5);

  ++ pg;
  ++ pg;
  ++ pg;

  PermGen pg1(pg);

  LONGS_EQUAL( 5, pg1.n() );
  LONGS_EQUAL( 5, pg1.k() );

  ymuint base = 5 * 3;
  for ( ; !pg1.is_end(); ++ pg1) {
    for (ymuint i = 0; i < 5; ++ i) {
      LONGS_EQUAL( exp_data[base + i], pg1(i) );
    }
    base += 5;
  }
}

// 代入演算子のテスト1
TEST(PermGenTestGroup, assign1)
{
  PermGen pg(5, 5);

  PermGen pg1(2, 1);

  pg1 = pg;
  LONGS_EQUAL( 5, pg1.n() );
  LONGS_EQUAL( 5, pg1.k() );

  ymuint base = 0;
  for ( ; !pg1.is_end(); ++ pg1) {
    for (ymuint i = 0; i < 5; ++ i) {
      LONGS_EQUAL( exp_data[base + i], pg1(i) );
    }
    base += 5;
  }
}

// 代入演算子のテスト2
TEST(PermGenTestGroup, assign2)
{
  PermGen pg(5, 5);

  ++ pg;
  ++ pg;
  ++ pg;

  PermGen pg1 = pg;

  LONGS_EQUAL( 5, pg1.n() );
  LONGS_EQUAL( 5, pg1.k() );

  ymuint base = 5 * 3;
  for ( ; !pg1.is_end(); ++ pg1) {
    for (ymuint i = 0; i < 5; ++ i) {
      LONGS_EQUAL( exp_data[base + i], pg1(i) );
    }
    base += 5;
  }
}

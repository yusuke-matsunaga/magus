
/// @file CombiGenTest.cc
/// @brief CombiGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"
#include "utils/CombiGen.h"


using namespace nsYm;


TEST_GROUP(libym_utils_CombiGenTestGroup)
{
  ymuint* exp_data;

  TEST_SETUP() {
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
TEST(libym_utils_CombiGenTestGroup, empty)
{
}

// 組み合わせ列挙のテスト
TEST(libym_utils_CombiGenTestGroup, combigen)
{
  CombiGen cg(5, 3);

  LONGS_EQUAL( 5, cg.n() );
  LONGS_EQUAL( 3, cg.k() );

  ymuint base = 0;
  for ( ; !cg.is_end(); ++ cg) {
    for (ymuint i = 0; i < 3; ++ i) {
      LONGS_EQUAL( exp_data[base + i], cg(i) );
    }
    base += 3;
  }

  // init() のテスト
  base = 0;
  for (cg.init(); !cg.is_end(); ++ cg) {
    for (ymuint i = 0; i < 3; ++ i) {
      LONGS_EQUAL( exp_data[base + i], cg(i) );
    }
    base += 3;
  }
}

// コピーコンストラクタのテスト1
TEST(libym_utils_CombiGenTestGroup, copy1)
{
  CombiGen cg(5, 3);

  CombiGen cg1(cg);

  LONGS_EQUAL( 5, cg1.n() );
  LONGS_EQUAL( 3, cg1.k() );

  ymuint base = 0;
  for ( ; !cg1.is_end(); ++ cg1) {
    for (ymuint i = 0; i < 3; ++ i) {
      LONGS_EQUAL( exp_data[base + i], cg1(i) );
    }
    base += 3;
  }
}

// コピーコンストラクタのテスト2
TEST(libym_utils_CombiGenTestGroup, copy2)
{
  CombiGen cg(5, 3);

  ++ cg;
  ++ cg;
  ++ cg;

  CombiGen cg1(cg);

  LONGS_EQUAL( 5, cg1.n() );
  LONGS_EQUAL( 3, cg1.k() );

  ymuint base = 3 * 3;
  for ( ; !cg1.is_end(); ++ cg1) {
    for (ymuint i = 0; i < 3; ++ i) {
      LONGS_EQUAL( exp_data[base + i], cg1(i) );
    }
    base += 3;
  }
}

// 代入演算子のテスト1
TEST(libym_utils_CombiGenTestGroup, assign1)
{
  CombiGen cg(5, 3);

  CombiGen cg1(2, 1);

  cg1 = cg;
  LONGS_EQUAL( 5, cg1.n() );
  LONGS_EQUAL( 3, cg1.k() );

  ymuint base = 0;
  for ( ; !cg1.is_end(); ++ cg1) {
    for (ymuint i = 0; i < 3; ++ i) {
      LONGS_EQUAL( exp_data[base + i], cg1(i) );
    }
    base += 3;
  }
}

// 代入演算子のテスト2
TEST(libym_utils_CombiGenTestGroup, assign2)
{
  CombiGen cg(5, 3);

  ++ cg;
  ++ cg;
  ++ cg;

  CombiGen cg1 = cg;

  LONGS_EQUAL( 5, cg1.n() );
  LONGS_EQUAL( 3, cg1.k() );

  ymuint base = 3 * 3;
  for ( ; !cg1.is_end(); ++ cg1) {
    for (ymuint i = 0; i < 3; ++ i) {
      LONGS_EQUAL( exp_data[base + i], cg1(i) );
    }
    base += 3;
  }
}

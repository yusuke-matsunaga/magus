
/// @file BtgMatchTest.cc
/// @brief BtgMatch のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "ym_utils/BtgMatch.h"


using namespace nsYm;

TEST_GROUP(libym_utils_BtgMatchTestGroup)
{
  BtgMatch* mBtgMatch;

  TEST_SETUP() {
    mBtgMatch = new BtgMatch();
  }

  TEST_TEARDOWN() {
    delete mBtgMatch;
  }
};

// setup(), teardown() でメモリリークが発生していないことを確認するためのテスト
TEST(libym_utils_BtgMatchTestGroup, empty)
{
}

// resie() のテスト
TEST(libym_utils_BtgMatchTestGroup, resize)
{
  mBtgMatch->resize(10, 10);
}

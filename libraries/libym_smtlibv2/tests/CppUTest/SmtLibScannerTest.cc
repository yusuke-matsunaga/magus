
/// @file SmtLibScannerTest.cc
/// @brief SmtLibScanner のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"
#include "SmtLibScanner.h"


using namespace nsYm;
using namespace nsYm::nsSmtLibV2;


TEST_GROUP(SmtLibScannerTestGroup)
{
  SmtLibScanner* mScanner;

  TEST_SETUP() {
    //mScanner = new SmtLibScanner;
    mScanner = NULL;
  }

  TEST_TEARDOWN() {
    delete mScanner;
  }
};

// setup(), teardown() で問題が起きないことのテスト
TEST(SmtLibScannerTestGroup, empty)
{
}

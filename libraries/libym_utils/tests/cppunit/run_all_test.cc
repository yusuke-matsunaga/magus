
/// @file run_all_test.cc
/// @brief libym_utils のテスト用プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "YmUtilsTestRegistry.h"


int
main(int argc,
     char** argv)
{
  static YmUtilsTestRegistry ym_utils_test;

  CppUnit::Test* suite = CppUnit::TestFactoryRegistry::getRegistry("ym_utils").makeTest();

  CppUnit::TextTestRunner runner;
  runner.addTest(suite);

  bool wasSuccessful = runner.run();

  return wasSuccessful ? 0 : 1;
}

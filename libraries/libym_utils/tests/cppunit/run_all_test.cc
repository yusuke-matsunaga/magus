
/// @file run_all_test.cc
/// @brief libym_utils のテスト用プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include "alloc/SimpleAllocTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( nsYm::SimpleAllocTest );


int
main(int argc,
     char** argv)
{
  CppUnit::Test* suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  CppUnit::TextTestRunner runner;
  runner.addTest(suite);

  bool wasSuccessful = runner.run();

  return wasSuccessful ? 0 : 1;
}


/// @file AllocTestRegistry.cc
/// @brief AllocTestRegistry の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AllocTestRegistry.h"
#include "SimpleAllocTest.h"


// @brief コンストラクタ
AllocTestRegistry::AllocTestRegistry() :
  CppUnit::TestFactoryRegistry("alloc")
{
  CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( nsYm::SimpleAllocTest, "alloc" );
}

// @brief デストラクタ
AllocTestRegistry::~AllocTestRegistry()
{
}

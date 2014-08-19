
/// @file YmUtilsTestRegistry.cc
/// @brief YmUtilsTestRegistry の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtilsTestRegistry.h"
#include <cppunit/extensions/HelperMacros.h>
#include "alloc/AllocTestRegistry.h"
#include "mincov/MincovTestRegistry.h"


// @brief コンストラクタ
YmUtilsTestRegistry::YmUtilsTestRegistry() :
  CppUnit::TestFactoryRegistry("ym_utils")
{
  static AllocTestRegistry alloc_test;
  static MincovTestRegistry mincov_test;

  CPPUNIT_REGISTRY_ADD( "alloc", "ym_utils" );
  CPPUNIT_REGISTRY_ADD( "mincov", "ym_utils" );
}

// @brief デストラクタ
YmUtilsTestRegistry::~YmUtilsTestRegistry()
{
}

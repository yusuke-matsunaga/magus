
/// @file MincovTestRegistry.cc
/// @brief MincovTestRegistry の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MincovTestRegistry.h"
#include "MincovMatrixTest.h"


// @brief コンストラクタ
MincovTestRegistry::MincovTestRegistry() :
  CppUnit::TestFactoryRegistry("mincov")
{
  CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( nsYm::MincovMatrixTest, "mincov" );
}

// @brief デストラクタ
MincovTestRegistry::~MincovTestRegistry()
{
}

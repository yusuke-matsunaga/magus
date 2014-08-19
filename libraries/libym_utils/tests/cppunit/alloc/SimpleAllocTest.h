#ifndef SIMPLEALLOCTEST_H
#define SIMPLEALLOCTEST_H

/// @file SimpleAllocTest.h
/// @brief SimpleAllocTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include <cppunit/extensions/HelperMacros.h>
#include "utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

using namespace CppUnit;


//////////////////////////////////////////////////////////////////////
/// @class SimpleAllocTest SimpleAllocTest.h
/// @brief SimpleAlloc 用のテストフィクスチャクラス
//////////////////////////////////////////////////////////////////////
class SimpleAllocTest :
  public TestFixture
{
  // テストスイートの登録
  CPPUNIT_TEST_SUITE( SimpleAllocTest );
  CPPUNIT_TEST( test_alloc );
  CPPUNIT_TEST( test_destroy );
  CPPUNIT_TEST( test_destroy2 );
  CPPUNIT_TEST( test_set_mem_limit1 );
  CPPUNIT_TEST( test_set_mem_limit2 );
  CPPUNIT_TEST_SUITE_END();

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief setup 関数
  void
  setUp();

  /// @brief teardown 関数
  void
  tearDown();

  /// @brief メモリ確保のテスト
  void
  test_alloc();

  /// @brief destroy() のテスト
  void
  test_destroy();

  /// @brief destroy() のテスト
  void
  test_destroy2();

  /// @brief set_mem_limit() -> mem_limit() のテスト
  void
  test_set_mem_limit1();

  /// @brief set_mem_limit() が効いているかのテスト
  void
  test_set_mem_limit2();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SimpleAlloc のオブジェクト
  SimpleAlloc* mAllocPtr;

};

END_NAMESPACE_YM

#endif // SIMPLEALLOCTEST_H


/// @file LogExprTest.cc
/// @brief LogExpr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "ym_logic/LogExpr.h"

using namespace nsYm;


TEST_GROUP(libym_logic_LogExprTestGroup)
{
  TEST_SETUP() {
  }

  TEST_TEARDOWN() {
  }

};


// 空のテスト
TEST(libym_logic_LogExprTestGroup, empty)
{
}

// 定数0ノードの検査
TEST(libym_logic_LogExprTestGroup, const0)
{
  LogExpr zero = LogExpr::make_zero();
  CHECK( zero.is_zero() );
  CHECK( !zero.is_one() );
  CHECK( zero.is_constant() );
  CHECK( !zero.is_posiliteral() );
  CHECK( !zero.is_negaliteral() );
  CHECK( !zero.is_literal() );
  CHECK( !zero.is_and() );
  CHECK( !zero.is_or() );
  CHECK( !zero.is_xor() );
  CHECK( !zero.is_op() );
  LONGS_EQUAL( 0, zero.child_num() );
  CHECK( !zero.is_simple() );
  CHECK( !zero.is_simple_and() );
  CHECK( !zero.is_simple_or() );
  CHECK( !zero.is_simple_xor() );
  CHECK( !zero.is_sop() );
  LONGS_EQUAL( 0, zero.litnum() );
  LONGS_EQUAL( 0, zero.input_size() );
  LONGS_EQUAL( 0, zero.sop_cubenum() );
  LONGS_EQUAL( 0, zero.sop_litnum() );
}

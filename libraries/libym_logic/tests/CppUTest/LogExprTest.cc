
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
    LogExpr::clear_memory();
  }

};


// 空のテスト
TEST(libym_logic_LogExprTestGroup, empty)
{
}

// 定数0ノードの検査
TEST(libym_logic_LogExprTestGroup, const0)
{
  LogExpr expr = LogExpr::make_zero();
  CHECK( expr.is_zero() );
  CHECK( !expr.is_one() );
  CHECK( expr.is_constant() );
  CHECK( !expr.is_posiliteral() );
  CHECK( !expr.is_negaliteral() );
  CHECK( !expr.is_literal() );
  CHECK( !expr.is_and() );
  CHECK( !expr.is_or() );
  CHECK( !expr.is_xor() );
  CHECK( !expr.is_op() );
  LONGS_EQUAL( 0, expr.child_num() );
  CHECK( expr.is_simple() );
  CHECK( !expr.is_simple_and() );
  CHECK( !expr.is_simple_or() );
  CHECK( !expr.is_simple_xor() );
  CHECK( expr.is_sop() );
  LONGS_EQUAL( 0, expr.litnum() );
  LONGS_EQUAL( 0, expr.input_size() );
  LONGS_EQUAL( 0, expr.sop_cubenum() );
  LONGS_EQUAL( 0, expr.sop_litnum() );
}

// 定数1ノードの検査
TEST(libym_logic_LogExprTestGroup, const1)
{
  LogExpr expr = LogExpr::make_one();
  CHECK( !expr.is_zero() );
  CHECK( expr.is_one() );
  CHECK( expr.is_constant() );
  CHECK( !expr.is_posiliteral() );
  CHECK( !expr.is_negaliteral() );
  CHECK( !expr.is_literal() );
  CHECK( !expr.is_and() );
  CHECK( !expr.is_or() );
  CHECK( !expr.is_xor() );
  CHECK( !expr.is_op() );
  LONGS_EQUAL( 0, expr.child_num() );
  CHECK( expr.is_simple() );
  CHECK( !expr.is_simple_and() );
  CHECK( !expr.is_simple_or() );
  CHECK( !expr.is_simple_xor() );
  CHECK( expr.is_sop() );
  LONGS_EQUAL( 0, expr.litnum() );
  LONGS_EQUAL( 0, expr.input_size() );
  LONGS_EQUAL( 0, expr.sop_cubenum() );
  LONGS_EQUAL( 0, expr.sop_litnum() );
}

// リテラルノードの検査(1)
TEST(libym_logic_LogExprTestGroup, literal1)
{
  const ymuint vnum = 1;
  const VarId vid(vnum);
  const tPol pol = kPolPosi;
  LogExpr expr = LogExpr::make_literal(vid, pol);

  CHECK( !expr.is_zero() );
  CHECK( !expr.is_one() );
  CHECK( !expr.is_constant() );
  CHECK( expr.is_posiliteral() );
  CHECK( !expr.is_negaliteral() );
  CHECK( expr.is_literal() );
  CHECK( vid == expr.varid() );
  CHECK( !expr.is_and() );
  CHECK( !expr.is_or() );
  CHECK( !expr.is_xor() );
  CHECK( !expr.is_op() );
  LONGS_EQUAL( 0, expr.child_num() );
  CHECK( expr.is_simple() );
  CHECK( !expr.is_simple_and() );
  CHECK( !expr.is_simple_or() );
  CHECK( !expr.is_simple_xor() );
  CHECK( expr.is_sop() );
  LONGS_EQUAL( 1, expr.litnum() );
  LONGS_EQUAL( vnum + 1, expr.input_size() );
  LONGS_EQUAL( 1, expr.sop_cubenum() );
  LONGS_EQUAL( 1, expr.sop_litnum() );
}

// リテラルノードの検査(2)
TEST(libym_logic_LogExprTestGroup, literal2)
{
  const ymuint vnum = 2;
  const VarId vid(vnum);
  const tPol pol = kPolPosi;
  const Literal lit(vid, pol);
  LogExpr expr = LogExpr::make_literal(lit);

  CHECK( !expr.is_zero() );
  CHECK( !expr.is_one() );
  CHECK( !expr.is_constant() );
  CHECK( expr.is_posiliteral() );
  CHECK( !expr.is_negaliteral() );
  CHECK( expr.is_literal() );
  CHECK( vid == expr.varid() );
  CHECK( !expr.is_and() );
  CHECK( !expr.is_or() );
  CHECK( !expr.is_xor() );
  CHECK( !expr.is_op() );
  LONGS_EQUAL( 0, expr.child_num() );
  CHECK( expr.is_simple() );
  CHECK( !expr.is_simple_and() );
  CHECK( !expr.is_simple_or() );
  CHECK( !expr.is_simple_xor() );
  CHECK( expr.is_sop() );
  LONGS_EQUAL( 1, expr.litnum() );
  LONGS_EQUAL( vnum + 1, expr.input_size() );
  LONGS_EQUAL( 1, expr.sop_cubenum() );
  LONGS_EQUAL( 1, expr.sop_litnum() );
}

// リテラルノードの検査(3)
TEST(libym_logic_LogExprTestGroup, literal3)
{
  const ymuint vnum = 3;
  const VarId vid(vnum);
  LogExpr expr = LogExpr::make_posiliteral(vid);

  CHECK( !expr.is_zero() );
  CHECK( !expr.is_one() );
  CHECK( !expr.is_constant() );
  CHECK( expr.is_posiliteral() );
  CHECK( !expr.is_negaliteral() );
  CHECK( expr.is_literal() );
  CHECK( vid == expr.varid() );
  CHECK( !expr.is_and() );
  CHECK( !expr.is_or() );
  CHECK( !expr.is_xor() );
  CHECK( !expr.is_op() );
  LONGS_EQUAL( 0, expr.child_num() );
  CHECK( expr.is_simple() );
  CHECK( !expr.is_simple_and() );
  CHECK( !expr.is_simple_or() );
  CHECK( !expr.is_simple_xor() );
  CHECK( expr.is_sop() );
  LONGS_EQUAL( 1, expr.litnum() );
  LONGS_EQUAL( vnum + 1, expr.input_size() );
  LONGS_EQUAL( 1, expr.sop_cubenum() );
  LONGS_EQUAL( 1, expr.sop_litnum() );
}

// リテラルノードの検査(4)
TEST(libym_logic_LogExprTestGroup, literal4)
{
  const ymuint vnum = 4;
  const VarId vid(vnum);
  LogExpr expr = LogExpr::make_negaliteral(vid);

  CHECK( !expr.is_zero() );
  CHECK( !expr.is_one() );
  CHECK( !expr.is_constant() );
  CHECK( !expr.is_posiliteral() );
  CHECK( expr.is_negaliteral() );
  CHECK( expr.is_literal() );
  CHECK( vid == expr.varid() );
  CHECK( !expr.is_and() );
  CHECK( !expr.is_or() );
  CHECK( !expr.is_xor() );
  CHECK( !expr.is_op() );
  LONGS_EQUAL( 0, expr.child_num() );
  CHECK( expr.is_simple() );
  CHECK( !expr.is_simple_and() );
  CHECK( !expr.is_simple_or() );
  CHECK( !expr.is_simple_xor() );
  CHECK( expr.is_sop() );
  LONGS_EQUAL( 1, expr.litnum() );
  LONGS_EQUAL( vnum + 1, expr.input_size() );
  LONGS_EQUAL( 1, expr.sop_cubenum() );
  LONGS_EQUAL( 1, expr.sop_litnum() );
}

// make_andの検査(1)
TEST(libym_logic_LogExprTestGroup, make_and1)
{
  Literal lit0(VarId(0), kPolPosi);
  LogExpr expr0 = LogExpr::make_literal(lit0);

  Literal lit1(VarId(1), kPolNega);
  LogExpr expr1 = LogExpr::make_literal(lit1);

  Literal lit2(VarId(2), kPolNega);
  LogExpr expr2 = LogExpr::make_literal(lit2);

  LogExprVector child_list(3);
  child_list[0] = expr0;
  child_list[1] = expr1;
  child_list[2] = expr2;

  LogExpr expr = LogExpr::make_and(child_list);

  CHECK( !expr.is_zero() );
  CHECK( !expr.is_one() );
  CHECK( !expr.is_constant() );
  CHECK( !expr.is_posiliteral() );
  CHECK( !expr.is_negaliteral() );
  CHECK( !expr.is_literal() );
  CHECK( expr.is_and() );
  CHECK( !expr.is_or() );
  CHECK( !expr.is_xor() );
  CHECK( expr.is_op() );
  LONGS_EQUAL( child_list.size(), expr.child_num() );
  CHECK( expr.is_simple() );
  CHECK( expr.is_simple_and() );
  CHECK( !expr.is_simple_or() );
  CHECK( !expr.is_simple_xor() );
  CHECK( expr.is_sop() );
  LONGS_EQUAL( child_list.size(), expr.litnum() );
  LONGS_EQUAL( 3, expr.input_size() );
  LONGS_EQUAL( 1, expr.sop_cubenum() );
  LONGS_EQUAL( 3, expr.sop_litnum() );
}



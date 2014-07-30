
/// @file ExprTest.cc
/// @brief Expr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "YmLogic/Expr.h"

using namespace nsYm;


TEST_GROUP(libym_logic_ExprTestGroup)
{
  TEST_SETUP() {
  }

  TEST_TEARDOWN() {
    Expr::clear_memory();
  }

};


// 空のテスト
TEST(libym_logic_ExprTestGroup, empty)
{
}

// 定数0ノードの検査
TEST(libym_logic_ExprTestGroup, const0)
{
  Expr expr = Expr::make_zero();
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
TEST(libym_logic_ExprTestGroup, const1)
{
  Expr expr = Expr::make_one();
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
TEST(libym_logic_ExprTestGroup, literal1)
{
  const ymuint vnum = 1;
  const VarId vid(vnum);
  const bool inv = false;
  Expr expr = Expr::make_literal(vid, inv);

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
TEST(libym_logic_ExprTestGroup, literal2)
{
  const ymuint vnum = 2;
  const VarId vid(vnum);
  const bool inv = false;
  const Literal lit(vid, inv);
  Expr expr = Expr::make_literal(lit);

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
TEST(libym_logic_ExprTestGroup, literal3)
{
  const ymuint vnum = 3;
  const VarId vid(vnum);
  Expr expr = Expr::make_posiliteral(vid);

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
TEST(libym_logic_ExprTestGroup, literal4)
{
  const ymuint vnum = 4;
  const VarId vid(vnum);
  Expr expr = Expr::make_negaliteral(vid);

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
TEST(libym_logic_ExprTestGroup, make_and1)
{
  Literal lit0(VarId(0), false);
  Expr expr0 = Expr::make_literal(lit0);

  Literal lit1(VarId(1), true);
  Expr expr1 = Expr::make_literal(lit1);

  Literal lit2(VarId(2), true);
  Expr expr2 = Expr::make_literal(lit2);

  ExprVector child_list(3);
  child_list[0] = expr0;
  child_list[1] = expr1;
  child_list[2] = expr2;

  Expr expr = Expr::make_and(child_list);

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

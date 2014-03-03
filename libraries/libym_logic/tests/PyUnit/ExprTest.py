#! /usr/bin/env python
# -*- coding: utf-8 -*-

import YmLogic
import unittest

class ExprTest(unittest.TestCase) :

    # 初期化
    def setUp(self) :
        pass

    # 終了処理
    def tearDown(self) :
        pass

    # 空のテスト
    def test_empty(self) :
        pass

    # 空のコンストラクタのテスト
    def test_constructor1(self) :
        expr = YmLogic.Expr()

    # 定数0ノードの検査
    def test_const0(self) :
        expr = YmLogic.Expr.make_zero()

        self.assertTrue( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertTrue( expr.is_constant() )
        self.assertFalse( expr.is_posiliteral() )
        self.assertFalse( expr.is_negaliteral() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, expr.child_num() )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 0, expr.litnum() )
        self.assertEqual( 0, expr.input_size() )
        self.assertEqual( 0, expr.sop_cubenum() )
        self.assertEqual( 0, expr.sop_litnum() )

    # 定数1ノードの検査
    def test_const1(self) :
        expr = YmLogic.Expr.make_one()

        self.assertFalse( expr.is_zero() )
        self.assertTrue( expr.is_one() )
        self.assertTrue( expr.is_constant() )
        self.assertFalse( expr.is_posiliteral() )
        self.assertFalse( expr.is_negaliteral() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, expr.child_num() )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 0, expr.litnum() )
        self.assertEqual( 0, expr.input_size() )
        self.assertEqual( 0, expr.sop_cubenum() )
        self.assertEqual( 0, expr.sop_litnum() )

    # リテラルノードの検査(1)
    def test_literal1(self) :
        vnum = 1
        varid = YmLogic.VarId(vnum)
        inv = False
        expr = YmLogic.Expr.make_literal(varid, inv)

        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertTrue( expr.is_posiliteral() )
        self.assertFalse( expr.is_negaliteral() )
        self.assertTrue( expr.is_literal() )
        self.assertEqual( varid, expr.varid() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, expr.child_num() )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 1, expr.litnum() )
        self.assertEqual( vnum + 1, expr.input_size() )
        self.assertEqual( 1, expr.sop_cubenum() )
        self.assertEqual( 1, expr.sop_litnum() )

    # リテラルノードの検査(2)
    def test_literal2(self) :
        vnum = 2
        varid = YmLogic.VarId(vnum)
        inv = True
        expr = YmLogic.Expr.make_literal(varid, inv)

        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posiliteral() )
        self.assertTrue( expr.is_negaliteral() )
        self.assertTrue( expr.is_literal() )
        self.assertEqual( varid, expr.varid() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, expr.child_num() )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 1, expr.litnum() )
        self.assertEqual( vnum + 1, expr.input_size() )
        self.assertEqual( 1, expr.sop_cubenum() )
        self.assertEqual( 1, expr.sop_litnum() )

    # リテラルノードの検査(3)
    def test_literal3(self) :
        vnum = 100
        varid = YmLogic.VarId(vnum)
        expr = YmLogic.Expr.make_posiliteral(varid)

        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertTrue( expr.is_posiliteral() )
        self.assertFalse( expr.is_negaliteral() )
        self.assertTrue( expr.is_literal() )
        self.assertEqual( varid, expr.varid() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, expr.child_num() )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 1, expr.litnum() )
        self.assertEqual( vnum + 1, expr.input_size() )
        self.assertEqual( 1, expr.sop_cubenum() )
        self.assertEqual( 1, expr.sop_litnum() )

    # リテラルノードの検査(4)
    def test_literal4(self) :
        vnum = 102
        varid = YmLogic.VarId(vnum)
        expr = YmLogic.Expr.make_negaliteral(varid)

        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posiliteral() )
        self.assertTrue( expr.is_negaliteral() )
        self.assertTrue( expr.is_literal() )
        self.assertEqual( varid, expr.varid() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, expr.child_num() )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 1, expr.litnum() )
        self.assertEqual( vnum + 1, expr.input_size() )
        self.assertEqual( 1, expr.sop_cubenum() )
        self.assertEqual( 1, expr.sop_litnum() )

    # make_and の検査(1)
    def test_make_and1(self) :
        varid0 = YmLogic.VarId(0)
        varid1 = YmLogic.VarId(1)
        varid2 = YmLogic.VarId(2)

        lit0 = YmLogic.Literal(varid0, False)
        lit1 = YmLogic.Literal(varid1, False)
        lit2 = YmLogic.Literal(varid2, False)

        expr0 = YmLogic.Expr.make_literal(lit0)
        expr1 = YmLogic.Expr.make_literal(lit1)
        expr2 = YmLogic.Expr.make_literal(lit2)
        chd_list = [ expr0, expr1, expr2 ]
        expr = YmLogic.Expr.make_and(chd_list)

        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posiliteral() )
        self.assertFalse( expr.is_negaliteral() )
        self.assertFalse( expr.is_literal() )
        self.assertTrue( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 3, expr.child_num() )
        self.assertTrue( expr.is_simple() )
        self.assertTrue( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 3, expr.litnum() )
        self.assertEqual( 3, expr.input_size() )
        self.assertEqual( 1, expr.sop_cubenum() )
        self.assertEqual( 3, expr.sop_litnum() )
        self.assertTrue( expr_equal(expr0, expr.child(0)) )
        self.assertTrue( expr_equal(expr1, expr.child(1)) )
        self.assertTrue( expr_equal(expr2, expr.child(2)) )

    # make_and の検査(2)
    def test_make_and1(self) :
        varid0 = YmLogic.VarId(0)
        varid1 = YmLogic.VarId(1)
        varid2 = YmLogic.VarId(2)

        lit0 = YmLogic.Literal(varid0, False)
        lit1 = YmLogic.Literal(varid1, False)
        lit2 = YmLogic.Literal(varid2, False)

        expr0 = YmLogic.Expr.make_literal(lit0)
        expr1 = YmLogic.Expr.make_literal(lit1)
        expr2 = YmLogic.Expr.make_literal(lit2)
        chd_list = [ expr0, expr1, expr2 ]
        expr = YmLogic.Expr.make_and(chd_list)

        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posiliteral() )
        self.assertFalse( expr.is_negaliteral() )
        self.assertFalse( expr.is_literal() )
        self.assertTrue( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 3, expr.child_num() )
        self.assertTrue( expr.is_simple() )
        self.assertTrue( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 3, expr.litnum() )
        self.assertEqual( 3, expr.input_size() )
        self.assertEqual( 1, expr.sop_cubenum() )
        self.assertEqual( 3, expr.sop_litnum() )
        self.assertTrue( expr_equal(expr0, expr.child(0)) )
        self.assertTrue( expr_equal(expr1, expr.child(1)) )
        self.assertTrue( expr_equal(expr2, expr.child(2)) )

def expr_equal(left, right) :
    if left.is_zero() :
        return right.is_zero()

    if left.is_one() :
        return right.is_one()

    if left.is_posiliteral() :
        return right.is_posiliteral() and lit_equal(left, right)

    if left.is_negaliteral() :
        return right.is_negaliteral() and lit_equal(left, right)

    if left.is_and() :
        return right.is_and() and op_equal(left, right)

    if left.is_or() :
        return right.is_or() and op_equal(left, right)

    if left.is_xor() :
        return right.is_xor() and op_equal(left, right)

    return False

def lit_equal(left, right) :
    return left.varid() == right.varid()

def opt_equal(left, right) :
    if left.child_num() != right.child_num() :
        return False

    for i in range(left.child_num()) :
        if not expr_equal(left.child(i), right.child(i)) :
            return False

    return True

# ExprTest のテストスイート
suite = unittest.TestLoader().loadTestsFromTestCase(ExprTest)


if __name__ == '__main__' :
    unittest.TextTestRunner(verbosity=4).run(suite)

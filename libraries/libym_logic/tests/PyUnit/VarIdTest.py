#! /usr/bin/env python
# -*- coding: utf-8 -*-

import YmLogic
import unittest

class VarIdTest(unittest.TestCase) :

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
        varid = YmLogic.VarId()

    # 値を指定したコンストラクタのテスト
    def test_constructor2(self) :
        varid = YmLogic.VarId(1)
        self.assertEqual( 1, varid.val() )

    # 等価比較演算子のテスト
    def test_equal(self) :
        varid1 = YmLogic.VarId(0)
        varid2 = YmLogic.VarId(0)
        varid3 = YmLogic.VarId(1)
        self.assertTrue( varid1 == varid2 )
        self.assertFalse( varid1 != varid2 )
        self.assertFalse( varid1 == varid3 )
        self.assertTrue( varid1 != varid3 )

    # 大小比較演算子のテスト
    def test_compare(self) :
        varid1 = YmLogic.VarId(0)
        varid2 = YmLogic.VarId(1)
        self.assertTrue( varid1 < varid2 )
        self.assertTrue( varid1 <= varid2 )
        self.assertFalse( varid1 > varid2 )
        self.assertFalse( varid1 >= varid2 )

    # __repr__ のテスト
    def test_repr(self) :
        varid = YmLogic.VarId(0)
        str = varid.__repr__()
        self.assertEqual( '0', str )

# VarIdTest のテストスイート
suite = unittest.TestLoader().loadTestsFromTestCase(VarIdTest)


if __name__ == '__main__' :
    unittest.TextTestRunner(verbosity=2).run(suite)

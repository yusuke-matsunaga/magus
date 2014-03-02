#! /usr/bin/env python
# -*- coding: utf-8 -*-

import YmUtils
import unittest

class CombiGenTest(unittest.TestCase) :

    # 初期化
    def setUp(self) :
        pass

    # 終了処理
    def tearDown(self) :
        pass

    # 空のテスト
    def test_empty(self) :
        pass

    # 組み合わせ列挙のテスト
    def test_combigen(self) :
        cg = YmUtils.CombiGen(5, 3)

        self.assertEqual( 5, cg.n() )
        self.assertEqual( 3, cg.k() )

        exp_data = [
            [0, 1, 2],
            [0, 1, 3],
            [0, 1, 4],
            [0, 2, 3],
            [0, 2, 4],
            [0, 3, 4],
            [1, 2, 3],
            [1, 2, 4],
            [1, 3, 4],
            [2, 3, 4]
        ]

        index = 0
        while not cg.is_end() :
            combi = cg.get()
            exp_combi = exp_data[index]
            self.assertEqual( combi, exp_combi )
            cg.next()
            index = index + 1

        # init() のテスト
        cg.init()
        index = 0
        while not cg.is_end() :
            combi = cg.get()
            exp_combi = exp_data[index]
            self.assertEqual( combi, exp_combi )
            cg.next()
            index = index + 1

# CombiGenTest のテストスイート
suite = unittest.TestLoader().loadTestsFromTestCase(CombiGenTest)


if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite)

#! /usr/bin/env python
# -*- coding: utf-8 -*-

import YmUtils
import unittest

class PermGenTest(unittest.TestCase) :

    # 初期化
    def setUp(self) :
        pass

    # 終了処理
    def tearDown(self) :
        pass

    # 空のテスト
    def test_empty(self) :
        pass

    # 順列列挙のテスト
    def test_permgen(self) :
        pg = YmUtils.PermGen(5, 3)

        self.assertEqual( 5, pg.n() )
        self.assertEqual( 3, pg.k() )

        exp_data = [
            [0, 1, 2],
            [0, 1, 3],
            [0, 1, 4],
            [0, 2, 1],
            [0, 2, 3],
            [0, 2, 4],
            [0, 3, 1],
            [0, 3, 2],
            [0, 3, 4],
            [0, 4, 1],
            [0, 4, 2],
            [0, 4, 3],
            [1, 0, 2],
            [1, 0, 3],
            [1, 0, 4],
            [1, 2, 0],
            [1, 2, 3],
            [1, 2, 4],
            [1, 3, 0],
            [1, 3, 2],
            [1, 3, 4],
            [1, 4, 0],
            [1, 4, 2],
            [1, 4, 3],
            [2, 0, 1],
            [2, 0, 3],
            [2, 0, 4],
            [2, 1, 0],
            [2, 1, 3],
            [2, 1, 4],
            [2, 3, 0],
            [2, 3, 1],
            [2, 3, 4],
            [2, 4, 0],
            [2, 4, 1],
            [2, 4, 3],
            [3, 0, 1],
            [3, 0, 2],
            [3, 0, 4],
            [3, 1, 0],
            [3, 1, 2],
            [3, 1, 4],
            [3, 2, 0],
            [3, 2, 1],
            [3, 2, 4],
            [3, 4, 0],
            [3, 4, 1],
            [3, 4, 2],
            [4, 0, 1],
            [4, 0, 2],
            [4, 0, 3],
            [4, 1, 0],
            [4, 1, 2],
            [4, 1, 3],
            [4, 2, 0],
            [4, 2, 1],
            [4, 2, 3],
            [4, 3, 0],
            [4, 3, 1],
            [4, 3, 2]
        ]

        index = 0
        while not pg.is_end() :
            combi = pg.get()
            exp_combi = exp_data[index]
            self.assertEqual( combi, exp_combi )
            pg.next()
            index = index + 1

        # init() のテスト
        pg.init()
        index = 0
        while not pg.is_end() :
            combi = pg.get()
            exp_combi = exp_data[index]
            self.assertEqual( combi, exp_combi )
            pg.next()
            index = index + 1


# PermGenTest のテストスイート
suite = unittest.TestLoader().loadTestsFromTestCase(PermGenTest)


if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite)

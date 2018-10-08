#! /usr/bin/env python3

### @file read_iscas89_test.py
### @brief read_iscas89 のテストスクリプト
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

import unittest
import os
from magus_core import BnNetwork

# read_iscas89 のテスト用クラス
class ReadIscas89Test(unittest.TestCase) :

    def runTest(self) :
        TESTDATA_DIR = os.environ.get('TESTDATA_DIR')
        filename = os.path.join(TESTDATA_DIR, 'b10.bench')
        network = BnNetwork.read_iscas89(filename)
        ni = 11
        no = 6
        nd = 17
        ng = 172
        self.assertEqual( ni + nd + 1, network.input_num )
        self.assertEqual( no + nd + nd , network.output_num )
        self.assertEqual( ng, network.logic_num )
        self.assertEqual( ni + no + 1, network.port_num )
        self.assertEqual( nd, network.dff_num )


if __name__ == '__main__' :

    unittest.main()

#! /usr/bin/env python3

### @file read_blif_test.py
### @brief read_blif のテスト
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

import unittest
import os
from magus_core import BnNetwork

# read_blif のテスト用クラス
class ReadBlifTest(unittest.TestCase) :

    def runTest(self) :
        TESTDATA_DIR = os.environ.get('TESTDATA_DIR')
        filename = os.path.join(TESTDATA_DIR, 's5378.blif')
        network = BnNetwork.read_blif(filename)
        ni = 35
        no = 49
        nd = 179
        ng = 2779
        self.assertEqual( ni + nd + 1, network.input_num )
        self.assertEqual( no + nd + nd , network.output_num )
        self.assertEqual( ng, network.logic_num )
        self.assertEqual( ni + no + 1, network.port_num )
        self.assertEqual( nd, network.dff_num )


if __name__ == '__main__' :

    unittest.main()

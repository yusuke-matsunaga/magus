#! /usr/bin/env python3

### @file equiv_test.py
### @brief equiv のテスト
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

import unittest
import os
from magus_core import BnNetwork
from magus_core import EquivMgr
from magus_core import SatBool3

# EquivMgr のテスト用クラス
class EquivTest(unittest.TestCase) :

    def runTest(self) :
        TESTDATA_DIR = os.environ.get('TESTDATA_DIR')

        filename1 = os.path.join(TESTDATA_DIR, 'C499.blif')
        network1 = BnNetwork.read_blif(filename1)
        filename2 = os.path.join(TESTDATA_DIR, 'C1355.blif')
        network2 = BnNetwork.read_blif(filename2)

        input1_list = network1.input_id_list
        input2_list = network2.input_id_list
        output1_list = network1.output_id_list
        output2_list = network2.output_id_list

        eqmgr = EquivMgr(1000)
        stat, stat_list = eqmgr.check(network1, input1_list, output1_list,
                                      network2, input2_list, output2_list)
        self.assertEqual( stat, SatBool3._True )
        for stat1 in stat_list :
            self.assertEqual( stat1, SatBool3._True )

if __name__ == '__main__' :

    unittest.main()

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
        self.test1()
        self.test2()

    def test1(self) :
        TESTDATA_DIR = os.environ.get('TESTDATA_DIR')

        filename1 = os.path.join(TESTDATA_DIR, 'C499.blif')
        network1 = BnNetwork.read_blif(filename1)
        filename2 = os.path.join(TESTDATA_DIR, 'C1355.blif')
        network2 = BnNetwork.read_blif(filename2)

        eqmgr = EquivMgr(1)
        stat, stat_list = eqmgr.check_by_order(network1, network2)
        self.assertEqual( stat, SatBool3._True )
        for stat1 in stat_list :
            self.assertEqual( stat1, SatBool3._True )

    def test2(self) :
        TESTDATA_DIR = os.environ.get('TESTDATA_DIR')

        filename1 = os.path.join(TESTDATA_DIR, 'C499.blif')
        network1 = BnNetwork.read_blif(filename1)
        filename2 = os.path.join(TESTDATA_DIR, 'C1355.blif')
        network2 = BnNetwork.read_blif(filename2)

        ni = network1.input_num
        no = network1.output_num
        input2_list = [ i for i in range(ni) ]
        output2_list = [ i for i in range(no) ]

        eqmgr = EquivMgr(1)
        stat, stat_list = eqmgr.check(network1, network2, input2_list, output2_list)
        self.assertEqual( stat, SatBool3._True )
        for stat1 in stat_list :
            self.assertEqual( stat1, SatBool3._True )

    def test3(self) :
        TESTDATA_DIR = os.environ.get('TESTDATA_DIR')

        filename1 = os.path.join(TESTDATA_DIR, 'C499.blif')
        network1 = BnNetwork.read_blif(filename1)
        filename2 = os.path.join(TESTDATA_DIR, 'C499_reordered.blif')
        network2 = BnNetwork.read_blif(filename2)

        ni = network1.input_num
        no = network1.output_num

        input_map = dict()
        for pos in range(ni) :
            id1 = network1.input_id(pos)
            node = network1.node(id1)
            input_map[node.name] = pos

        output_map = dict()
        for pos in range(no) :
            id1 = network1.output_id(pos)
            node = network2.node(id1)
            output_map[node.name] = pos

        input2_list = []
        for pos in range(ni) :
            id2 = network2.input_id(pos)
            node2 = network2.node(id2)
            self.assertTrue( node2.name in input_map )
            pos1 = input_map[node2.name]
            input2_list.append(pos1)

        output2_list = []
        for pos in range(no) :
            id2 = network2.output_id(pos)
            node2 = network2.node(id2)
            self.assertTrue( node2.name in output_map )
            pos1 = output_map[node2.name]
            output2_list.append(pos1)

        eqmgr = EquivMgr(1)
        stat, stat_list = eqmgr.check(network1, network2, input2_list, output2_list)
        self.assertEqual( stat, SatBool3._True )
        for stat1 in stat_list :
            self.assertEqual( stat1, SatBool3._True )

if __name__ == '__main__' :

    unittest.main()

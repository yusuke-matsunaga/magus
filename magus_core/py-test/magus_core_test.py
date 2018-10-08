#! /usr/bin/env python3

### @file magus_core_test.py
### @brief magus_core のテストスクリプト
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

import unittest
from read_blif_test import ReadBlifTest
from read_iscas89_test import ReadIscas89Test
from equiv_test import EquivTest

suite = unittest.TestSuite()
suite.addTest(ReadBlifTest())
suite.addTest(ReadIscas89Test())
suite.addTest(EquivTest())


if __name__ == '__main__' :

    unittest.main()

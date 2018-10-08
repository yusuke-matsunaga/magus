
### @file CXX_SbjPort.pxd
### @brief SbjPort 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp.string cimport string
from CXX_SbjNode cimport SbjNode

cdef extern from "SbjPort.h" namespace "nsYm::nsMagus" :

    ### @brief SbjPort の cython バージョン
    cdef cppclass SbjPort :
        string name()
        int bit_width()
        const SbjNode* bit(int)

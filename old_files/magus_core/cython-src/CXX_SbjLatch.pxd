
### @file CXX_SbjLatch.pxd
### @brief SbjLatch 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from CXX_SbjNode cimport SbjNode

cdef extern from "SbjLatch.h" namespace "nsYm::nsMagus" :

    ### @brief SbjLatch の cython バージョン
    cdef cppclass SbjLatch :
        int id()
        const SbjNode* data_input()
        const SbjNode* enable()
        const SbjNode* clear()
        const SbjNode* preset()
        const SbjNode* data_output()

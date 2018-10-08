
### @file CXX_SbjDff.pxd
### @brief SbjDff 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from CXX_SbjNode cimport SbjNode

cdef extern from "SbjDff.h" namespace "nsYm::nsMagus" :

    ### @brief SbjDff の cython バージョン
    cdef cppclass SbjDff :
        int id()
        const SbjNode* data_input()
        const SbjNode* clock()
        const SbjNode* clear()
        const SbjNode* preset()
        const SbjNode* data_output()

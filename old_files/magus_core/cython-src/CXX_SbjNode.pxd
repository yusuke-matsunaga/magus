
### @file CXX_SbjNode.pxd
### @brief SbjNode 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool

cdef extern from "SbjNode.h" namespace "nsYm::nsMagus" :

    ### @brief SbjNode の cython バージョン
    cdef cppclass SbjNode :
        int id()
        bool is_input()
        bool is_bipol()
        bool is_output()
        bool is_logic()
        bool is_and()
        bool is_xor()
        int subid()
        const SbjNode* output_fanin()
        bool output_fanin_inv()
        const SbjNode* fanin0()
        bool fanin0_inv()
        const SbjNode* fanin1()
        bool fanin1_inv()
        int fanout_num()
        const SbjNode* fanout(int)
        bool pomark()
        int level()

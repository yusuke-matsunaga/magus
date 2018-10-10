
### @file CXX_LutmapMgr.pxd
### @brief LutmapMgr 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp.string cimport string
from CXX_BnNetwork cimport BnNetwork

cdef extern from "LutmapMgr.h" namespace "nsYm::nsMagus" :

    ### @brief LutmapMgr の cython バージョン
    cdef cppclass LutmapMgr :
        LutmapMgr()
        void set_lut_size(int)
        void set_option(const string&)
        void area_map(const BnNetwork&, BnNetwork&)
        void delay_map(const BnNetwork&, int, BnNetwork&)
        int lut_num()
        int depth()

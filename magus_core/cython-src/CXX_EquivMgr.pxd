
### @file CXX_EquivMgr.pxd
### @brief EquivMgr 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp.vector cimport vector
from CXX_SatSolverType cimport SatSolverType
from CXX_SatBool3 cimport SatBool3
from CXX_BnNetwork cimport BnNetwork

cdef extern from "EquivMgr.h" namespace "nsYm::nsMagus" :

    ### @brief EquivMgr の cython バージョン
    cdef cppclass EquivMgr :
        EquivMgr(int, const SatSolverType&)
        SatBool3 check(const BnNetwork&,
                       const vector[int]&,
                       const vector[int]&,
                       const BnNetwork&,
                       const vector[int]&,
                       const vector[int]&,
                       vector[SatBool3]& eq_stats)
        void set_loglevel(int level)
        void set_loop_lomit(int loop_limit)


### @file CXX_EquivMgr.pxd
### @brief EquivMgr 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp.pair cimport pair
from libcpp.vector cimport vector
from CXX_SatSolverType cimport SatSolverType
from CXX_SatBool3 cimport SatBool3
from CXX_BnNetwork cimport BnNetwork

cdef extern from "EquivMgr.h" namespace "nsYm::nsMagus" :

    ### @brief EquivResult の cython バージョン
    cdef cppclass EquivResult :
        SatBool3 result()
        const vector[SatBool3]& output_results()

    ### @brief EquivMgr の cython バージョン
    cdef cppclass EquivMgr :
        EquivMgr(int, const SatSolverType&)
        EquivResult check(const BnNetwork&,
                          const BnNetwork&)
        EquivResult check(const BnNetwork&,
                          const BnNetwork&,
                          const vector[pair[int, int]]&,
                          const vector[pair[int, int]]&)
        void set_signature_size(int sig_size)
        void set_sat_solver_type(const SatSolverType& solver_type)
        void set_loglevel(int level)
        void set_loop_lomit(int loop_limit)

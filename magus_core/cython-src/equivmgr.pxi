
### @file equivmgr.pxi
### @brief equivmgr の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp.vector cimport vector
from CXX_EquivMgr cimport EquivMgr as CXX_EquivMgr

### @brief EquivMgr の Python バージョン
cdef class EquivMgr :

    # データメンバ
    cdef CXX_EquivMgr* _thisptr

    ### @brief 初期化
    def __cinit__(self, int sig_size, SatSolverType solver_type = SatSolverType()) :
        self._thisptr = new CXX_EquivMgr(sig_size, solver_type._this)

    ### @brief 終了処理
    def __dealloc__(self) :
        if self._thisptr :
            del self._thisptr

    ### @brief 等価検証を行う．
    def check_by_order(self, BnNetwork network1, BnNetwork network2) :
        cdef int no = network1.output_num
        cdef vector[CXX_SatBool3] c_eq_stats
        cdef CXX_SatBool3 c_stat
        c_eq_stats.resize(network1.output_num)
        c_stat = self._thisptr.check(network1._this, network2._this, c_eq_stats)
        eq_stats = [ to_SatBool3(c_eq_stats[i]) for i in range(no) ]
        return to_SatBool3(c_stat), eq_stats

    ### @brief 等価検証を行う．
    def check(self,
              BnNetwork network1, input1_list, output1_list,
              BnNetwork network2, input2_list, output2_list) :
        cdef vector[int] c_input1_list
        cdef vector[int] c_output1_list
        cdef vector[int] c_input2_list
        cdef vector[int] c_output2_list
        cdef vector[CXX_SatBool3] c_eq_stats
        cdef int ni = network1.input_num
        cdef int no = network1.output_num

        assert len(input1_list) == ni
        assert len(input2_list) == ni
        assert len(output1_list) == no
        assert len(output2_list) == no

        c_input1_list.resize(ni)
        c_input2_list.resize(ni)
        for i in range(ni) :
            c_input1_list[i] = input1_list[i]
            c_input2_list[i] = input2_list[i]

        c_output1_list.resize(no)
        c_output2_list.resize(no)
        c_eq_stats.resize(no)
        for i in range(no) :
            c_output1_list[i] = output1_list[i]
            c_output2_list[i] = output2_list[i]

        c_stat = self._thisptr.check(network1._this, c_input1_list, c_output1_list,
                                     network2._this, c_input2_list, c_output2_list,
                                     c_eq_stats)

        eq_stats = [ to_SatBool3(c_eq_stats[i]) for i in range(no) ]
        return to_SatBool3(c_stat), eq_stats

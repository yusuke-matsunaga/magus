
### @file equivmgr.pxi
### @brief equivmgr の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp.pair cimport pair
from libcpp.vector cimport vector
from CXX_EquivMgr cimport EquivResult as CXX_EquivResult
from CXX_EquivMgr cimport EquivMgr as CXX_EquivMgr

### @brief EquivMgr の Python バージョン
cdef class EquivMgr :

    # データメンバ
    cdef CXX_EquivMgr _this

    ### @brief 等価検証を行う．
    def check_by_order(self, BnNetwork network1, BnNetwork network2) :
        cdef int no = network1.output_num
        cdef CXX_EquivResult c_result
        c_result = self._this.check(network1._this, network2._this)
        eq_stats = [ to_SatBool3(c_result.output_results()[i]) for i in range(no) ]
        return to_SatBool3(c_result.result()), eq_stats

    ### @brief 等価検証を行う．
    def check(self,
              BnNetwork network1,
              BnNetwork network2,
              input_pair_list, output_pair_list) :
        cdef vector[pair[int, int]] c_input_pair_list
        cdef vector[pair[int, int]] c_output_pair_list
        cdef int ni = network1.input_num
        cdef int no = network1.output_num
        cdef CXX_EquivResult c_result

        assert len(input_pair_list) == ni
        assert len(output_pair_list) == no

        c_input_pair_list.resize(ni)
        for i in range(ni) :
            id1, id2 = input_pair_list[i];
            c_input_pair_list[i].first = id1
            c_input_pair_list[i].second = id2

        c_output_pair_list.resize(no)
        for i in range(no) :
            id1, id2 = output_pair_list[i]
            c_output_pair_list[i].first = id1
            c_output_pair_list[i].second = id2

        c_result = self._this.check(network1._this, network2._this,
                                    c_input_pair_list, c_output_pair_list)

        eq_stats = [ to_SatBool3(c_result.output_results()[i]) for i in range(no) ]
        return to_SatBool3(c_result.result()), eq_stats

    ### @brief シグネチャサイズをセットする．
    def set_signature_size(self, int size) :
        self._this.set_signature_size(size)

    ### @brief SATソルバの種類をセットする．
    def set_sat_solver_type(self, SatSolverType solver_type) :
        self._this.set_sat_solver_type(solver_type._this)

    ### @brief ログレベルをセットする．
    def set_loglevel(self, int level) :
        self._this.set_loglevel(level)

    ### @brief ループ回数をセットする．
    def set_loop_limit(self, int loop_limit) :
        self._this.set_loop_limit(loop_limit)

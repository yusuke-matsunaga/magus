
### @file CXX_SbjGraph.pxd
### @brief SbjGraph 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp.string cimport string
from CXX_SbjPort cimport SbjPort
from CXX_SbjNode cimport SbjNode
from CXX_SbjDff cimport SbjDff
from CXX_SbjLatch cimport SbjLatch

### @brief SbjGraph の cython バージョン
cdef extern from "SbjGraph.h" namespace "nsYm::nsMagus" :

    cdef cppclass SbjGraph :
        SbjGraph()
        SbjGraph(const SbjGraph&)
        string name()
        int port_num()
        const SbjPort* port(int)
        int node_num()
        const SbjNode* node(int)
        int input_num()
        const SbjNode* input(int)
        int output_num()
        const SbjNode* output(int)
        int logic_num()
        const SbjNode* logic(int)
        int level()
        int dff_num()
        const SbjDff* dff(int)
        int latch_num()
        const SbjLatch* latch(int)

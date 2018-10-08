
### @file sbjgraph.pxi
### @brief sbjgraph の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from CXX_Bn2Sbj cimport Bn2Sbj as CXX_Bn2Sbj
from CXX_SbjGraph cimport SbjGraph as CXX_SbjGraph
from CXX_SbjPort cimport SbjPort as CXX_SbjPort
from CXX_SbjDff cimport SbjDff as CXX_SbjDff
from CXX_SbjLatch cimport SbjLatch as CXX_SbjLatch
from CXX_SbjNode cimport SbjNode as CXX_SbjNode

### @brief SbjGraph の python バージョン
cdef class SbjGraph :

    # データメンバ
    cdef CXX_SbjGraph _this

    ### @brief 初期化
    def __init__(self, BnNetwork src_network = None) :
        if src_network :
            self.set_from_bnet(src_network)

    ### @brief BnNetwork から変換する．
    def set_from_bnet(self, BnNetwork src_network) :
        cdef CXX_Bn2Sbj bn2sbj
        bn2sbj.convert(src_network._this, self._this)

    ### @brief 名前を返す．
    @property
    def name(self) :
        return self._this.name().decode('UTF-8')

    ### @brief ポート数を返す．
    @property
    def port_num(self) :
        return self._this.port_num()

    ### @brief ポートの情報を得る．
    ### @param[in] port_id ポート番号 ( 0 <= port_id < port_num )
    def port(self, int port_id) :
        cdef const CXX_SbjPort* c_port = self._this.port(port_id)
        cdef str name = c_port.name().decode('UTF-8')
        cdef int n = c_port.bit_width()
        cdef const CXX_SbjNode* c_node
        bit_list = []
        for i in range(n) :
            c_node = c_port.bit(i)
            bit_list.append(c_node.id())
        return SbjPort(name, bit_list)

    ### @brief DFF数を返す．
    @property
    def dff_num(self) :
        return self._this.dff_num()

    ### @brief DFFを返す．
    def dff(self, int dff_id) :
        cdef const CXX_SbjDff* c_dff = self._this.dff(dff_id)
        cdef int id = c_dff.id()
        cdef const CXX_SbjNode* c_node
        c_node = c_dff.data_input()
        if c_node :
            data_input = c_node.id()
        else :
            data_input = -1
        c_node = c_dff.clock()
        if c_node :
            clock = c_node.id()
        else :
            clock = -1
        c_node = c_dff.clear()
        if c_node :
            clear = c_node.id()
        else :
            clear = -1
        c_node = c_dff.preset()
        if c_node :
            preset = c_node.id()
        else :
            preset = -1
        c_node = c_dff.data_output()
        if c_node :
            data_output = c_node.id()
        else :
            data_output = -1
        return SbjDff(id, data_input, clock, clear, preset, data_output)

    ### @brief ラッチ数を返す．
    @property
    def latch_num(self) :
        return self._this.latch_num()

    ### @brief ラッチを返す．
    def latch(self, int latch_id) :
        cdef const CXX_SbjLatch* c_latch = self._this.latch(latch_id)
        cdef int id = c_latch.id()
        cdef const CXX_SbjNode* c_node
        c_node = c_latch.data_input()
        if c_node :
            data_input = c_node.id()
        else :
            data_input = -1
        c_node = c_latch.enable()
        if c_node :
            enable = c_node.id()
        else :
            enable = -1
        c_node = c_latch.clear()
        if c_node :
            clear = c_node.id()
        else :
            clear = -1
        c_node = c_latch.preset()
        if c_node :
            preset = c_node.id()
        else :
            preset = -1
        c_node = c_latch.data_output()
        if c_node :
            data_output = c_node.id()
        else :
            data_output = -1
        return SbjLatch(id, data_input, enable, clear, preset, data_output)

    ### @brief ノード数を返す．
    @property
    def node_num(self) :
        return self._this.node_num()

    ### @brief ノードを返す．
    ### @param[in] node_id ノード番号 ( 0 <= node_id < node_num )
    def node(self, int node_id) :
        cdef const CXX_SbjNode* c_node = self._this.node(node_id)
        cdef int id = c_node.id()
        cdef bool pomark = c_node.pomark()
        cdef int level = c_node.level()
        cdef const CXX_SbjNode* c_node0
        cdef bool inv0
        cdef const CXX_SbjNode* c_node1
        cdef bool inv1
        cdef int fonum = c_node.fanout_num()
        fanout_list = []
        for i in range(fonum) :
            c_node0 = c_node.fanout(i)
            fanout_list.append(c_node0.id())
        ans = SbjNode(id, fanout_list, pomark, level)
        if c_node.is_input() :
            ans.set_input(c_node.subid())
        elif c_node.is_output() :
            c_node0 = c_node.output_fanin()
            inv0 = c_node.output_fanin_inv()
            ans.set_output(c_node.subid(), c_node0.id(), inv0)
        elif c_node.is_and() :
            c_node0 = c_node.fanin0()
            inv0 = c_node.fanin0_inv()
            c_node1 = c_node.fanin1()
            inv1 = c_node.fanin1_inv()
            ans.set_and(c_node0.id(), inv0, c_node1.id(), inv1)
        elif c_node.is_xor() :
            c_node0 = c_node.fanin0()
            inv0 = c_node.fanin0_inv()
            c_node1 = c_node.fanin1()
            inv1 = c_node.fanin1_inv()
            ans.set_xor(c_node0.id(), inv0, c_node1.id(), inv1)
        return ans

    ### @brief 入力数を返す．
    @property
    def input_num(self) :
        return self._this.input_num()

    ### @brief 入力ノードを返す．
    ### @param[in] input_id 入力番号 ( 0 = input_id < input_num )
    def input(self, int input_id) :
        cdef const CXX_SbjNode* c_node = self._this.input(input_id)
        return self.node(c_node.id())

    ### @brief 出力数を返す．
    @property
    def output_num(self) :
        return self._this.output_num()

    ### @brief 出力ノードを返す．
    ### @param[in] output_id 出力番号 ( 0 <= output_id < output_id )
    def output(self, int output_id) :
        cdef const CXX_SbjNode* c_node = self._this.output(output_id)
        return self.node(c_node.id())

    ### @brief 論理ノード数を返す．
    @property
    def logic_num(self) :
        return self._this.logic_num()

    ### @brief 論理ノードを返す．
    ### @param[in] logic_id 論理ノード番号 ( 0 <= logic_id < logic_num )
    def logic(self, int logic_id) :
        cdef const CXX_SbjNode* c_node = self._this.logic(logic_id)
        return self.node(c_node.id())

    ### @brief 最大レベルを返す．
    @property
    def level(self) :
        return self._this.level()


### @file sbjnode.pxi
### @brief sbjnode の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.


### @brief SbjNode の Python バージョン
cdef class SbjNode :

    # データメンバ
    cdef int __id
    cdef int __type  # 0: input, 1: output, 2: and, 3: xor
    cdef int __fanin0_id
    cdef bool __fanin0_inv
    cdef int __fanin1_id
    cdef bool __fanin1_inv
    cdef list __fanout_list
    cdef bool __pomark
    cdef int __level

    ### @brief 初期化
    def __init__(self, int id, fanout_list, bool pomark, int level) :
        self.__id = id
        self.__fanin0_id = -1
        self.__fanin0_inv = False
        self.__fanin1_id = -1
        self.__fanin1_inv = False
        self.__fanout_list = fanout_list
        self.__pomark = pomark
        self.__level = level

    ### @brief 外部入力用の設定を行う．
    def set_input(self, int input_id) :
        self.__type = 0
        self.__fanin1_id = input_id

    ### @brief 外部出力の設定を行う．
    def set_output(self, int output_id, int fanin_id, int fanin_inv) :
        self.__type = 1
        self.__fanin0_id = fanin_id
        self.__fanin0_inv = fanin_inv
        self.__fanin1_id = output_id

    ### @brief ANDノードの設定を行う．
    def set_and(self, int fanin0_id, bool fanin0_inv, int fanin1_id, bool fanin1_inv) :
        self.__type = 2
        self.__fanin0_id = fanin0_id
        self.__fanin0_inv = fanin0_inv
        self.__fanin1_id = fanin1_id
        self.__fanin1_inv = fanin1_id

    ### @brief XORノードの設定を行う．
    def set_xor(self, int fanin0_id, bool fanin0_inv, int fanin1_id, bool fanin1_inv) :
        self.__type = 3
        self.__fanin0_id = fanin0_id
        self.__fanin0_inv = fanin0_inv
        self.__fanin1_id = fanin1_id
        self.__fanin1_inv = fanin1_id

    ### @brief ID番号を返す．
    @property
    def id(self) :
        return self.__id

    ### @brief 外部入力の時に True を返す．
    def is_input(self) :
        return self.__type == 0

    ### @brief 外部出力の時に True を返す．
    def is_output(self) :
        return self.__type == 1

    ### @brief ANDの時に True を返す．
    def is_and(self) :
        return self.__type == 2

    ### @brief XORの時に True を返す．
    def is_xor(self) :
        return self.__type == 3

    ### @brief 外部入力/外部出力の時に subid を返す．
    def subid(self) :
        return self.__fanin1_id

    ### @brief fanin0 の情報を返す．
    ### (int node_id, bool inv) のタプルを返す．
    def fanin0(self) :
        return self.__fanin0_id, self.__fanin0.inv

    ### @brief fanin1 の情報を返す．
    ### (int node_id, bool inv) のタプルを返す．
    def fanin1(self) :
        return self.__fanin1_id, self.__fanin1.inv

    ### @brief ファンアウトリストを返す．
    def fanout_list(self) :
        return self.__fanout_list

    ### @brief 外部出力にファンアウトしている時 True を返す．
    @property
    def pomark(self) :
        return self.__pomark

    ### @brief レベルを返す．
    @property
    def level(self) :
        return self.__level

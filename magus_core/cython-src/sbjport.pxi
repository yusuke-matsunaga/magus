
### @file sbjport.pxi
### @brief sbjport の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.


### @brief SbjPort の Python バージョン
cdef class SbjPort :

    # データメンバ
    cdef str __name
    cdef list __bit_list

    ### @brief 初期化
    def __init__(self, str name, bit_list) :
        self.__name = name
        self.__bit_list = bit_list

    ### @brief 名前を返す．
    @property
    def name(self) :
        return self.__name

    ### @brief ビット数を返す．
    @property
    def bit_width(self) :
        return len(self.__bit_list)

    ### @brief ビットの内容(ノード番号)を返す．
    ### @param[in] bit_pos ビット位置 ( 0 <= bit_pos < bit_width )
    def bit(self, int bit_pos) :
        return self.__bit_list[bit_pos]

    ### @brief ビットの内容をリストで返す．
    @property
    def bits(self) :
        return self.__bit_list

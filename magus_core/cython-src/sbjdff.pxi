
### @file sbjdff.pxi
### @brief sbjdff の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.


### @brief SbjDff の Python バージョン
cdef class SbjDff :

    # データメンバ
    cdef int __id
    cdef int __data_input
    cdef int __clock
    cdef int __clear
    cdef int __preset
    cdef int __data_output

    ### @brief 初期化
    def __init__(self, int id, int data_input, int clock, int data_output, int clear = -1, int preset = -1 ) :
        self.__id = id
        self.__data_input = data_input
        self.__clock = clock
        self.__clear = clear
        self.__preset = preset
        self.__data_output = data_output

    ### @brief ID番号を返す．
    @property
    def id(self) :
        return self.__id

    ### @brief データ入力のノード番号を返す．
    @property
    def data_input(self) :
        return self.__data_input

    ### @brief クロック入力のノード番号を返す．
    @property
    def clock(self) :
        return self.__clock

    ### @brief クリア入力のノード番号を返す．
    @property
    def clear(self) :
        return self.__clear

    ### @brief プリセット入力のノード番号を返す．
    @property
    def preset(self) :
        return self.__preset

    ### @brief データ出力のノード番号を返す．
    @property
    def data_output(self) :
        return self.__data_output

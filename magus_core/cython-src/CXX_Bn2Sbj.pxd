
### @file CXX_Bn2Sbj.pxd
### @brief CXX_Bn2Sbj 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from CXX_BnNetwork cimport BnNetwork
from CXX_SbjGraph cimport SbjGraph

cdef extern from "Bn2Sbj.h" namespace "nsYm::nsMagus" :

    ### @brief Bn2Sbj の cython バージョン
    cdef cppclass Bn2Sbj :
        Bn2Sbj()
        void convert(const BnNetwork&, SbjGraph&)

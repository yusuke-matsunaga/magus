
### @file lutmapmgr.pxi
### @brief LutmapMgr の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from CXX_LutmapMgr cimport LutmapMgr as CXX_LutmapMgr


### @brief LutmapMgr の Python バージョン
cdef class LutmapMgr :

    # データメンバ
    cdef CXX_LutmapMgr _this

    ### @brief 初期化
    def __init__(self, int lut_size = 5, str option = '') :
        cdef string c_option = option.encode('UTF-8')
        self._this.set_lut_size(lut_size)
        self._this.set_option(c_option)

    ### @brief 面積モードでマッピングする．
    def area_map(self, BnNetwork src_network) :
        dst_network = BnNetwork()
        self._this.area_map(src_network._this, dst_network._this)
        lut_num = self._this.lut_num()
        depth = self._this.depth()
        return dst_network, lut_num, depth

    ### @brief 遅延(段数)モードでマッピングする．
    def area_map(self, BnNetwork src_network, int slack) :
        dst_network = BnNetwork()
        self._this.delay_map(src_network._this, slack, dst_network._this)
        lut_num = self._this.lut_num()
        depth = self._this.depth()
        return dst_network, lut_num, depth

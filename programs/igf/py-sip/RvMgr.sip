
/// @file RvMgr.sip
/// @brief RvMgr の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class RvMgr
{
%TypeHeaderCode
#include "RvMgr.h"
using nsYm::nsIgf::RvMgr;
%End

  /// @brief データを読み込む．
  /// @param[in] filename 読み込み元のファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_data(const char* filename);
  %MethodCode
  std::ifstream ifs(a0);
  sipRes = false;
  if ( ifs ) {
    sipRes = sipCpp->read_data(ifs);
  }
  %End

  /// @brief ベクタのサイズを得る．
  unsigned int
  vect_size() const;

  /// @brief ベクタのリストを得る．
  SIP_PYLIST
  vect_list() const;
  %MethodCode
  const std::vector<const RegVect*> vect_list = sipCpp->vect_list();
  ymuint n = vect_list.size();
  sipRes = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = sipConvertFronNewType(vect_list[i], sipType_RegVect, NULL);
    PyLiset_SetItem(sipRes, i, obj1);
  }
  %End

  /// @brief インデックスのサイズを得る．
  unsigned int
  index_size() const;

  /// @brief 内容を出力する．
  void
  dump(const char* filename) const;
  %MethodCode
  std::ofstream ofs(a0);
  if ( ofs ) {
    sipCpp->dump(ofs);
  }
  %End

};

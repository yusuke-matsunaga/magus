
/// @file PhfGen.h
/// @brief PhfGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class PhfGen
{
%TypeHeaderCode
#include "PhfGen.h"
using nsYm::nsIgf::PhfGen;
%End

  /// @brief マッピングを求める．
  /// @param[in] func_list 関数のリスト
  /// @param[out] g_list マッピングのリスト
  SIP_PYLIST
  mapping(SIP_PYLIST);
  %MethodCode
  ymuint n = PyList_Size(a0);
  std::vector<const FuncVector*> func_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GetItem(a0, i);
    if ( !sipCanConvertToType(obj1, sipType_FuncVect, 0) ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    void* p = sipConvertToType(obj1, sipType_FuncVect, NULL, 0, NULL, &sipIsErr);
    if ( sipIsErr ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    const FuncVect* fv = reinterpret_cast<const FuncVect*>(p);
    func_list[i] = fv;
  }
  std::vector<vector<ymuint32> > g_list;
  bool stat = sipCpp->mapping(func_list, g_list);
  if ( stat ) {
    ymuint n = g_list.size();
    sipRes = PyList_New(n);
    for (ymuint i = 0; i < n; ++ i) {
      const std::vector<ymuint32>& vals = g_list[i];
      ymuint n1 = vals.size();
      PyObject* obj1 = PyList_New(n1);
      for (ymuint j = 0; j < n1; ++ j) {
	PyObject* obj2 = sipBuildResult(NULL, "i", vals[j]);
	Py_INCREF(obj2);
	PyList_SetItem(obj1, j, obj2);
      }
      PyList_SetItem(sipRes, i, obj1);
    }
  }
  else {
    Py_INCREF(Py_None);
    return Py_None;
  }
  %End

  /// @brief collision free 分割を行う．
  /// @param[in] func_list 関数のリスト
  /// @param[out] block_map 分割を表す配列
  SIP_PYLIST
  cf_partition(SIP_PYLIST);
  %MethodCode
  ymuint n = PyList_Size(a0);
  std::vector<const FuncVector*> func_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GetItem(a0, i);
    if ( !sipCanConvertToType(obj1, sipType_FuncVect, 0) ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    void* p = sipConvertToType(obj1, sipType_FuncVect, NULL, 0, NULL, &sipIsErr);
    if ( sipIsErr ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    const FuncVect* fv = reinterpret_cast<const FuncVect*>(p);
    func_list[i] = fv;
  }
  vector<ymuint> block_map;
  bool stat = sipCpp->cf_partition(func_list, block_map);
  if ( stat ) {
    ymuint n = block_map.size();
    sipRes = PyList_New(n);
    for (ymuint i = 0; i < n; ++ i) {
      PyObject* obj1 = sipBuildResult(NULL, "i", block_map[i]);
      PyList_SetItem(sipRes, i, obj1);
    }
  }
  else {
    Py_INCREF(Py_None);
    return Py_None;
  }
  %End

  /// @brief displace_decomposition を行う．
  SIP_PYLIST
  displace_decomposition(const FuncVect* func1,
			 const FuncVect* func2,
			 bool use_xor);
  %MethodCode
  std::vector<ymuint> d_map;
  bool stat = sipCpp->displace_decompostion(a0, a1, d_map, a2);
  if ( stat ) {
    ymuint n = d_map.size();
    sipRes = PyList_New(n);
    for (ymuint i = 0; i < n; ++ i) {
      PyObject* obj1 = sipBuildResult(NULL, "i", d_map[i]);
      PyList_SetItem(sipRes, i, obj1);
    }
  }
  else {
    Py_INCREF(Py_None);
    return Py_None;
  }
  %End

};

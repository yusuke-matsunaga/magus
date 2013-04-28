
/// @file pyutils_init.cc
/// @brief pyutils の初期化関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// pyutils モジュール
//////////////////////////////////////////////////////////////////////

#include "ym_utils/pyutils.h"

BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッドテーブル
PyMethodDef utils_methods[] = {
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE

void
FileInfoObject_init(PyObject* module);

void
FileLocObject_init(PyObject* module);

void
FileRegionObject_init(PyObject* module);

void
USTimeObject_init(PyObject* module);

void
StopWatchObject_init(PyObject* module);

void
MFSetObject_init(PyObject* module);

void
MStopWatchObject_init(PyObject* module);

void
MsgTypeObject_init(PyObject* module);

void
MsgMgrObject_init(PyObject* module);

void
RandGenObject_init(PyObject* module);

void
RandPermGenObject_init(PyObject* module);

void
RandCombiGenObject_init(PyObject* module);

void
ItvlMgrObject_init(PyObject* module);

void
NameMgrObject_init(PyObject* module);

void
FileBinIObject_init(PyObject* module);

void
FileBinOObject_init(PyObject* module);

void
CombiGenObject_init(PyObject* module);

void
PermGenObject_init(PyObject* module);

void
MultiCombiGenObject_init(PyObject* module);

void
MultiPermGenObject_init(PyObject* module);

void
MultiSetCombiGenObject_init(PyObject* module);

void
MultiSetPermGenObject_init(PyObject* module);

END_NAMESPACE_YM


PyMODINIT_FUNC
utils_init()
{
  using namespace nsYm;

  //////////////////////////////////////////////////////////////////////
  // モジュールオブジェクトの生成
  //////////////////////////////////////////////////////////////////////
  PyDoc_STRVAR(module_doc,
	       "Module for libym_utils");
  PyObject* m = Py_InitModule3("utils", utils_methods, module_doc);
  if ( m == NULL ) {
    return;
  }

  //////////////////////////////////////////////////////////////////////
  // タイプオブジェクトの登録
  //////////////////////////////////////////////////////////////////////

  FileInfoObject_init(m);

  FileLocObject_init(m);

  FileRegionObject_init(m);

  MFSetObject_init(m);

  MsgTypeObject_init(m);

  MsgMgrObject_init(m);

  USTimeObject_init(m);

  StopWatchObject_init(m);

  MStopWatchObject_init(m);

  RandGenObject_init(m);

  RandPermGenObject_init(m);

  RandCombiGenObject_init(m);

  ItvlMgrObject_init(m);

  NameMgrObject_init(m);

  FileBinIObject_init(m);

  FileBinOObject_init(m);

  CombiGenObject_init(m);

  PermGenObject_init(m);

  MultiCombiGenObject_init(m);

  MultiPermGenObject_init(m);

  MultiSetCombiGenObject_init(m);

  MultiSetPermGenObject_init(m);

}

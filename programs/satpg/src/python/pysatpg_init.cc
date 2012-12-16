
/// @file pysatpg_init.cc
/// @brief pysatpg の初期化関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// pysatpg モジュール
//////////////////////////////////////////////////////////////////////

#include "pysatpg.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

// メソッドテーブル
PyMethodDef satpg_methods[] = {
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE
#if 0
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
#endif
END_NAMESPACE_YM_PYTHON


PyMODINIT_FUNC
satpg_init()
{
  using namespace nsYm::nsPython;

  //////////////////////////////////////////////////////////////////////
  // モジュールオブジェクトの生成
  //////////////////////////////////////////////////////////////////////
  PyDoc_STRVAR(module_doc,
	       "Module for satpg");
  PyObject* m = Py_InitModule3("satpg", satpg_methods, module_doc);
  if ( m == NULL ) {
    return;
  }

  //////////////////////////////////////////////////////////////////////
  // タイプオブジェクトの登録
  //////////////////////////////////////////////////////////////////////

#if 0
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
#endif
}

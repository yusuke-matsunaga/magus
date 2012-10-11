
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

BEGIN_NAMESPACE_YM_PYTHON

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
MStopWatchObject_init(PyObject* module);

void
MsgTypeObject_init(PyObject* module);

void
MsgHandlerObject_init(PyObject* module);

void
RandGenObject_init(PyObject* module);

void
RandGenPermObject_init(PyObject* module);

void
RandGenCombiObject_init(PyObject* module);

END_NAMESPACE_YM_PYTHON


PyMODINIT_FUNC
utils_init()
{
  using namespace nsYm::nsPython;

  //////////////////////////////////////////////////////////////////////
  // タイプオブジェクトの初期化
  //////////////////////////////////////////////////////////////////////

  if ( PyType_Ready(&FileInfoType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&FileLocType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&FileRegionType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&MsgTypeType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&MsgHandlerType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&USTimeType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&StopWatchType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&MStopWatchType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&RandGenType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&RandPermGenType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&RandCombiGenType) < 0 ) {
    return;
  }


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

  MsgTypeObject_init(m);

  MsgHandlerObject_init(m);

  USTimeObject_init(m);

  StopWatchObject_init(m);

  MStopWatchObject_init(m);

  RandGenObject_init(m);

  RandPermGenObject_init(m);

  RandCombiGenObject_init(m);

}

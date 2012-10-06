
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

#if 0
  if ( PyType_Ready(&RandPermGenType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&RandCombiGenType) < 0 ) {
    return;
  }
#endif

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

  PyModule_AddObject(m, "FileInfo", (PyObject*)&FileInfoType);

  PyModule_AddObject(m, "USTime", (PyObject*)&USTimeType);

  PyModule_AddObject(m, "StopWatch", (PyObject*)&StopWatchType);

  PyModule_AddObject(m, "MStopWatch", (PyObject*)&StopWatchType);

  PyModule_AddObject(m, "RandGen", (PyObject*)&RandGenType);

#if 0
  PyModule_AddObject(m, "RandPermGen", (PyObject*)&RandPermGenType);

  PyModule_AddObject(m, "RandCombiGen", (PyObject*)&RandCombiGenType);
#endif
}

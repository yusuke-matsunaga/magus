
/// @file pylogic_init.cc
/// @brief libym_logic の Python 用拡張モジュールの初期化
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// pylogic モジュール
//////////////////////////////////////////////////////////////////////

#include "ym_logic/pylogic.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

// メソッドテーブル
PyMethodDef logic_methods[] = {
  {NULL, NULL}
};

END_NONAMESPACE

void
Bool3Object_init(PyObject* m);

void
PolObject_init(PyObject* m);

END_NAMESPACE_YM_PYTHON


PyMODINIT_FUNC
logic_init()
{
  using namespace nsYm::nsPython;

  // タイプオブジェクトの初期化
  if ( PyType_Ready(&Bool3Type) < 0 ) {
    return;
  }

  if ( PyType_Ready(&VarIdType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&PolType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&LiteralType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&LogExprType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&BddMgrType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&BddType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&AigMgrType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&AigType) < 0 ) {
    return;
  }

  if ( PyType_Ready(&SatSolverType) < 0 ) {
    return;
  }

  // モジュールオブジェクトの生成
  PyDoc_STRVAR(module_doc,
	       "Module for libym_logic");
  PyObject* m = Py_InitModule3("logic", logic_methods, module_doc);
  if ( m == NULL ) {
    return;
  }

#if 0
  // エラーオブジェクトの生成
  if ( ErrorObject == NULL ) {
    // PyErr_NewException の第1引数は char* なので変数を用意する．
    PyDoc_STRVAR(logic_error_name, "logic.error");
    ErrorObject = PyErr_NewException(logic_error_name, NULL, NULL);
    if ( ErrorObject == NULL ) {
      return;
    }
  }
  Py_INCREF(ErrorObject);

  // logic モジュールに追加
  PyModule_AddObject(m, "error", ErrorObject);
#endif

  // Bool3Object 関係の初期化
  Bool3Object_init(m);

  // PolObject 関係の初期化
  PolObject_init(m);

  // VarId オブジェクトタイプの登録
  PyModule_AddObject(m, "VarId", (PyObject*)&VarIdType);

  // Literal オブジェクトタイプの登録
  PyModule_AddObject(m, "Literal", (PyObject*)&LiteralType);

  // LogExpr オブジェクトタイプの登録
  PyModule_AddObject(m, "LogExpr", (PyObject*)&LogExprType);

  // BddMgr オブジェクトタイプの登録
  PyModule_AddObject(m, "BddMgr", (PyObject*)&BddMgrType);

  // Bdd オブジェクトタイプの登録
  PyModule_AddObject(m, "Bdd", (PyObject*)&BddType);

  // AigMgr オブジェクトタイプの登録
  PyModule_AddObject(m, "AigMgr", (PyObject*)&AigMgrType);

  // Aig オブジェクトタイプの登録
  PyModule_AddObject(m, "Aig", (PyObject*)&AigType);

  // SatSolver オブジェクトタイプの登録
  PyModule_AddObject(m, "SatSolver", (PyObject*)&SatSolverType);

}

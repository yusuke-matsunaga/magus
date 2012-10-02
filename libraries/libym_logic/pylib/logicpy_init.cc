
/// @file logicpy_init.cc
/// @brief libym_logic の Python 用拡張モジュールの初期化
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// logicpy モジュール
//////////////////////////////////////////////////////////////////////

#include "logicpy.h"


BEGIN_NAMESPACE_LOGICPY

// エラー用のオブジェクト
PyObject* ErrorObject = NULL;

// メソッドテーブル
PyMethodDef logic_methods[] = {
  {NULL, NULL}
};

END_NAMESPACE_LOGICPY


PyMODINIT_FUNC
logic_init()
{
  using namespace nsYm::nsLogicpy;

  PyObject* m;

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
  m = Py_InitModule3("logic", logic_methods, module_doc);
  if ( m == NULL ) {
    return;
  }

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

  // VarId オブジェクトタイプの登録
  PyModule_AddObject(m, "VarId", (PyObject*)&VarIdType);

  // Pol オブジェクトタイプの登録
  PyModule_AddObject(m, "Pol", (PyObject*)&PolType);

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


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

void
VarIdObject_init(PyObject* m);

void
LiteralObject_init(PyObject* m);

void
LogExprObject_init(PyObject* m);

void
BddObject_init(PyObject* m);

void
BddMgrObject_init(PyObject* m);

void
AigObject_init(PyObject* m);

void
AigMgrObject_init(PyObject* m);

void
SatSolverObject_init(PyObject* m);

END_NAMESPACE_YM_PYTHON


PyMODINIT_FUNC
logic_init()
{
  using namespace nsYm::nsPython;

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

  // VarIdObject 関係の初期化
  VarIdObject_init(m);

  // LiteralObject 関係の初期化
  LiteralObject_init(m);

  // LogExprObject 関係の初期化
  LogExprObject_init(m);

  // BddMgrObject 関係の初期化
  BddMgrObject_init(m);

  // BddObject 関係の初期化
  BddObject_init(m);

  // AigMgrObject 関係の初期化
  AigMgrObject_init(m);

  // AigObject 関係の初期化
  AigObject_init(m);

  // SatSolverObject 関係の初期化
  SatSolverObject_init(m);

}

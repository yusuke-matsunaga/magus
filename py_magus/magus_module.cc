
/// @file magus_module.cc
/// @brief Python 用の magus モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "magus.h"
#include "ym/PyClibCellLibrary.h"
#include "PyMt19937.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

// メソッド定義構造体
PyMethodDef magus_methods[] = {
  {nullptr, nullptr, 0, nullptr},
};

// モジュール定義構造体
PyModuleDef magus_module = {
  PyModuleDef_HEAD_INIT,
  "magus",
  PyDoc_STR("Magus: Extension module for Magus"),
  -1,
  magus_methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_magus()
{
  auto m = PyModule_Create(&magus_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyMt19937::init(m) ) {
    goto error;
  }

  if ( !PyClibCellLibrary::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_MAGUS

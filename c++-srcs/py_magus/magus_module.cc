
/// @file magus_module.cc
/// @brief Python 用の magus モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "magus.h"
#include "pym/ymbase.h"
#include "pym/ymbnet.h"
#include "pym/ymcell.h"
#include "pym/ymsat.h"
#include "pym/ymlogic.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_MAGUS

extern "C" PyObject* PyInit_equiv();

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

// サブモジュールを登録する．
bool
reg_submodule(
  PyObject* module,      // 親モジュール
  PyObject* sys_modules, // sys.modules オブジェクト
  PyObject* submodule,   // 登録するサブモジュール
  const char* name       // サブモジュール名
)
{
  if ( !PyModule::reg_item(module, name, submodule) ) {
    return false;
  }
  // これで from magus.equiv import XXX が行えるようになる．
  string submod_name = string{"magus."} + name;
  if ( PyDict_SetItemString(sys_modules,
			    submod_name.c_str(), submodule) < 0 ) {
    return false;
  }
  return true;
}

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_magus()
{
  auto m = PyModule::init(&magus_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyModule::reg_submodule(m, "ymbase", PyInit_ymbase()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "ymbnet", PyInit_ymbnet()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "ymcell", PyInit_ymcell()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "ymsat", PyInit_ymsat()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "ymlogic", PyInit_ymlogic()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "equiv", PyInit_equiv()) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_MAGUS


/// @file techmap_module.cc
/// @brief techmap モジュールの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "CellMap.h"
#include "pym/PyModule.h"
#include "pym/PyBnNetwork.h"
#include "pym/PySatBool3.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

PyObject*
cellmap_cmd(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    "",
    "loglevel",
    nullptr
  };
#if 0
  PyObject* net1_obj = nullptr;
  PyObject* net2_obj = nullptr;
  int match_by_name = false;
  int sig_size = -1;
  int loglevel = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!|$pii",
				    const_cast<char**>(kwlist),
				    PyBnNetwork::_typeobject(), &net1_obj,
				    PyBnNetwork::_typeobject(), &net2_obj,
				    &match_by_name, &sig_size, &loglevel) ) {
    return nullptr;
  }

  EquivMgr mgr;
  if ( sig_size != -1 ) {
    mgr.set_signature_size(sig_size);
  }
  if ( loglevel != -1 ) {
    mgr.set_loglevel(loglevel);
  }
  auto net1 = PyBnNetwork::Get(net1_obj);
  auto net2 = PyBnNetwork::Get(net2_obj);
  auto result = mgr.check(net1, net2, match_by_name);
  const auto& oresults = result.output_results();
  SizeType no = oresults.size();
  auto oresults_obj = PyTuple_New(no);
  for ( SizeType i = 0; i < no; ++ i ) {
    auto obj1 = PySatBool3::ToPyObject(oresults[i]);
    PyTuple_SetItem(oresults_obj, i, obj1);
  }
  auto obj2 = PySatBool3::ToPyObject(result.result());
  return Py_BuildValue("OO", obj2, oresults_obj);
#else
  return nullptr;
#endif
}

// メソッド定義構造体
PyMethodDef techmap_methods[] = {
  {"cellmap", reinterpret_cast<PyCFunction>(cellmap_cmd),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("do technology mapping for cell library")},
  {nullptr, nullptr, 0, nullptr},
};

// モジュール定義構造体
PyModuleDef techmap_module = {
  PyModuleDef_HEAD_INIT,
  "techmap",
  PyDoc_STR("techmap: Extension module for technology mapping"),
  -1,
  techmap_methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_techmap()
{
  auto m = PyModule::init(&techmap_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_MAGUS

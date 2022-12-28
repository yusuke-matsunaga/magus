
/// @file py_magus.cc
/// @brief py_magus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "magus.h"


BEGIN_NAMESPACE_MAGUS

extern "C"
PyObject* PyInit_magus();

END_NAMESPACE_MAGUS

int
main(
  int argc,
  char** argv
)
{
  PyImport_AppendInittab("magus", &MAGUS_NAMESPACE::PyInit_magus);

  return Py_BytesMain(argc, argv);
}

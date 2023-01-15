
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

extern "C" PyObject* PyInit_magus();

static struct _inittab init_table[] = {
  {"magus", PyInit_magus},
  {nullptr, nullptr}
};

void
magus_init()
{
  PyImport_ExtendInittab(init_table);
}

END_NAMESPACE_MAGUS

int
main(
  int argc,
  char** argv
)
{
  MAGUS_NAMESPACE::magus_init();

  return Py_BytesMain(argc, argv);
}


/// @file pycell_init.cc
/// @brief libym_cell の Python 用拡張モジュールの初期化
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// pycell モジュール
//////////////////////////////////////////////////////////////////////

#include "ym_cell/pycell.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッドテーブル
PyMethodDef cell_methods[] = {
  {NULL, NULL}
};

END_NONAMESPACE


void
CellDirObject_init(PyObject* m);

void
CellAreaObject_init(PyObject* m);

void
CellCapacitanceObject_init(PyObject* m);

void
CellResistanceObject_init(PyObject* m);

void
CellTimeObject_init(PyObject* m);

void
CellLibraryObject_init(PyObject* m);

void
CellClassObject_init(PyObject* m);

void
CellGroupObject_init(PyObject* m);

void
CellObject_init(PyObject* m);

void
CellPinObject_init(PyObject* m);

void
CellTechnologyObject_init(PyObject* m);

void
CellDelayModelObject_init(PyObject* m);

void
CellTimingObject_init(PyObject* m);

void
CellPatTypeObject_init(PyObject* m);

void
CellPatGraphObject_init(PyObject* m);

void
CellTimingTypeObject_init(PyObject* m);

void
CellLutObject_init(PyObject* m);


END_NAMESPACE_YM


PyMODINIT_FUNC
cell_init()
{
  using namespace nsYm;

  // モジュールオブジェクトの生成
  PyDoc_STRVAR(module_doc,
	       "Module for libym_cell");
  PyObject* m = Py_InitModule3("cell_lib", cell_methods, module_doc);
  if ( m == NULL ) {
    return;
  }

  // CellDirObject 関係の初期化
  CellDirObject_init(m);

  // CellAreaObject 関係の初期化
  CellAreaObject_init(m);

  // CellCapacitanceObject 関係の初期化
  CellCapacitanceObject_init(m);

  // CellTimeObject 関係の初期化
  CellTimeObject_init(m);

  // CellResistanceObject 関係の初期化
  CellResistanceObject_init(m);

  // CellLibraryObject 関係の初期化
  CellLibraryObject_init(m);

  // CellClassObject 関係の初期化
  CellClassObject_init(m);

  // CellGroupObject 関係の初期化
  CellGroupObject_init(m);

  // CellObject 関係の初期化
  CellObject_init(m);

  // CellPinObject 関係の初期化
  CellPinObject_init(m);

  // CellTechnologyObject 関係の初期化
  CellTechnologyObject_init(m);

  // CellDelayModelObject 関係の初期化
  CellDelayModelObject_init(m);

  // CellTimingObject 関係の初期化
  CellTimingObject_init(m);

  // CellTimingTypeObject 関係の初期化
  CellTimingTypeObject_init(m);

  // CellPatType 関係の初期化
  CellPatTypeObject_init(m);

  // CellPatGraph 関係の初期化
  CellPatGraphObject_init(m);

  // CelLut 関係の初期化
  CellLutObject_init(m);

}

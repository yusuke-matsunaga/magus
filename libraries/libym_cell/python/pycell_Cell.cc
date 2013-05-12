
/// @file pycell_Cell.cc
/// @brief Cell の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"


BEGIN_NAMESPACE_YM

/// @brief CellPin から PyObject を作る．
/// @param[in] pin CellPin へのポインタ
/// @return cell を表す PyObject
extern
PyObject*
PyCellPin_FromCellPin(const CellPin* pin);

/// @brief CellTiming から PyObject を作る．
/// @param[in] timing CellTiming へのポインタ
/// @return timing を表す PyObject
extern
PyObject*
PyCellTiming_FromCellTiming(const CellTiming* timing);


BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// Cell を表す型
struct CellObject
{
  // Python のお約束
  PyObject_HEAD

  // Cell
  const Cell* mCell;

  // 親のグループ
  PyObject* mGroup;

  // ピンの配列
  PyObject** mPinArray;

  // タイミング情報の配列
  PyObject** mTimingArray;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellObject を開放する関数
void
Cell_dealloc(CellObject* self)
{
  Py_DECREF(self->mGroup);

  ymuint np = self->mCell->pin_num();
  for (ymuint i = 0; i < np; ++ i) {
    Py_DECREF(self->mPinArray[i]);
  }
  delete [] self->mPinArray;

  ymuint nt = self->mCell->timing_num();
  for (ymuint i = 0; i < nt; ++ i) {
    Py_DECREF(self->mTimingArray[i]);
  }
  delete [] self->mTimingArray;

  PyObject_Del(self);
}

// id 関数
PyObject*
Cell_id(CellObject* self,
	PyObject* args)
{
  return PyObject_FromYmuint32(self->mCell->id());
}

// name 関数
PyObject*
Cell_name(CellObject* self,
	  PyObject* args)
{
  return PyObject_FromString(self->mCell->name());
}

// area 関数
PyObject*
Cell_area(CellObject* self,
	  PyObject* args)
{
  return PyCellArea_FromCellArea(self->mCell->area());
}

// pin 関数
PyObject*
Cell_pin(CellObject* self,
	 PyObject* args)
{
  char* name;
  if ( !PyArg_ParseTuple(args, "s", &name) ) {
    return NULL;
  }

  const CellPin* pin = self->mCell->pin(name);
  if ( pin == NULL ) {
    PyErr_SetString(PyExc_ValueError, "No such pin");
    return NULL;
  }

  PyObject* result = self->mPinArray[pin->pin_id()];

  Py_INCREF(result);
  return result;
}

// pin_list 関数
PyObject*
Cell_pin_list(CellObject* self,
	      PyObject* args)
{
  ymuint np = self->mCell->pin_num();
  PyObject* pin_list = PyList_New(np);
  for (ymuint i = 0; i < np; ++ i) {
    PyObject* obj = self->mPinArray[i];
    Py_INCREF(obj);
    PyList_SetItem(pin_list, i, obj);
  }
  return pin_list;
}

// input_pin_list 関数
PyObject*
Cell_input_pin_list(CellObject* self,
		    PyObject* args)
{
  ymuint n = self->mCell->input_num();
  PyObject* pin_list = PyList_New(n);
  ymuint np = self->mCell->pin_num();
  for (ymuint i = 0; i < np; ++ i) {
    const CellPin* pin = self->mCell->pin(i);
    if ( pin->is_input() ) {
      PyObject* obj = self->mPinArray[i];
      Py_INCREF(obj);
      PyList_SetItem(pin_list, pin->input_id(), obj);
    }
  }
  return pin_list;
}

// output_pin_list 関数
PyObject*
Cell_output_pin_list(CellObject* self,
		     PyObject* args)
{
  ymuint n = self->mCell->output_num();
  PyObject* pin_list = PyList_New(n);
  ymuint np = self->mCell->pin_num();
  for (ymuint i = 0; i < np; ++ i) {
    const CellPin* pin = self->mCell->pin(i);
    if ( pin->is_output() ) {
      PyObject* obj = self->mPinArray[i];
      Py_INCREF(obj);
      PyList_SetItem(pin_list, pin->output_id(), obj);
    }
  }
  return pin_list;
}

// inout_pin_list 関数
PyObject*
Cell_inout_pin_list(CellObject* self,
		    PyObject* args)
{
  ymuint n = self->mCell->inout_num();
  PyObject* pin_list = PyList_New(n);
  ymuint np = self->mCell->pin_num();
  ymuint pos = 0;
  for (ymuint i = 0; i < np; ++ i) {
    const CellPin* pin = self->mCell->pin(i);
    if ( pin->is_inout() ) {
      PyObject* obj = self->mPinArray[i];
      Py_INCREF(obj);
      PyList_SetItem(pin_list, pos, obj);
      ++ pos;
    }
  }
  return pin_list;
}

// internal_pin_list 関数
PyObject*
Cell_internal_pin_list(CellObject* self,
		       PyObject* args)
{
  ymuint n = self->mCell->internal_num();
  PyObject* pin_list = PyList_New(n);
  ymuint np = self->mCell->pin_num();
  for (ymuint i = 0; i < np; ++ i) {
    const CellPin* pin = self->mCell->pin(i);
    if ( pin->is_internal() ) {
      PyObject* obj = self->mPinArray[i];
      Py_INCREF(obj);
      PyList_SetItem(pin_list, pin->internal_id(), obj);
    }
  }
  return pin_list;
}

// timing_list 関数
PyObject*
Cell_timing_list(CellObject* self,
		 PyObject* args)
{
  ymuint n = self->mCell->timing_num();
  PyObject* timing_list = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = self->mTimingArray[i];
    Py_INCREF(obj1);
    PyList_SetItem(timing_list, i, obj1);
  }

  return timing_list;
}

// timing_list2 関数
PyObject*
Cell_timing_list2(CellObject* self,
		  PyObject* args)
{
  ymuint ipos = 0;
  ymuint opos = 0;
  PyObject* obj1 = NULL;
  if ( !PyArg_ParseTuple(args, "IIO!", &ipos, &opos, &PyCellTimingSense_Type, &obj1) ) {
    return NULL;
  }
  tCellTimingSense tsense = PyCellTimingSense_AsCellTimingSense(obj1);

  ymuint n = self->mCell->timing_num(ipos, opos, tsense);
  PyObject* timing_list = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const CellTiming* timing = self->mCell->timing(ipos, opos, tsense, i);
    PyObject* obj1 = self->mTimingArray[timing->id()];
    Py_INCREF(obj1);
    PyList_SetItem(timing_list, i, obj1);
  }

  return timing_list;
}


//////////////////////////////////////////////////////////////////////
// CellObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef Cell_methods[] = {
  // PyMethodDef のフィールド
  //   char*       ml_name;
  //   PyCFunction ml_meth;
  //   int         ml_flags;
  //   char*       ml_doc;
  // ml_flags で使用可能なマクロは以下の通り
  //  - METH_VARARGS
  //  - METH_KEYWORDS
  //  - METH_NOARGS
  //  - METH_O
  //  - METH_OLDARGS (obsolete)
  //  - METH_CLASS
  //  - METH_STATIC
  //  - METH_COEXIST

  {"id", (PyCFunction)Cell_id, METH_NOARGS,
   PyDoc_STR("return ID")},
  {"name", (PyCFunction)Cell_name, METH_NOARGS,
   PyDoc_STR("return name")},
  {"area", (PyCFunction)Cell_area, METH_NOARGS,
   PyDoc_STR("return area")},
  {"pin_by_name", (PyCFunction)Cell_pin, METH_VARARGS,
   PyDoc_STR("return pin (name: string)")},
  {"pin_list", (PyCFunction)Cell_pin_list, METH_NOARGS,
   PyDoc_STR("return pin-list")},

  {"input_pin_list", (PyCFunction)Cell_input_pin_list, METH_NOARGS,
   PyDoc_STR("return input-pin-list")},

  {"output_pin_list", (PyCFunction)Cell_output_pin_list, METH_NOARGS,
   PyDoc_STR("return output-pin-list")},

  {"inout_pin_list", (PyCFunction)Cell_inout_pin_list, METH_NOARGS,
   PyDoc_STR("return inout-pin-list")},

  {"internal_pin_list", (PyCFunction)Cell_internal_pin_list, METH_NOARGS,
   PyDoc_STR("return internal-pin-list")},

  {"timing_list", (PyCFunction)Cell_timing_list, METH_NOARGS,
   PyDoc_STR("return timing list")},
  {"timing_list2", (PyCFunction)Cell_timing_list2, METH_VARARGS,
   PyDoc_STR("return timing list of (ipos, opos, timing_sense)")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellCell_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.Cell",                  // tp_name
  sizeof(CellObject),           // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)Cell_dealloc,     // tp_dealloc
  (printfunc)0,                 // tp_print
  (getattrfunc)0,               // tp_getattr
  (setattrfunc)0,               // tp_setattr
  (cmpfunc)0,                   // tp_compare
  (reprfunc)0,                  // tp_repr

  // Method suites for standard classes
  0,                            // tp_as_number
  0,                            // tp_as_sequence
  0,                            // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                  // tp_hash
  (ternaryfunc)0,               // tp_call
  (reprfunc)0,                  // tp_str
  (getattrofunc)0,              // tp_getattro
  (setattrofunc)0,              // tp_setattro

  // Functions to access object as input/output buffer
  0,                            // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,           // tp_flags

  // Documentation string
  "Cell",                       // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,              // tp_traverse

  // delete references to contained objects
  (inquiry)0,                   // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,               // tp_richcompare

  // weak reference enabler
  (long)0,                      // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,               // tp_iter
  (iternextfunc)0,              // tp_iternext

  // Attribute descriptor and subclassing stuff
  Cell_methods,                 // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)0,                  // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)0,                   // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と Cell の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief Cell から PyObject を作る．
// @param[in] cell Cell へのポインタ
// @return cell を表す PyObject
PyObject*
PyCellCell_FromCell(const Cell* cell)
{
  CellObject* self = PyObject_New(CellObject, &PyCellCell_Type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mCell = cell;
  self->mGroup = NULL;

  ymuint np = cell->pin_num();
  self->mPinArray = new PyObject*[np];
  for (ymuint i = 0; i < np; ++ i) {
    const CellPin* pin = cell->pin(i);
    self->mPinArray[i] = PyCellPin_FromCellPin(pin);
  }

  ymuint nt = cell->timing_num();
  self->mTimingArray = new PyObject*[nt];
  for (ymuint i = 0; i < nt; ++ i) {
    const CellTiming* timing = cell->timing(i);
    self->mTimingArray[i] = PyCellTiming_FromCellTiming(timing);
  }

  Py_INCREF(self);
  return (PyObject*)self;
}

// @brief Cell にセルグループを設定する．
void
PyCellCell_set_group(PyObject* py_obj,
		     PyObject* group_obj)
{
  assert_cond( PyCellCell_Check(py_obj), __FILE__, __LINE__);
  CellObject* cell_obj = (CellObject*)py_obj;
  cell_obj->mGroup = group_obj;
  Py_INCREF(group_obj);
}

// @brief PyObject から Cell へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return Cell へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
const Cell*
PyCellCell_AsCellPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellCell_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.Cell is expected");
    return NULL;
  }

  // 強制的にキャスト
  CellObject* my_obj = (CellObject*)py_obj;

  return my_obj->mCell;
}

// CellObject 関係の初期化を行う．
void
CellObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellCell_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Cell", (PyObject*)&PyCellCell_Type);
}

END_NAMESPACE_YM

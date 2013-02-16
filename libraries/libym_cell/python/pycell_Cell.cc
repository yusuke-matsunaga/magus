
/// @file pycell_Cell.cc
/// @brief Cell の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/Cell.h"
#include "PyCell.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// Cell を表す型
struct CellObject
{
  // Python のお約束
  PyObject_HEAD

  // Cell の本体
  const Cell* mBody;

  // PyCell
  PyCell* mCell;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellObject の生成関数
CellObject*
Cell_new(PyTypeObject* type)
{
  CellObject* self = PyObject_New(CellObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mCell = new PyCell();

  return self;
}

// CellObject を開放する関数
void
Cell_dealloc(CellObject* self)
{
  delete self->mCell;

  PyObject_Del(self);
}

// id 関数
PyObject*
Cell_id(CellObject* self,
	PyObject* args)
{
  return conv_to_pyobject(self->mBody->id());
}

// name 関数
PyObject*
Cell_name(CellObject* self,
	  PyObject* args)
{
  return conv_to_pyobject(self->mBody->name());
}

// area 関数
PyObject*
Cell_area(CellObject* self,
	  PyObject* args)
{
  return PyCellArea_FromCellArea(self->mBody->area());
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

  const CellPin* pin = self->mBody->pin(name);
  if ( pin == NULL ) {
    PyErr_SetString(PyExc_ValueError, "No such pin");
    return NULL;
  }

  return self->mCell->get_CellPin(pin);
}

// pin_list 関数
PyObject*
Cell_pin_list(CellObject* self,
	      PyObject* args)
{
  ymuint n = self->mBody->pin_num();
  PyObject* pin_list = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const CellPin* pin = self->mBody->pin(i);
    PyObject* obj = self->mCell->get_CellPin(pin);
    PyList_SetItem(pin_list, i, obj);
  }
  return pin_list;
}

// input_pin_list 関数
PyObject*
Cell_input_pin_list(CellObject* self,
		    PyObject* args)
{
  ymuint n = self->mBody->input_num();
  PyObject* pin_list = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const CellPin* pin = self->mBody->input(i);
    PyObject* obj = self->mCell->get_CellPin(pin);
    PyList_SetItem(pin_list, i, obj);
  }
  return pin_list;
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

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCell_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell.Cell",                  // tp_name
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
  0,                            // tp_init
  (allocfunc)0,                 // tp_alloc
  0,                            // tp_new
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

// @brief PyObject から Cell を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] p_obj Cell のポインタを格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が CellObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   const Cell*& p_obj)
{
  // 型のチェック
  if ( !CellObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  CellObject* my_obj = (CellObject*)py_obj;

  p_obj = my_obj->mBody;

  return true;
}

// CellObject 関係の初期化を行う．
void
CellObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCell_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Cell", (PyObject*)&PyCell_Type);
}

END_NAMESPACE_YM

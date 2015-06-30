
/// @file pycell_CellLut.cc
/// @brief CellLut の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/pycell.h"
#include "YmCell/CellLut.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellLut を表す型
struct LutObject
{
  // Python のお約束
  PyObject_HEAD

  // CellLut
  const CellLut* mLut;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// LutObject を開放する関数
void
Lut_dealloc(LutObject* self)
{
  PyObject_Del(self);
}

// template_name 関数
PyObject*
Lut_template_name(LutObject* self,
		  PyObject* args)
{
  return PyObject_FromString(self->mLut->template_name());
}

// dimension 関数
PyObject*
Lut_dimension(LutObject* self,
	      PyObject* args)
{
  ymuint dim = self->mLut->dimension();
  return PyObject_FromYmuint32(dim);
}

// variable_type 関数
PyObject*
Lut_variable_type(LutObject* self,
		  PyObject* args)
{
  ymuint var = 0;
  if ( !PyArg_ParseTuple(args, "I", &var) ) {
    return NULL;
  }

  tCellVarType var_type = self->mLut->variable_type(var);
  return PyCellVarType_FromCellVarType(var_type);
}

// index_num 関数
PyObject*
Lut_index_num(LutObject* self,
	      PyObject* args)
{
  ymuint var = 0;
  if ( !PyArg_ParseTuple(args, "I", &var) ) {
    return NULL;
  }

  ymuint n = self->mLut->index_num(var);

  return PyObject_FromYmuint32(n);
}

// index 関数
PyObject*
Lut_index(LutObject* self,
	  PyObject* args)
{
  ymuint var = 0;
  ymuint pos = 0;
  if ( !PyArg_ParseTuple(args, "II", &var, &pos) ) {
    return NULL;
  }

  double index = self->mLut->index(var, pos);

  return PyObject_FromDouble(index);
}

// grid_value 関数
PyObject*
Lut_grid_value(LutObject* self,
	       PyObject* args)
{
  if ( !PySequence_Check(args) ) {
    PyErr_SetString(PyExc_TypeError, "sequence type is expected.");
    return NULL;
  }

  ymuint n = PySequence_Size(args);
  vector<ymuint32> pos_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PySequence_GetItem(args, i);
    if ( !PyNumber_Check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "number type is expected.");
      return NULL;
    }
    PyObject* obj2 = PyNumber_Long(obj1);
    if ( obj2 == NULL ) {
      PyErr_SetString(PyExc_TypeError, "integer type is expected.");
      return NULL;
    }
    ymuint pos = PyLong_AsLong(obj2);
    pos_array[i] = pos;
  }

  double val = self->mLut->grid_value(pos_array);

  return PyObject_FromDouble(val);
}

// value 関数
PyObject*
Lut_value(LutObject* self,
	  PyObject* args)
{
  if ( !PySequence_Check(args) ) {
    PyErr_SetString(PyExc_TypeError, "sequence type is expected.");
    return NULL;
  }

  ymuint n = PySequence_Size(args);
  vector<double> val_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PySequence_GetItem(args, i);
    if ( !PyNumber_Check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "number type is expected.");
      return NULL;
    }
    PyObject* obj2 = PyNumber_Float(obj1);
    if ( obj2 == NULL ) {
      PyErr_SetString(PyExc_TypeError, "float type is expected.");
      return NULL;
    }
    double val = PyFloat_AsDouble(obj2);
    val_array[i] = val;
  }

  double val = self->mLut->value(val_array);

  return PyObject_FromDouble(val);
}


//////////////////////////////////////////////////////////////////////
// LutTemplateObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef Lut_methods[] = {
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

  {"template_name", (PyCFunction)Lut_template_name, METH_NOARGS,
   PyDoc_STR("return template's name (NONE)")},
  {"dimension", (PyCFunction)Lut_dimension, METH_NOARGS,
   PyDoc_STR("return dimension (NONE)")},
  {"variable_type", (PyCFunction)Lut_variable_type, METH_VARARGS,
   PyDoc_STR("return variable type (var: unsigned int)")},
  {"index_num", (PyCFunction)Lut_index_num, METH_VARARGS,
   PyDoc_STR("return index number (var: unsigned int")},
  {"index", (PyCFunction)Lut_index, METH_VARARGS,
   PyDoc_STR("return default index value (var: unsigned int, pos: unsigned int")},
  {"grid_value", (PyCFunction)Lut_grid_value, METH_VARARGS,
   PyDoc_STR("return grid value (sequence of unsigned int)")},
  {"value", (PyCFunction)Lut_value, METH_VARARGS,
   PyDoc_STR("return value (sequence of float)")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// LutObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellLut_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellLut",           // tp_name
  sizeof(LutObject),            // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)Lut_dealloc,      // tp_dealloc
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
  "Look-up Table",              // tp_doc

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
  Lut_methods,                  // tp_methods
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
// PyObject と CellLut の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief CellLut から CellLutObject を生成する．
// @param[in] lut LUT
PyObject*
PyCellLut_FromCellLut(const CellLut* lut)
{
  LutObject* self = PyObject_New(LutObject, &PyCellLut_Type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mLut = lut;

  return (PyObject*)self;
}

// @brief PyObject から CellLut へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return CellLut へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
const CellLut*
PyCellLut_AsCellLutPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellLut_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellLut is expected.");
    return NULL;
  }

  // 強制的にキャスト
  LutObject* my_obj = (LutObject*)py_obj;

  return my_obj->mLut;
}

// CellLutObject 関係の初期化を行なう．
void
CellLutObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellLut_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellLut", (PyObject*)&PyCellLut_Type);
}

END_NAMESPACE_YM

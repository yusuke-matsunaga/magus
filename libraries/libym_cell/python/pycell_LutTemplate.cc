
/// @file pycell_LutTemplate.cc
/// @brief CellLutTemplate の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/CellLut.h"
#include "PyLutTemplate.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellLutTemplate を表す型
struct LutTemplateObject
{
  // Python のお約束
  PyObject_HEAD

  // PyLutTemplate
  PyLutTemplate* mLutTemplate;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// LutTemplateObject を開放する関数
void
LutTemplate_dealloc(LutTemplateObject* self)
{
  delete self->mLutTemplate;

  PyObject_Del(self);
}

// name 関数
PyObject*
LutTemplate_name(LutTemplateObject* self,
		 PyObject* args)
{
  PyObject* result = self->mLutTemplate->name();

  Py_INCREF(result);
  return result;
}

// dimension 関数
PyObject*
LutTemplate_dimension(LutTemplateObject* self,
		      PyObject* args)
{
  ymuint dim = self->mLutTemplate->lut_template()->dimension();
  return PyObject_FromYmuint32(dim);
}

// variable_type 関数
PyObject*
LutTemplate_variable_type(LutTemplateObject* self,
			  PyObject* args)
{
  ymuint var = 0;
  if ( !PyArg_ParseTuple(args, "I", &var) ) {
    return NULL;
  }

  PyObject* result = self->mLutTemplate->variable_type(var);

  Py_INCREF(result);
  return result;
}

// index_num 関数
PyObject*
LutTemplate_index_num(LutTemplateObject* self,
		      PyObject* args)
{
  ymuint var = 0;
  if ( !PyArg_ParseTuple(args, "I", &var) ) {
    return NULL;
  }

  ymuint n = self->mLutTemplate->lut_template()->index_num(var);

  return PyObject_FromYmuint32(n);
}

// index 関数
PyObject*
LutTemplate_index(LutTemplateObject* self,
		  PyObject* args)
{
  ymuint var = 0;
  ymuint pos = 0;
  if ( !PyArg_ParseTuple(args, "II", &var, &pos) ) {
    return NULL;
  }

  PyObject* result = self->mLutTemplate->index(var, pos);

  Py_INCREF(result);
  return result;
}


//////////////////////////////////////////////////////////////////////
// LutTemplateObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef LutTemplate_methods[] = {
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

  {"name", (PyCFunction)LutTemplate_name, METH_NOARGS,
   PyDoc_STR("return name")},
  {"dimension", (PyCFunction)LutTemplate_dimension, METH_NOARGS,
   PyDoc_STR("return dimension")},
  {"variable_type", (PyCFunction)LutTemplate_variable_type, METH_VARARGS,
   PyDoc_STR("return variable type (var: unsigned int)")},
  {"index_num", (PyCFunction)LutTemplate_index_num, METH_VARARGS,
   PyDoc_STR("return index number (var: unsigned int")},
  {"input", (PyCFunction)LutTemplate_index, METH_VARARGS,
   PyDoc_STR("return default index value (var: unsigned int, pos: unsigned int")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// LutTemplateObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellLutTemplate_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellLutTemplate",   // tp_name
  sizeof(LutTemplateObject),    // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)LutTemplate_dealloc,     // tp_dealloc
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
  "LUT template",                       // tp_doc

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
  LutTemplate_methods,          // tp_methods
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
// PyObject と LutTemplate の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief CellLutTemplate から CellLutTemplateObject を生成する．
// @param[in] lut_template LUTテンプレート
PyObject*
PyCellLutTemplate_FromCellLutTemplate(const CellLutTemplate* lut_template)
{
  LutTemplateObject* self = PyObject_New(LutTemplateObject, &PyCellLutTemplate_Type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mLutTemplate = new PyLutTemplate(lut_template);

  Py_INCREF(self);
  return (PyObject*)self;
}

// @brief PyObject から CellLutTemplate へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return CellLutTemplate へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
const CellLutTemplate*
PyCellLutTemplate_AsCellLutTemplatePtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellLutTemplate_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellLutTemplate is expected");
    return NULL;
  }

  // 強制的にキャスト
  LutTemplateObject* my_obj = (LutTemplateObject*)py_obj;

  return my_obj->mLutTemplate->lut_template();
}

// CellLutTemplateObject 関係の初期化を行なう．
void
CellLutTemplateObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellLutTemplate_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellLutTemplate", (PyObject*)&PyCellLutTemplate_Type);
}

END_NAMESPACE_YM

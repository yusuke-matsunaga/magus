
/// @file pycell_CellTimingType.cc
/// @brief tCellTimingType の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// tCellTimingType を表す型
struct CellTimingTypeObject
{
  // Python のお約束
  PyObject_HEAD

  // 実際の値
  tCellTimingType mVal;

};

// CellTimingType_kCombinational の実体
CellTimingTypeObject CellTimingType_kCombinationalStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingCombinational
};

// repr 用の文字列オブジェクト
PyObject* CellTimingType_kCombinationalString = NULL;


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellTimingTypeObject の生成関数
// 実際には既存のオブジェクトを返す．
CellTimingTypeObject*
CellTimingType_new(PyTypeObject* type,
		   PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }

  return (CellTimingTypeObject*)PyCellTimingType_FromString(str);
}

// CellTimingTypeObject を開放する関数
void
CellTimingType_dealloc(CellTimingTypeObject* self)
{
  PyObject_Del(self);
}

// repr 関数
PyObject*
CellTimingType_repr(CellTimingTypeObject* self)
{
  PyObject* result = NULL;
  switch ( self->mVal ) {
  case kCellTimingCombinational:    result = CellTimingType_kCombinationalString; break;
  default: assert_not_reached(__FILE__, __LINE__); break;
  }

  Py_INCREF(result);
  return result;
}


//////////////////////////////////////////////////////////////////////
// CellTimingTypeObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellTimingType_methods[] = {
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
// CellTimingTypeObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellTimingType_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.TimingType",              // tp_name
  sizeof(CellTimingTypeObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellTimingType_dealloc,     // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)CellTimingType_repr,          // tp_repr

  // Method suites for standard classes
  0,                               // tp_as_number
  0,                               // tp_as_sequence
  0,                               // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                     // tp_hash
  (ternaryfunc)0,                  // tp_call
  (reprfunc)0,                     // tp_str
  (getattrofunc)0,                 // tp_getattro
  (setattrofunc)0,                 // tp_setattro

  // Functions to access object as input/output buffer
  0,                               // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,              // tp_flags

  // Documentation string
  "Timing Type",                 // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                 // tp_traverse

  // delete references to contained objects
  (inquiry)0,                      // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                  // tp_richcompare

  // weak reference enabler
  (long)0,                         // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                  // tp_iter
  (iternextfunc)0,                 // tp_iternext

  // Attribute descriptor and subclassing stuff
  CellTimingType_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)0,                     // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)CellTimingType_new,            // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と CellTimingType の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief tCellTimingType から CellTimingTypeObject を生成する．
// @param[in] obj CellTimingType オブジェクト
PyObject*
PyCellTimingType_FromCellTimingType(tCellTimingType timing_type)
{
  PyObject* result = NULL;
  switch ( timing_type ) {
  case kCellTimingCombinational:    result = Py_CellTimingType_kCombinational; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// @brief 文字列から CellTimingTypeObject を生成する．
// @param[in] str 文字列
PyObject*
PyCellTimingType_FromString(const char* str)
{
  PyObject* result = NULL;
  if ( str == NULL ) {
    // デフォルトは combinational
    result = Py_CellTimingType_kCombinational;
  }
  else if ( strcmp(str, "combinational") == 0 ) {
    result = Py_CellTimingType_kCombinational;
  }
  else {
    PyErr_SetString(PyExc_ValueError, "Illegal string for CellTimingType");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から tCellTimingType を取り出す．
// @param[in] py_obj Python オブジェクト
// @return tCellTimingType を返す．
// @note 変換が失敗したら TypeError を送出し，tCellTimingCombinational を返す．
tCellTimingType
PyCellTimingType_AsCellTimingType(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellTimingType_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellTimingType is expected");
    return kCellTimingCombinational;
  }

  // 強制的にキャスト
  CellTimingTypeObject* my_obj = (CellTimingTypeObject*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// CellTimingTypeObject の外部変数
//////////////////////////////////////////////////////////////////////

// kCellTimingCombinational を表すオブジェクト
PyObject* Py_CellTimingType_kCombinational = NULL;


BEGIN_NONAMESPACE

// CellTimingType の定数を設定する関数
void
timing_type_set(CellTimingTypeObject& type_obj,
		PyObject*& py_obj,
		PyObject* module,
		const char* name)
{
  py_obj = (PyObject*)&type_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

END_NONAMESPACE

// CellTimingTypeObject 関係の初期化を行う．
void
CellTimingTypeObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellTimingType_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellTimingType", (PyObject*)&PyCellTimingType_Type);

  // 定数オブジェクトの生成と登録
  timing_type_set(CellTimingType_kCombinationalStruct,    Py_CellTimingType_kCombinational,    m, "kTimingTypeCombinational");

  // 定数オブジェクト用の文字列オブジェクトの生成
  CellTimingType_kCombinationalString    = PyString_FromString("combinational");
}

END_NAMESPACE_YM

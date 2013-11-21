
/// @file pycell_CellTimingSense.cc
/// @brief tCellTimingSense の Python 用ラッパ
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

// tCellTimingSense を表す型
struct CellTimingSenseObject
{
  // Python のお約束
  PyObject_HEAD

  // 実際の値
  tCellTimingSense mVal;

};

// kCellPosiUnate の実体
CellTimingSenseObject kCellPosiUnateStruct = {
  PyObject_HEAD_INIT(&PyCellTimingSense_Type)
  kCellPosiUnate
};

// kCellNegaUnate の実体
CellTimingSenseObject kCellNegaUnateStruct = {
  PyObject_HEAD_INIT(&PyCellTimingSense_Type)
  kCellNegaUnate
};

// kCellNonUnate の実体
CellTimingSenseObject kCellNonUnateStruct = {
  PyObject_HEAD_INIT(&PyCellTimingSense_Type)
  kCellNonUnate
};

// repr 用の文字列オブジェクト
PyObject* kCellPosiUnateString = NULL;
PyObject* kCellNegaUnateString = NULL;
PyObject* kCellNonUnateString  = NULL;

// 個々の定数を表す文字列
const char* kCellPosiUnateName = "posi_unate";
const char* kCellNegaUnateName = "nega_unate";
const char* kCellNonUnateName  = "non_unate";


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellTimingSenseObject の生成関数
// 実際には既存のオブジェクトを返す．
CellTimingSenseObject*
CellTimingSense_new(PyTypeObject* type,
		   PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }

  return (CellTimingSenseObject*)PyCellTimingSense_FromString(str);
}

// CellTimingSenseObject を開放する関数
void
CellTimingSense_dealloc(CellTimingSenseObject* self)
{
  PyObject_Del(self);
}

// repr 関数
PyObject*
CellTimingSense_repr(CellTimingSenseObject* self)
{
  PyObject* result = NULL;
  switch ( self->mVal ) {
  case kCellPosiUnate: result = kCellPosiUnateString; break;
  case kCellNegaUnate: result = kCellNegaUnateString; break;
  case kCellNonUnate:  result = kCellNonUnateString; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

//////////////////////////////////////////////////////////////////////
// CellTimingSenseObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellTimingSense_methods[] = {
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
// CellTimingSenseObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellTimingSense_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellTimingSense",              // tp_name
  sizeof(CellTimingSenseObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellTimingSense_dealloc,     // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)CellTimingSense_repr,          // tp_repr

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
  "Timing Sense",                 // tp_doc

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
  CellTimingSense_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)0,                     // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)CellTimingSense_new,            // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と tCellTimingSense の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief tCellTimingSense から CellTimingSenseObject を生成する．
// @param[in] val tCellTimingSense の値
PyObject*
PyCellTimingSense_FromCellTimingSense(tCellTimingSense val)
{
  PyObject* result = NULL;
  switch ( val ) {
  case kCellPosiUnate: result = Py_kCellPosiUnate; break;
  case kCellNegaUnate: result = Py_kCellNegaUnate; break;
  case kCellNonUnate:  result = Py_kCellNonUnate; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// @brief 文字列から CellTimingSenseObject を生成する．
// @param[in] str 文字列
PyObject*
PyCellTimingSense_FromString(const char* str)
{
  PyObject* result = NULL;
  if ( str == NULL ) {
    // デフォルト値
    result = Py_kCellNonUnate;
  }
  else if ( strcmp(str, kCellPosiUnateName) == 0 ) {
    result = Py_kCellPosiUnate;
  }
  else if ( strcmp(str, kCellNegaUnateName) == 0 ) {
    result = Py_kCellNegaUnate;
  }
  else if ( strcmp(str, kCellNonUnateName) == 0 ) {
    result = Py_kCellNonUnate;
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "Illegal string for tCellTimingSense");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から tCellTimingSense を取り出す．
// @param[in] py_obj Python オブジェクト
// @return tCellTimingSense を返す．
// @note 変換が失敗したら TypeError を送出し，kCellNonUnate を返す．
tCellTimingSense
PyCellTimingSense_AsCellTimingSense(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellTimingSense_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellTimingSense is expected");
    return kCellNonUnate;
  }

  // 強制的にキャスト
  CellTimingSenseObject* my_obj = (CellTimingSenseObject*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// CellTimingSenseObject の外部変数
//////////////////////////////////////////////////////////////////////

// kCellPosiUnate を表すオブジェクト
PyObject* Py_kCellPosiUnate = NULL;

// kCellNegaUnate を表すオブジェクト
PyObject* Py_kCellNegaUnate = NULL;

// kCellNonUnate を表すオブジェクト
PyObject* Py_kCellNonUnate  = NULL;


BEGIN_NONAMESPACE

// tCellTimingSense の定数を設定する関数
void
obj_set(CellTimingSenseObject& my_obj,
	PyObject*& py_obj,
	PyObject* module,
	const char* name)
{
  py_obj = (PyObject*)&my_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

END_NONAMESPACE

// CellTimingSenseObject 関係の初期化を行う．
void
CellTimingSenseObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellTimingSense_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellTimingSense", (PyObject*)&PyCellTimingSense_Type);

  // 定数オブジェクトの生成と登録
  obj_set(kCellPosiUnateStruct,
          Py_kCellPosiUnate, m,
          "kCellPosiUnate");

  obj_set(kCellNegaUnateStruct,
          Py_kCellNegaUnate, m,
          "kCellNegaUnate");

  obj_set(kCellNonUnateStruct,
          Py_kCellNonUnate, m,
          "kCellNonUnate");

  // 定数オブジェクト用の文字列オブジェクトの生成
  kCellPosiUnateString = PyString_FromString(kCellPosiUnateName);
  kCellNegaUnateString = PyString_FromString(kCellNegaUnateName);
  kCellNonUnateString  = PyString_FromString(kCellNonUnateName);

}

END_NAMESPACE_YM

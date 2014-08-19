
/// @file pycell_CellPatType.cc
/// @brief tCellPatType の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_YmCell/pycell.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// tCellPatType を表す型
struct CellPatTypeObject
{
  // Python のお約束
  PyObject_HEAD

  // 実際の値
  tCellPatType mVal;

};

// kCellPatInput の実体
CellPatTypeObject kCellPatInputStruct = {
  PyObject_HEAD_INIT(&PyCellPatType_Type)
  kCellPatInput
};

// kCellPatAnd の実体
CellPatTypeObject kCellPatAndStruct = {
  PyObject_HEAD_INIT(&PyCellPatType_Type)
  kCellPatAnd
};

// kCellPatXor の実体
CellPatTypeObject kCellPatXorStruct = {
  PyObject_HEAD_INIT(&PyCellPatType_Type)
  kCellPatXor
};

// repr 用の文字列オブジェクト
PyObject* kCellPatInputString = NULL;
PyObject* kCellPatAndString   = NULL;
PyObject* kCellPatXorString   = NULL;

// 個々の定数を表す文字列
const char* kCellPatInputName = "input";
const char* kCellPatAndName   = "and";
const char* kCellPatXorName   = "xor";


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellPatTypeObject の生成関数
// 実際には既存のオブジェクトを返す．
CellPatTypeObject*
CellPatType_new(PyTypeObject* type,
		   PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }

  return (CellPatTypeObject*)PyCellPatType_FromString(str);
}

// CellPatTypeObject を開放する関数
void
CellPatType_dealloc(CellPatTypeObject* self)
{
  PyObject_Del(self);
}

// repr 関数
PyObject*
CellPatType_repr(CellPatTypeObject* self)
{
  PyObject* result = NULL;
  switch ( self->mVal ) {
  case kCellPatInput: result = kCellPatInputString; break;
  case kCellPatAnd:   result = kCellPatAndString; break;
  case kCellPatXor:   result = kCellPatXorString; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

//////////////////////////////////////////////////////////////////////
// CellPatTypeObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellPatType_methods[] = {
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
// CellPatTypeObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellPatType_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellPatType",              // tp_name
  sizeof(CellPatTypeObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellPatType_dealloc,     // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)CellPatType_repr,          // tp_repr

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
  "Pattern Node Type",                 // tp_doc

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
  CellPatType_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)0,                     // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)CellPatType_new,            // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と tCellPatType の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief tCellPatType から CellPatTypeObject を生成する．
// @param[in] val tCellPatType の値
PyObject*
PyCellPatType_FromCellPatType(tCellPatType val)
{
  PyObject* result = NULL;
  switch ( val ) {
  case kCellPatInput: result = Py_kCellPatInput; break;
  case kCellPatAnd:   result = Py_kCellPatAnd; break;
  case kCellPatXor:   result = Py_kCellPatXor; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// @brief 文字列から CellPatTypeObject を生成する．
// @param[in] str 文字列
PyObject*
PyCellPatType_FromString(const char* str)
{
  PyObject* result = NULL;
  if ( str == NULL ) {
    // デフォルト値
    result = Py_kCellPatInput;
  }
  else if ( strcmp(str, kCellPatInputName) == 0 ) {
    result = Py_kCellPatInput;
  }
  else if ( strcmp(str, kCellPatAndName) == 0 ) {
    result = Py_kCellPatAnd;
  }
  else if ( strcmp(str, kCellPatXorName) == 0 ) {
    result = Py_kCellPatXor;
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "Illegal string for tCellPatType");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から tCellPatType を取り出す．
// @param[in] py_obj Python オブジェクト
// @return tCellPatType を返す．
// @note 変換が失敗したら TypeError を送出し，kCellPatInput を返す．
tCellPatType
PyCellPatType_AsCellPatType(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellPatType_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellPatType is expected");
    return kCellPatInput;
  }

  // 強制的にキャスト
  CellPatTypeObject* my_obj = (CellPatTypeObject*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// CellPatTypeObject の外部変数
//////////////////////////////////////////////////////////////////////

// kCellPatInput を表すオブジェクト
PyObject* Py_kCellPatInput = NULL;

// kCellPatAnd を表すオブジェクト
PyObject* Py_kCellPatAnd   = NULL;

// kCellPatXor を表すオブジェクト
PyObject* Py_kCellPatXor   = NULL;


BEGIN_NONAMESPACE

// tCellPatType の定数を設定する関数
void
obj_set(CellPatTypeObject& my_obj,
	PyObject*& py_obj,
	PyObject* module,
	const char* name)
{
  py_obj = (PyObject*)&my_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

END_NONAMESPACE

// CellPatTypeObject 関係の初期化を行う．
void
CellPatTypeObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellPatType_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellPatType", (PyObject*)&PyCellPatType_Type);

  // 定数オブジェクトの生成と登録
  obj_set(kCellPatInputStruct,
          Py_kCellPatInput, m,
          "kCellPatInput");

  obj_set(kCellPatAndStruct,
          Py_kCellPatAnd, m,
          "kCellPatAnd");

  obj_set(kCellPatXorStruct,
          Py_kCellPatXor, m,
          "kCellPatXor");

  // 定数オブジェクト用の文字列オブジェクトの生成
  kCellPatInputString = PyString_FromString(kCellPatInputName);
  kCellPatAndString   = PyString_FromString(kCellPatAndName);
  kCellPatXorString   = PyString_FromString(kCellPatXorName);

}

END_NAMESPACE_YM

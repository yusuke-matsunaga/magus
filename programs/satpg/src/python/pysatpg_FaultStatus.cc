
/// @file pysatpg_FaultStatus.cc
/// @brief FaultStatus の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "pysatpg.h"
#include "FaultStatus.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// FaultStatus を表す型
struct FaultStatusObject
{
  // Python のお約束
  PyObject_HEAD

  // FaultStatus の値
  FaultStatus mVal;

};

//////////////////////////////////////////////////////////////////////
// FaultStatusObject の外部変数
//////////////////////////////////////////////////////////////////////

// kFsUndetected を表すオブジェクト
PyObject* Py_kFsUndetected;

// kFsDetected を表すオブジェクト
PyObject* Py_kFsDetected;

// kFsUntestable を表すオブジェクト
PyObject* Py_kFsUntestable;

// kFsPartiallyUntestable を表すオブジェクト
PyObject* Py_kFsPartiallyUntestable;

// kFsAborted を表すオブジェクト
PyObject* Py_kFsAborted;

// Py_kFsUndetected の実体
FaultStatusObject Py_kFsUndetectedStruct = {
  PyObject_HEAD_INIT(&PyFaultStatus_Type)
  kFsUndetected
};

// Py_kFsDetected の実体
FaultStatusObject Py_kFsDetectedStruct = {
  PyObject_HEAD_INIT(&PyFaultStatus_Type)
  kFsDetected
};

// Py_kFsUntestable の実体
FaultStatusObject Py_kFsUntestableStruct = {
  PyObject_HEAD_INIT(&PyFaultStatus_Type)
  kFsUntestable
};

// Py_kFsPartiallyUntestable の実体
FaultStatusObject Py_kFsPartiallyUntestableStruct = {
  PyObject_HEAD_INIT(&PyFaultStatus_Type)
  kFsPartiallyUntestable
};

// Py_kFsAborted の実体
FaultStatusObject Py_kFsAbortedStruct = {
  PyObject_HEAD_INIT(&PyFaultStatus_Type)
  kFsAborted
};

// repr 用の文字列オブジェクト
PyObject* Py_kFsUndetectedString = NULL;
PyObject* Py_kFsDetectedString = NULL;
PyObject* Py_kFsUntestableString = NULL;
PyObject* Py_kFsPartiallyUntestableString = NULL;
PyObject* Py_kFsAbortedString = NULL;


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// FaultStatusObject の生成関数
// 実際には存在しているオブジェクトを返すだけ．
FaultStatusObject*
FaultStatus_new(PyTypeObject* type,
		PyObject* args)
{
  // 引数は
  // - ()
  // - (str) {"undetected"|"detected"|"untestable"|"parially untestable"|"aborted"}
  ymuint n = PyTuple_GET_SIZE(args);
  if ( n == 0 ) {
    // デフォルトは kFsUndetected
    Py_INCREF(Py_kFsUndetected);
    return (FaultStatusObject*)Py_kFsUndetected;
  }
  if ( n == 1 ) {
    PyObject* obj = PyTuple_GET_ITEM(args, 0);
    if ( PyString_Check(obj) ) {
      char* str = PyString_AsString(obj);
      return (FaultStatusObject*)PyFaultStatus_FromString(str);
    }
  }
}

// repr 関数
PyObject*
FaultStatus_repr(FaultStatusObject* self)
{
  switch ( self->mVal ) {
  case kFsUndetected:          return Py_kFsUndetectedString;
  case kFsDetected:            return Py_kFsDetectedString;
  case kFsUntestable:          return Py_kFsUntestableString;
  case kFsPartiallyUntestable: return Py_kFsPartiallyUntestableString;
  case kFsAborted:             return Py_kFsAbortedString;
  default: break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// FaultStatusObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef FaultStatus_methods[] = {
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// FaultStatusObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyFaultStatus_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "satpg.FaultStatus",            // tp_name
  sizeof(FaultStatusObject),      // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)0,                  // tp_dealloc
  (printfunc)0,                   // tp_print
  (getattrfunc)0,                 // tp_getattr
  (setattrfunc)0,                 // tp_setattr
  (cmpfunc)0,                     // tp_compare
  (reprfunc)FaultStatus_repr,     // tp_repr

  // Method suites for standard classes
  0,                              // tp_as_number
  0,                              // tp_as_sequence
  0,                              // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                    // tp_hash
  (ternaryfunc)0,                 // tp_call
  (reprfunc)0,                    // tp_str
  (getattrofunc)0,                // tp_getattro
  (setattrofunc)0,                // tp_setattro

  // Functions to access object as input/output buffer
  0,                              // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,             // tp_flags

  // Documentation string
  "fault status",                 // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                // tp_traverse

  // delete references to contained objects
  (inquiry)0,                     // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                 // tp_richcompare

  // weak reference enabler
  (long)0,                        // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                 // tp_iter
  (iternextfunc)0,                // tp_iternext

  // Attribute descriptor and subclassing stuff
  FaultStatus_methods,            // tp_methods
  0,                              // tp_members
  0,                              // tp_getset
  (struct _typeobject*)0,         // tp_base
  (PyObject*)0,                   // tp_dict
  (descrgetfunc)0,                // tp_descr_get
  (descrsetfunc)0,                // tp_descr_set
  (long)0,                        // tp_dictoffset
  (initproc)0,                    // tp_init
  (allocfunc)0,                   // tp_alloc
  (newfunc)FaultStatus_new,       // tp_new
  (freefunc)0,                    // tp_free
  (inquiry)0,                     // tp_is_gc

  (PyObject*)0,                   // tp_bases
  (PyObject*)0,                   // tp_mro (method resolution order)
  (PyObject*)0,                   // tp_cache
  (PyObject*)0,                   // tp_subclasses
  (PyObject*)0                    // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と FaultStatus の間の変換関数
//////////////////////////////////////////////////////////////////////

// FaultStatus からの変換関数
PyObject*
PyFaultStatus_FromFaultStatus(FaultStatus val)
{
  PyObject* result = NULL;
  switch ( val ) {
  case kFsUndetected:          result = Py_kFsUndetected; break;
  case kFsDetected:            result = Py_kFsDetected; break;
  case kFsUntestable:          result = Py_kFsUntestable; break;
  case kFsPartiallyUntestable: result = Py_kFsPartiallyUntestable; break;
  case kFsAborted:             result = Py_kFsAborted; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// 文字列からの変換関数
PyObject*
PyFaultStatus_FromString(const char* str)
{
  if ( str == NULL ) {
    PyErr_SetString(PyExc_ValueError,
		    "NULL pointer is not allowed here");
    return NULL;
  }

  PyObject* result = NULL;
  if ( strcmp(str, "undetected") == 0 ) {
    result = Py_kFsUndetected;
  }
  else if ( strcmp(str, "detected") == 0 ) {
    result = Py_kFsDetected;
  }
  else if ( strcmp(str, "untestable") == 0 ) {
    result = Py_kFsUntestable;
  }
  else if ( strcmp(str, "partially untestable") == 0 ) {
    result = Py_kFsPartiallyUntestable;
  }
  else if ( strcmp(str, "aborted") == 0 ) {
    result = Py_kFsAborted;
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "Only 'undetected', 'detected', 'untestable', 'partially untestable' and 'aborted' are allowed here");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

/// @brief PyObject から FaultStatus を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return FaultStatus を返す．
/// @note 変換が失敗したら TypeError を送出し，kFsUndetected を返す．
extern
FaultStatus
PyFaultStatus_AsFaultStatus(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyFaultStatus_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "satpg.FaultStatus is expected");
    return kFsUndetected;
  }

  // 強制的にキャスト
  FaultStatusObject* my_obj = (FaultStatusObject*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// 初期化用の関数
//////////////////////////////////////////////////////////////////////

// FaultStatus の定数を設定する関数
inline
void
FaultStatus_set(FaultStatusObject& fs_obj,
		PyObject*& py_obj,
		PyObject* module,
		const char* name)
{
  py_obj = (PyObject*)&fs_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

// 文字列用オブジェクトの生成を行う．
inline
PyObject*
new_string(const char* str)
{
  PyObject* py_obj = PyString_InternFromString(str);
  Py_INCREF(py_obj);
  return py_obj;
}

// FaultStatusObject 関係の初期化を行う．
void
FaultStatusObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyFaultStatus_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "FaultStatus", (PyObject*)&PyFaultStatus_Type);

  // 定数オブジェクトの生成と登録
  FaultStatus_set(Py_kFsUndetectedStruct, Py_kFsUndetected, m, "kFsUndetected");
  FaultStatus_set(Py_kFsDetectedStruct,   Py_kFsDetected,   m, "kFsDetected");
  FaultStatus_set(Py_kFsUntestableStruct, Py_kFsUntestable, m, "kFsUntestable");
  FaultStatus_set(Py_kFsPartiallyUntestableStruct, Py_kFsPartiallyUntestable, m, "kFsPariallyUntestable");
  FaultStatus_set(Py_kFsAbortedStruct,    Py_kFsAborted,    m, "kFsAborted");
}

END_NAMESPACE_YM_SATPG

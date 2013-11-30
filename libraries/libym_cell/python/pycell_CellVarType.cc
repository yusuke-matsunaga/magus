
/// @file pycell_CellVarType.cc
/// @brief tCellVarType の Python 用ラッパ
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

// tCellVarType を表す型
struct CellVarTypeObject
{
  // Python のお約束
  PyObject_HEAD

  // 実際の値
  tCellVarType mVal;

};

// kCellVarInputNetTransition の実体
CellVarTypeObject kCellVarInputNetTransitionStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarInputNetTransition
};

// kCellVarTotalOutputNetCapacitance の実体
CellVarTypeObject kCellVarTotalOutputNetCapacitanceStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarTotalOutputNetCapacitance
};

// kCellVarOutputNetLength の実体
CellVarTypeObject kCellVarOutputNetLengthStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarOutputNetLength
};

// kCellVarOutputNetWireCap の実体
CellVarTypeObject kCellVarOutputNetWireCapStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarOutputNetWireCap
};

// kCellVarOutputNetPinCap の実体
CellVarTypeObject kCellVarOutputNetPinCapStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarOutputNetPinCap
};

// kCellVarRelatedOutTotalOutputNetCapacitance の実体
CellVarTypeObject kCellVarRelatedOutTotalOutputNetCapacitanceStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarRelatedOutTotalOutputNetCapacitance
};

// kCellVarRelatedOutOutputNetLength の実体
CellVarTypeObject kCellVarRelatedOutOutputNetLengthStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarRelatedOutOutputNetLength
};

// kCellVarRelatedOutOutputNetWireCap の実体
CellVarTypeObject kCellVarRelatedOutOutputNetWireCapStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarRelatedOutOutputNetWireCap
};

// kCellVarRelatedOutOutputNetPinCap の実体
CellVarTypeObject kCellVarRelatedOutOutputNetPinCapStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarRelatedOutOutputNetPinCap
};

// kCellVarConstrainedPinTransition の実体
CellVarTypeObject kCellVarConstrainedPinTransitionStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarConstrainedPinTransition
};

// kCellVarRelatedPinTransition の実体
CellVarTypeObject kCellVarRelatedPinTransitionStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarRelatedPinTransition
};

// kCellVarNone の実体
CellVarTypeObject kCellVarNoneStruct = {
  PyObject_HEAD_INIT(&PyCellVarType_Type)
  kCellVarNone
};

// repr 用の文字列オブジェクト
PyObject* kCellVarInputNetTransitionString                  = NULL;
PyObject* kCellVarTotalOutputNetCapacitanceString           = NULL;
PyObject* kCellVarOutputNetLengthString                     = NULL;
PyObject* kCellVarOutputNetWireCapString                    = NULL;
PyObject* kCellVarOutputNetPinCapString                     = NULL;
PyObject* kCellVarRelatedOutTotalOutputNetCapacitanceString = NULL;
PyObject* kCellVarRelatedOutOutputNetLengthString           = NULL;
PyObject* kCellVarRelatedOutOutputNetWireCapString          = NULL;
PyObject* kCellVarRelatedOutOutputNetPinCapString           = NULL;
PyObject* kCellVarConstrainedPinTransitionString            = NULL;
PyObject* kCellVarRelatedPinTransitionString                = NULL;
PyObject* kCellVarNoneString                                = NULL;

// 個々の定数を表す文字列
const char* kCellVarInputNetTransitionName                  = "input_net_transition";
const char* kCellVarTotalOutputNetCapacitanceName           = "total_output_net_capacitance";
const char* kCellVarOutputNetLengthName                     = "output_net_length";
const char* kCellVarOutputNetWireCapName                    = "output_net_wire_cap";
const char* kCellVarOutputNetPinCapName                     = "output_net_pin_cap";
const char* kCellVarRelatedOutTotalOutputNetCapacitanceName = "related_out_total_output_net_capacitance";
const char* kCellVarRelatedOutOutputNetLengthName           = "related_out_output_net_length";
const char* kCellVarRelatedOutOutputNetWireCapName          = "related_out_output_net_wire_cap";
const char* kCellVarRelatedOutOutputNetPinCapName           = "related_out_output_net_pin_cap";
const char* kCellVarConstrainedPinTransitionName            = "constrained_pin_transition";
const char* kCellVarRelatedPinTransitionName                = "related_pin_transition";
const char* kCellVarNoneName                                = "none";


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellVarTypeObject の生成関数
// 実際には既存のオブジェクトを返す．
CellVarTypeObject*
CellVarType_new(PyTypeObject* type,
		   PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }

  return (CellVarTypeObject*)PyCellVarType_FromString(str);
}

// CellVarTypeObject を開放する関数
void
CellVarType_dealloc(CellVarTypeObject* self)
{
  PyObject_Del(self);
}

// repr 関数
PyObject*
CellVarType_repr(CellVarTypeObject* self)
{
  PyObject* result = NULL;
  switch ( self->mVal ) {
  case kCellVarInputNetTransition:                  result = kCellVarInputNetTransitionString; break;
  case kCellVarTotalOutputNetCapacitance:           result = kCellVarTotalOutputNetCapacitanceString; break;
  case kCellVarOutputNetLength:                     result = kCellVarOutputNetLengthString; break;
  case kCellVarOutputNetWireCap:                    result = kCellVarOutputNetWireCapString; break;
  case kCellVarOutputNetPinCap:                     result = kCellVarOutputNetPinCapString; break;
  case kCellVarRelatedOutTotalOutputNetCapacitance: result = kCellVarRelatedOutTotalOutputNetCapacitanceString; break;
  case kCellVarRelatedOutOutputNetLength:           result = kCellVarRelatedOutOutputNetLengthString; break;
  case kCellVarRelatedOutOutputNetWireCap:          result = kCellVarRelatedOutOutputNetWireCapString; break;
  case kCellVarRelatedOutOutputNetPinCap:           result = kCellVarRelatedOutOutputNetPinCapString; break;
  case kCellVarConstrainedPinTransition:            result = kCellVarConstrainedPinTransitionString; break;
  case kCellVarRelatedPinTransition:                result = kCellVarRelatedPinTransitionString; break;
  case kCellVarNone:                                result = kCellVarNoneString; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

//////////////////////////////////////////////////////////////////////
// CellVarTypeObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellVarType_methods[] = {
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
// CellVarTypeObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellVarType_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellVarType",              // tp_name
  sizeof(CellVarTypeObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellVarType_dealloc,     // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)CellVarType_repr,          // tp_repr

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
  "Variable Type",                 // tp_doc

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
  CellVarType_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)0,                     // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)CellVarType_new,            // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と tCellVarType の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief tCellVarType から CellVarTypeObject を生成する．
// @param[in] val tCellVarType の値
PyObject*
PyCellVarType_FromCellVarType(tCellVarType val)
{
  PyObject* result = NULL;
  switch ( val ) {
  case kCellVarInputNetTransition:                  result = Py_kCellVarInputNetTransition; break;
  case kCellVarTotalOutputNetCapacitance:           result = Py_kCellVarTotalOutputNetCapacitance; break;
  case kCellVarOutputNetLength:                     result = Py_kCellVarOutputNetLength; break;
  case kCellVarOutputNetWireCap:                    result = Py_kCellVarOutputNetWireCap; break;
  case kCellVarOutputNetPinCap:                     result = Py_kCellVarOutputNetPinCap; break;
  case kCellVarRelatedOutTotalOutputNetCapacitance: result = Py_kCellVarRelatedOutTotalOutputNetCapacitance; break;
  case kCellVarRelatedOutOutputNetLength:           result = Py_kCellVarRelatedOutOutputNetLength; break;
  case kCellVarRelatedOutOutputNetWireCap:          result = Py_kCellVarRelatedOutOutputNetWireCap; break;
  case kCellVarRelatedOutOutputNetPinCap:           result = Py_kCellVarRelatedOutOutputNetPinCap; break;
  case kCellVarConstrainedPinTransition:            result = Py_kCellVarConstrainedPinTransition; break;
  case kCellVarRelatedPinTransition:                result = Py_kCellVarRelatedPinTransition; break;
  case kCellVarNone:                                result = Py_kCellVarNone; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// @brief 文字列から CellVarTypeObject を生成する．
// @param[in] str 文字列
PyObject*
PyCellVarType_FromString(const char* str)
{
  PyObject* result = NULL;
  if ( str == NULL ) {
    // デフォルト値
    result = Py_kCellVarNone;
  }
  else if ( strcmp(str, kCellVarInputNetTransitionName) == 0 ) {
    result = Py_kCellVarInputNetTransition;
  }
  else if ( strcmp(str, kCellVarTotalOutputNetCapacitanceName) == 0 ) {
    result = Py_kCellVarTotalOutputNetCapacitance;
  }
  else if ( strcmp(str, kCellVarOutputNetLengthName) == 0 ) {
    result = Py_kCellVarOutputNetLength;
  }
  else if ( strcmp(str, kCellVarOutputNetWireCapName) == 0 ) {
    result = Py_kCellVarOutputNetWireCap;
  }
  else if ( strcmp(str, kCellVarOutputNetPinCapName) == 0 ) {
    result = Py_kCellVarOutputNetPinCap;
  }
  else if ( strcmp(str, kCellVarRelatedOutTotalOutputNetCapacitanceName) == 0 ) {
    result = Py_kCellVarRelatedOutTotalOutputNetCapacitance;
  }
  else if ( strcmp(str, kCellVarRelatedOutOutputNetLengthName) == 0 ) {
    result = Py_kCellVarRelatedOutOutputNetLength;
  }
  else if ( strcmp(str, kCellVarRelatedOutOutputNetWireCapName) == 0 ) {
    result = Py_kCellVarRelatedOutOutputNetWireCap;
  }
  else if ( strcmp(str, kCellVarRelatedOutOutputNetPinCapName) == 0 ) {
    result = Py_kCellVarRelatedOutOutputNetPinCap;
  }
  else if ( strcmp(str, kCellVarConstrainedPinTransitionName) == 0 ) {
    result = Py_kCellVarConstrainedPinTransition;
  }
  else if ( strcmp(str, kCellVarRelatedPinTransitionName) == 0 ) {
    result = Py_kCellVarRelatedPinTransition;
  }
  else if ( strcmp(str, kCellVarNoneName) == 0 ) {
    result = Py_kCellVarNone;
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "Illegal string for tCellVarType");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から tCellVarType を取り出す．
// @param[in] py_obj Python オブジェクト
// @return tCellVarType を返す．
// @note 変換が失敗したら TypeError を送出し，kCellVarNone を返す．
tCellVarType
PyCellVarType_AsCellVarType(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellVarType_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellVarType is expected");
    return kCellVarNone;
  }

  // 強制的にキャスト
  CellVarTypeObject* my_obj = (CellVarTypeObject*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// CellVarTypeObject の外部変数
//////////////////////////////////////////////////////////////////////

// kCellVarInputNetTransition を表すオブジェクト
PyObject* Py_kCellVarInputNetTransition                  = NULL;

// kCellVarTotalOutputNetCapacitance を表すオブジェクト
PyObject* Py_kCellVarTotalOutputNetCapacitance           = NULL;

// kCellVarOutputNetLength を表すオブジェクト
PyObject* Py_kCellVarOutputNetLength                     = NULL;

// kCellVarOutputNetWireCap を表すオブジェクト
PyObject* Py_kCellVarOutputNetWireCap                    = NULL;

// kCellVarOutputNetPinCap を表すオブジェクト
PyObject* Py_kCellVarOutputNetPinCap                     = NULL;

// kCellVarRelatedOutTotalOutputNetCapacitance を表すオブジェクト
PyObject* Py_kCellVarRelatedOutTotalOutputNetCapacitance = NULL;

// kCellVarRelatedOutOutputNetLength を表すオブジェクト
PyObject* Py_kCellVarRelatedOutOutputNetLength           = NULL;

// kCellVarRelatedOutOutputNetWireCap を表すオブジェクト
PyObject* Py_kCellVarRelatedOutOutputNetWireCap          = NULL;

// kCellVarRelatedOutOutputNetPinCap を表すオブジェクト
PyObject* Py_kCellVarRelatedOutOutputNetPinCap           = NULL;

// kCellVarConstrainedPinTransition を表すオブジェクト
PyObject* Py_kCellVarConstrainedPinTransition            = NULL;

// kCellVarRelatedPinTransition を表すオブジェクト
PyObject* Py_kCellVarRelatedPinTransition                = NULL;

// kCellVarNone を表すオブジェクト
PyObject* Py_kCellVarNone                                = NULL;


BEGIN_NONAMESPACE

// tCellVarType の定数を設定する関数
void
obj_set(CellVarTypeObject& my_obj,
	PyObject*& py_obj,
	PyObject* module,
	const char* name)
{
  py_obj = (PyObject*)&my_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

END_NONAMESPACE

// CellVarTypeObject 関係の初期化を行う．
void
CellVarTypeObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellVarType_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellVarType", (PyObject*)&PyCellVarType_Type);

  // 定数オブジェクトの生成と登録
  obj_set(kCellVarInputNetTransitionStruct,
          Py_kCellVarInputNetTransition, m,
          "kCellVarInputNetTransition");

  obj_set(kCellVarTotalOutputNetCapacitanceStruct,
          Py_kCellVarTotalOutputNetCapacitance, m,
          "kCellVarTotalOutputNetCapacitance");

  obj_set(kCellVarOutputNetLengthStruct,
          Py_kCellVarOutputNetLength, m,
          "kCellVarOutputNetLength");

  obj_set(kCellVarOutputNetWireCapStruct,
          Py_kCellVarOutputNetWireCap, m,
          "kCellVarOutputNetWireCap");

  obj_set(kCellVarOutputNetPinCapStruct,
          Py_kCellVarOutputNetPinCap, m,
          "kCellVarOutputNetPinCap");

  obj_set(kCellVarRelatedOutTotalOutputNetCapacitanceStruct,
          Py_kCellVarRelatedOutTotalOutputNetCapacitance, m,
          "kCellVarRelatedOutTotalOutputNetCapacitance");

  obj_set(kCellVarRelatedOutOutputNetLengthStruct,
          Py_kCellVarRelatedOutOutputNetLength, m,
          "kCellVarRelatedOutOutputNetLength");

  obj_set(kCellVarRelatedOutOutputNetWireCapStruct,
          Py_kCellVarRelatedOutOutputNetWireCap, m,
          "kCellVarRelatedOutOutputNetWireCap");

  obj_set(kCellVarRelatedOutOutputNetPinCapStruct,
          Py_kCellVarRelatedOutOutputNetPinCap, m,
          "kCellVarRelatedOutOutputNetPinCap");

  obj_set(kCellVarConstrainedPinTransitionStruct,
          Py_kCellVarConstrainedPinTransition, m,
          "kCellVarConstrainedPinTransition");

  obj_set(kCellVarRelatedPinTransitionStruct,
          Py_kCellVarRelatedPinTransition, m,
          "kCellVarRelatedPinTransition");

  obj_set(kCellVarNoneStruct,
          Py_kCellVarNone, m,
          "kCellVarNone");

  // 定数オブジェクト用の文字列オブジェクトの生成
  kCellVarInputNetTransitionString                  = PyString_FromString(kCellVarInputNetTransitionName);
  kCellVarTotalOutputNetCapacitanceString           = PyString_FromString(kCellVarTotalOutputNetCapacitanceName);
  kCellVarOutputNetLengthString                     = PyString_FromString(kCellVarOutputNetLengthName);
  kCellVarOutputNetWireCapString                    = PyString_FromString(kCellVarOutputNetWireCapName);
  kCellVarOutputNetPinCapString                     = PyString_FromString(kCellVarOutputNetPinCapName);
  kCellVarRelatedOutTotalOutputNetCapacitanceString = PyString_FromString(kCellVarRelatedOutTotalOutputNetCapacitanceName);
  kCellVarRelatedOutOutputNetLengthString           = PyString_FromString(kCellVarRelatedOutOutputNetLengthName);
  kCellVarRelatedOutOutputNetWireCapString          = PyString_FromString(kCellVarRelatedOutOutputNetWireCapName);
  kCellVarRelatedOutOutputNetPinCapString           = PyString_FromString(kCellVarRelatedOutOutputNetPinCapName);
  kCellVarConstrainedPinTransitionString            = PyString_FromString(kCellVarConstrainedPinTransitionName);
  kCellVarRelatedPinTransitionString                = PyString_FromString(kCellVarRelatedPinTransitionName);
  kCellVarNoneString                                = PyString_FromString(kCellVarNoneName);

}

END_NAMESPACE_YM

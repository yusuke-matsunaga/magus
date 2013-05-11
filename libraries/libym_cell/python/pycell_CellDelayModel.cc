
/// @file pycell_CellDelayModel.cc
/// @brief tCellDelayModel の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellDelayModel を表す型
struct CellDelayModelObject
{
  // Python のお約束
  PyObject_HEAD

  // 実際の値
  tCellDelayModel mVal;

};

// kCellDelayGenericCmos の実体
CellDelayModelObject kCellDelayGenericCmosStruct = {
  PyObject_HEAD_INIT(&PyCellDelayModel_Type)
  kCellDelayGenericCmos
};

// kCellDelayTableLookup の実体
CellDelayModelObject kCellDelayTableLookupStruct = {
  PyObject_HEAD_INIT(&PyCellDelayModel_Type)
  kCellDelayTableLookup
};

// kCellDelayPiecewiseCmos の実体
CellDelayModelObject kCellDelayPiecewiseCmosStruct = {
  PyObject_HEAD_INIT(&PyCellDelayModel_Type)
  kCellDelayPiecewiseCmos
};

// kCellDelayCmos2 の実体
CellDelayModelObject kCellDelayCmos2Struct = {
  PyObject_HEAD_INIT(&PyCellDelayModel_Type)
  kCellDelayCmos2
};

// kCellDelayDcm の実体
CellDelayModelObject kCellDelayDcmStruct = {
  PyObject_HEAD_INIT(&PyCellDelayModel_Type)
  kCellDelayDcm
};

// repr 用の文字列オブジェクト
PyObject* kCellDelayGenericCmosString   = NULL;
PyObject* kCellDelayTableLookupString   = NULL;
PyObject* kCellDelayPiecewiseCmosString = NULL;
PyObject* kCellDelayCmos2String         = NULL;
PyObject* kCellDelayDcmString           = NULL;


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellDelayModelObject の生成関数
// 実際には既存のオブジェクトを返す．
CellDelayModelObject*
CellDelayModel_new(PyTypeObject* type,
		   PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }

  return (CellDelayModelObject*)PyCellDelayModel_FromString(str);
}

// CellDelayModelObject を開放する関数
void
CellDelayModel_dealloc(CellDelayModelObject* self)
{
  PyObject_Del(self);
}

// repr 関数
PyObject*
CellDelayModel_repr(CellDelayModelObject* self)
{
  PyObject* result = NULL;
  switch ( self->mVal ) {
  case kCellDelayGenericCmos:   result = kCellDelayGenericCmosString; break;
  case kCellDelayTableLookup:   result = kCellDelayTableLookupString; break;
  case kCellDelayPiecewiseCmos: result = kCellDelayPiecewiseCmosString; break;
  case kCellDelayCmos2:         result = kCellDelayCmos2String; break;
  case kCellDelayDcm:           result = kCellDelayDcmString; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

//////////////////////////////////////////////////////////////////////
// CellDelayModelObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellDelayModel_methods[] = {
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
// CellDelayModelObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellDelayModel_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellDelayModel",              // tp_name
  sizeof(CellDelayModelObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellDelayModel_dealloc,     // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)CellDelayModel_repr,          // tp_repr

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
  "Delay Model",                 // tp_doc

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
  CellDelayModel_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)0,                     // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)CellDelayModel_new,            // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と tCellDelayModel の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief tCellDelayModel から CellDelayModelObject を生成する．
// @param[in] delay_model tCellDelayModel の値
PyObject*
PyCellDelayModel_FromCellDelayModel(tCellDelayModel delay_model)
{
  PyObject* result = NULL;
  switch ( delay_model ) {
  case kCellDelayGenericCmos:   result = Py_kCellDelayGenericCmos; break;
  case kCellDelayTableLookup:   result = Py_kCellDelayTableLookup; break;
  case kCellDelayPiecewiseCmos: result = Py_kCellDelayPiecewiseCmos; break;
  case kCellDelayCmos2:         result = Py_kCellDelayCmos2; break;
  case kCellDelayDcm:           result = Py_kCellDelayDcm; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// @brief 文字列から CellDelayModelObject を生成する．
// @param[in] str 文字列
PyObject*
PyCellDelayModel_FromString(const char* str)
{
  PyObject* result = NULL;
  if ( str == NULL ) {
    // デフォルトは generic_cmos
    result = Py_kCellDelayGenericCmos;
  }
  else if ( strcmp(str, "geneirc_cmos") == 0 ) {
    result = Py_kCellDelayGenericCmos;
  }
  else if ( strcmp(str, "table_lookup") == 0 ) {
    result = Py_kCellDelayTableLookup;
  }
  else if ( strcmp(str, "piecewise_cmos") == 0 ) {
    result = Py_kCellDelayPiecewiseCmos;
  }
  else if ( strcmp(str, "cmos2") == 0 ) {
    result = Py_kCellDelayCmos2;
  }
  else if ( strcmp(str, "dcm") == 0 ) {
    result = Py_kCellDelayDcm;
  }
  else {
    PyErr_SetString(PyExc_ValueError, "Illegal string for tCellDelayModel");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から tCellDelayModel を取り出す．
// @param[in] py_obj Python オブジェクト
// @return tCellDelayModel を返す．
// @note 変換が失敗したら TypeError を送出し，kCellDelayGeneircCmos を返す．
tCellDelayModel
PyCellDelayModel_AsCellDelayModel(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellDelayModel_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellDelayModel is expected");
    return kCellDelayGenericCmos;
  }

  // 強制的にキャスト
  CellDelayModelObject* my_obj = (CellDelayModelObject*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// CellDelayModelObject の外部変数
//////////////////////////////////////////////////////////////////////

// kCellDelayGenericCmos を表すオブジェクト
PyObject* Py_kCellDelayGenericCmos = NULL;

// kCellDelayTableLookup を表すオブジェクト
PyObject* Py_kCellDelayTableLookup = NULL;

// kCellDelayPiecewiseCmos を表すオブジェクト
PyObject* Py_kCellDelayPiecewiseCmos = NULL;

// kCellDelayCmos2 を表すオブジェクト
PyObject* Py_kCellDelayCmos2 = NULL;

// kCellDelayDcm を表すオブジェクト
PyObject* Py_kCellDelayDcm = NULL;


BEGIN_NONAMESPACE

// tCellDelayModel の定数を設定する関数
void
obj_set(CellDelayModelObject& dm_obj,
	PyObject*& py_obj,
	PyObject* module,
	const char* name)
{
  py_obj = (PyObject*)&dm_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

END_NONAMESPACE

// CellDelayModelObject 関係の初期化を行う．
void
CellDelayModelObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellDelayModel_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellDelayModel", (PyObject*)&PyCellDelayModel_Type);

  // 定数オブジェクトの生成と登録
  obj_set(kCellDelayGenericCmosStruct,
	  Py_kCellDelayGenericCmos, m,
	  "kCellDelayGenericCmos");
  obj_set(kCellDelayTableLookupStruct,
	  Py_kCellDelayTableLookup, m,
	  "kCellDelayTableLookup");
  obj_set(kCellDelayPiecewiseCmosStruct,
	  Py_kCellDelayPiecewiseCmos, m,
	  "kCellDelayPiecewiseCmos");
  obj_set(kCellDelayCmos2Struct,
	  Py_kCellDelayCmos2, m,
	  "kCellDelayCmos2");
  obj_set(kCellDelayDcmStruct,
	  Py_kCellDelayDcm, m,
	  "kCellDelayDcm");

  // 定数オブジェクト用の文字列オブジェクトの生成
  kCellDelayGenericCmosString   = PyString_FromString("generic_cmos");
  kCellDelayTableLookupString   = PyString_FromString("table_lookup");
  kCellDelayPiecewiseCmosString = PyString_FromString("piecewise_cmos");
  kCellDelayCmos2String         = PyString_FromString("cmos2");
  kCellDelayDcmString           = PyString_FromString("dcm");
}

END_NAMESPACE_YM

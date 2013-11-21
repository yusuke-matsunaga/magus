
/// @file pycell_CellTechnology.cc
/// @brief tCellTechnology の Python 用ラッパ
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

// tCellTechnology を表す型
struct CellTechnologyObject
{
  // Python のお約束
  PyObject_HEAD

  // 実際の値
  tCellTechnology mVal;

};

// kCellTechCmos の実体
CellTechnologyObject kCellTechCmosStruct = {
  PyObject_HEAD_INIT(&PyCellTechnology_Type)
  kCellTechCmos
};

// kCellTechFpga の実体
CellTechnologyObject kCellTechFpgaStruct = {
  PyObject_HEAD_INIT(&PyCellTechnology_Type)
  kCellTechFpga
};

// repr 用の文字列オブジェクト
PyObject* kCellTechCmosString = NULL;
PyObject* kCellTechFpgaString = NULL;

// 個々の定数を表す文字列
const char* kCellTechCmosName = "cmos";
const char* kCellTechFpgaName = "fpga";


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellTechnologyObject の生成関数
// 実際には既存のオブジェクトを返す．
CellTechnologyObject*
CellTechnology_new(PyTypeObject* type,
		   PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }

  return (CellTechnologyObject*)PyCellTechnology_FromString(str);
}

// CellTechnologyObject を開放する関数
void
CellTechnology_dealloc(CellTechnologyObject* self)
{
  PyObject_Del(self);
}

// repr 関数
PyObject*
CellTechnology_repr(CellTechnologyObject* self)
{
  PyObject* result = NULL;
  switch ( self->mVal ) {
  case kCellTechCmos: result = kCellTechCmosString; break;
  case kCellTechFpga: result = kCellTechFpgaString; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

//////////////////////////////////////////////////////////////////////
// CellTechnologyObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellTechnology_methods[] = {
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
// CellTechnologyObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellTechnology_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellTechnology",              // tp_name
  sizeof(CellTechnologyObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellTechnology_dealloc,     // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)CellTechnology_repr,          // tp_repr

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
  "Technology Type",                 // tp_doc

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
  CellTechnology_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)0,                     // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)CellTechnology_new,            // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と tCellTechnology の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief tCellTechnology から CellTechnologyObject を生成する．
// @param[in] val tCellTechnology の値
PyObject*
PyCellTechnology_FromCellTechnology(tCellTechnology val)
{
  PyObject* result = NULL;
  switch ( val ) {
  case kCellTechCmos: result = Py_kCellTechCmos; break;
  case kCellTechFpga: result = Py_kCellTechFpga; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// @brief 文字列から CellTechnologyObject を生成する．
// @param[in] str 文字列
PyObject*
PyCellTechnology_FromString(const char* str)
{
  PyObject* result = NULL;
  if ( str == NULL ) {
    // デフォルト値
    result = Py_kCellTechCmos;
  }
  else if ( strcmp(str, kCellTechCmosName) == 0 ) {
    result = Py_kCellTechCmos;
  }
  else if ( strcmp(str, kCellTechFpgaName) == 0 ) {
    result = Py_kCellTechFpga;
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "Illegal string for tCellTechnology");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から tCellTechnology を取り出す．
// @param[in] py_obj Python オブジェクト
// @return tCellTechnology を返す．
// @note 変換が失敗したら TypeError を送出し，kCellTechCmos を返す．
tCellTechnology
PyCellTechnology_AsCellTechnology(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellTechnology_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellTechnology is expected");
    return kCellTechCmos;
  }

  // 強制的にキャスト
  CellTechnologyObject* my_obj = (CellTechnologyObject*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// CellTechnologyObject の外部変数
//////////////////////////////////////////////////////////////////////

// kCellTechCmos を表すオブジェクト
PyObject* Py_kCellTechCmos = NULL;

// kCellTechFpga を表すオブジェクト
PyObject* Py_kCellTechFpga = NULL;


BEGIN_NONAMESPACE

// tCellTechnology の定数を設定する関数
void
obj_set(CellTechnologyObject& my_obj,
	PyObject*& py_obj,
	PyObject* module,
	const char* name)
{
  py_obj = (PyObject*)&my_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

END_NONAMESPACE

// CellTechnologyObject 関係の初期化を行う．
void
CellTechnologyObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellTechnology_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellTechnology", (PyObject*)&PyCellTechnology_Type);

  // 定数オブジェクトの生成と登録
  obj_set(kCellTechCmosStruct,
          Py_kCellTechCmos, m,
          "kCellTechCmos");

  obj_set(kCellTechFpgaStruct,
          Py_kCellTechFpga, m,
          "kCellTechFpga");

  // 定数オブジェクト用の文字列オブジェクトの生成
  kCellTechCmosString = PyString_FromString(kCellTechCmosName);
  kCellTechFpgaString = PyString_FromString(kCellTechFpgaName);

}

END_NAMESPACE_YM

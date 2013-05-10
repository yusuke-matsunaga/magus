
/// @file pycell_CellDir.cc
/// @brief CellPin::tDirection の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellDir を表す型
struct CellDirObject
{
  // Python のお約束
  PyObject_HEAD

  // 実際の値
  CellPin::tDirection mVal;

};

// CellDir_kInput の実体
CellDirObject CellDir_kInputStruct = {
  PyObject_HEAD_INIT(&PyCellDir_Type)
  CellPin::kDirInput
};

// CellDir_kOutput の実体
CellDirObject CellDir_kOutputStruct = {
  PyObject_HEAD_INIT(&PyCellDir_Type)
  CellPin::kDirOutput
};

// CellDir_kInout の実体
CellDirObject CellDir_kInoutStruct = {
  PyObject_HEAD_INIT(&PyCellDir_Type)
  CellPin::kDirInout
};

// CellDir_kInternal の実体
CellDirObject CellDir_kInternalStruct = {
  PyObject_HEAD_INIT(&PyCellDir_Type)
  CellPin::kDirInternal
};

// repr 用の文字列オブジェクト
PyObject* CellDir_kInputString = NULL;
PyObject* CellDir_kOutputString = NULL;
PyObject* CellDir_kInoutString = NULL;
PyObject* CellDir_kInternalString = NULL;


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellDirObject の生成関数
// 実際には既存のオブジェクトを返す．
CellDirObject*
CellDir_new(PyTypeObject* type,
	    PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }

  return (CellDirObject*)PyCellDir_FromString(str);
}

// CellDirObject を開放する関数
void
CellDir_dealloc(CellDirObject* self)
{
  PyObject_Del(self);
}

// repr 関数
PyObject*
CellDir_repr(CellDirObject* self)
{
  PyObject* result = NULL;
  switch ( self->mVal ) {
  case CellPin::kDirInput:    result = CellDir_kInputString; break;
  case CellPin::kDirOutput:   result = CellDir_kOutputString; break;
  case CellPin::kDirInout:    result = CellDir_kInoutString; break;
  case CellPin::kDirInternal: result = CellDir_kInternalString; break;
  default: assert_not_reached(__FILE__, __LINE__); break;
  }

  Py_INCREF(result);
  return result;
}

// is_input 関数
PyObject*
CellDir_is_input(CellDirObject* self,
		 PyObject* args)
{
  return PyObject_FromBool(self->mVal == CellPin::kDirInput);
}

// is_output 関数
PyObject*
CellDir_is_output(CellDirObject* self,
		  PyObject* args)
{
  return PyObject_FromBool(self->mVal == CellPin::kDirOutput);
}

// is_inout 関数
PyObject*
CellDir_is_inout(CellDirObject* self,
		 PyObject* args)
{
  return PyObject_FromBool(self->mVal == CellPin::kDirInout);
}

// is_internal 関数
PyObject*
CellDir_is_internal(CellDirObject* self,
		    PyObject* args)
{
  return PyObject_FromBool(self->mVal == CellPin::kDirInternal);
}

//////////////////////////////////////////////////////////////////////
// CellDirObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellDir_methods[] = {
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

  {"is_input", (PyCFunction)CellDir_is_input, METH_NOARGS,
   PyDoc_STR("return TRUE if INPUT (NONE)")},
  {"is_output", (PyCFunction)CellDir_is_output, METH_NOARGS,
   PyDoc_STR("return TRUE if OUTPUT (NONE)")},
  {"is_inout", (PyCFunction)CellDir_is_inout, METH_NOARGS,
   PyDoc_STR("return TRUE if INOUT (NONE)")},
  {"is_internal", (PyCFunction)CellDir_is_internal, METH_NOARGS,
   PyDoc_STR("return TRUE if INTERNAL (NONE)")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellDirObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellDir_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.PinDir",              // tp_name
  sizeof(CellDirObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellDir_dealloc,     // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)CellDir_repr,          // tp_repr

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
  "Pin Direction",                 // tp_doc

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
  CellDir_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)0,                     // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)CellDir_new,            // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と CellDir の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief CellDir から CellDirObject を生成する．
// @param[in] obj CellDir オブジェクト
PyObject*
PyCellDir_FromCellDir(CellPin::tDirection dir)
{
  PyObject* result = NULL;
  switch ( dir ) {
  case CellPin::kDirInput:    result = Py_kDirInput; break;
  case CellPin::kDirOutput:   result = Py_kDirOutput; break;
  case CellPin::kDirInout:    result = Py_kDirInout; break;
  case CellPin::kDirInternal: result = Py_kDirInternal; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// @brief 文字列から CellDirObject を生成する．
// @param[in] str 文字列
PyObject*
PyCellDir_FromString(const char* str)
{
  PyObject* result = NULL;
  if ( str == NULL ) {
    // デフォルトは input
    result = Py_kDirInput;
  }
  else if ( strcmp(str, "input") == 0 ) {
    result = Py_kDirInput;
  }
  else if ( strcmp(str, "output") == 0 ) {
    result = Py_kDirOutput;
  }
  else if ( strcmp(str, "inout") == 0 ) {
    result = Py_kDirInout;
  }
  else if ( strcmp(str, "internal") == 0 ) {
    result = Py_kDirInternal;
  }
  else {
    PyErr_SetString(PyExc_ValueError, "Illegal string for CellDir");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から CellPin::tDirection を取り出す．
// @param[in] py_obj Python オブジェクト
// @return CellDir を返す．
// @note 変換が失敗したら TypeError を送出し，CellPin::kDirInput を返す．
CellPin::tDirection
PyCellDir_AsCellPinDirection(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellDir_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.PinDir is expected");
    return CellPin::kDirInput;
  }

  // 強制的にキャスト
  CellDirObject* my_obj = (CellDirObject*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// CellDirObject の外部変数
//////////////////////////////////////////////////////////////////////

// kDirInput を表すオブジェクト
PyObject* Py_kDirInput = NULL;

// kDirOutput を表すオブジェクト
PyObject* Py_kDirOutput = NULL;

// kDirInout を表すオブジェクト
PyObject* Py_kDirInout = NULL;

// kDirInternal を表すオブジェクト
PyObject* Py_kDirInternal = NULL;


BEGIN_NONAMESPACE

// CellDir の定数を設定する関数
void
Dir_set(CellDirObject& dir_obj,
	PyObject*& py_obj,
	PyObject* module,
	const char* name)
{
  py_obj = (PyObject*)&dir_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

END_NONAMESPACE

// CellDirObject 関係の初期化を行う．
void
CellDirObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellDir_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "PinDir", (PyObject*)&PyCellDir_Type);

  // 定数オブジェクトの生成と登録
  Dir_set(CellDir_kInputStruct,    Py_kDirInput,    m, "kDirInput");
  Dir_set(CellDir_kOutputStruct,   Py_kDirOutput,   m, "kDirOutput");
  Dir_set(CellDir_kInoutStruct,    Py_kDirInout,    m, "kDirInout");
  Dir_set(CellDir_kInternalStruct, Py_kDirInternal, m, "kDirInternal");

  // 定数オブジェクト用の文字列オブジェクトの生成
  CellDir_kInputString    = PyString_FromString("input");
  CellDir_kOutputString   = PyString_FromString("output");
  CellDir_kInoutString    = PyString_FromString("inout");
  CellDir_kInternalString = PyString_FromString("internal");
}

END_NAMESPACE_YM

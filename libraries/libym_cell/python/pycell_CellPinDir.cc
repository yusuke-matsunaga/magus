
/// @file pycell_CellPinDir.cc
/// @brief tCellPinDirection の Python 用ラッパ
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

// tCellPinDirection を表す型
struct CellPinDirObject
{
  // Python のお約束
  PyObject_HEAD

  // 実際の値
  tCellPinDirection mVal;

};

// kCellPinInput の実体
CellPinDirObject kCellPinInputStruct = {
  PyObject_HEAD_INIT(&PyCellPinDir_Type)
  kCellPinInput
};

// kCellPinOutput の実体
CellPinDirObject kCellPinOutputStruct = {
  PyObject_HEAD_INIT(&PyCellPinDir_Type)
  kCellPinOutput
};

// kCellPinInout の実体
CellPinDirObject kCellPinInoutStruct = {
  PyObject_HEAD_INIT(&PyCellPinDir_Type)
  kCellPinInout
};

// kCellPinInternal の実体
CellPinDirObject kCellPinInternalStruct = {
  PyObject_HEAD_INIT(&PyCellPinDir_Type)
  kCellPinInternal
};

// repr 用の文字列オブジェクト
PyObject* kCellPinInputString    = NULL;
PyObject* kCellPinOutputString   = NULL;
PyObject* kCellPinInoutString    = NULL;
PyObject* kCellPinInternalString = NULL;

// 個々の定数を表す文字列
const char* kCellPinInputName    = "input";
const char* kCellPinOutputName   = "output";
const char* kCellPinInoutName    = "inout";
const char* kCellPinInternalName = "internal";


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellPinDirObject の生成関数
// 実際には既存のオブジェクトを返す．
CellPinDirObject*
CellPinDir_new(PyTypeObject* type,
		   PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }

  return (CellPinDirObject*)PyCellPinDir_FromString(str);
}

// CellPinDirObject を開放する関数
void
CellPinDir_dealloc(CellPinDirObject* self)
{
  PyObject_Del(self);
}

// repr 関数
PyObject*
CellPinDir_repr(CellPinDirObject* self)
{
  PyObject* result = NULL;
  switch ( self->mVal ) {
  case kCellPinInput:    result = kCellPinInputString; break;
  case kCellPinOutput:   result = kCellPinOutputString; break;
  case kCellPinInout:    result = kCellPinInoutString; break;
  case kCellPinInternal: result = kCellPinInternalString; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// is_input 関数
PyObject*
CellPinDir_is_input(CellPinDirObject* self,
		    PyObject* args)
{
  return PyObject_FromBool(self->mVal == kCellPinInput);
}

// is_output 関数
PyObject*
CellPinDir_is_output(CellPinDirObject* self,
		     PyObject* args)
{
  return PyObject_FromBool(self->mVal == kCellPinOutput);
}

// is_inout 関数
PyObject*
CellPinDir_is_inout(CellPinDirObject* self,
		    PyObject* args)
{
  return PyObject_FromBool(self->mVal == kCellPinInout);
}

// is_internal 関数
PyObject*
CellPinDir_is_internal(CellPinDirObject* self,
		       PyObject* args)
{
  return PyObject_FromBool(self->mVal == kCellPinInternal);
}

//////////////////////////////////////////////////////////////////////
// CellPinDirObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellPinDir_methods[] = {
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

  {"is_input", (PyCFunction)CellPinDir_is_input, METH_NOARGS,
   PyDoc_STR("return TRUE if INPUT (NONE)")},
  {"is_output", (PyCFunction)CellPinDir_is_output, METH_NOARGS,
   PyDoc_STR("return TRUE if OUTPUT (NONE)")},
  {"is_inout", (PyCFunction)CellPinDir_is_inout, METH_NOARGS,
   PyDoc_STR("return TRUE if INOUT (NONE)")},
  {"is_internal", (PyCFunction)CellPinDir_is_internal, METH_NOARGS,
   PyDoc_STR("return TRUE if INTERNAL (NONE)")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellPinDirObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellPinDir_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellPinDir",              // tp_name
  sizeof(CellPinDirObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellPinDir_dealloc,     // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)CellPinDir_repr,          // tp_repr

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
  CellPinDir_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)0,                     // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)CellPinDir_new,            // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と tCellPinDirection の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief tCellPinDirection から CellPinDirObject を生成する．
// @param[in] val tCellPinDirection の値
PyObject*
PyCellPinDir_FromCellPinDir(tCellPinDirection val)
{
  PyObject* result = NULL;
  switch ( val ) {
  case kCellPinInput:    result = Py_kCellPinInput; break;
  case kCellPinOutput:   result = Py_kCellPinOutput; break;
  case kCellPinInout:    result = Py_kCellPinInout; break;
  case kCellPinInternal: result = Py_kCellPinInternal; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// @brief 文字列から CellPinDirObject を生成する．
// @param[in] str 文字列
PyObject*
PyCellPinDir_FromString(const char* str)
{
  PyObject* result = NULL;
  if ( str == NULL ) {
    // デフォルト値
    result = Py_kCellPinInput;
  }
  else if ( strcmp(str, kCellPinInputName) == 0 ) {
    result = Py_kCellPinInput;
  }
  else if ( strcmp(str, kCellPinOutputName) == 0 ) {
    result = Py_kCellPinOutput;
  }
  else if ( strcmp(str, kCellPinInoutName) == 0 ) {
    result = Py_kCellPinInout;
  }
  else if ( strcmp(str, kCellPinInternalName) == 0 ) {
    result = Py_kCellPinInternal;
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "Illegal string for tCellPinDirection");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から tCellPinDirection を取り出す．
// @param[in] py_obj Python オブジェクト
// @return tCellPinDirection を返す．
// @note 変換が失敗したら TypeError を送出し，kCellPinInput を返す．
tCellPinDirection
PyCellPinDir_AsCellPinDir(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellPinDir_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellPinDir is expected");
    return kCellPinInput;
  }

  // 強制的にキャスト
  CellPinDirObject* my_obj = (CellPinDirObject*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// CellPinDirObject の外部変数
//////////////////////////////////////////////////////////////////////

// kCellPinInput を表すオブジェクト
PyObject* Py_kCellPinInput    = NULL;

// kCellPinOutput を表すオブジェクト
PyObject* Py_kCellPinOutput   = NULL;

// kCellPinInout を表すオブジェクト
PyObject* Py_kCellPinInout    = NULL;

// kCellPinInternal を表すオブジェクト
PyObject* Py_kCellPinInternal = NULL;


BEGIN_NONAMESPACE

// tCellPinDirection の定数を設定する関数
void
obj_set(CellPinDirObject& my_obj,
	PyObject*& py_obj,
	PyObject* module,
	const char* name)
{
  py_obj = (PyObject*)&my_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

END_NONAMESPACE

// CellPinDirObject 関係の初期化を行う．
void
CellPinDirObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellPinDir_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellPinDir", (PyObject*)&PyCellPinDir_Type);

  // 定数オブジェクトの生成と登録
  obj_set(kCellPinInputStruct,
          Py_kCellPinInput, m,
          "kCellPinInput");

  obj_set(kCellPinOutputStruct,
          Py_kCellPinOutput, m,
          "kCellPinOutput");

  obj_set(kCellPinInoutStruct,
          Py_kCellPinInout, m,
          "kCellPinInout");

  obj_set(kCellPinInternalStruct,
          Py_kCellPinInternal, m,
          "kCellPinInternal");

  // 定数オブジェクト用の文字列オブジェクトの生成
  kCellPinInputString    = PyString_FromString(kCellPinInputName);
  kCellPinOutputString   = PyString_FromString(kCellPinOutputName);
  kCellPinInoutString    = PyString_FromString(kCellPinInoutName);
  kCellPinInternalString = PyString_FromString(kCellPinInternalName);

}

END_NAMESPACE_YM

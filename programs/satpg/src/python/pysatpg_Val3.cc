
/// @file pysatpg_Val3.cc
/// @brief Val3 の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "pysatpg.h"
#include "Val3.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// Val3 を表す型
struct Val3Object
{
  // Python のお約束
  PyObject_HEAD

  // Val3 の値
  Val3 mVal;
};

//////////////////////////////////////////////////////////////////////
// Val3Object の外部変数
//////////////////////////////////////////////////////////////////////

// @brief kValX を表すオブジェクト
PyObject* Py_kValX;

// @brief kVal0 を表すオブジェクト
PyObject* Py_kVal0;

// @brief kVal1 を表すオブジェクト
PyObject* Py_kVal1;

// Py_kValX の実体
Val3Object Py_kValXStruct = {
  PyObject_HEAD_INIT(&PyVal3_Type)
  kValX
};

// Py_kVal0 の実体
Val3Object Py_kVal0Struct = {
  PyObject_HEAD_INIT(&PyVal3_Type)
  kVal0
};

// Py_kVal1 の実体
Val3Object Py_kVal1Struct = {
  PyObject_HEAD_INIT(&PyVal3_Type)
  kVal1
};

// repr 用の文字列オブジェクト
PyObject* Py_kValXString = NULL;
PyObject* Py_kVal0String = NULL;
PyObject* Py_kVal1String = NULL;


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// Val3Object の生成関数
// 実際には存在しているオブジェクトを返すだけ．
Val3Object*
Val3_new(PyTypeObject* type,
	 PyObject* args)
{
  // 引数は
  // - ()
  // - (str) {x|X|0|1}
  // - (uint) {0, 1, other}
  ymuint n = PyTuple_GET_SIZE(args);
  if ( n == 0 ) {
    // デフォルトは kValX
    Py_INCREF(Py_kValX);
    return (Val3Object*)Py_kValX;
  }
  if ( n == 1 ) {
    PyObject* obj = PyTuple_GET_ITEM(args, 0);
    if ( PyString_Check(obj) ) {
      char* str = PyString_AsString(obj);
      return (Val3Object*)PyVal3_FromString(str);
    }
    if ( PyInt_Check(obj) ) {
      long val = PyInt_AS_LONG(obj);
      return (Val3Object*)PyVal3_FromLong(val);
    }
  }

  PyErr_SetString(PyExc_TypeError, "string or integer valueis expected");
  return NULL;
}

// repr 関数
PyObject*
Val3_repr(Val3Object* self)
{
  switch ( self->mVal ) {
  case kValX: return Py_kValXString;
  case kVal0: return Py_kVal0String;
  case kVal1: return Py_kVal1String;
  default: break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// inv 演算
PyObject*
Val3_inv(PyObject* left)
{
  if ( !PyVal3_Check(left) ) {
    PyErr_SetString(PyExc_TypeError, "satpg.Val3 is expected");
    return NULL;
  }

  Val3 val = PyVal3_AsVal3(left);

  return PyVal3_FromVal3(~val);
}

// and 演算
PyObject*
Val3_and(PyObject* left,
	 PyObject* right)
{
  if ( !PyVal3_Check(left) || !PyVal3_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "satpg.Val3 is expected");
    return NULL;
  }

  Val3 val1 = PyVal3_AsVal3(left);
  Val3 val2 = PyVal3_AsVal3(right);

  return PyVal3_FromVal3(val1 & val2);
}

// or 演算
PyObject*
Val3_or(PyObject* left,
	PyObject* right)
{
  if ( !PyVal3_Check(left) || !PyVal3_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "satpg.Val3 is expected");
    return NULL;
  }

  Val3 val1 = PyVal3_AsVal3(left);
  Val3 val2 = PyVal3_AsVal3(right);

  return PyVal3_FromVal3(val1 | val2);
}

// xor 演算
PyObject*
Val3_xor(PyObject* left,
	 PyObject* right)
{
  if ( !PyVal3_Check(left) || !PyVal3_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "satpg.Val3 is expected");
    return NULL;
  }

  Val3 val1 = PyVal3_AsVal3(left);
  Val3 val2 = PyVal3_AsVal3(right);

  return PyVal3_FromVal3(val1 ^ val2);
}


//////////////////////////////////////////////////////////////////////
// Val3Object の NumberMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PyNumberMethods Val3_nbmethods = {
  (binaryfunc)0,               // nb_add
  (binaryfunc)0,               // nb_subtract
  (binaryfunc)0,               // nb_multiply
  (binaryfunc)0,               // nb_divide
  (binaryfunc)0,               // nb_remainder
  (binaryfunc)0,               // nb_divmod
  (ternaryfunc)0,              // nb_power
  (unaryfunc)0,                // nb_negative
  (unaryfunc)0,                // nb_positive
  (unaryfunc)0,                // nb_absolute
  (inquiry)0,                  // nb_nonzero
  (unaryfunc)Val3_inv,         // nb_invert
  (binaryfunc)0,               // nb_lshift
  (binaryfunc)0,               // nb_rshift
  (binaryfunc)Val3_and,        // nb_and
  (binaryfunc)Val3_xor,        // nb_xor
  (binaryfunc)Val3_or,         // nb_or
  (coercion)0,                 // nb_coerce
  (unaryfunc)0,                // nb_int
  (unaryfunc)0,                // nb_long
  (unaryfunc)0,                // nb_float
  (unaryfunc)0,                // nb_oct
  (unaryfunc)0,                // nb_hex

  // Added in release 2.0
  (binaryfunc)0,               // nb_inplace_add
  (binaryfunc)0,               // nb_inplace_subtract
  (binaryfunc)0,               // nb_inplace_multiply
  (binaryfunc)0,               // nb_inplace_divide
  (binaryfunc)0,               // nb_inplace_remainder
  (ternaryfunc)0,              // nb_inplace_power
  (binaryfunc)0,               // nb_inplace_lshift
  (binaryfunc)0,               // nb_inplace_rshift
  (binaryfunc)0,               // nb_inplace_and
  (binaryfunc)0,               // nb_inplace_xor
  (binaryfunc)0,               // nb_inplace_or

  // Added in release 2.2
  (binaryfunc)0,               // nb_floor_divide
  (binaryfunc)0,               // nb_true_divide
  (binaryfunc)0,               // nb_inplace_floor_divide
  (binaryfunc)0,               // nb_inplace_true_divide

  // Added in release 2.5
  (unaryfunc)0                 // nb_index
};


//////////////////////////////////////////////////////////////////////
// Val3Object のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef Val3_methods[] = {
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// Val3Object 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyVal3_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "satpg.Val3",                   // tp_name
  sizeof(Val3Object),             // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)0,                  // tp_dealloc
  (printfunc)0,                   // tp_print
  (getattrfunc)0,                 // tp_getattr
  (setattrfunc)0,                 // tp_setattr
  (cmpfunc)0,                     // tp_compare
  (reprfunc)Val3_repr,            // tp_repr

  // Method suites for standard classes
  &Val3_nbmethods,                // tp_as_number
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
  "ternary value type",           // tp_doc

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
  Val3_methods,                   // tp_methods
  0,                              // tp_members
  0,                              // tp_getset
  (struct _typeobject*)0,         // tp_base
  (PyObject*)0,                   // tp_dict
  (descrgetfunc)0,                // tp_descr_get
  (descrsetfunc)0,                // tp_descr_set
  (long)0,                        // tp_dictoffset
  (initproc)0,                    // tp_init
  (allocfunc)0,                   // tp_alloc
  (newfunc)Val3_new,              // tp_new
  (freefunc)0,                    // tp_free
  (inquiry)0,                     // tp_is_gc

  (PyObject*)0,                   // tp_bases
  (PyObject*)0,                   // tp_mro (method resolution order)
  (PyObject*)0,                   // tp_cache
  (PyObject*)0,                   // tp_subclasses
  (PyObject*)0                    // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と Val3 の間の変換関数
//////////////////////////////////////////////////////////////////////

// Val3 からの変換関数
PyObject*
PyVal3_FromVal3(Val3 val)
{
  PyObject* result = NULL;
  switch ( val ) {
  case kValX: result = Py_kValX; break;
  case kVal0: result = Py_kVal0; break;
  case kVal1: result = Py_kVal1; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// 文字列からの変換関数
PyObject*
PyVal3_FromString(const char* str)
{
  if ( str == NULL ) {
    PyErr_SetString(PyExc_ValueError,
		    "NULL pointer is not allowed here");
    return NULL;
  }

  PyObject* result = NULL;
  if ( strlen(str) == 1 ) {
    switch ( str[0] ) {
    case 'x':
    case 'X': result = Py_kValX; break;
    case '0': result = Py_kVal0; break;
    case '1': result = Py_kVal1; break;
    }
  }
  if ( result == NULL ) {
    PyErr_SetString(PyExc_ValueError,
		    "Only '0', '1', and 'x'('X') are allowed here");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// long からの変換関数
PyObject*
PyVal3_FromLong(long val)
{
  // 0, 1 以外は X だと思う．
  PyObject* result = NULL;
  switch ( val ) {
  case 0:  result = Py_kVal0; break;
  case 1:  result = Py_kVal1; break;
  default: result = Py_kValX; break;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から Val3 を取り出す．
// @param[in] py_obj Python オブジェクト
// @return Val3 を返す．
// @note 変換が失敗したら TypeError を送出し，kVal3X を返す．
Val3
PyVal3_AsVal3(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyVal3_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "satpg.Val3 is expected");
    return kValX;
  }

  // 強制的にキャスト
  Val3Object* my_obj = (Val3Object*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// 初期化用の関数
//////////////////////////////////////////////////////////////////////

// Val3 の定数を設定する関数
inline
void
Val3_set(Val3Object& val3_obj,
	 PyObject*& py_obj,
	 PyObject* module,
	 const char* name)
{
  py_obj = (PyObject*)&val3_obj;
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

// Val3Object 関係の初期化を行う．
void
Val3Object_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyVal3_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Val3", (PyObject*)&PyVal3_Type);

  // 定数オブジェクトの生成と登録
  Val3_set(Py_kValXStruct, Py_kValX, m, "kValX");
  Val3_set(Py_kVal0Struct, Py_kVal0, m, "kVal0");
  Val3_set(Py_kVal1Struct, Py_kVal1, m, "kVal1");

  // 定数オブジェクト用の文字列オブジェクトの生成
  Py_kValXString = new_string("X");
  Py_kVal0String = new_string("0");
  Py_kVal1String = new_string("1");
}

END_NAMESPACE_YM_SATPG

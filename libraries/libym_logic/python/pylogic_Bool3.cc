
/// @file pylogic_Bool3.cc
/// @brief Bool3 の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/pylogic.h"
#include "YmLogic/Bool3.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// Bool3Object の外部変数
//////////////////////////////////////////////////////////////////////

// @brief kB3True を表すオブジェクト
PyObject* Py_kB3True = NULL;

// @brief kB3False を表すオブジェクト
PyObject* Py_kB3False = NULL;

// @brief kB3X を表すオブジェクト
PyObject* Py_kB3X = NULL;


BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// Bool3 を表す型
struct Bool3Object
{
  // Python のお約束
  PyObject_HEAD

  // Bool3 本体
  Bool3 mVal;

};


// Py_kB3True の本体
Bool3Object Py_kB3TrueStruct = {
  PyObject_HEAD_INIT(&PyBool3_Type)
  kB3True
};

// Py_kB3False の本体
Bool3Object Py_kB3FalseStruct = {
  PyObject_HEAD_INIT(&PyBool3_Type)
  kB3False
};

// Py_kB3X の本体
Bool3Object Py_kB3XStruct = {
  PyObject_HEAD_INIT(&PyBool3_Type)
  kB3X
};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// Bool3Object の生成関数
// 実際には既存のオブジェクトを返す．
Bool3Object*
Bool3_new(PyTypeObject* type,
	  PyObject* args)
{
  // 引数は
  // - ()       : x
  // - (str)    : {"true"|"false"|"x"}
  // - (PyBool) : True -> true, False -> false
  // - (long)   : 0 -> false, otherwise -> true
  ymuint n = PyTuple_GET_SIZE(args);

  if ( n == 0 ) {
    // デフォルトは kB3X
    Py_INCREF(Py_kB3X);
    return (Bool3Object*)Py_kB3X;
  }
  else if ( n == 1 ) {
    PyObject* obj = PyTuple_GET_ITEM(args, 0);
    if ( PyString_Check(obj) ) {
      const char* str = PyString_AsString(obj);
      return (Bool3Object*)PyBool3_FromString(str);
    }
    if ( PyInt_Check(obj) ) {
      ymlong val = PyInt_AS_LONG(obj);
      return (Bool3Object*)PyBool3_FromLong(val);
    }
  }

  PyErr_SetString(PyExc_TypeError, "string or long int is expected");
  return NULL;
}

// repr 用の文字列オブジェクト
PyObject* Py_kB3TrueString = NULL;
PyObject* Py_kB3FalseString = NULL;
PyObject* Py_kB3XString = NULL;

// repr 関数
PyObject*
Bool3_repr(Bool3Object* self)
{
  PyObject* obj = NULL;
  switch ( self->mVal ) {
  case kB3True:  obj = Py_kB3TrueString; break;
  case kB3False: obj = Py_kB3FalseString; break;
  case kB3X:     obj = Py_kB3XString; break;
  default:
    ASSERT_NOT_REACHED;
    break;
  }

  Py_INCREF(obj);
  return obj;
}

// inv 関数
PyObject*
Bool3_inv(PyObject* left)
{
  Bool3Object* b3_obj = (Bool3Object*)left;
  return PyBool3_FromBool3(~b3_obj->mVal);
}

// and 関数
PyObject*
Bool3_and(PyObject* left,
	  PyObject* right)
{
  if ( PyBool3_Check(left) && PyBool3_Check(right) ) {
    Bool3 v1 = PyBool3_AsBool3(left);
    Bool3 v2 = PyBool3_AsBool3(right);
    Bool3 val = kB3X;
    if ( v1 == kB3True && v2 == kB3True ) {
      val = kB3True;
    }
    else if ( v1 == kB3False || v2 == kB3False ) {
      val = kB3False;
    }
    return PyBool3_FromBool3(val);
  }
  // デフォルトでは数値型の演算を行なう．
  return PyInt_Type.tp_as_number->nb_and(left, right);
}

// xor 関数
PyObject*
Bool3_xor(PyObject* left,
	  PyObject* right)
{
  if ( PyBool3_Check(left) && PyBool3_Check(right) ) {
    Bool3 v1 = PyBool3_AsBool3(left);
    Bool3 v2 = PyBool3_AsBool3(right);
    Bool3 val = kB3X;
    if ( v1 == kB3True && v2 == kB3False ||
	 v1 == kB3False && v2 == kB3True ) {
      val = kB3True;
    }
    else if ( v1 == kB3False && v2 == kB3False ||
	      v1 == kB3True && v2 == kB3True ) {
      val = kB3False;
    }
    return PyBool3_FromBool3(val);
  }
  // デフォルトでは数値型の演算を行なう．
  return PyInt_Type.tp_as_number->nb_xor(left, right);
}

// or 関数
PyObject*
Bool3_or(PyObject* left,
	 PyObject* right)
{
  if ( PyBool3_Check(left) && PyBool3_Check(right) ) {
    Bool3 v1 = PyBool3_AsBool3(left);
    Bool3 v2 = PyBool3_AsBool3(right);
    Bool3 val = kB3X;
    if ( v1 == kB3True || v2 == kB3True ) {
      val = kB3True;
    }
    else if ( v1 == kB3False && v2 == kB3False ) {
      val = kB3False;
    }
    return PyBool3_FromBool3(val);
  }
  // デフォルトでは数値型の演算を行なう．
  return PyInt_Type.tp_as_number->nb_or(left, right);
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// PolObject の NumberMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PyNumberMethods Bool3_nbmethods = {
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
  (unaryfunc)Bool3_inv,        // nb_invert
  (binaryfunc)0,               // nb_lshift
  (binaryfunc)0,               // nb_rshift
  (binaryfunc)Bool3_and,       // nb_and
  (binaryfunc)Bool3_xor,       // nb_xor
  (binaryfunc)Bool3_or,        // nb_or
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


// Bool3Object 用のタイプオブジェクト
PyTypeObject PyBool3_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logic.Bool3",                /*tp_name*/
  sizeof(Bool3Object),          /*tp_basicsize*/
  0,                            /*tp_itemsize*/
  /* methods */
  0,                            /*tp_dealloc*/
  0,                            /*tp_print*/
  0,                            /*tp_getattr*/
  0,                            /*tp_setattr*/
  0,                            /*tp_compare*/
  (reprfunc)Bool3_repr,         /*tp_repr*/
  &Bool3_nbmethods,             /*tp_as_number*/
  0,                            /*tp_as_sequence*/
  0,                            /*tp_as_mapping*/
  0,                            /*tp_hash*/
  0,                            /*tp_call*/
  0,                            /*tp_str*/
  0,                            /*tp_getattro*/
  0,                            /*tp_setattro*/
  0,                            /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,           /*tp_flags*/
  "Ternary Logic Value",        /*tp_doc*/
  0,                            /*tp_traverse*/
  0,                            /*tp_clear*/
  0,                            /*tp_richcompare*/
  0,                            /*tp_weaklistoffset*/
  0,                            /*tp_iter*/
  0,                            /*tp_iternext*/
  0,                            /*tp_methods*/
  0,                            /*tp_members*/
  0,                            /*tp_getset*/
  0,                            /*tp_base*/
  0,                            /*tp_dict*/
  0,                            /*tp_descr_get*/
  0,                            /*tp_descr_set*/
  0,                            /*tp_dictoffset*/
  0,                            /*tp_init*/
  0,                            /*tp_alloc*/
  (newfunc)Bool3_new,           /*tp_new*/
  0,                            /*tp_free*/
  0,                            /*tp_is_gc*/
};


//////////////////////////////////////////////////////////////////////
// PyObject と Bool3 の間の変換関数
//////////////////////////////////////////////////////////////////////

/// @brief Bool3 から PyObject を生成する．
/// @param[in] val Bool3 オブジェクト
PyObject*
PyBool3_FromBool3(Bool3 val)
{
  PyObject* result = NULL;
  switch ( val ) {
  case kB3True:  result = Py_kB3True; break;
  case kB3False: result = Py_kB3False; break;
  case kB3X:     result = Py_kB3X; break;
  default:
    PyErr_SetString(PyExc_ValueError, "value is out-of-range");
    return NULL;
  }

  Py_INCREF(result);
  return (PyObject*)result;
}

// 文字列からの変換関数
PyObject*
PyBool3_FromString(const char* str)
{
  if ( str == NULL ) {
    PyErr_SetString(PyExc_ValueError,
		    "NULL pointer is not allowed here");
    return NULL;
  }

  // 小文字に変換する．
  // 効率が悪いけど元の文字列を変更したくないので
  // 新たな領域を確保する．
  ymuint n = strlen(str);
  char* buf = new char[n + 1];
  for (ymuint i = 0; i < n; ++ i) {
    buf[i] = tolower(str[i]);
  }
  buf[n] = '\0';

  Bool3 b3val;
  if ( strcmp(buf, "true") == 0 ) {
    b3val = kB3True;
  }
  else if ( strcmp(buf, "false") == 0 ) {
    b3val = kB3False;
  }
  else if ( strcmp(buf, "x") == 0 ) {
    b3val = kB3X;
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "parameter must be \"true\" or \"false\" or \"x\"");
    return NULL;
  }

  return PyBool3_FromBool3(b3val);
}

// @brief long から Bool3Object への変換関数
// @param[in] val 値
// @note 0 を kB3False, それ以外を kB3True に対応させる．
PyObject*
PyBool3_FromLong(ymlong val)
{
  Bool3 b3val = val ? kB3True : kB3False;
  return PyBool3_FromBool3(b3val);
}

// @brief PyObject から Bool3 を取り出す．
// @param[in] py_obj Python オブジェクト
// @return Bool3 を返す．
// @note 変換が失敗したら TypeError を送出し，kB3X を返す．
Bool3
PyBool3_AsBool3(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyBool3_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "logic.Bool3 is expected");
    return kB3X;
  }

  // 強制的にキャスト
  Bool3Object* bool3_obj = (Bool3Object*)py_obj;

  return bool3_obj->mVal;
}


BEGIN_NONAMESPACE

// Bool3 の定数を設定する関数
inline
void
Bool3_set(Bool3Object& b3_obj,
	  PyObject*& py_obj,
	  PyObject* module,
	  const char* name)
{
  py_obj = (PyObject*)&b3_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

// 文字列用オブジェクトが生成されていなければ生成する．
inline
PyObject*
new_string(const char* str)
{
  PyObject* py_obj = PyString_InternFromString(str);

  Py_INCREF(py_obj);
  return py_obj;
}

END_NONAMESPACE


// Bool3Object 関係の初期化
void
Bool3Object_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyBool3_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Bool3", (PyObject*)&PyBool3_Type);

  // 定数オブジェクトの登録
  Bool3_set(Py_kB3TrueStruct,  Py_kB3True,  m, "kB3True");
  Bool3_set(Py_kB3FalseStruct, Py_kB3False, m, "kB3False");
  Bool3_set(Py_kB3XStruct,     Py_kB3X,     m, "kB3X");

  // 定数オブジェクトの名前を表す文字列オブジェクトの生成
  Py_kB3TrueString = new_string("true");
  Py_kB3FalseString = new_string("false");
  Py_kB3XString = new_string("x");
}

END_NAMESPACE_YM

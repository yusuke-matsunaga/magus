
/// @file pylogic_Pol.cc
/// @brief tPol の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/pylogic.h"
#include "ym_logic/Pol.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// PolObject の外部変数
//////////////////////////////////////////////////////////////////////

// @brief kPolPosi を表すオブジェクト
PyObject* Py_kPolPosi = NULL;

// @brief kPolNega を表すオブジェクト
PyObject* Py_kPolNega = NULL;


BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// tPol を表す型
struct PolObject
{
  // Python のお約束
  PyObject_HEAD

  // tPol 本体
  tPol mPol;

};

// Py_kPolPosi の本体
PolObject Py_kPolPosiStruct = {
  PyObject_HEAD_INIT(&PyPol_Type)
  kPolPosi
};

// Py_kPolNega の本体
PolObject Py_kPolNegaStruct = {
  PyObject_HEAD_INIT(&PyPol_Type)
  kPolNega
};

// repr 用の文字列オブジェクト
PyObject* Py_kPolPosiString = NULL;
PyObject* Py_kPolNegaString = NULL;


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// PolObject の生成関数
// 実際には既存のオブジェクトを返す．
PolObject*
Pol_new(PyTypeObject* type,
	PyObject* args)
{
  // 引数は
  // - ()
  // - (str) {positive|negative}
  const char* str = "positive";
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }
  return (PolObject*)PyPol_FromString(str);
}

// 初期化関数
// "positive" か "negative" の文字列を引数にとりうる．
// 引数がなければ kPolPosi に初期化される．
int
Pol_init(PolObject* self,
	 PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return -1;
  }

  if ( str != NULL ) {
    if ( strcmp(str, "positive") == 0 ) {
      self->mPol = kPolPosi;
    }
    else if ( strcmp(str, "negative") == 0 ) {
      self->mPol = kPolNega;
    }
    else {
      ostringstream buf;
      buf << "\"" << str << "\": illegal string for polarity initializer";
      PyErr_SetString(PyExc_ValueError, buf.str().c_str());
      return -1;
    }
  }
  else {
    self->mPol = kPolPosi;
  }

  return 0;
}

// repr 関数
PyObject*
Pol_repr(PolObject* self)
{
  PyObject* result = NULL;
  if ( self->mPol == kPolPosi ) {
    result = Py_kPolPosiString;
  }
  else if ( self->mPol == kPolNega ) {
    result = Py_kPolNegaString;
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  Py_INCREF(result);
  return result;
}

// invert 関数
PyObject*
Pol_inv(PyObject* obj)
{
  PolObject* pol_obj = (PolObject*)obj;
  return PyPol_FromPol(~pol_obj->mPol);
}


//////////////////////////////////////////////////////////////////////
// PolObject の NumberMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PyNumberMethods Pol_nbmethods = {
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
  (unaryfunc)Pol_inv,          // nb_invert
  (binaryfunc)0,               // nb_lshift
  (binaryfunc)0,               // nb_rshift
  (binaryfunc)0,               // nb_and
  (binaryfunc)0,               // nb_xor
  (binaryfunc)0,               // nb_or
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

END_NONAMESPACE


// PolObject 用のタイプオブジェクト
PyTypeObject PyPol_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logic.Pol",                /*tp_name*/
  sizeof(PolObject),          /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  0,                          /*tp_dealloc*/
  0,                          /*tp_print*/
  0,                          /*tp_getattr*/
  0,                          /*tp_setattr*/
  0,                          /*tp_compare*/
  (reprfunc)Pol_repr,         /*tp_repr*/
  &Pol_nbmethods,             /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  0,                          /*tp_hash*/
  0,                          /*tp_call*/
  0,                          /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "Polarity",                 /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  0,                          /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  0,                          /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)Pol_new,           /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};


//////////////////////////////////////////////////////////////////////
// PyObject と tPol の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から tPol を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj tPol を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が PolObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   tPol& obj)
{
  // 型のチェック
  if ( !PolObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  PolObject* pol_obj = (PolObject*)py_obj;

  obj = pol_obj->mPol;

  return true;
}

// @brief tPol から PyObject を生成する．
// @param[in] obj tPol オブジェクト
PyObject*
PyPol_FromPol(tPol obj)
{
  PyObject* result = NULL;
  if ( obj == kPolPosi ) {
    result = Py_kPolPosi;
  }
  else {
    result = Py_kPolNega;
  }
  Py_INCREF(result);
  return result;
}

// 文字列からの変換関数
PyObject*
PyPol_FromString(const char* str)
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

  tPol pol;
  if ( strcmp(buf, "positive") == 0 ) {
    pol = kPolPosi;
  }
  else if ( strcmp(buf, "negative") == 0 ) {
    pol = kPolNega;
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "parameter must be \"positive\" or \"negative\"");
    delete [] buf;
    return NULL;
  }

  delete [] buf;

  return conv_to_pyobject(pol);
}


BEGIN_NONAMESPACE

// Pol の定数を設定する関数
inline
void
Pol_set(PolObject& pol_obj,
	PyObject*& py_obj,
	PyObject* module,
	const char* name)
{
  py_obj = (PyObject*)&pol_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

// 文字列用オブジェクトを生成する．
inline
PyObject*
new_string(const char* str)
{
  PyObject* py_obj = PyString_InternFromString(str);
  Py_INCREF(py_obj);
  return py_obj;
}

END_NONAMESPACE


// PolObject 関係の初期化を行う．
void
PolObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyPol_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Pol", (PyObject*)&PyPol_Type);

  // 定数オブジェクトの登録
  Pol_set(Py_kPolPosiStruct, Py_kPolPosi, m, "kPolPosi");
  Pol_set(Py_kPolNegaStruct, Py_kPolNega, m, "kPolNega");

  // 定数オブジェクト用の文字列オブジェクトの生成
  Py_kPolPosiString = new_string("positive");
  Py_kPolNegaString = new_string("negative");
}

END_NAMESPACE_YM

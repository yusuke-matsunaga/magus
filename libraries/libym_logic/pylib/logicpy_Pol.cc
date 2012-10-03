
/// @file logicpy_Pol.cc
/// @brief tPol の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "logicpy.h"
#include "ym_logic/Pol.h"


BEGIN_NAMESPACE_LOGICPY

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


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// PolObject の生成関数
PolObject*
Pol_new(PyTypeObject* type)
{
  PolObject* self = PyObject_New(PolObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // mPol は生成されている．
  // とりあえず初期化しておく
  self->mPol = kPolPosi;

  return self;
}

// PolObject を開放する関数
void
Pol_dealloc(PolObject* self)
{
  // mPol は開放の必要がない．

  PyObject_Del(self);
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
      PyErr_SetString(ErrorObject, buf.str().c_str());
      return -1;
    }
  }
  else {
    self->mPol = kPolPosi;
  }

  return 0;
}

// str 関数
PyObject*
Pol_str(PolObject* self)
{
  const char* str = NULL;
  if ( self->mPol == kPolPosi ) {
    str = "positive";
  }
  else {
    str = "negative";
  }
  return Py_BuildValue("s", str);
}

// make_positive 関数
PyObject*
Pol_make_positive(PyTypeObject* type_obj,
		  PyObject* args)
{
  return conv_to_pyobject(kPolPosi);
}

// make_negative 関数
PyObject*
Pol_make_negative(PyTypeObject* type_obj,
		  PyObject* args)
{
  return conv_to_pyobject(kPolNega);
}

// is_positive 関数
PyObject*
Pol_is_positive(PolObject* self,
		PyObject* args)
{
  bool v = (self->mPol == kPolPosi);
  return Py_BuildValue("i", v);
}

// is_negative 関数
PyObject*
Pol_is_negative(PolObject* self,
		PyObject* args)
{
  bool v = (self->mPol == kPolNega);
  return Py_BuildValue("i", v);
}

// alternate 関数
PyObject*
Pol_alternate(PolObject* self,
	      PyObject* args)
{
  return conv_to_pyobject(~self->mPol);
}

// PolObject のメソッドテーブル
PyMethodDef Pol_methods[] = {
  {"make_positive", (PyCFunction)Pol_make_positive, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make positive polarity")},
  {"make_negative", (PyCFunction)Pol_make_negative, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make negative polarity")},
  {"is_positive", (PyCFunction)Pol_is_positive, METH_NOARGS,
   PyDoc_STR("check if positive")},
  {"is_negative", (PyCFunction)Pol_is_negative, METH_NOARGS,
   PyDoc_STR("check if negative")},
  {"alternate", (PyCFunction)Pol_alternate, METH_NOARGS,
   PyDoc_STR("alternate")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


// PolObject 用のタイプオブジェクト
PyTypeObject PolType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logic.Pol",                /*tp_name*/
  sizeof(PolObject),          /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)Pol_dealloc,    /*tp_dealloc*/
  0,                          /*tp_print*/
  0,                          /*tp_getattr*/
  0,                          /*tp_setattr*/
  0,                          /*tp_compare*/
  0,                          /*tp_repr*/
  0,                          /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  0,                          /*tp_hash*/
  0,                          /*tp_call*/
  (reprfunc)Pol_str,          /*tp_str*/
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
  Pol_methods,                /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)Pol_init,         /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)Pol_new,           /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

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
conv_to_pyobject(tPol obj)
{
  PolObject* pol_obj = Pol_new(&PolType);
  if ( pol_obj == NULL ) {
    return NULL;
  }

  pol_obj->mPol = obj;

  Py_INCREF(pol_obj);
  return (PyObject*)pol_obj;
}

END_NAMESPACE_LOGICPY

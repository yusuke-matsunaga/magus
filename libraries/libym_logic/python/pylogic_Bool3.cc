
/// @file pylogic_Bool3.cc
/// @brief Bool3 の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/pylogic.h"
#include "ym_logic/Bool3.h"


BEGIN_NAMESPACE_YM_PYTHON

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


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// Bool3Object の生成関数
Bool3Object*
Bool3_new(PyTypeObject* type)
{
  Bool3Object* self = PyObject_New(Bool3Object, type);
  if ( self == NULL ) {
    return NULL;
  }

  // X で初期化
  self->mVal = kB3X;

  return self;
}

// Bool3Object を開放する関数
void
Bool3_dealloc(Bool3Object* self)
{
  PyObject_Del(self);
}

// 初期化関数
// 文字列を受け取り false, true, x に
// 応じて値の設定を行う．
int
Bool3_init(Bool3Object* self,
	   PyObject* args)
{
  char* val_s;
  if ( !PyArg_ParseTuple(args, "|s", &val_s) ) {
    return -1;
  }

  if ( val_s != NULL ) {
    ymuint l = strlen(val_s);
    for (ymuint i = 0; i < l; ++ i) {
      val_s[i] = tolower(val_s[i]);
    }
    if ( strcmp(val_s, "0") == 0 || strcmp(val_s, "false") == 0 ) {
      self->mVal = kB3False;
    }
    else if ( strcmp(val_s, "1") == 0 || strcmp(val_s, "true") == 0 ) {
      self->mVal = kB3True;
    }
    else if ( strcmp(val_s, "x") == 0 ) {
      self->mVal = kB3X;
    }
    else {
      PyErr_SetString(ErrorObject, "only 0|1|x|false|true is allowed here");
      return -1;
    }
  }
  else {
    // 引数がなければ X で初期化する．
    self->mVal = kB3X;
  }

  return 0;
}

// str 関数
PyObject*
Bool3_str(Bool3Object* self)
{
  const char* str = NULL;
  switch ( self->mVal ) {
  case kB3True:  str = "True"; break;
  case kB3False: str = "False"; break;
  case kB3X:     str = "X"; break;
  }
  return conv_to_pyobject(str);
}

// is_true 関数
PyObject*
Bool3_is_true(Bool3Object* self,
	      PyObject* args)
{
  return conv_to_pyobject(self->mVal == kB3True);
}

// is_false 関数
PyObject*
Bool3_is_false(Bool3Object* self,
	       PyObject* args)
{
  return conv_to_pyobject(self->mVal == kB3False);
}

// is_x 関数
PyObject*
Bool3_is_x(Bool3Object* self,
	   PyObject* args)
{
  return conv_to_pyobject(self->mVal == kB3X);
}

// set_true 関数
PyObject*
Bool3_set_true(Bool3Object* self,
	       PyObject* args)
{
  self->mVal = kB3True;

  Py_INCREF(Py_None);
  return Py_None;
}

// set_false 関数
PyObject*
Bool3_set_false(Bool3Object* self,
		PyObject* args)
{
  self->mVal = kB3False;

  Py_INCREF(Py_None);
  return Py_None;
}

// set_x 関数
PyObject*
Bool3_set_x(Bool3Object* self,
	    PyObject* args)
{
  self->mVal = kB3X;

  Py_INCREF(Py_None);
  return Py_None;
}

// Bool3 のメソッドテーブル
PyMethodDef Bool3_methods[] = {
  {"is_true", (PyCFunction)Bool3_is_true, METH_NOARGS,
   PyDoc_STR("check if TRUE (NONE)")},
  {"is_false", (PyCFunction)Bool3_is_false, METH_NOARGS,
   PyDoc_STR("check if FALSE (NONE)")},
  {"is_x", (PyCFunction)Bool3_is_x, METH_NOARGS,
   PyDoc_STR("check if X (NONE)")},
  {"set_true", (PyCFunction)Bool3_set_true, METH_NOARGS,
   PyDoc_STR("set TRUE (NONE)")},
  {"set_false", (PyCFunction)Bool3_set_false, METH_NOARGS,
   PyDoc_STR("set FALSE (NONE)")},
  {"set_x", (PyCFunction)Bool3_set_x, METH_NOARGS,
   PyDoc_STR("set X (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


// Bool3Object 用のタイプオブジェクト
PyTypeObject Bool3Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logic.Bool3",                /*tp_name*/
  sizeof(Bool3Object),          /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)Bool3_dealloc,    /*tp_dealloc*/
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
  (reprfunc)Bool3_str,          /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "Bool3arity",                 /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  Bool3_methods,                /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)Bool3_init,         /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)Bool3_new,           /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

// @brief PyObject から tBool3 を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj tBool3 を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が Bool3Object ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   Bool3& obj)
{
  // 型のチェック
  if ( !Bool3Object_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  Bool3Object* bool3_obj = (Bool3Object*)py_obj;

  obj = bool3_obj->mVal;

  return true;
}

// @brief tBool3 から PyObject を生成する．
// @param[in] obj tBool3 オブジェクト
PyObject*
conv_to_pyobject(Bool3 obj)
{
  Bool3Object* bool3_obj = Bool3_new(&Bool3Type);
  if ( bool3_obj == NULL ) {
    return NULL;
  }

  bool3_obj->mVal = obj;

  Py_INCREF(bool3_obj);
  return (PyObject*)bool3_obj;
}

END_NAMESPACE_YM_PYTHON


/// @file pyutils_USTime.cc
/// @brief USTime の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/USTime.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// USTime を表す型
struct USTimeObject
{
  // Python のお約束
  PyObject_HEAD

  // USTime の値
  USTime mTime;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// USTimeObject の生成関数
USTimeObject*
USTime_new(PyTypeObject* type)
{
  USTimeObject* self = PyObject_New(USTimeObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  return self;
}

// USTimeObject を開放する関数
void
USTime_dealloc(USTimeObject* self)
{
  PyObject_Del(self);
}

// 初期化関数
int
USTime_init(USTimeObject* self,
	    PyObject* args)
{
  double u = 0.0;
  double s = 0.0;
  double r = 0.0;
  if ( !PyArg_ParseTuple(args, "|ddd", &u, &s, &r) ) {
    return -1;
  }

  self->mTime.set(u, s, r);

  return 0;
}

// str 関数
PyObject*
USTime_str(USTimeObject* self)
{
  ostringstream buf;
  buf << self->mTime;
  return conv_to_pyobject(buf.str());
}
// set 関数
PyObject*
USTime_set(USTimeObject* self,
	   PyObject* args)
{
  double u = 0.0;
  double s = 0.0;
  double r = 0.0;
  if ( !PyArg_ParseTuple(args, "ddd", &u, &s, &r) ) {
    return NULL;
  }

  self->mTime.set(u, s, r);

  Py_INCREF(Py_None);
  return Py_None;
}

// usr_time_usec 関数
PyObject*
USTime_usr_time_usec(USTimeObject* self,
		     PyObject* args)
{
  return conv_to_pyobject(self->mTime.usr_time_usec());
}

// sys_time_usec 関数
PyObject*
USTime_sys_time_usec(USTimeObject* self,
		     PyObject* args)
{
  return conv_to_pyobject(self->mTime.sys_time_usec());
}

// real_time_usec 関数
PyObject*
USTime_real_time_usec(USTimeObject* self,
		      PyObject* args)
{
  return conv_to_pyobject(self->mTime.real_time_usec());
}

// usr_time 関数
PyObject*
USTime_usr_time(USTimeObject* self,
		PyObject* args)
{
  return conv_to_pyobject(self->mTime.usr_time());
}

// sys_time 関数
PyObject*
USTime_sys_time(USTimeObject* self,
		PyObject* args)
{
  return conv_to_pyobject(self->mTime.sys_time());
}

// real_time 関数
PyObject*
USTime_real_time(USTimeObject* self,
		 PyObject* args)
{
  return conv_to_pyobject(self->mTime.real_time());
}

// add 関数
PyObject*
USTime_add(PyObject* left,
	   PyObject* right)
{
  USTime time1;
  if ( !conv_from_pyobject(left, time1) ) {
    // TODO: エラーメッセージを作る．
    return NULL;
  }

  USTime time2;
  if ( !conv_from_pyobject(right, time2) ) {
    // TODO: エラーメッセージを作る．
    return NULL;
  }

  return conv_to_pyobject(time1 + time2);
}

// sub 関数
PyObject*
USTime_sub(PyObject* left,
	   PyObject* right)
{
  USTime time1;
  if ( !conv_from_pyobject(left, time1) ) {
    return NULL;
  }

  USTime time2;
  if ( !conv_from_pyobject(right, time2) ) {
    return NULL;
  }

  return conv_to_pyobject(time1 - time2);
}

// inplace add 関数
PyObject*
USTime_add2(PyObject* left,
	    PyObject* right)
{
  if ( !USTimeObject_Check(left) ) {
    // TODO: エラーメッセージを作る．
    return NULL;
  }
  USTimeObject* obj1 = (USTimeObject*)left;

  USTime time2;
  if ( !conv_from_pyobject(right, time2) ) {
    // TODO: エラーメッセージを作る．
    return NULL;
  }

  obj1->mTime += time2;

  Py_INCREF(left);
  return left;
}

// inplace sub 関数
PyObject*
USTime_sub2(PyObject* left,
	    PyObject* right)
{
  if ( !USTimeObject_Check(left) ) {
    // TODO: エラーメッセージを作る．
    return NULL;
  }
  USTimeObject* obj1 = (USTimeObject*)left;

  USTime time2;
  if ( !conv_from_pyobject(right, time2) ) {
    return NULL;
  }

  obj1->mTime -= time2;

  Py_INCREF(left);
  return left;
}

// USTimeObject のメソッドテーブル
PyMethodDef USTime_methods[] = {
  {"set", (PyCFunction)USTime_set, METH_VARARGS,
   PyDoc_STR("set time (double, double, double)")},
  {"usr_time_usec", (PyCFunction)USTime_usr_time_usec, METH_NOARGS,
   PyDoc_STR("return user-time in usec (NONE)")},
  {"sys_time_usec", (PyCFunction)USTime_sys_time_usec, METH_NOARGS,
   PyDoc_STR("return system-time in usec (NONE)")},
  {"real_time_usec", (PyCFunction)USTime_real_time_usec, METH_NOARGS,
   PyDoc_STR("return real-time in usec (NONE)")},
  {"usr_time", (PyCFunction)USTime_usr_time, METH_NOARGS,
   PyDoc_STR("return user-time in sec (NONE)")},
  {"sys_time", (PyCFunction)USTime_sys_time, METH_NOARGS,
   PyDoc_STR("return system-time in sec (NONE)")},
  {"real_time", (PyCFunction)USTime_real_time, METH_NOARGS,
   PyDoc_STR("return real-time in sec (NONE)")},
  {NULL, NULL, 0, NULL}
};

// USTimeObject の NumberMethod テーブル
PyNumberMethods USTime_nbmethods = {
  (binaryfunc)USTime_add,      // nb_add
  (binaryfunc)USTime_sub,      // nb_subtract
  (binaryfunc)0,               // nb_multiply
  (binaryfunc)0,               // nb_divide
  (binaryfunc)0,               // nb_remainder
  (binaryfunc)0,               // nb_divmod
  (ternaryfunc)0,              // nb_power
  (unaryfunc)0,                // nb_negative
  (unaryfunc)0,                // nb_positive
  (unaryfunc)0,                // nb_absolute
  (inquiry)0,                  // nb_nonzero
  (unaryfunc)0,                // nb_invert
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
  (binaryfunc)USTime_add2,     // nb_inplace_add
  (binaryfunc)USTime_add2,     // nb_inplace_subtract
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


// USTimeObject 用のタイプオブジェクト
PyTypeObject USTimeType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.USTime",             /*tp_name*/
  sizeof(USTimeObject),       /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)USTime_dealloc, /*tp_dealloc*/
  0,                          /*tp_print*/
  0,                          /*tp_getattr*/
  0,                          /*tp_setattr*/
  0,                          /*tp_compare*/
  0,                          /*tp_repr*/
  &USTime_nbmethods,          /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  0,                          /*tp_hash*/
  0,                          /*tp_call*/
  (reprfunc)USTime_str,       /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "user/system/real time",    /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  USTime_methods,             /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)USTime_init,      /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)USTime_new,        /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

// @brief PyObject から USTime を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj USTime を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が USTimeObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   USTime& obj)
{
  // 型のチェック
  if ( !USTimeObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  USTimeObject* time_obj = (USTimeObject*)py_obj;

  obj = time_obj->mTime;

  return true;
}

// @brief USTime から PyObject を生成する．
// @param[in] obj USTime オブジェクト
PyObject*
conv_to_pyobject(const USTime& obj)
{
  USTimeObject* py_obj = USTime_new(&USTimeType);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mTime = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

END_NAMESPACE_YM_PYTHON

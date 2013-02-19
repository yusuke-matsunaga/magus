
/// @file pyutils_USTime.cc
/// @brief USTime の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/USTime.h"


BEGIN_NAMESPACE_YM

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
  // 引数の形式は
  // - ()
  // - (double, double, double)
  // ただし， (double) (double, double) でも受け付けてしまう．
  double u = 0.0;
  double s = 0.0;
  double r = 0.0;
  if ( !PyArg_ParseTuple(args, "|ddd", &u, &s, &r) ) {
    return -1;
  }

  self->mTime.set(u, s, r);

  return 0;
}

// repr 関数
PyObject*
USTime_repr(USTimeObject* self)
{
  ostringstream buf;
  buf << self->mTime.usr_time_usec()
      << ", "
      << self->mTime.sys_time_usec()
      << ", "
      << self->mTime.real_time_usec();
  return PyObject_FromString(buf.str());
}

// str 関数
PyObject*
USTime_str(USTimeObject* self)
{
  ostringstream buf;
  buf << self->mTime;
  return PyObject_FromString(buf.str());
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

// user_time_usec 関数
PyObject*
USTime_user_time_usec(USTimeObject* self,
		      PyObject* args)
{
  return PyObject_FromDouble(self->mTime.usr_time_usec());
}

// system_time_usec 関数
PyObject*
USTime_system_time_usec(USTimeObject* self,
			PyObject* args)
{
  return PyObject_FromDouble(self->mTime.sys_time_usec());
}

// real_time_usec 関数
PyObject*
USTime_real_time_usec(USTimeObject* self,
		      PyObject* args)
{
  return PyObject_FromDouble(self->mTime.real_time_usec());
}

// user_time 関数
PyObject*
USTime_user_time(USTimeObject* self,
		 PyObject* args)
{
  return PyObject_FromDouble(self->mTime.usr_time());
}

// system_time 関数
PyObject*
USTime_system_time(USTimeObject* self,
		   PyObject* args)
{
  return PyObject_FromDouble(self->mTime.sys_time());
}

// real_time 関数
PyObject*
USTime_real_time(USTimeObject* self,
		 PyObject* args)
{
  return PyObject_FromDouble(self->mTime.real_time());
}

// add 関数
PyObject*
USTime_add(PyObject* left,
	   PyObject* right)
{
  USTime time1 = PyUSTime_AsUSTime(left);
  // TODO: エラーメッセージを作る．

  USTime time2 = PyUSTime_AsUSTime(right);
  // TODO: エラーメッセージを作る．

  return PyUSTime_FromUSTime(time1 + time2);
}

// sub 関数
PyObject*
USTime_sub(PyObject* left,
	   PyObject* right)
{
  USTime time1 = PyUSTime_AsUSTime(left);

  USTime time2 = PyUSTime_AsUSTime(right);

  return PyUSTime_FromUSTime(time1 - time2);
}

// inplace add 関数
PyObject*
USTime_iadd(PyObject* left,
	    PyObject* right)
{
  if ( !PyUSTime_Check(left) ) {
    // TODO: エラーメッセージを作る．
    return NULL;
  }
  USTimeObject* obj1 = (USTimeObject*)left;

  USTime time2 = PyUSTime_AsUSTime(right);
  // TODO: エラーメッセージを作る．

  obj1->mTime += time2;

  Py_INCREF(left);
  return left;
}

// inplace sub 関数
PyObject*
USTime_isub(PyObject* left,
	    PyObject* right)
{
  if ( !PyUSTime_Check(left) ) {
    // TODO: エラーメッセージを作る．
    return NULL;
  }
  USTimeObject* obj1 = (USTimeObject*)left;

  USTime time2 = PyUSTime_AsUSTime(right);

  obj1->mTime -= time2;

  Py_INCREF(left);
  return left;
}


//////////////////////////////////////////////////////////////////////
// USTimeObject の NumberMethod テーブル
//////////////////////////////////////////////////////////////////////
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
  (binaryfunc)USTime_iadd,     // nb_inplace_add
  (binaryfunc)USTime_isub,     // nb_inplace_subtract
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
// USTimeObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef USTime_methods[] = {
  {"set", (PyCFunction)USTime_set, METH_VARARGS,
   PyDoc_STR("set time (double, double, double)")},
  {"user_time_usec", (PyCFunction)USTime_user_time_usec, METH_NOARGS,
   PyDoc_STR("return user-time in usec (NONE)")},
  {"system_time_usec", (PyCFunction)USTime_system_time_usec, METH_NOARGS,
   PyDoc_STR("return system-time in usec (NONE)")},
  {"real_time_usec", (PyCFunction)USTime_real_time_usec, METH_NOARGS,
   PyDoc_STR("return real-time in usec (NONE)")},
  {"user_time", (PyCFunction)USTime_user_time, METH_NOARGS,
   PyDoc_STR("return user-time in sec (NONE)")},
  {"system_time", (PyCFunction)USTime_system_time, METH_NOARGS,
   PyDoc_STR("return system-time in sec (NONE)")},
  {"real_time", (PyCFunction)USTime_real_time, METH_NOARGS,
   PyDoc_STR("return real-time in sec (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// USTimeObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyUSTime_Type = {
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
  (reprfunc)USTime_repr,      /*tp_repr*/
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


//////////////////////////////////////////////////////////////////////
// PyObject と USTime 間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief USTime から USTimeObject を生成する．
// @param[in] obj USTime オブジェクト
PyObject*
PyUSTime_FromUSTime(const USTime& obj)
{
  USTimeObject* py_obj = USTime_new(&PyUSTime_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mTime = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief PyObject から USTime を取り出す．
// @param[in] py_obj Python オブジェクト
// @return USTime を返す．
// @note 変換が失敗したら TypeError を送出し，適当な値を返す．
USTime
PyUSTime_AsUSTime(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyUSTime_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.USTime is expected");
    return USTime();
  }

  // 強制的にキャスト
  USTimeObject* time_obj = (USTimeObject*)py_obj;

  return time_obj->mTime;
}

// USTimeObject 関係の初期化を行う．
void
USTimeObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyUSTime_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "USTime", (PyObject*)&PyUSTime_Type);
}

END_NAMESPACE_YM

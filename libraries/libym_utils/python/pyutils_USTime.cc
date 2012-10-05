
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
  return Py_BuildValue("d", self->mTime.usr_time_usec());
}

// sys_time_usec 関数
PyObject*
USTime_sys_time_usec(USTimeObject* self,
		     PyObject* args)
{
  return Py_BuildValue("d", self->mTime.sys_time_usec());
}

// real_time_usec 関数
PyObject*
USTime_real_time_usec(USTimeObject* self,
		      PyObject* args)
{
  return Py_BuildValue("d", self->mTime.real_time_usec());
}

// usr_time 関数
PyObject*
USTime_usr_time(USTimeObject* self,
		PyObject* args)
{
  return Py_BuildValue("d", self->mTime.usr_time());
}

// sys_time 関数
PyObject*
USTime_sys_time(USTimeObject* self,
		PyObject* args)
{
  return Py_BuildValue("d", self->mTime.sys_time());
}

// real_time 関数
PyObject*
USTime_real_time(USTimeObject* self,
		 PyObject* args)
{
  return Py_BuildValue("d", self->mTime.real_time());
}

// add 関数
PyObject*
USTime_add(PyTypeObject* type_obj,
	   PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  if ( !PyArg_ParseTuple(args, "O!O!", &USTimeType, &obj1, &USTimeType, &obj2) ) {
    return NULL;
  }

  USTime time1;
  if ( !conv_from_pyobject(obj1, time1) ) {
    return NULL;
  }

  USTime time2;
  if ( !conv_from_pyobject(obj2, time2) ) {
    return NULL;
  }

  return conv_to_pyobject(time1 + time2);
}

// sub 関数
PyObject*
USTime_sub(PyTypeObject* type_obj,
	   PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  if ( !PyArg_ParseTuple(args, "O!O!", &USTimeType, &obj1, &USTimeType, &obj2) ) {
    return NULL;
  }

  USTime time1;
  if ( !conv_from_pyobject(obj1, time1) ) {
    return NULL;
  }

  USTime time2;
  if ( !conv_from_pyobject(obj2, time2) ) {
    return NULL;
  }

  return conv_to_pyobject(time1 - time2);
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
  {"add", (PyCFunction)USTime_add, METH_STATIC | METH_VARARGS,
   PyDoc_STR("addition (USTime, USTime)")},
  {"sub", (PyCFunction)USTime_sub, METH_STATIC | METH_VARARGS,
   PyDoc_STR("subtraction (USTime, USTime)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


// USTimeObject 用のタイプオブジェクト
PyTypeObject USTimeType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logic.USTime",            /*tp_name*/
  sizeof(USTimeObject),      /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)USTime_dealloc,    /*tp_dealloc*/
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
  0,                          /*tp_str*/
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
  USTime_methods,                /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)USTime_init,         /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)USTime_new,           /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

// @brief PyObject から USTime を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj USTime を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が PolObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   USTime& obj)
{
  // 型のチェック
  if ( !USTimeObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  USTimeObject* ustime_obj = (USTimeObject*)py_obj;

  obj = ustime_obj->mTime;

  return true;
}

// @brief USTime から PyObject を生成する．
// @param[in] obj USTime オブジェクト
PyObject*
conv_to_pyobject(const USTime& obj)
{
  USTimeObject* ustime_obj = USTime_new(&USTimeType);
  if ( ustime_obj == NULL ) {
    return NULL;
  }

  ustime_obj->mTime = obj;

  Py_INCREF(ustime_obj);
  return (PyObject*)ustime_obj;
}

END_NAMESPACE_YM_PYTHON


/// @file pyutils_StopWatch.cc
/// @brief StopWatch の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/USTime.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// StopWatch を表す型
struct StopWatchObject
{
  // Python のお約束
  PyObject_HEAD

  // StopWatch の本体
  StopWatch mStopWatch;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// StopWatchObject の生成関数
StopWatchObject*
StopWatch_new(PyTypeObject* type)
{
  StopWatchObject* self = PyObject_New(StopWatchObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mStopWatch.reset();

  return self;
}

// StopWatchObject を開放する関数
void
StopWatch_dealloc(StopWatchObject* self)
{
  PyObject_Del(self);
}

// 初期化関数
int
StopWatch_init(StopWatchObject* self,
	       PyObject* args)
{
  self->mStopWatch.reset();

  return 0;
}

// reset 関数
PyObject*
StopWatch_reset(StopWatchObject* self,
		PyObject* args)
{
  self->mStopWatch.reset();

  Py_INCREF(Py_None);
  return Py_None;
}

// start 関数
PyObject*
StopWatch_start(StopWatchObject* self,
		PyObject* args)
{
  self->mStopWatch.start();

  Py_INCREF(Py_None);
  return Py_None;
}

// stop 関数
PyObject*
StopWatch_stop(StopWatchObject* self,
	       PyObject* args)
{
  self->mStopWatch.stop();

  Py_INCREF(Py_None);
  return Py_None;
}

// time 関数
PyObject*
StopWatch_time(StopWatchObject* self,
	       PyObject* args)
{
  return conv_to_pyobject(self->mStopWatch.time());
}

// StopWatchObject のメソッドテーブル
PyMethodDef StopWatch_methods[] = {
  {"reset", (PyCFunction)StopWatch_reset, METH_NOARGS,
   PyDoc_STR("reset (NONE)")},
  {"start", (PyCFunction)StopWatch_start, METH_NOARGS,
   PyDoc_STR("start (NONE)")},
  {"stop", (PyCFunction)StopWatch_stop, METH_NOARGS,
   PyDoc_STR("stop (NONE)")},
  {"time", (PyCFunction)StopWatch_time, METH_NOARGS,
   PyDoc_STR("return current time (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


// StopWatchObject 用のタイプオブジェクト
PyTypeObject StopWatchType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.StopWatch",            /*tp_name*/
  sizeof(StopWatchObject),      /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)StopWatch_dealloc,    /*tp_dealloc*/
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
  "stopwatch",                /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  StopWatch_methods,                /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)StopWatch_init,         /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)StopWatch_new,           /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

END_NAMESPACE_YM_PYTHON

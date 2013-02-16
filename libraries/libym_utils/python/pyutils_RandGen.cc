
/// @file pyutils_RandGen.cc
/// @brief RandGen の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// RandGen を表す型
struct RandGenObject
{
  // Python のお約束
  PyObject_HEAD

  // RandGen の本体
  RandGen* mRandGen;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// RandGenObject の生成関数
RandGenObject*
RandGen_new(PyTypeObject* type)
{
  RandGenObject* self = PyObject_New(RandGenObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mRandGen = new RandGen;

  return self;
}

// RandGenObject を開放する関数
void
RandGen_dealloc(RandGenObject* self)
{
  delete self->mRandGen;

  PyObject_Del(self);
}

// 初期化関数
int
RandGen_init(RandGenObject* self,
	     PyObject* args)
{
  ymuint32 s = 0;
  if ( !PyArg_ParseTuple(args, "|k", &s) ) {
    return -1;
  }

  if ( s != 0 ) {
    self->mRandGen->init(s);
  }

  return 0;
}

// init_by_array 関数
PyObject*
RandGen_init_by_array(RandGenObject* self,
		      PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &PyList_Type, &obj) ) {
    return NULL;
  }
  ymuint n = PyList_GET_SIZE(obj);
  ymuint32* init_key = new ymuint32[n];
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GET_ITEM(obj, i);
    ymuint32 val = 0;
    if ( !PyArg_ParseTuple(obj1, "k", &val) ) {
      return NULL;
    }
    init_key[i] = val;
  }

  self->mRandGen->init_by_array(init_key, n);

  delete [] init_key;

  Py_INCREF(Py_None);
  return Py_None;
}

// int32 関数
PyObject*
RandGen_int32(RandGenObject* self,
	      PyObject* args)
{
  return conv_to_pyobject(self->mRandGen->int32());
}

// int31 関数
PyObject*
RandGen_int31(RandGenObject* self,
	      PyObject* args)
{
  return conv_to_pyobject(self->mRandGen->int31());
}

// ulong 関数
PyObject*
RandGen_ulong(RandGenObject* self,
	      PyObject* args)
{
  return conv_to_pyobject(self->mRandGen->ulong());
}

// real1 関数
PyObject*
RandGen_real1(RandGenObject* self,
	      PyObject* args)
{
  return conv_to_pyobject(self->mRandGen->real1());
}

// real2 関数
PyObject*
RandGen_real2(RandGenObject* self,
	      PyObject* args)
{
  return conv_to_pyobject(self->mRandGen->real2());
}

// real3 関数
PyObject*
RandGen_real3(RandGenObject* self,
	      PyObject* args)
{
  return conv_to_pyobject(self->mRandGen->real3());
}

// res53 関数
PyObject*
RandGen_res53(RandGenObject* self,
	      PyObject* args)
{
  return conv_to_pyobject(self->mRandGen->res53());
}


//////////////////////////////////////////////////////////////////////
// RandGenObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef RandGen_methods[] = {
  {"init_by_array", (PyCFunction)RandGen_init_by_array, METH_VARARGS,
   PyDoc_STR("initialize by array (sequence of uint)")},
  {"int32", (PyCFunction)RandGen_int32, METH_NOARGS,
   PyDoc_STR("return int32 (NONE)")},
  {"int31", (PyCFunction)RandGen_int31, METH_NOARGS,
   PyDoc_STR("return int31 (NONE)")},
  {"ulong", (PyCFunction)RandGen_ulong, METH_NOARGS,
   PyDoc_STR("return int32 (NONE)")},
  {"real1", (PyCFunction)RandGen_real1, METH_NOARGS,
   PyDoc_STR("return real1 (NONE)")},
  {"real2", (PyCFunction)RandGen_real2, METH_NOARGS,
   PyDoc_STR("return real2 (NONE)")},
  {"real3", (PyCFunction)RandGen_real3, METH_NOARGS,
   PyDoc_STR("return real3 (NONE)")},
  {"res53", (PyCFunction)RandGen_res53, METH_NOARGS,
   PyDoc_STR("return res53 (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// RandGenObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyRandGen_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.RandGen",             // tp_name
  sizeof(RandGenObject),       // tp_basicsize
  0,                           // tp_itemsize
  /* methods */
  (destructor)RandGen_dealloc, // tp_dealloc
  0,                           // tp_print
  0,                           // tp_getattr
  0,                           // tp_setattr
  0,                           // tp_compare
  0,                           // tp_repr
  0,                           // tp_as_number
  0,                           // tp_as_sequence
  0,                           // tp_as_mapping
  0,                           // tp_hash
  0,                           // tp_call
  0,                           // tp_str
  0,                           // tp_getattro
  0,                           // tp_setattro
  0,                           // tp_as_buffer
  Py_TPFLAGS_DEFAULT,          // tp_flags
  "random number generator",   // tp_doc
  0,                           // tp_traverse
  0,                           // tp_clear
  0,                           // tp_richcompare
  0,                           // tp_weaklistoffset
  0,                           // tp_iter
  0,                           // tp_iternext
  RandGen_methods,             // tp_methods
  0,                           // tp_members
  0,                           // tp_getset
  0,                           // tp_base
  0,                           // tp_dict
  0,                           // tp_descr_get
  0,                           // tp_descr_set
  0,                           // tp_dictoffset
  (initproc)RandGen_init,      // tp_init
  0,                           // tp_alloc
  (newfunc)RandGen_new,        // tp_new
  0,                           // tp_free
  0,                           // tp_is_gc
};


//////////////////////////////////////////////////////////////////////
// PyObject と RandGen の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から RandGen を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj RandGen を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した． py_obj が RandGenObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   RandGen*& obj)
{
  // 型のチェック
  if ( !RandGenObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  RandGenObject* my_obj = (RandGenObject*)py_obj;

  obj = my_obj->mRandGen;

  return true;
}

// RandGenObject 関係の初期化を行う．
void
RandGenObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyRandGen_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "RandGen", (PyObject*)&PyRandGen_Type);
}

END_NAMESPACE_YM

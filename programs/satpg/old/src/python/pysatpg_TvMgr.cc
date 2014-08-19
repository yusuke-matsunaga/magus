
/// @file pysatpg_TvMgr.cc
/// @brief TvMgr の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "pysatpg.h"
#include "TvMgr.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// TvMgr を表す型
struct TvMgrObject
{
  // Python のお約束
  PyObject_HEAD

  // TvMgr のポインタ
  TvMgr* mPtr;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// TvMgrObject の生成関数
TvMgrObject*
TvMgr_new(PyTypeObject* type)
{
  TvMgrObject* self = PyObject_New(TvMgrObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ここではポインタを NULL にしておく．
  self->mPtr = NULL;

  return self;
}

// TvMgrObject を開放する関数
void
TvMgr_dealloc(TvMgr* self)
{
  // TvMgr は開放しない．

  PyObject_Del(self);
}

// 初期化関数
int
TvMgr_init(TvMgrObject* self,
	   PyObject* args)
{
  return 0;
}

// "clear" 関数
PyObject*
TvMgr_clear(TvMgrObject* self,
	    PyObject* args)
{
  self->mPtr->clear();

  Py_INCREF(Py_None);
  return Py_None;
}

// "init" 関数
PyObject*
TvMgr_init_func(TvMgrObject* self,
		PyObject* args)
{
  ymuint ni = 0;
  if ( !PyArg_ParseTuple(args, "l", &ni) ) {
    return NULL;
  }

  self->mPtr->init(ni);

  Py_INCREF(Py_None);
  return Py_None;
}

// "new_vector" 関数
PyObject*
TvMgr_new_vector(TvMgrObject* self,
		 PyObject* args)
{
  TestVector* tv = self->mPtr->new_vector();

  return PyTestVector_FromTestVector(tv);
}

// "delete_vector" 関数
PyObject*
TvMgr_delete_vector(TvMgrObject* self,
		    PyObject* args)
{
  PyObject* obj1 = NULL;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyTestVector_Type, &obj1) ) {
    return NULL;
  }

  TestVector* tv = PyTestVector_AsTestVectorPtr(obj1);
  assert_cond( tv != NULL, __FILE__, __LINE__);

  self->mPtr->delete_vector(tv);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// TvMgrObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef TvMgr_methods[] = {
  {"clear", (PyCFunction)TvMgr_clear, METH_NOARGS,
   PyDoc_STR("clear (NONE)")},
  {"init", (PyCFunction)TvMgr_init_func, METH_VARARGS,
   PyDoc_STR("initialize (int: number of inputs)")},
  {"new_vector", (PyCFunction)TvMgr_new_vector, METH_NOARGS,
   PyDoc_STR("new vector (NONE)")},
  {"delete_vector", (PyCFunction)TvMgr_delete_vector, METH_VARARGS,
   PyDoc_STR("delete vector (TestVector: vector to be deleted)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// TvMgr 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyTvMgr_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "satpg.TvMgr",                // tp_name
  sizeof(TvMgrObject),          // tp_basicsize
  0,                            // tp_itemsize
  /* methods */
  (destructor)TvMgr_dealloc,    // tp_dealloc
  0,                            // tp_print
  0,                            // tp_getattr
  0,                            // tp_setattr
  0,                            // tp_compare
  0,                            // tp_repr
  0,                            // tp_as_number
  0,                            // tp_as_sequence
  0,                            // tp_as_mapping
  0,                            // tp_hash
  0,                            // tp_call
  (reprfunc)0,                  // tp_str
  0,                            // tp_getattro
  0,                            // tp_setattro
  0,                            // tp_as_buffer
  Py_TPFLAGS_DEFAULT,           // tp_flags
  "test vector manager",        // tp_doc
  0,                            // tp_traverse
  0,                            // tp_clear
  0,                            // tp_richcompare
  0,                            // tp_weaklistoffset
  0,                            // tp_iter
  0,                            // tp_iternext
  TvMgr_methods,                // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)TvMgr_init,         // tp_init
  0,                            // tp_alloc
  (newfunc)TvMgr_new,           // tp_new
  0,                            // tp_free
  0,                            // tp_is_gc
};


//////////////////////////////////////////////////////////////////////
// PyObject と TvMgr 間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief TvMgr から PyObject を生成する．
// @param[in] obj TvMgr オブジェクト
PyObject*
PyTvMgr_FromTvMgr(TvMgr* obj)
{
  TvMgrObject* py_obj = TvMgr_new(&PyTvMgr_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mPtr = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief PyObject から TvMgr へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return TvMgr へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
TvMgr*
PyTvMgr_AsTvMgrPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyTvMgr_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "satpg.TvMgr is expected");
    return NULL;
  }

  // 強制的にキャスト
  TvMgrObject* my_obj = (TvMgrObject*)py_obj;

  return my_obj->mPtr;
}

// TvMgrObject 関係の初期化を行う．
void
TvMgrObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyTvMgr_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "TvMgr", (PyObject*)&PyTvMgr_Type);
}


END_NAMESPACE_YM_SATPG

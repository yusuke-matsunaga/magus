
/// @file pysatpg_AtpgtMgr.cc
/// @brief AtpgMgr の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "pysatpg.h"
#include "AtpgMgr.h"
#include "RtpgStats.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// AtpgtMgr を表す型
struct AtpgMgrObject
{
  // Python のお約束
  PyObject_HEAD

  // AtpgMgr のポインタ
  AtpgMgr* mPtr;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// AtpgMgrObject の生成関数
AtpgMgrObject*
AtpgMgr_new(PyTypeObject* type)
{
  AtpgMgrObject* self = PyObject_New(AtpgMgrObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mPtr = new AtpgMgr();

  return self;
}

// AtpgMgrObject を開放する関数
void
AtpgMgr_dealloc(AtpgMgrObject* self)
{
  delete self->mPtr;

  PyObject_Del(self);
}

// 初期化関数
int
AtpgMgr_init(AtpgMgrObject* self,
	     PyObject* args)
{
  // なにもしない．
  return 0;
}

// read_blif 関数
PyObject*
AtpgMgr_read_blif(AtpgMgrObject* self,
		  PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "s", &str) ) {
    return NULL;
  }

  bool stat = self->mPtr->read_blif(str);
  if ( !stat ) {
    ostringstream buf;
    buf << "Error in reading " << str;
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return NULL;
  }

  Py_INCREF(Py_None);
  return Py_None;
}

// read_iscas89 関数
PyObject*
AtpgMgr_read_iscas89(AtpgMgrObject* self,
		     PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "s", &str) ) {
    return NULL;
  }

  bool stat = self->mPtr->read_iscas89(str);
  if ( !stat ) {
    ostringstream buf;
    buf << "Error in reading " << str;
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return NULL;
  }

  Py_INCREF(Py_None);
  return Py_None;
}

// rtpg 関数
PyObject*
AtpgMgr_rtpg(AtpgMgrObject* self,
	     PyObject* args)
{
  int max_pat = 100000;
  int max_i   = 4;
  int min_f   = 0;
  if ( !PyArg_ParseTuple(args, "iii", &min_f, &max_i, &max_pat) ) {
    return NULL;
  }

  RtpgStats stats;
  self->mPtr->rtpg(min_f, max_i, max_pat, stats);

  // TODO: stats の内容を PyObject 化して返り値とする．

  Py_INCREF(Py_None);
  return Py_None;
}

// dtpg 関数
PyObject*
AtpgMgr_dtpg(AtpgMgrObject* self,
	     PyObject* args)
{

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// AtpgMgrObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef AtpgMgr_methods[] = {
  {"read_blif", (PyCFunction)AtpgMgr_read_blif, METH_VARARGS,
   PyDoc_STR("read BLIF file (string: filename)")},
  {"read_iscas89", (PyCFunction)AtpgMgr_read_iscas89, METH_VARARGS,
   PyDoc_STR("read ISCAS89(.bench) file (string: filename)")},
  {"rtpg", (PyCFunction)AtpgMgr_rtpg, METH_VARARGS,
   PyDoc_STR("do Random Test Pattern Generation (misc opts)")},
  {"dtpg", (PyCFunction)AtpgMgr_dtpg, METH_VARARGS,
   PyDoc_STR("do Deterministic Test Pattern Generation (misc opts)")},
  {NULL, NULL, 0, NULL}
};


END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// AtpgMgr 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyAtpgMgr_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "satpg.AtpgMgr",             // tp_name
  sizeof(AtpgMgrObject),       // tp_basicsize
  0,                            // tp_itemsize
  /* methods */
  (destructor)AtpgMgr_dealloc, // tp_dealloc
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
  "ATPG manager",              // tp_doc
  0,                            // tp_traverse
  0,                            // tp_clear
  0,                            // tp_richcompare
  0,                            // tp_weaklistoffset
  0,                            // tp_iter
  0,                            // tp_iternext
  AtpgMgr_methods,             // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)AtpgMgr_init,      // tp_init
  0,                            // tp_alloc
  (newfunc)AtpgMgr_new,        // tp_new
  0,                            // tp_free
  0,                            // tp_is_gc
};


//////////////////////////////////////////////////////////////////////
// PyObject と AtpgMgr 間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から AtpgMgr へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return AtpgMgr へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
AtpgMgr*
PyAtpgMgr_AsAtpgMgrPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyAtpgMgr_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "satpg.AtpgMgr is expected");
    return NULL;
  }

  // 強制的にキャスト
  AtpgMgrObject* my_obj = (AtpgMgrObject*)py_obj;

  return my_obj->mPtr;
}

// AtpgMgrObject 関係の初期化を行う．
void
AtpgMgrObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyAtpgMgr_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "AtpgMgr", (PyObject*)&PyAtpgMgr_Type);
}


END_NAMESPACE_YM_SATPG

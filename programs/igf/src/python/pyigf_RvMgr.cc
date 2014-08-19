
/// @file pyigf_RvMgr.cc
/// @brief RvMgr の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "pyigf.h"
#include "RvMgr.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// RvMgr を表す型
struct RvMgrObject
{
  // Python のお約束
  PyObject_HEAD

  // RvMgr の本体
  RvMgr* mBody;

  // mVectArray のサイズ
  ymuint32 mVectNum;

  // RegVect を表すオブジェクトの配列
  PyObject** mVectArray;

};

// mVectArray をクリアする．
void
clear_vect_array(RvMgrObject* self)
{
  for (ymuint i = 0; i < self->mVectNum; ++ i) {
    Py_DECREF(self->mVectArray[i]);
  }
  delete [] self->mVectArray;

  self->mVectNum = 0;
  self->mVectArray = NULL;
}

//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// RvMgrObject の生成関数
RvMgrObject*
RvMgr_new(PyTypeObject* type)
{
  RvMgrObject* self = PyObject_New(RvMgrObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mBody = new RvMgr;

  self->mVectNum = 0;
  self->mVectArray = NULL;

  return self;
}

// RvMgrObject を開放する関数
void
RvMgr_dealloc(RvMgrObject* self)
{
  clear_vect_array(self);

  // RvMgr の開放を行なう．
  delete self->mBody;

  PyObject_Del(self);
}

// read_data 関数
PyObject*
RvMgr_read_data(RvMgrObject* self,
		PyObject* args)
{
  const char* str = NULL;
  if ( !PyArg_ParseTuple(args, "s", &str) ) {
    return NULL;
  }

  ifstream is(str);
  if ( !is ) {
    PyErr_SetString(PyExc_ValueError, "No such file");
    return NULL;
  }

  clear_vect_array(self);

  bool stat = self->mBody->read_data(is);
  if ( stat ) {
    const vector<const RegVect*>& vect_list = self->mBody->vect_list();
    self->mVectNum = vect_list.size();
    self->mVectArray = new PyObject*[self->mVectNum];
    for (ymuint i = 0; i < self->mVectNum; ++ i) {
      PyObject* obj = PyRegVect_FromRegVect(vect_list[i]);
      self->mVectArray[i] = obj;
    }
  }

  return PyObject_FromBool(stat);
}

// vect_size 関数
PyObject*
RvMgr_vect_size(RvMgrObject* self,
		PyObject* args)
{
  return PyObject_FromYmuint32(self->mBody->vect_size());
}

// vect_list 関数
PyObject*
RvMgr_vect_list(RvMgrObject* self,
		PyObject* args)
{
  PyObject* vect_list = PyList_New(self->mVectNum);
  for (ymuint i = 0; i < self->mVectNum; ++ i) {
    PyObject* obj = self->mVectArray[i];
    Py_INCREF(obj);
    PyList_SetItem(vect_list, i, obj);
  }

  return vect_list;
}

// index_size 関数
PyObject*
RvMgr_index_size(RvMgrObject* self,
		 PyObject* args)
{
  return PyObject_FromYmuint32(self->mBody->index_size());
}


//////////////////////////////////////////////////////////////////////
// RvMgrObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef RvMgr_methods[] = {
  // PyMethodDef のフィールド
  //   char*       ml_name;
  //   PyCFunction ml_meth;
  //   int         ml_flags;
  //   char*       ml_doc;
  // ml_flags で使用可能なマクロは以下の通り
  //  - METH_VARARGS
  //  - METH_KEYWORDS
  //  - METH_NOARGS
  //  - METH_O
  //  - METH_OLDARGS (obsolete)
  //  - METH_CLASS
  //  - METH_STATIC
  //  - METH_COEXIST
  {"read_data", (PyCFunction)RvMgr_read_data, METH_VARARGS,
   PyDoc_STR("read data (str)")},
  {"vect_size", (PyCFunction)RvMgr_vect_size, METH_NOARGS,
   PyDoc_STR("return vector size (none)")},
  {"vect_list", (PyCFunction)RvMgr_vect_list, METH_NOARGS,
   PyDoc_STR("return list of vectors (none)")},
  {"index_size", (PyCFunction)RvMgr_index_size, METH_NOARGS,
   PyDoc_STR("return index size (none)")},
  {NULL, NULL, 0, NULL} // end-marker
};


END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// RvMgrObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyRvMgr_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "igf.RvMgr",                // tp_name
  sizeof(RvMgrObject),          // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)RvMgr_dealloc,    // tp_dealloc
  (printfunc)0,                 // tp_print
  (getattrfunc)0,               // tp_getattr
  (setattrfunc)0,               // tp_setattr
  (cmpfunc)0,                   // tp_compare
  (reprfunc)0,                  // tp_repr

  // Method suites for standard classes
  0,                            // tp_as_number
  0,                            // tp_as_sequence
  0,                            // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                  // tp_hash
  (ternaryfunc)0,               // tp_call
  (reprfunc)0,                  // tp_str
  (getattrofunc)0,              // tp_getattro
  (setattrofunc)0,              // tp_setattro

  // Functions to access object as input/output buffer
  0,                            // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,           // tp_flags

  // Documentation string
  "Registered Vector Manager",             // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,              // tp_traverse

  // delete references to contained objects
  (inquiry)0,                   // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,               // tp_richcompare

  // weak reference enabler
  (long)0,                      // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,               // tp_iter
  (iternextfunc)0,              // tp_iternext

  // Attribute descriptor and subclassing stuff
  RvMgr_methods,                // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)0,         // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)RvMgr_new,           // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と RvMgr の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から RvMgr へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return RvMgr へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
RvMgr*
PyRvMgr_AsRvMgrPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyRvMgr_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "igf.RvMgr is expected");
    return NULL;
  }

  // 強制的にキャスト
  RvMgrObject* my_obj = (RvMgrObject*)py_obj;

  return my_obj->mBody;
}

// RvMgrObject 関係の初期化を行う．
void
RvMgrObject_init(PyObject* m)
{
  if ( PyType_Ready(&PyRvMgr_Type) < 0 ) {
    return;
  }

  // タイプモジュールの登録を行う．
  PyModule_AddObject(m, "RvMgr", (PyObject*)&PyRvMgr_Type);
}

END_NAMESPACE_YM_IGF

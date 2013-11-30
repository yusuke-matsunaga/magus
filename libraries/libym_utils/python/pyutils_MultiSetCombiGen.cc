
/// @file pyutils_MultiSetCombiGen.cc
/// @brief MultiSetCombiGen の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/MultiSetCombiGen.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// MultiSetCombiGen を表す型
struct MultiSetCombiGenObject
{
  // Python のお約束
  PyObject_HEAD

  // MultiSetCombiGen の本体
  MultiSetCombiGen* mPtr;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// MultiSetCombiGenObject の生成関数
MultiSetCombiGenObject*
MultiSetCombiGen_new(PyTypeObject* type)
{
  MultiSetCombiGenObject* self = PyObject_New(MultiSetCombiGenObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mPtr = NULL;

  return self;
}

// MultiSetCombiGenObject を開放する関数
void
MultiSetCombiGen_dealloc(MultiSetCombiGenObject* self)
{
  delete self->mPtr;

  PyObject_Del(self);
}

// 初期化関数
int
MultiSetCombiGen_init(MultiSetCombiGenObject* self,
		      PyObject* args)
{
  PyObject* obj = NULL;
  ymuint k = 0;
  if ( !PyArg_ParseTuple(args, "OI", &obj, &k) ) {
    return -1;
  }

  if ( PyTuple_Check(obj) == 0 ) {
    PyErr_SetString(PyExc_TypeError, "tuple type is expected.");
    return -1;
  }

  ymuint ng = PyTuple_Size(obj);
  vector<ymuint> num_array(ng);
  for (ymuint i = 0; i < ng; ++ i) {
    PyObject* obj1 = PyTuple_GetItem(obj, i);
    if ( !PyNumber_Check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "integer type is expected.");
      return -1;
    }
    ymuint n = PyLong_AsLong(obj1);
    num_array[i] = n;
  }

  delete self->mPtr;
  self->mPtr = new MultiSetCombiGen(num_array, k);

  return 0;
}

// group_num() 関数
PyObject*
MultiSetCombiGen_group_num(MultiSetCombiGenObject* self,
			   PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->group_num());
}

// n(grp) 関数
PyObject*
MultiSetCombiGen_n(MultiSetCombiGenObject* self,
		   PyObject* args)
{
  ymuint grp = 0;
  if ( !PyArg_ParseTuple(args, "I", &grp) ) {
    return NULL;
  }

  return PyObject_FromYmuint32(self->mPtr->n(grp));
}

// k() 関数
PyObject*
MultiSetCombiGen_k(MultiSetCombiGenObject* self,
		   PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->k());
}

// reset() 関数
PyObject*
MultiSetCombiGen_reset(MultiSetCombiGenObject* self,
		       PyObject* args)
{
  self->mPtr->init();

  Py_INCREF(Py_None);
  return Py_None;
}

// get() 関数
PyObject*
MultiSetCombiGen_get(MultiSetCombiGenObject* self,
		     PyObject* args)
{
  ymuint k = self->mPtr->k();
  PyObject* list_obj = PyList_New(k);
  for (ymuint i = 0; i < k; ++ i) {
    ymuint v = (*self->mPtr)(i);
    PyObject* obj1 = PyObject_FromYmuint32(v);
    PyList_SetItem(list_obj, i, obj1);
  }

  return list_obj;
}

// is_end() 関数
PyObject*
MultiSetCombiGen_is_end(MultiSetCombiGenObject* self,
			PyObject* args)
{
  return PyObject_FromBool(self->mPtr->is_end());
}

// next() 関数
PyObject*
MultiSetCombiGen_next(MultiSetCombiGenObject* self,
		      PyObject* args)
{
  ++ (*self->mPtr);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// MultiSetCombiGenObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef MultiSetCombiGen_methods[] = {
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

  {"group_num", (PyCFunction)MultiSetCombiGen_group_num, METH_NOARGS,
   PyDoc_STR("return group number")},
  {"n", (PyCFunction)MultiSetCombiGen_n, METH_VARARGS,
   PyDoc_STR("return N(grp)")},
  {"k", (PyCFunction)MultiSetCombiGen_k, METH_NOARGS,
   PyDoc_STR("return K(grp)")},
  {"reset", (PyCFunction)MultiSetCombiGen_reset, METH_NOARGS,
   PyDoc_STR("reset")},
  {"get", (PyCFunction)MultiSetCombiGen_get, METH_NOARGS,
   PyDoc_STR("get current combination")},
  {"is_end", (PyCFunction)MultiSetCombiGen_is_end, METH_NOARGS,
   PyDoc_STR("return TRUE if end-of-combination")},
  {"next", (PyCFunction)MultiSetCombiGen_next, METH_NOARGS,
   PyDoc_STR("move to next combination")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// MultiSetCombiGenObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyMultiSetCombiGen_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.MultiSetCombiGen",        // tp_name
  sizeof(MultiSetCombiGenObject),  // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)MultiSetCombiGen_dealloc, // tp_dealloc
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
  "Multi-Set Combination Generator",      // tp_doc

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
  MultiSetCombiGen_methods,        // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)MultiSetCombiGen_init,      // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)MultiSetCombiGen_new,        // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と MultiSetCombiGen の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から MultiSetCombiGen へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return MultiSetCombiGen へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
MultiSetCombiGen*
PyMultiSetCombiGen_AsMultiSetCombiGenPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyMultiSetCombiGen_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.MultiSetCombiGen is expected");
    return NULL;
  }

  // 強制的にキャスト
  MultiSetCombiGenObject* my_obj = (MultiSetCombiGenObject*)py_obj;

  return my_obj->mPtr;
}

// MultiSetCombiGenObject 関係の初期化を行なう．
void
MultiSetCombiGenObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyMultiSetCombiGen_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "MultiSetCombiGen", (PyObject*)&PyMultiSetCombiGen_Type);
}

END_NAMESPACE_YM



/// @file pyutils_MultiCombiGen.cc
/// @brief MultiCombiGen の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/MultiCombiGen.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// MultiCombiGen を表す型
struct MultiCombiGenObject
{
  // Python のお約束
  PyObject_HEAD

  // MultiCombiGen の本体
  MultiCombiGen* mPtr;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// MultiCombiGenObject の生成関数
MultiCombiGenObject*
MultiCombiGen_new(PyTypeObject* type)
{
  MultiCombiGenObject* self = PyObject_New(MultiCombiGenObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mPtr = NULL;

  return self;
}

// MultiCombiGenObject を開放する関数
void
MultiCombiGen_dealloc(MultiCombiGenObject* self)
{
  delete self->mPtr;

  PyObject_Del(self);
}

// 初期化関数
int
MultiCombiGen_init(MultiCombiGenObject* self,
		   PyObject* args)
{
  if ( PyTuple_Check(args) == 0 ) {
    PyErr_SetString(PyExc_TypeError, "sequence type is expected.");
    return -1;
  }

  ymuint ng = PyTuple_Size(args);
  vector<pair<ymuint, ymuint> > nk_array(ng);
  for (ymuint i = 0; i < ng; ++ i) {
    PyObject* obj1 = PyTuple_GetItem(args, i);
    ymuint n = 0;
    ymuint k = 0;
    if ( !PyArg_ParseTuple(obj1, "II", &n, &k) ) {
      return -1;
    }
    nk_array[i] = make_pair(n, k);
  }

  delete self->mPtr;
  self->mPtr = new MultiCombiGen(nk_array);

  return 0;
}

// group_num() 関数
PyObject*
MultiCombiGen_group_num(MultiCombiGenObject* self,
			PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->group_num());
}

// n(grp) 関数
PyObject*
MultiCombiGen_n(MultiCombiGenObject* self,
		PyObject* args)
{
  ymuint grp = 0;
  if ( !PyArg_ParseTuple(args, "I", &grp) ) {
    return NULL;
  }

  return PyObject_FromYmuint32(self->mPtr->n(grp));
}

// k(grp) 関数
PyObject*
MultiCombiGen_k(MultiCombiGenObject* self,
		PyObject* args)
{
  ymuint grp = 0;
  if ( !PyArg_ParseTuple(args, "I", &grp) ) {
    return NULL;
  }

  return PyObject_FromYmuint32(self->mPtr->k(grp));
}

// reset() 関数
PyObject*
MultiCombiGen_reset(MultiCombiGenObject* self,
		    PyObject* args)
{
  self->mPtr->init();

  Py_INCREF(Py_None);
  return Py_None;
}

// get() 関数
PyObject*
MultiCombiGen_get(MultiCombiGenObject* self,
		  PyObject* args)
{
  ymuint ng = self->mPtr->group_num();
  PyObject* list_obj = PyList_New(ng);
  for (ymuint g = 0; g < ng; ++ g) {
    ymuint k = self->mPtr->k(g);
    PyObject* list_obj1 = PyList_New(k);
    for (ymuint i = 0; i < k; ++ i) {
      ymuint v = (*self->mPtr)(g, i);
      PyObject* obj1 = PyObject_FromYmuint32(v);
      PyList_SetItem(list_obj1, i, obj1);
    }
    PyList_SetItem(list_obj, g, list_obj1);
  }
  return list_obj;
}

// is_end() 関数
PyObject*
MultiCombiGen_is_end(MultiCombiGenObject* self,
		     PyObject* args)
{
  return PyObject_FromBool(self->mPtr->is_end());
}

// next() 関数
PyObject*
MultiCombiGen_next(MultiCombiGenObject* self,
		   PyObject* args)
{
  ++ (*self->mPtr);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// MultiCombiGenObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef MultiCombiGen_methods[] = {
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

  {"group_num", (PyCFunction)MultiCombiGen_group_num, METH_NOARGS,
   PyDoc_STR("return group number")},
  {"n", (PyCFunction)MultiCombiGen_n, METH_VARARGS,
   PyDoc_STR("return N(grp)")},
  {"k", (PyCFunction)MultiCombiGen_k, METH_VARARGS,
   PyDoc_STR("return K(grp)")},
  {"reset", (PyCFunction)MultiCombiGen_reset, METH_NOARGS,
   PyDoc_STR("reset")},
  {"get", (PyCFunction)MultiCombiGen_get, METH_NOARGS,
   PyDoc_STR("get current combination")},
  {"is_end", (PyCFunction)MultiCombiGen_is_end, METH_NOARGS,
   PyDoc_STR("return TRUE if end-of-combination")},
  {"next", (PyCFunction)MultiCombiGen_next, METH_NOARGS,
   PyDoc_STR("move to next combination")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// MultiCombiGenObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyMultiCombiGen_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.MultiCombiGen",        // tp_name
  sizeof(MultiCombiGenObject),  // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)MultiCombiGen_dealloc, // tp_dealloc
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
  "Combination Array Generator",      // tp_doc

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
  MultiCombiGen_methods,        // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)MultiCombiGen_init,      // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)MultiCombiGen_new,        // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と MultiCombiGen の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から MultiCombiGen へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return MultiCombiGen へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
MultiCombiGen*
PyMultiCombiGen_AsMultiCombiGenPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyMultiCombiGen_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.MultiCombiGen is expected");
    return NULL;
  }

  // 強制的にキャスト
  MultiCombiGenObject* my_obj = (MultiCombiGenObject*)py_obj;

  return my_obj->mPtr;
}

// MultiCombiGenObject 関係の初期化を行なう．
void
MultiCombiGenObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyMultiCombiGen_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "MultiCombiGen", (PyObject*)&PyMultiCombiGen_Type);
}

END_NAMESPACE_YM


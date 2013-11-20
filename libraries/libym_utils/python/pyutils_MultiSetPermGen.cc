
/// @file pyutils_MultiSetPermGen.cc
/// @brief MultiSetPermGen の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/MultiSetPermGen.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// MultiSetPermGen を表す型
struct MultiSetPermGenObject
{
  // Python のお約束
  PyObject_HEAD

  // MultiSetPermGen の本体
  MultiSetPermGen* mPtr;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// MultiSetPermGenObject の生成関数
MultiSetPermGenObject*
MultiSetPermGen_new(PyTypeObject* type)
{
  MultiSetPermGenObject* self = PyObject_New(MultiSetPermGenObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mPtr = NULL;

  return self;
}

// MultiSetPermGenObject を開放する関数
void
MultiSetPermGen_dealloc(MultiSetPermGenObject* self)
{
  delete self->mPtr;

  PyObject_Del(self);
}

// 初期化関数
int
MultiSetPermGen_init(MultiSetPermGenObject* self,
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
  self->mPtr = new MultiSetPermGen(num_array, k);

  return 0;
}

// group_num() 関数
PyObject*
MultiSetPermGen_group_num(MultiSetPermGenObject* self,
			   PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->group_num());
}

// n(grp) 関数
PyObject*
MultiSetPermGen_n(MultiSetPermGenObject* self,
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
MultiSetPermGen_k(MultiSetPermGenObject* self,
		   PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->k());
}

// reset() 関数
PyObject*
MultiSetPermGen_reset(MultiSetPermGenObject* self,
		       PyObject* args)
{
  self->mPtr->init();

  Py_INCREF(Py_None);
  return Py_None;
}

// get() 関数
PyObject*
MultiSetPermGen_get(MultiSetPermGenObject* self,
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
MultiSetPermGen_is_end(MultiSetPermGenObject* self,
			PyObject* args)
{
  return PyObject_FromBool(self->mPtr->is_end());
}

// next() 関数
PyObject*
MultiSetPermGen_next(MultiSetPermGenObject* self,
		      PyObject* args)
{
  ++ (*self->mPtr);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// MultiSetPermGenObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef MultiSetPermGen_methods[] = {
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

  {"group_num", (PyCFunction)MultiSetPermGen_group_num, METH_NOARGS,
   PyDoc_STR("return group number")},
  {"n", (PyCFunction)MultiSetPermGen_n, METH_VARARGS,
   PyDoc_STR("return N(grp)")},
  {"k", (PyCFunction)MultiSetPermGen_k, METH_NOARGS,
   PyDoc_STR("return K(grp)")},
  {"reset", (PyCFunction)MultiSetPermGen_reset, METH_NOARGS,
   PyDoc_STR("reset")},
  {"get", (PyCFunction)MultiSetPermGen_get, METH_NOARGS,
   PyDoc_STR("get current combination")},
  {"is_end", (PyCFunction)MultiSetPermGen_is_end, METH_NOARGS,
   PyDoc_STR("return TRUE if end-of-combination")},
  {"next", (PyCFunction)MultiSetPermGen_next, METH_NOARGS,
   PyDoc_STR("move to next combination")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// MultiSetPermGenObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyMultiSetPermGen_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.MultiSetPermGen",        // tp_name
  sizeof(MultiSetPermGenObject),  // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)MultiSetPermGen_dealloc, // tp_dealloc
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
  "Multi-Set Permnation Generator",      // tp_doc

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
  MultiSetPermGen_methods,        // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)MultiSetPermGen_init,      // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)MultiSetPermGen_new,        // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と MultiSetPermGen の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から MultiSetPermGen へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return MultiSetPermGen へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
MultiSetPermGen*
PyMultiSetPermGen_AsMultiSetPermGenPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyMultiSetPermGen_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.MultiSetPermGen is expected");
    return NULL;
  }

  // 強制的にキャスト
  MultiSetPermGenObject* my_obj = (MultiSetPermGenObject*)py_obj;

  return my_obj->mPtr;
}

// MultiSetPermGenObject 関係の初期化を行なう．
void
MultiSetPermGenObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyMultiSetPermGen_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "MultiSetPermGen", (PyObject*)&PyMultiSetPermGen_Type);
}

END_NAMESPACE_YM


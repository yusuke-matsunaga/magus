
/// @file pyutils_PermGen.cc
/// @brief PermGen の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/PermGen.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// PermGen を表す型
struct PermGenObject
{
  // Python のお約束
  PyObject_HEAD

  // PermGen の本体
  PermGen* mPtr;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// PermGenObject の生成関数
PermGenObject*
PermGen_new(PyTypeObject* type)
{
  PermGenObject* self = PyObject_New(PermGenObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mPtr = NULL;

  return self;
}

// PermGenObject を開放する関数
void
PermGen_dealloc(PermGenObject* self)
{
  delete self->mPtr;

  PyObject_Del(self);
}

// 初期化関数
int
PermGen_init(PermGenObject* self,
	     PyObject* args)
{
  ymuint n = 0;
  ymuint k = 0;
  if ( !PyArg_ParseTuple(args, "II", &n, &k) ) {
    return -1;
  }

  delete self->mPtr;
  self->mPtr = new PermGen(n, k);

  return 0;
}

// n() 関数
PyObject*
PermGen_n(PermGenObject* self,
	  PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->n());
}

// k() 関数
PyObject*
PermGen_k(PermGenObject* self,
	  PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->k());
}

// reset() 関数
PyObject*
PermGen_reset(PermGenObject* self,
	      PyObject* args)
{
  self->mPtr->init();

  Py_INCREF(Py_None);
  return Py_None;
}

// get() 関数
PyObject*
PermGen_get(PermGenObject* self,
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
PermGen_is_end(PermGenObject* self,
	       PyObject* args)
{
  return PyObject_FromBool(self->mPtr->is_end());
}

// next() 関数
PyObject*
PermGen_next(PermGenObject* self,
	     PyObject* args)
{
  ++ (*self->mPtr);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// PermGenObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef PermGen_methods[] = {
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

  {"n", (PyCFunction)PermGen_n, METH_NOARGS,
   PyDoc_STR("return N")},
  {"k", (PyCFunction)PermGen_k, METH_NOARGS,
   PyDoc_STR("return K")},
  {"reset", (PyCFunction)PermGen_reset, METH_NOARGS,
   PyDoc_STR("reset")},
  {"get", (PyCFunction)PermGen_get, METH_NOARGS,
   PyDoc_STR("get current permutation")},
  {"is_end", (PyCFunction)PermGen_is_end, METH_NOARGS,
   PyDoc_STR("return TRUE if end-of-permutation")},
  {"next", (PyCFunction)PermGen_next, METH_NOARGS,
   PyDoc_STR("move to next permutation")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// PermGenObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyPermGen_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.PermGen",             // tp_name
  sizeof(PermGenObject),       // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)PermGen_dealloc, // tp_dealloc
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
  "Permutation Generator",      // tp_doc

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
  PermGen_methods,              // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)PermGen_init,       // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)PermGen_new,         // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と PermGen の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から PermGen へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return PermGen へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
PermGen*
PyPermGen_AsPermGenPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyPermGen_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.PermGen is expected");
    return NULL;
  }

  // 強制的にキャスト
  PermGenObject* my_obj = (PermGenObject*)py_obj;

  return my_obj->mPtr;
}

// PermGenObject 関係の初期化を行なう．
void
PermGenObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyPermGen_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "PermGen", (PyObject*)&PyPermGen_Type);
}

END_NAMESPACE_YM


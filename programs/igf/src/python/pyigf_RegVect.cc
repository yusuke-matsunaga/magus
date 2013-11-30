
/// @file pyigf_RegVect.cc
/// @brief RegVect の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "pyigf.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// RegVect を表す型
struct RegVectObject
{
  // Python のお約束
  PyObject_HEAD

  // RegVect の本体
  const RegVect* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// RegVectObject を開放する関数
void
RegVect_dealloc(RegVectObject* self)
{
  // RegVect の開放は行わない．

  PyObject_Del(self);
}

// size 関数
PyObject*
RegVect_size(RegVectObject* self,
	     PyObject* args)
{
  return PyObject_FromYmuint32(self->mBody->size());
}

// index 関数
PyObject*
RegVect_index(RegVectObject* self,
	      PyObject* args)
{
  return PyObject_FromYmuint32(self->mBody->index());
}

// val 関数
PyObject*
RegVect_val(RegVectObject* self,
	    PyObject* args)
{
  const RegVect* rv = self->mBody;
  ymuint size = rv->size();
  PyObject* ans = PyList_New(size);
  for (ymuint i = 0; i < size; ++ i) {
    PyObject* obj = PyObject_FromYmuint32(rv->val(i));
    PyList_SetItem(ans, i, obj);
  }

  return ans;
}



//////////////////////////////////////////////////////////////////////
// RegVectObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef RegVect_methods[] = {
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
  {"size", (PyCFunction)RegVect_size, METH_NOARGS,
   PyDoc_STR("return size() (none)")},
  {"index", (PyCFunction)RegVect_index, METH_NOARGS,
   PyDoc_STR("return index() (none)")},
  {"val", (PyCFunction)RegVect_val, METH_NOARGS,
   PyDoc_STR("return val() (none)")},
  {NULL, NULL, 0, NULL} // end-marker
};


END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// RegVectObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyRegVect_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "igf.RegVect",                // tp_name
  sizeof(RegVectObject),          // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)RegVect_dealloc,    // tp_dealloc
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
  "Registered Vector",             // tp_doc

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
  RegVect_methods,                // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)0,         // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)0,           // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と RegVect の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief RegVect から Python オブジェクトを生成する．
// @param[in] obj RegVect オブジェクト
// @return 変換した Python オブジェクトを返す．
PyObject*
PyRegVect_FromRegVect(const RegVect* obj)
{
  RegVectObject* self = PyObject_New(RegVectObject, &PyRegVect_Type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mBody = obj;

  return (PyObject*)self;
}

// @brief PyObject から RegVect へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return RegVect へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
const RegVect*
PyRegVect_AsRegVectPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyRegVect_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "igf.RegVect is expected");
    return NULL;
  }

  // 強制的にキャスト
  RegVectObject* my_obj = (RegVectObject*)py_obj;

  return my_obj->mBody;
}

// RegVectObject 関係の初期化を行う．
void
RegVectObject_init(PyObject* m)
{
  if ( PyType_Ready(&PyRegVect_Type) < 0 ) {
    return;
  }

  // タイプモジュールの登録を行う．
  PyModule_AddObject(m, "RegVect", (PyObject*)&PyRegVect_Type);
}

END_NAMESPACE_YM_IGF

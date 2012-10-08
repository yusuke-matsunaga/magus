
/// @file pyutils_RandCombiGen.cc
/// @brief RandCombiGen の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/RandCombiGen.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// RandCombiGen を表す型
struct RandCombiGenObject
{
  // Python のお約束
  PyObject_HEAD

  // RandCombiGen の本体
  RandCombiGen* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// RandCombiGenObject の生成関数
RandCombiGenObject*
RandCombiGen_new(PyTypeObject* type)
{
  RandCombiGenObject* self = PyObject_New(RandCombiGenObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ここでは NULL のまにしておく．
  self->mBody = NULL;

  return self;
}

// RandCombiGenObject を開放する関数
void
RandCombiGen_dealloc(RandCombiGenObject* self)
{
  // RandCombiGen の開放を行なう．
  delete self->mBody;

  PyObject_Del(self);
}

// 初期化関数
int
RandCombiGen_init(RandCombiGenObject* self,
		  PyObject* args)
{
  // 引数の形式は
  // - (uint, uint)
  ymuint n = 0;
  ymuint k = 0;
  if ( !PyArg_ParseTuple(args, "kk", &n, &k) ) {
    return NULL;
  }

  if ( self->mBody != NULL ) {
    delete self->mBody;
  }
  self->mBody = new RandCombiGen(n, k);

  return 0;
}

// num 関数
PyObject*
RandCombiGen_num(RandCombiGenObject* self,
		 PyObject* args)
{
  return conv_to_pyobject(self->mBody->num());
}

// combi_num 関数
PyObject*
RandCombiGen_combi_num(RandCombiGenObject* self,
		       PyObject* args)
{
  return conv_to_pyobject(self->mBody->combi_num());
}

// generate 関数
PyObject*
RandCombiGen_generate(RandCombiGenObject* self,
		      PyObject* args)
{
  // 引数の形式は
  // - (RandGen)
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &RandGenType, &obj) ) {
    return NULL;
  }
  RandGen* p_rg;
  if ( !conv_from_pyobject(obj, p_rg) ) {
    return NULL;
  }

  self->mBody->generate(*p_rg);

  Py_INCREF(Py_None);
  return Py_None;
}

// elem 関数
PyObject*
RandCombiGen_elem(RandCombiGenObject* self,
		  PyObject* args)
{
  // 引数の形式は
  // - (uint)
  ymuint pos = 0;
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return NULL;
  }

  return conv_to_pyobject(self->mBody->elem(pos));
}


//////////////////////////////////////////////////////////////////////
// RandCombiGenObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef RandCombiGen_methods[] = {
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
  {"num", (PyCFunction)RandCombiGen_num, METH_NOARGS,
   PyDoc_STR("return the number of all elements (NONE)")},
  {"combi_num", (PyCFunction)RandCombiGen_combi_num, METH_NOARGS,
   PyDoc_STR("return the number of combinations (NONE)")},
  {"generate", (PyCFunction)RandCombiGen_generate, METH_VARARGS,
   PyDoc_STR("generate random combination (RandGen)")},
  {"elem", (PyCFunction)RandCombiGen_elem, METH_VARARGS,
   PyDoc_STR("return element (uint)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// RandCombiGenObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject RandCombiGenType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.RandCombiGen",                // tp_name
  sizeof(RandCombiGenObject),          // tp_basicsize
  (int)0,                              // tp_itemsize

  // Methods to implement standard operations

  (destructor)RandCombiGen_dealloc,    // tp_dealloc
  (printfunc)0,                        // tp_print
  (getattrfunc)0,                      // tp_getattr
  (setattrfunc)0,                      // tp_setattr
  (cmpfunc)0,                          // tp_compare
  (reprfunc)0,                         // tp_repr

  // Method suites for standard classes
  0,                                   // tp_as_number
  0,                                   // tp_as_sequence
  0,                                   // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                         // tp_hash
  (ternaryfunc)0,                      // tp_call
  (reprfunc)0,                         // tp_str
  (getattrofunc)0,                     // tp_getattro
  (setattrofunc)0,                     // tp_setattro

  // Functions to access object as input/output buffer
  0,                                   // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,                  // tp_flags

  // Documentation string
  "random combination generator",      // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                     // tp_traverse

  // delete references to contained objects
  (inquiry)0,                          // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                      // tp_richcompare

  // weak reference enabler
  (long)0,                             // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                      // tp_iter
  (iternextfunc)0,                     // tp_iternext

  // Attribute descriptor and subclassing stuff
  RandCombiGen_methods,                // tp_methods
  0,                                   // tp_members
  0,                                   // tp_getset
  (struct _typeobject*)0,              // tp_base
  (PyObject*)0,                        // tp_dict
  (descrgetfunc)0,                     // tp_descr_get
  (descrsetfunc)0,                     // tp_descr_set
  (long)0,                             // tp_dictoffset
  (initproc)RandCombiGen_init,         // tp_init
  (allocfunc)0,                        // tp_alloc
  (newfunc)RandCombiGen_new,           // tp_new
  (freefunc)0,                         // tp_free
  (inquiry)0,                          // tp_is_gc

  (PyObject*)0,                        // tp_bases
  (PyObject*)0,                        // tp_mro (method resolution order)
  (PyObject*)0,                        // tp_cache
  (PyObject*)0,                        // tp_subclasses
  (PyObject*)0                         // tp_weaklist
};

END_NAMESPACE_YM_PYTHON

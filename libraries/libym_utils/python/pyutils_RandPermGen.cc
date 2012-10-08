
/// @file pyutils_RandPermGen
/// @brief RandPermGen の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/RandPermGen.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// RandPermGen を表す型
struct RandPermGenObject
{
  // Python のお約束
  PyObject_HEAD

  // RandPermGen の本体
  RandPermGen* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// RandPermGenObject の生成関数
RandPermGenObject*
RandPermGen_new(PyTypeObject* type)
{
  RandPermGenObject* self = PyObject_New(RandPermGenObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ここでは NULL に初期化しておく
  self->mBody = NULL;

  return self;
}

// RandPermGenObject を開放する関数
void
RandPermGen_dealloc(RandPermGenObject* self)
{
  // RandPermGen の開放を行なう．
  delete self->mBody;

  PyObject_Del(self);
}

// 初期化関数
int
RandPermGen_init(RandPermGenObject* self,
		 PyObject* args)
{
  // 引数の形式は
  // - (uint)
  ymuint n = 0;
  if ( !PyArg_ParseTuple(args, "k", &n) ) {
    return NULL;
  }

  if ( self->mBody != NULL ) {
    delete self->mBody;
  }
  self->mBody = new RandPermGen(n);

  return 0;
}

// num 関数
PyObject*
RandPermGen_num(RandPermGenObject* self,
		PyObject* args)
{
  return conv_to_pyobject(self->mBody->num());
}

// generate 関数
PyObject*
RandPermGen_generate(RandPermGenObject* self,
		     PyObject* args)
{
  // 引数の形式は
  // - (RandGen)
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &RandGenType, &obj) ) {
    return NULL;
  }
  RandGen* p_rg = NULL;
  if ( !conv_from_pyobject(obj, p_rg) ) {
    return NULL;
  }

  self->mBody->generate(*p_rg);

  Py_INCREF(Py_None);
  return Py_None;
}

// elem 関数
PyObject*
RandPermGen_elem(RandPermGenObject* self,
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
// RandPermGenObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef RandPermGen_methods[] = {
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
  {"num", (PyCFunction)RandPermGen_num, METH_NOARGS,
   PyDoc_STR("return the number of all elements (NONE)")},
  {"generate", (PyCFunction)RandPermGen_generate, METH_VARARGS,
   PyDoc_STR("generate permutation (RandGen)")},
  {"elem", (PyCFunction)RandPermGen_elem, METH_VARARGS,
   PyDoc_STR("return element (uint)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// RandPermGenObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject RandPermGenType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.RandPermGen",                // tp_name
  sizeof(RandPermGenObject),          // tp_basicsize
  (int)0,                             // tp_itemsize

  // Methods to implement standard operations

  (destructor)RandPermGen_dealloc,    // tp_dealloc
  (printfunc)0,                       // tp_print
  (getattrfunc)0,                     // tp_getattr
  (setattrfunc)0,                     // tp_setattr
  (cmpfunc)0,                         // tp_compare
  (reprfunc)0,                        // tp_repr

  // Method suites for standard classes
  0,                                  // tp_as_number
  0,                                  // tp_as_sequence
  0,                                  // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                        // tp_hash
  (ternaryfunc)0,                     // tp_call
  (reprfunc)0,                        // tp_str
  (getattrofunc)0,                    // tp_getattro
  (setattrofunc)0,                    // tp_setattro

  // Functions to access object as input/output buffer
  0,                                  // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,                 // tp_flags

  // Documentation string
  "documentation string",             // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                    // tp_traverse

  // delete references to contained objects
  (inquiry)0,                         // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                     // tp_richcompare

  // weak reference enabler
  (long)0,                            // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                     // tp_iter
  (iternextfunc)0,                    // tp_iternext

  // Attribute descriptor and subclassing stuff
  RandPermGen_methods,                // tp_methods
  0,                                  // tp_members
  0,                                  // tp_getset
  (struct _typeobject*)0,             // tp_base
  (PyObject*)0,                       // tp_dict
  (descrgetfunc)0,                    // tp_descr_get
  (descrsetfunc)0,                    // tp_descr_set
  (long)0,                            // tp_dictoffset
  (initproc)RandPermGen_init,         // tp_init
  (allocfunc)0,                       // tp_alloc
  (newfunc)RandPermGen_new,           // tp_new
  (freefunc)0,                        // tp_free
  (inquiry)0,                         // tp_is_gc

  (PyObject*)0,                       // tp_bases
  (PyObject*)0,                       // tp_mro (method resolution order)
  (PyObject*)0,                       // tp_cache
  (PyObject*)0,                       // tp_subclasses
  (PyObject*)0                        // tp_weaklist
};

END_NAMESPACE_YM_PYTHON

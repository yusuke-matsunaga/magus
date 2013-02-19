
/// @file pyutils_NameMgr.cc
/// @brief NameMgr の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/NameMgr.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// NameMgr を表す型
struct NameMgrObject
{
  // Python のお約束
  PyObject_HEAD

  // NameMgr の本体
  NameMgr* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// NameMgrObject の生成関数
NameMgrObject*
NameMgr_new(PyTypeObject* type)
{
  NameMgrObject* self = PyObject_New(NameMgrObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // NameMgr の生成を行なう．
  self->mBody = new NameMgr("", "");

  return self;
}

// NameMgrObject を開放する関数
void
NameMgr_dealloc(NameMgrObject* self)
{
  // NameMgr の開放を行なう．
  delete self->mBody;

  PyObject_Del(self);
}

// 初期化関数
int
NameMgr_init(NameMgrObject* self,
	     PyObject* args)
{
  // 引数の形式は
  // - (str, str)
  char* prefix;
  char* suffix;
  if ( !PyArg_ParseTuple(args, "ss", &prefix, &suffix) ) {
    return -1;
  }

  self->mBody->change(prefix, suffix);

  // 正常に終了したら 0 を返す．
  return 0;
}

// change 関数
PyObject*
NameMgr_change(NameMgrObject* self,
	       PyObject* args)
{
  // 引数の形式は
  // - (str, str)
  char* prefix;
  char* suffix;
  if ( !PyArg_ParseTuple(args, "ss", &prefix, &suffix) ) {
    return NULL;
  }

  self->mBody->change(prefix, suffix);

  Py_INCREF(Py_None);
  return Py_None;
}

// clear 関数
PyObject*
NameMgr_clear(NameMgrObject* self,
	      PyObject* args)
{
  self->mBody->clear();

  Py_INCREF(Py_None);
  return Py_None;
}

// prefix 関数
PyObject*
NameMgr_prefix(NameMgrObject* self,
	       PyObject* args)
{
  return PyObject_FromString(self->mBody->prefix());
}

// suffix 関数
PyObject*
NameMgr_suffix(NameMgrObject* self,
	       PyObject* args)
{
  return PyObject_FromString(self->mBody->suffix());
}

// new_name 関数
PyObject*
NameMgr_new_name(NameMgrObject* self,
		 PyObject* args)
{
  int b;
  if ( !PyArg_ParseTuple(args, "b", &b) ) {
    return NULL;
  }

  bool add_name = b ? true : false;
  string result = self->mBody->new_name(add_name);
  return PyObject_FromString(result);
}

// add 関数
PyObject*
NameMgr_add(NameMgrObject* self,
	    PyObject* args)
{
  char* name;
  if ( !PyArg_ParseTuple(args, "s", &name) ) {
    return NULL;
  }
  self->mBody->add(name);

  Py_INCREF(Py_None);
  return Py_None;
}

// erase 関数
PyObject*
NameMgr_erase(NameMgrObject* self,
	      PyObject* args)
{
  char* name;
  if ( !PyArg_ParseTuple(args, "s", &name) ) {
    return NULL;
  }
  self->mBody->erase(name);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// NameMgrObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef NameMgr_methods[] = {
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
  {"change", (PyCFunction)NameMgr_change, METH_VARARGS,
   PyDoc_STR("change prefix and suffix (str, str)")},
  {"clear", (PyCFunction)NameMgr_clear, METH_NOARGS,
   PyDoc_STR("clear (NONE)")},
  {"prefix", (PyCFunction)NameMgr_prefix, METH_NOARGS,
   PyDoc_STR("return prefix (NONE)")},
  {"suffix", (PyCFunction)NameMgr_suffix, METH_NOARGS,
   PyDoc_STR("return suffix (NONE)")},
  {"new_name", (PyCFunction)NameMgr_new_name, METH_VARARGS,
   PyDoc_STR("return new name (bool)")},
  {"add", (PyCFunction)NameMgr_add, METH_VARARGS,
   PyDoc_STR("add name (str)")},
  {"erase", (PyCFunction)NameMgr_erase, METH_VARARGS,
   PyDoc_STR("erase name (str)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// NameMgrObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyNameMgr_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.NameMgr",                // tp_name
  sizeof(NameMgrObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)NameMgr_dealloc,    // tp_dealloc
  (printfunc)0,                   // tp_print
  (getattrfunc)0,                 // tp_getattr
  (setattrfunc)0,                 // tp_setattr
  (cmpfunc)0,                     // tp_compare
  (reprfunc)0,                    // tp_repr

  // Method suites for standard classes
  0,                              // tp_as_number
  0,                              // tp_as_sequence
  0,                              // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                    // tp_hash
  (ternaryfunc)0,                 // tp_call
  (reprfunc)0,                    // tp_str
  (getattrofunc)0,                // tp_getattro
  (setattrofunc)0,                // tp_setattro

  // Functions to access object as input/output buffer
  0,                              // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,             // tp_flags

  // Documentation string
  "name manager",                 // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                // tp_traverse

  // delete references to contained objects
  (inquiry)0,                     // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                 // tp_richcompare

  // weak reference enabler
  (long)0,                        // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                 // tp_iter
  (iternextfunc)0,                // tp_iternext

  // Attribute descriptor and subclassing stuff
  NameMgr_methods,                // tp_methods
  0,                              // tp_members
  0,                              // tp_getset
  (struct _typeobject*)0,         // tp_base
  (PyObject*)0,                   // tp_dict
  (descrgetfunc)0,                // tp_descr_get
  (descrsetfunc)0,                // tp_descr_set
  (long)0,                        // tp_dictoffset
  (initproc)NameMgr_init,         // tp_init
  (allocfunc)0,                   // tp_alloc
  (newfunc)NameMgr_new,           // tp_new
  (freefunc)0,                    // tp_free
  (inquiry)0,                     // tp_is_gc

  (PyObject*)0,                   // tp_bases
  (PyObject*)0,                   // tp_mro (method resolution order)
  (PyObject*)0,                   // tp_cache
  (PyObject*)0,                   // tp_subclasses
  (PyObject*)0                    // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と NameMgr の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から NameMgr へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return NameMgr へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
NameMgr*
PyNameMgr_AsNameMgrPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyNameMgr_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.NameMgr is expected");
    return NULL;
  }

  // 強制的にキャスト
  NameMgrObject* my_obj = (NameMgrObject*)py_obj;

  return my_obj->mBody;
}

// NameMgrobject 関係の初期化を行なう．
void
NameMgrObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyNameMgr_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "NameMgr", (PyObject*)&PyNameMgr_Type);
}

END_NAMESPACE_YM

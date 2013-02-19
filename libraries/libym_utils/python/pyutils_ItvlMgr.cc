
/// @file pyutils_ItvlMgr.cc
/// @brief ItvlMgr の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/ItvlMgr.h"
#include "ym_utils/FileBinI.h"
#include "ym_utils/FileBinO.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// ItvlMgr を表す型
struct ItvlMgrObject
{
  // Python のお約束
  PyObject_HEAD

  // ItvlMgr の本体
  ItvlMgr* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// ItvlMgrObject の生成関数
ItvlMgrObject*
ItvlMgr_new(PyTypeObject* type)
{
  ItvlMgrObject* self = PyObject_New(ItvlMgrObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ItvlMgr の生成を行なう．
  self->mBody = new ItvlMgr();

  return self;
}

// ItvlMgrObject を開放する関数
void
ItvlMgr_dealloc(ItvlMgrObject* self)
{
  // ItvlMgr の開放を行なう．
  delete self->mBody;

  PyObject_Del(self);
}

// clear 関数
PyObject*
ItvlMgr_clear(ItvlMgrObject* self,
	      PyObject* args)
{
  self->mBody->clear();

  Py_INCREF(Py_None);
  return Py_None;
}

// avail_num 関数
PyObject*
ItvlMgr_avail_num(ItvlMgrObject* self,
		  PyObject* args)
{
  return PyObject_FromYmuint32(self->mBody->avail_num());
}

// erase 関数
PyObject*
ItvlMgr_erase(ItvlMgrObject* self,
	      PyObject* args)
{
  int d1;
  int d2 = -1;
  if ( !PyArg_ParseTuple(args, "i|i", &d1, &d2) ) {
    return NULL;
  }

  if ( d2 == -1 ) {
    self->mBody->erase(d1);
  }
  else {
    self->mBody->erase(d1, d2);
  }

  Py_INCREF(Py_None);
  return Py_None;
}

// add 関数
PyObject*
ItvlMgr_add(ItvlMgrObject* self,
	    PyObject* args)
{
  int d1;
  int d2 = -1;
  if ( !PyArg_ParseTuple(args, "i|i", &d1, &d2) ) {
    return NULL;
  }

  if ( d2 == -1 ) {
    self->mBody->add(d1);
  }
  else {
    self->mBody->add(d1, d2);
  }

  Py_INCREF(Py_None);
  return Py_None;
}

// check 関数
PyObject*
ItvlMgr_check(ItvlMgrObject* self,
	      PyObject* args)
{
  int d1;
  int d2;
  if ( !PyArg_ParseTuple(args, "ii", &d1, &d2) ) {
    return NULL;
  }

  bool result = self->mBody->check(d1, d2);

  return PyObject_FromBool(result);
}

// min_id 関数
PyObject*
ItvlMgr_min_id(ItvlMgrObject* self,
	       PyObject* args)
{
  return PyObject_FromYmint32(self->mBody->min_id());
}

// max_id 関数
PyObject*
ItvlMgr_max_id(ItvlMgrObject* self,
	       PyObject* args)
{
  return PyObject_FromYmint32(self->mBody->max_id());
}

// dump 関数
PyObject*
ItvlMgr_dump(ItvlMgrObject* self,
	     PyObject* args)
{
  FileBinO* bp = parse_FileBinO(args);
  if ( bp == NULL ) {
    return NULL;
  }

  self->mBody->dump(*bp);

  Py_INCREF(Py_None);
  return Py_None;
}

// restore 関数
PyObject*
ItvlMgr_restore(ItvlMgrObject* self,
		PyObject* args)
{
  FileBinI* bp = parse_FileBinI(args);
  if ( bp == NULL ) {
    return NULL;
  }

  self->mBody->restore(*bp);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// ItvlMgrObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef ItvlMgr_methods[] = {
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
  {"clear", (PyCFunction)ItvlMgr_clear, METH_NOARGS,
   PyDoc_STR("clear (NONE)")},
  {"avail_num", (PyCFunction)ItvlMgr_avail_num, METH_NOARGS,
   PyDoc_STR("return available number (NONE)")},
  {"erase", (PyCFunction)ItvlMgr_erase, METH_VARARGS,
   PyDoc_STR("erase interval (int, ?int)")},
  {"add", (PyCFunction)ItvlMgr_add, METH_VARARGS,
   PyDoc_STR("add interval (int, ?int)")},
  {"check", (PyCFunction)ItvlMgr_check, METH_VARARGS,
   PyDoc_STR("check interval (int, int)")},
  {"min_id", (PyCFunction)ItvlMgr_min_id, METH_NOARGS,
   PyDoc_STR("return the minimum number (NONE)")},
  {"max_id", (PyCFunction)ItvlMgr_max_id, METH_NOARGS,
   PyDoc_STR("return the maximum number (NONE)")},
  {"dump", (PyCFunction)ItvlMgr_dump, METH_VARARGS,
   PyDoc_STR("dump (FileBinO)")},
  {"restore", (PyCFunction)ItvlMgr_restore, METH_VARARGS,
   PyDoc_STR("restore (FileBinI)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// ItvlMgrObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyItvlMgr_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.ItvlMgr",                // tp_name
  sizeof(ItvlMgrObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)ItvlMgr_dealloc,    // tp_dealloc
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
  "interval manager",             // tp_doc

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
  ItvlMgr_methods,                // tp_methods
  0,                              // tp_members
  0,                              // tp_getset
  (struct _typeobject*)0,         // tp_base
  (PyObject*)0,                   // tp_dict
  (descrgetfunc)0,                // tp_descr_get
  (descrsetfunc)0,                // tp_descr_set
  (long)0,                        // tp_dictoffset
  (initproc)0,                    // tp_init
  (allocfunc)0,                   // tp_alloc
  (newfunc)ItvlMgr_new,           // tp_new
  (freefunc)0,                    // tp_free
  (inquiry)0,                     // tp_is_gc

  (PyObject*)0,                   // tp_bases
  (PyObject*)0,                   // tp_mro (method resolution order)
  (PyObject*)0,                   // tp_cache
  (PyObject*)0,                   // tp_subclasses
  (PyObject*)0                    // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と ItvlMgr の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から ItvlMgr へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return ItvlMgr へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
ItvlMgr*
PyItvlMgr_AsItvlMgrPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyItvlMgr_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.ItvlMgr is expected");
    return NULL;
  }

  // 強制的にキャスト
  ItvlMgrObject* my_obj = (ItvlMgrObject*)py_obj;

  return my_obj->mBody;
}

// ItvlMgrObject 関係の初期化を行なう．
void
ItvlMgrObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyItvlMgr_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "ItvlMgr", (PyObject*)&PyItvlMgr_Type);
}

END_NAMESPACE_YM

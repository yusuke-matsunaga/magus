
/// @file pyutils_MsgMgr.cc
/// @brief MsgMgr の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/FileRegion.h"
#include "PyMsgHandler.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// MsgMgr を表す型
struct MsgMgrObject
{
  // Python のお約束
  PyObject_HEAD

  // PyObject* : PyMsgHandler* の辞書
  hash_map<ympuint, PyMsgHandler*>* mDict;

};

// 唯一のオブジェクト
MsgMgrObject Py_kMsgMgrStruct = {
  PyObject_HEAD_INIT(&PyMsgMgr_Type)
  NULL
};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// MsgMgrObject の生成関数
// 実は唯一のオブジェクトを返す．
MsgMgrObject*
MsgMgr_new(PyTypeObject* type,
	   PyObject* args)
{
  hash_map<ympuint, PyMsgHandler*>*& dict = Py_kMsgMgrStruct.mDict;
  if ( dict == NULL ) {
    dict = new hash_map<ympuint, PyMsgHandler*>();
    if ( dict == NULL ) {
      return NULL;
    }
  }

  MsgMgrObject* obj = &Py_kMsgMgrStruct;

  Py_INCREF(obj);
  return obj;
}

// reg_handler 関数
PyObject*
MsgMgr_reg_handler(PyTypeObject* type,
		   PyObject* args)
{
  PyObject* obj = NULL;
  ymuint32 mask = kMaskAll;
  if ( !PyArg_ParseTuple(args, "O|k", &obj, &mask) ) {
    return NULL;
  }

  if ( !PyCallable_Check(obj) ) {
    PyErr_SetString(PyExc_TypeError, "parameter must be callable");
    return NULL;
  }

  PyMsgHandler* handler = new PyMsgHandler(obj, mask);

  MsgMgr::reg_handler(handler);

  ympuint key = reinterpret_cast<ympuint>(obj);
  Py_kMsgMgrStruct.mDict->insert(make_pair(key, handler));

  Py_INCREF(Py_None);
  return Py_None;
}

// unreg_handler 関数
PyObject*
MsgMgr_unreg_handler(PyTypeObject* type,
		     PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O", &obj) ) {
    return NULL;
  }

  if ( !PyCallable_Check(obj) ) {
    PyErr_SetString(PyExc_TypeError, "parameter must be callable");
    return NULL;
  }

  ympuint key = reinterpret_cast<ympuint>(obj);
  hash_map<ympuint, PyMsgHandler*>::iterator p = Py_kMsgMgrStruct.mDict->find(key);
  if ( p == Py_kMsgMgrStruct.mDict->end() ) {
    PyErr_SetString(PyExc_ValueError, "parameter is not registered");
    return NULL;
  }

  PyMsgHandler* handler = p->second;

  MsgMgr::unreg_handler(handler);

  delete handler;

  Py_INCREF(Py_None);
  return Py_None;
}

// unreg_all_handlers 関数
PyObject*
MsgMgr_unreg_all_handlers(PyTypeObject* type,
			  PyObject* args)
{
  MsgMgr::unreg_all_handlers();

  Py_INCREF(Py_None);
  return Py_None;
}

// put_msg 関数
PyObject*
MsgMgr_put_msg(PyTypeObject* type_obj,
	       PyObject* args)
{
  // 引数は
  // - (str, int, FileRegion, MsgType, str, str)
  char* src_file;
  int src_line;
  PyObject* obj1;
  PyObject* obj2;
  char* label;
  char* msg;
  if ( !PyArg_ParseTuple(args, "siO!O!ss",
			 &src_file, &src_line,
			 &PyFileRegion_Type, &obj1,
			 &PyMsgType_Type, &obj2,
			 &label, &msg) ) {
    return NULL;
  }

  FileRegion fr = PyFileRegion_AsFileRegion(obj1);
  tMsgType type = PyMsgType_AsMsgType(obj2);

  MsgMgr::put_msg(src_file, src_line, fr, type, label, msg);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// MsgMgrObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef MsgMgr_methods[] = {
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
  {"reg_handler", (PyCFunction)MsgMgr_reg_handler, METH_STATIC | METH_VARARGS,
   PyDoc_STR("register message handler callback (Callable)")},
  {"unreg_handler", (PyCFunction)MsgMgr_unreg_handler, METH_STATIC | METH_VARARGS,
   PyDoc_STR("unregister message handler callback (Callable)")},
  {"unreg_all_handlers", (PyCFunction)MsgMgr_unreg_all_handlers, METH_STATIC | METH_NOARGS,
   PyDoc_STR("unregister all message handlers (NONE)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// MsgMgrObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyMsgMgr_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "MsgMgr",                      // tp_name
  sizeof(MsgMgrObject),          // tp_basicsize
  (int)0,                        // tp_itemsize

  // Methods to implement standard operations

  (destructor)0,                 // tp_dealloc
  (printfunc)0,                  // tp_print
  (getattrfunc)0,                // tp_getattr
  (setattrfunc)0,                // tp_setattr
  (cmpfunc)0,                    // tp_compare
  (reprfunc)0,                   // tp_repr

  // Method suites for standard classes
  0,                             // tp_as_number
  0,                             // tp_as_sequence
  0,                             // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                   // tp_hash
  (ternaryfunc)0,                // tp_call
  (reprfunc)0,                   // tp_str
  (getattrofunc)0,               // tp_getattro
  (setattrofunc)0,               // tp_setattro

  // Functions to access object as input/output buffer
  0,                             // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,            // tp_flags

  // Documentation string
  "message manager",             // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,               // tp_traverse

  // delete references to contained objects
  (inquiry)0,                    // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                // tp_richcompare

  // weak reference enabler
  (long)0,                       // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                // tp_iter
  (iternextfunc)0,               // tp_iternext

  // Attribute descriptor and subclassing stuff
  MsgMgr_methods,                // tp_methods
  0,                             // tp_members
  0,                             // tp_getset
  (struct _typeobject*)0,        // tp_base
  (PyObject*)0,                  // tp_dict
  (descrgetfunc)0,               // tp_descr_get
  (descrsetfunc)0,               // tp_descr_set
  (long)0,                       // tp_dictoffset
  (initproc)0,                   // tp_init
  (allocfunc)MsgMgr_new,         // tp_alloc
  (newfunc)0,                    // tp_new
  (freefunc)0,                   // tp_free
  (inquiry)0,                    // tp_is_gc

  (PyObject*)0,                  // tp_bases
  (PyObject*)0,                  // tp_mro (method resolution order)
  (PyObject*)0,                  // tp_cache
  (PyObject*)0,                  // tp_subclasses
  (PyObject*)0                   // tp_weaklist
};


// MsgMgrObject 関係の初期化を行う．
void
MsgMgrObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyMsgMgr_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "MsgMgr", (PyObject*)&PyMsgMgr_Type);
}

END_NAMESPACE_YM

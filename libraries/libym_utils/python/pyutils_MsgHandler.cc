
/// @file pyutils_MsgHandler.cc
/// @brief MsgHandler の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "PyMsgHandler.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// MsgHandler を表す型
struct MsgHandlerObject
{
  // Python のお約束
  PyObject_HEAD

  // MsgHandler の本体
  PyMsgHandler* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// MsgHandlerObject の生成関数
MsgHandlerObject*
MsgHandler_new(PyTypeObject* type)
{
  MsgHandlerObject* self = PyObject_New(MsgHandlerObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // PyMsgHandler の生成
  self->mBody = new PyMsgHandler();

  return self;
}

// MsgHandlerObject を開放する関数
void
MsgHandler_dealloc(MsgHandlerObject* self)
{
  // PyMsgHandler の開放を行なう．
  delete self->mBody;

  PyObject_Del(self);
}

// 初期化関数
int
MsgHandler_init(MsgHandlerObject* self,
		PyObject* args)
{
  // 引数の形式は
  // - ()
  // - (uint)
  ymuint tmp = 0U;
  if ( !PyArg_ParseTuple(args, "k", &tmp) ) {
    return -1;
  }

  self->mBody->set_mask(tmp);

  return 0;
}

// set_mask 関数
PyObject*
MsgHandler_set_mask(MsgHandlerObject* self,
		    PyObject* args)
{
  ymuint tmp = 0U;
  if ( !PyArg_ParseTuple(args, "k", &tmp) ) {
    return NULL;
  }

  self->mBody->set_mask(tmp);

  Py_INCREF(Py_None);
  return Py_None;
}

// mask 関数
PyObject*
MsgHandler_mask(MsgHandlerObject* self,
		PyObject* args)
{
  return conv_to_pyobject(self->mBody->mask());
}

// add_mask 関数
PyObject*
MsgHandler_add_mask(MsgHandlerObject* self,
		    PyObject* args)
{

  Py_INCREF(Py_None);
  return Py_None;
}

// delete_mask 関数
PyObject*
MsgHandler_delete_mask(MsgHandlerObject* self,
		       PyObject* args)
{

  Py_INCREF(Py_None);
  return Py_None;
}

// set_callback 関数
PyObject*
MsgHandler_set_callback(MsgHandlerObject* self,
			PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O:set_callback", &obj) ) {
    return NULL;
  }
  if ( !PyCallable_Check(obj) ) {
    PyErr_SetString(PyExc_TypeError, "parameter must be callable");
    return NULL;
  }

  PyObject* old_obj = self->mBody->set_callback(obj);

  Py_INCREF(old_obj);
  return old_obj;
}


//////////////////////////////////////////////////////////////////////
// MsgHandlerObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef MsgHandler_methods[] = {
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
  {"set_mask", (PyCFunction)MsgHandler_set_mask, METH_VARARGS,
   PyDoc_STR("set message mask (uint)")},
  {"mask", (PyCFunction)MsgHandler_mask, METH_NOARGS,
   PyDoc_STR("return message mask (NONE)")},
  {"add_mask", (PyCFunction)MsgHandler_add_mask, METH_VARARGS,
   PyDoc_STR("add message mask (MsgType)")},
  {"delete_mask", (PyCFunction)MsgHandler_add_mask, METH_VARARGS,
   PyDoc_STR("delete message mask (MsgType)")},
  {"set_callback", (PyCFunction)MsgHandler_set_callback, METH_VARARGS,
   PyDoc_STR("set callback function (Callable)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// MsgHandlerObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject MsgHandlerType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.MsgHandler",                // tp_name
  sizeof(MsgHandlerObject),          // tp_basicsize
  (int)0,                            // tp_itemsize

  // Methods to implement standard operations

  (destructor)MsgHandler_dealloc,    // tp_dealloc
  (printfunc)0,                      // tp_print
  (getattrfunc)0,                    // tp_getattr
  (setattrfunc)0,                    // tp_setattr
  (cmpfunc)0,                        // tp_compare
  (reprfunc)0,                       // tp_repr

  // Method suites for standard classes
  0,                                 // tp_as_number
  0,                                 // tp_as_sequence
  0,                                 // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                       // tp_hash
  (ternaryfunc)0,                    // tp_call
  (reprfunc)0,                       // tp_str
  (getattrofunc)0,                   // tp_getattro
  (setattrofunc)0,                   // tp_setattro

  // Functions to access object as input/output buffer
  0,                                 // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,                // tp_flags

  // Documentation string
  "Message Handler",                 // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                   // tp_traverse

  // delete references to contained objects
  (inquiry)0,                        // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                    // tp_richcompare

  // weak reference enabler
  (long)0,                           // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                    // tp_iter
  (iternextfunc)0,                   // tp_iternext

  // Attribute descriptor and subclassing stuff
  MsgHandler_methods,                // tp_methods
  0,                                 // tp_members
  0,                                 // tp_getset
  (struct _typeobject*)0,            // tp_base
  (PyObject*)0,                      // tp_dict
  (descrgetfunc)0,                   // tp_descr_get
  (descrsetfunc)0,                   // tp_descr_set
  (long)0,                           // tp_dictoffset
  (initproc)MsgHandler_init,         // tp_init
  (allocfunc)0,                      // tp_alloc
  (newfunc)MsgHandler_new,           // tp_new
  (freefunc)0,                       // tp_free
  (inquiry)0,                        // tp_is_gc

  (PyObject*)0,                      // tp_bases
  (PyObject*)0,                      // tp_mro (method resolution order)
  (PyObject*)0,                      // tp_cache
  (PyObject*)0,                      // tp_subclasses
  (PyObject*)0                       // tp_weaklist
};


// @brief PyObject から MsgHandler を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] p_obj MsgHandler のポインタを格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した． py_obj が MsgHandlerObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   MsgHandler*& p_obj)
{
  if ( !MsgHandlerObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  MsgHandlerObject* mh_obj = (MsgHandlerObject*)py_obj;

  p_obj = mh_obj->mBody;

  return true;
}

// MsgHandlerObject 関係の初期化を行う．
void
MsgHandlerObject_init(PyObject* m)
{
  // タイプオブジェクトの登録
  PyModule_AddObject(m, "MsgHandler", (PyObject*)&MsgHandlerType);
}

END_NAMESPACE_YM_PYTHON

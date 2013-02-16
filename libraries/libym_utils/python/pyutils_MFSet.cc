
/// @file pyutils_MFSet.cc
/// @brief MFSet の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/MFSet.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// MFSet を表す型
struct MFSetObject
{
  // Python のお約束
  PyObject_HEAD

  // MFSet の本体
  MFSet* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// MFSetObject の生成関数
MFSetObject*
MFSet_new(PyTypeObject* type)
{
  MFSetObject* self = PyObject_New(MFSetObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ここでは NULL のままにしておく．
  self->mBody = NULL;

  return self;
}

// MFSetObject を開放する関数
void
MFSet_dealloc(MFSetObject* self)
{
  // MFSet の開放を行なう．
  delete self->mBody;

  PyObject_Del(self);
}

// 初期化関数
int
MFSet_init(MFSetObject* self,
	   PyObject* args)
{
  // 引数の形式は
  // - (uint)
  ymuint n;
  if ( !PyArg_ParseTuple(args, "k", &n) ) {
    return NULL;
  }

  if ( self->mBody != NULL ) {
    delete self->mBody;
  }
  self->mBody = new MFSet(n);

  // 正常に終了したら 0 を返す．
  return 0;
}

// find 関数
PyObject*
MFSet_find(MFSetObject* self,
	   PyObject* args)
{
  // 引数の形式は
  // - (uint)
  ymuint x;
  if ( !PyArg_ParseTuple(args, "k", &x) ) {
    return NULL;
  }

  ymuint y = self->mBody->find(x);
  if ( y == MFSet::kBadID ) {
    PyErr_SetString(PyExc_ValueError, "parameter is out of range");
    return NULL;
  }

  return conv_to_pyobject(y);
}

// merge 関数
PyObject*
MFSet_merge(MFSetObject* self,
	    PyObject* args)
{
  // 引数の形式は
  // - (uint, uint)
  ymuint x;
  ymuint y;
  if ( !PyArg_ParseTuple(args, "ii", &x, &y) ) {
    return NULL;
  }

  ymuint z = self->mBody->merge(x, y);

  if ( z == MFSet::kBadID ) {
    PyErr_SetString(PyExc_ValueError, "parameter is out of range");
    return NULL;
  }

  return conv_to_pyobject(z);
}


//////////////////////////////////////////////////////////////////////
// MFSetObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef MFSet_methods[] = {
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
  {"find", (PyCFunction)MFSet_find, METH_VARARGS,
   PyDoc_STR("find representative (uint)")},
  {"merge", (PyCFunction)MFSet_merge, METH_VARARGS,
   PyDoc_STR("merge two sets (uint, uint)")},
  {NULL, NULL, 0, NULL} // end-marker
};


END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// MFSetObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyMFSet_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.MFSet",                // tp_name
  sizeof(MFSetObject),          // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)MFSet_dealloc,    // tp_dealloc
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
  "Merge/Find Set",             // tp_doc

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
  MFSet_methods,                // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)MFSet_init,         // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)MFSet_new,           // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と MFSet の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から MFSet を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj MFSet を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が MFSetObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   MFSet*& p_obj)
{
  // 型のチェック
  if ( !MFSetObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  MFSetObject* my_obj = (MFSetObject*)py_obj;

  p_obj = my_obj->mBody;

  return true;
}

// MFSetObject 関係の初期化を行う．
void
MFSetObject_init(PyObject* m)
{
  if ( PyType_Ready(&PyMFSet_Type) < 0 ) {
    return;
  }

  // タイプモジュールの登録を行う．
  PyModule_AddObject(m, "MFSet", (PyObject*)&PyMFSet_Type);
}

END_NAMESPACE_YM

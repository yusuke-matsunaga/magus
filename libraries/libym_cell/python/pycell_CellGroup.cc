
/// @file pycell_CellGroup.cc
/// @brief CellGroup の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/CellGroup.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellGroup を表す型
struct CellGroupObject
{
  // Python のお約束
  PyObject_HEAD

  // CellGroup の本体
  const CellGroup* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellGroupObject を開放する関数
void
CellGroup_dealloc(CellGroupObject* self)
{

  PyObject_Del(self);
}

// id 関数
PyObject*
CellGroup_id(CellGroupObject* self,
	     PyObject* args)
{
  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// CellGroupObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellGroup_methods[] = {
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

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellGroupObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject CellGroupType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell.CellGroup",             // tp_name
  sizeof(CellGroupObject),      // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellGroup_dealloc,// tp_dealloc
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
  "Cell Group",                 // tp_doc

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
  CellGroup_methods,            // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  0,                            // tp_init
  (allocfunc)0,                 // tp_alloc
  0,                            // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と CellGroup の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から CellGroup を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] p_obj CellGroup のポインタを格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が CellGroupObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   const CellGroup*& p_obj)
{
  // 型のチェック
  if ( !CellGroupObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  CellGroupObject* my_obj = (CellGroupObject*)py_obj;

  p_obj = my_obj->mBody;

  return true;
}

// CellGroupObject 関係の初期化を行う．
void
CellGroupObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&CellGroupType) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellGroup", (PyObject*)&CellGroupType);
}

END_NAMESPACE_YM_PYTHON

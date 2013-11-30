
/// @file pycell_CellGroup.cc
/// @brief CellGroup の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/CellGroup.h"
#include "ym_cell/Cell.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellGroup を表す型
struct CellGroupObject
{
  // Python のお約束
  PyObject_HEAD

  // CellGroup
  const CellGroup* mGroup;

  // 代表クラスを表すオブジェクト
  PyObject* mRepClass;

  // セルのオブジェクトのリスト
  PyObject* mCellList;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellGroupObject を開放する関数
void
CellGroup_dealloc(CellGroupObject* self)
{
  Py_DECREF(self->mRepClass);
  Py_DECREF(self->mCellList);

  PyObject_Del(self);
}

// id 関数
PyObject*
CellGroup_id(CellGroupObject* self,
	     PyObject* args)
{
  return PyObject_FromYmuint32(self->mGroup->id());
}

// cell_class 関数
PyObject*
CellGroup_cell_class(CellGroupObject* self,
		     PyObject* args)
{
  PyObject* result = self->mRepClass;

  Py_INCREF(result);
  return result;
}

// map 関数
PyObject*
CellGroup_map(CellGroupObject* self,
	      PyObject* args)
{
  PyObject* result = Py_None;

  Py_INCREF(result);
  return result;
}

// cell_list 関数
PyObject*
CellGroup_cell_list(CellGroupObject* self,
		    PyObject* args)
{
  PyObject* result = self->mCellList;

  Py_INCREF(result);
  return result;
}

// ff_info 関数
PyObject*
CellGroup_ff_info(CellGroupObject* self,
		  PyObject* args)
{
  PyObject* result = Py_None;

  Py_INCREF(result);
  return result;
}

// latch_info 関数
PyObject*
CellGroup_latch_info(CellGroupObject* self,
		     PyObject* args)
{
  PyObject* result = Py_None;

  Py_INCREF(result);
  return result;
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
  {"id", (PyCFunction)CellGroup_id, METH_NOARGS,
   PyDoc_STR("return ID")},

  {"cell_class", (PyCFunction)CellGroup_cell_class, METH_NOARGS,
   PyDoc_STR("return parent class")},
  {"rep_map", (PyCFunction)CellGroup_map, METH_NOARGS,
   PyDoc_STR("return NPN-map")},

  {"cell_list", (PyCFunction)CellGroup_cell_list, METH_NOARGS,
   PyDoc_STR("return cell list")},

  {"ff_info", (PyCFunction)CellGroup_ff_info, METH_NOARGS,
   PyDoc_STR("return FF info")},
  {"latch_info", (PyCFunction)CellGroup_latch_info, METH_NOARGS,
   PyDoc_STR("return latch info")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellGroupObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellGroup_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellGroup",             // tp_name
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
  (initproc)0,                  // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)0,                   // tp_new
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

// @brief CellGroup から CellGroupObject を生成する．
// @param[in] group グループ
PyObject*
PyCellGroup_FromCellGroup(const CellGroup* group,
			  PyObject* cell_list[])
{
  CellGroupObject* self = PyObject_New(CellGroupObject, &PyCellGroup_Type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mGroup = group;

  ymuint n = group->cell_num();
  self->mCellList = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const Cell* cell = group->cell(i);
    PyObject* obj = cell_list[cell->id()];
    Py_INCREF(obj);
    PyList_SetItem(self->mCellList, i, obj);
  }

  return (PyObject*)self;
}

// @brief CellGroup に代表クラスを設定する．
void
PyCellGroup_set_rep(PyObject* py_obj,
		    PyObject* rep_obj)
{
  assert_cond( PyCellGroup_Check(py_obj), __FILE__, __LINE__);

  CellGroupObject* group_obj = (CellGroupObject*)py_obj;
  group_obj->mRepClass = rep_obj;
  Py_INCREF(rep_obj);
}

// @brief PyObject から CellGroup へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return CellGroup へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
const CellGroup*
PyCellGroup_AsCellGroupPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellGroup_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellGroup is expected");
    return NULL;
  }

  // 強制的にキャスト
  CellGroupObject* my_obj = (CellGroupObject*)py_obj;

  return my_obj->mGroup;
}

// CellGroupObject 関係の初期化を行う．
void
CellGroupObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellGroup_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellGroup", (PyObject*)&PyCellGroup_Type);
}

END_NAMESPACE_YM

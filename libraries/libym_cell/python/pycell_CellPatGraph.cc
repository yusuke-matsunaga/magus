
/// @file pycell_CellPatGraph.cc
/// @brief CellPatGraph の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_YmCell/pycell.h"
#include "ym_YmCell/CellPatGraph.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellPatGraph を表す型
struct CellPatGraphObject
{
  // Python のお約束
  PyObject_HEAD

  // CellPatGraph
  const CellPatGraph* mPatGraph;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellPatGraphObject を開放する関数
void
CellPatGraph_dealloc(CellPatGraphObject* self)
{
  PyObject_Del(self);
}

// rep_id 関数
PyObject*
CellPatGraph_rep_id(CellPatGraphObject* self,
		    PyObject* args)
{
  return PyObject_FromYmuint32(self->mPatGraph->rep_id());
}

// root_info 関数
PyObject*
CellPatGraph_root_info(CellPatGraphObject* self,
		       PyObject* args)
{
  ymuint root_id = self->mPatGraph->root_id();
  bool root_inv = self->mPatGraph->root_inv();
  return Py_BuildValue("(Ii)", root_id, root_inv);
}

// edge_list 関数
PyObject*
CellPatGraph_edge_list(CellPatGraphObject* self,
		       PyObject* args)
{
  ymuint ne = self->mPatGraph->edge_num();
  PyObject* edge_list = PyList_New(ne);
  for (ymuint i = 0; i < ne; ++ i) {
    PyObject* obj = PyObject_FromYmuint32(self->mPatGraph->edge(i));
    PyList_SetItem(edge_list, i, obj);
  }
  return edge_list;
}


//////////////////////////////////////////////////////////////////////
// CellPatGraphObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellPatGraph_methods[] = {
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

  {"rep_id", (PyCFunction)CellPatGraph_rep_id, METH_NOARGS,
   "return rep id"},
  {"root_info", (PyCFunction)CellPatGraph_root_info, METH_NOARGS,
   "return root info"},
  {"edge_list", (PyCFunction)CellPatGraph_edge_list, METH_NOARGS,
   "return edge-list"},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellPatGraphObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellPatGraph_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.PatGraph",                  // tp_name
  sizeof(CellPatGraphObject),           // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellPatGraph_dealloc,     // tp_dealloc
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
  "Pattern Graph",              // tp_doc

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
  CellPatGraph_methods,         // tp_methods
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
// PyObject と CellPatGraph の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief CellPatGraph から PyObject を作る．
// @param[in] pat_graph CellPatGraph へのポインタ
// @return pat_graph を表す PyObject
PyObject*
PyCellPatGraph_FromCellPatGraph(const CellPatGraph* pat_graph)
{
  CellPatGraphObject* self = PyObject_New(CellPatGraphObject, &PyCellPatGraph_Type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mPatGraph = pat_graph;

  return (PyObject*)self;
}

// @brief PyObject から CellPatGraph へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return CellPatGraph へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
const CellPatGraph*
PyCellPatGraph_AsCellPatGraphPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellPatGraph_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.PatGraph is expected");
    return NULL;
  }

  // 強制的にキャスト
  CellPatGraphObject* my_obj = (CellPatGraphObject*)py_obj;

  return my_obj->mPatGraph;
}

// CellPatGraphObject 関係の初期化を行なう．
void
CellPatGraphObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellPatGraph_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Cell", (PyObject*)&PyCellPatGraph_Type);
}

END_NAMESPACE_YM


/// @file pyigf_PhfGen.cc
/// @brief PhfGen の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "pyigf.h"
#include "PhfGen.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// PhfGen を表す型
struct PhfGenObject
{
  // Python のお約束
  PyObject_HEAD

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// PhfGenObject の生成関数
PhfGenObject*
PhfGen_new(PyTypeObject* type)
{
  PhfGenObject* self = PyObject_New(PhfGenObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  return self;
}

// PhfGenObject を開放する関数
void
PhfGen_dealloc(PhfGenObject* self)
{
  PyObject_Del(self);
}

// mapping 関数
PyObject*
PhfGen_mapping(PhfGenObject* self,
	       PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &PyList_Type, &obj) ) {
    return NULL;
  }
  ymuint n = PyList_GET_SIZE(obj);
  vector<const FuncVect*> func_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GET_ITEM(obj, i);
    if ( !PyFuncVect_Check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "igf.FuncVect is expected");
      return NULL;
    }
    FuncVect* fv = PyFuncVect_AsFuncVectPtr(obj1);
    func_list[i] = fv;
  }
  vector<vector<ymuint32> > g_list;
  PhfGen pg;
  bool stat = pg.mapping(func_list, g_list);
  if ( stat ) {
    PyObject* ans_obj = PyList_New(n);
    for (ymuint i = 0; i < n; ++ i) {
      const vector<ymuint32>& g_map = g_list[i];
      ymuint s = g_map.size();
      PyObject* map_obj = PyList_New(s);
      for (ymuint j = 0; j < s; ++ j) {
	PyObject* obj2 = PyObject_FromYmuint32(g_map[j]);
	PyList_SetItem(map_obj, j, obj2);
      }
      PyList_SetItem(ans_obj, i, map_obj);
    }
    return ans_obj;
  }
  else {
    Py_INCREF(Py_None);
    return Py_None;
  }
}

// cf_partition 関数
PyObject*
PhfGen_cf_partition(PhfGenObject* self,
		    PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &PyList_Type, &obj) ) {
    return NULL;
  }
  ymuint n = PyList_GET_SIZE(obj);
  vector<const FuncVect*> func_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GET_ITEM(obj, i);
    if ( !PyFuncVect_Check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "igf.FuncVect is expected");
      return NULL;
    }
    FuncVect* fv = PyFuncVect_AsFuncVectPtr(obj1);
    func_list[i] = fv;
  }
  vector<ymuint> block_map;
  PhfGen pg;
  bool stat = pg.cf_partition(func_list, block_map);
  if ( stat ) {
    ymuint ns = block_map.size();
    PyObject* ans_obj = PyList_New(ns);
    for (ymuint i = 0; i < ns; ++ i) {
      PyObject* obj2 = PyObject_FromYmuint32(block_map[i]);
      PyList_SetItem(ans_obj, i, obj2);
    }
    return ans_obj;
  }
  else {
    Py_INCREF(Py_None);
    return Py_None;
  }
}

// displace_decomposition 関数
PyObject*
PhfGen_displace_decomposition(PhfGenObject* self,
			      PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  ymuint use_xor = 0;
  if ( !PyArg_ParseTuple(args, "O!O!b",
			 &PyFuncVect_Type, &obj1,
			 &PyFuncVect_Type, &obj2,
			 &use_xor) ) {
    return NULL;
  }
  FuncVect* func1 = PyFuncVect_AsFuncVectPtr(obj1);
  FuncVect* func2 = PyFuncVect_AsFuncVectPtr(obj2);
  vector<ymuint> d_map;
  PhfGen pg;
  bool stat = pg.displace_decomposition(func1, func2, d_map, use_xor);
  if ( stat ) {
    ymuint n = d_map.size();
    PyObject* ans_obj = PyList_New(n);
    for (ymuint i = 0; i < n; ++ i) {
      PyObject* obj1 = PyObject_FromYmuint32(d_map[i]);
      PyList_SetItem(ans_obj, i, obj1);
    }
    return ans_obj;
  }
  else {
    Py_INCREF(Py_None);
    return Py_None;
  }
}


//////////////////////////////////////////////////////////////////////
// PhfGenObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef PhfGen_methods[] = {
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
  {"mapping", (PyCFunction)PhfGen_mapping, METH_VARARGS,
   PyDoc_STR("mapping ([FuncVect])")},
  {"cf_partition", (PyCFunction)PhfGen_cf_partition, METH_VARARGS,
   PyDoc_STR("cf_partition ([FuncVect])")},
  {"displace_decomposition", (PyCFunction)PhfGen_displace_decomposition, METH_VARARGS,
   PyDoc_STR("displace_decomposition (FuncVect, FuncVect, bool)")},
  {NULL, NULL, 0, NULL} // end-marker
};


END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// PhfGenObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyPhfGen_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "igf.PhfGen",                // tp_name
  sizeof(PhfGenObject),          // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)PhfGen_dealloc,    // tp_dealloc
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
  "PhfGen",             // tp_doc

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
  PhfGen_methods,                // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)0,                  // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)PhfGen_new,           // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と PhfGen の間の変換関数
//////////////////////////////////////////////////////////////////////

// PhfGenObject 関係の初期化を行う．
void
PhfGenObject_init(PyObject* m)
{
  if ( PyType_Ready(&PyPhfGen_Type) < 0 ) {
    return;
  }

  // タイプモジュールの登録を行う．
  PyModule_AddObject(m, "PhfGen", (PyObject*)&PyPhfGen_Type);
}

END_NAMESPACE_YM_IGF

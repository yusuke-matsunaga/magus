
/// @file pyigf_FuncVect.cc
/// @brief FuncVect の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "pyigf.h"
#include "FuncVect.h"
#include "YmUtils/pyutils.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// FuncVect を表す型
struct FuncVectObject
{
  // Python のお約束
  PyObject_HEAD

  // FuncVect の本体
  FuncVect* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// FuncVectObject の生成関数
FuncVectObject*
FuncVect_new(PyTypeObject* type)
{
  FuncVectObject* self = PyObject_New(FuncVectObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ここでは NULL のままにしておく．
  self->mBody = NULL;

  return self;
}

// FuncVectObject を開放する関数
void
FuncVect_dealloc(FuncVectObject* self)
{
  // FuncVect の開放を行なう．
  delete self->mBody;

  PyObject_Del(self);
}

// 初期化関数
int
FuncVect_init(FuncVectObject* self,
	      PyObject* args)
{
  // 引数の形式は
  // - (uint, uint)
  ymuint max_val;
  ymuint input_size;
  if ( !PyArg_ParseTuple(args, "II", &input_size, &max_val) ) {
    return NULL;
  }

  if ( self->mBody != NULL ) {
    delete self->mBody;
  }
  self->mBody = new FuncVect(input_size, max_val);

  // 正常に終了したら 0 を返す．
  return 0;
}

// max_val 関数
PyObject*
FuncVect_max_val(FuncVectObject* self,
		 PyObject* args)
{
  return PyObject_FromYmuint32(self->mBody->max_val());
}

// input_size 関数
PyObject*
FuncVect_input_size(FuncVectObject* self,
		    PyObject* args)
{
  return PyObject_FromYmuint32(self->mBody->input_size());
}

// val 関数
PyObject*
FuncVect_val(FuncVectObject* self,
	     PyObject* args)
{
  ymuint id;
  if ( !PyArg_ParseTuple(args, "I", &id) ) {
    return NULL;
  }

  return PyObject_FromYmuint32(self->mBody->val(id));
}

// set_val 関数
PyObject*
FuncVect_set_val(FuncVectObject* self,
		 PyObject* args)
{
  ymuint id;
  ymuint val;
  if ( !PyArg_ParseTuple(args, "II", &id, &val) ) {
    return NULL;
  }

  self->mBody->set_val(id, val);

  Py_INCREF(Py_None);
  return Py_None;
}

// set_random_val 関数
PyObject*
FuncVect_set_random_val(FuncVectObject* self,
			PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &PyRandGen_Type, &obj) ) {
    return NULL;
  }

  RandGen* rg = PyRandGen_AsRandGenPtr(obj);
  ymuint input_size = self->mBody->input_size();
  ymuint max_val = self->mBody->max_val();
  for (ymuint i = 0; i < input_size; ++ i) {
    ymuint val = rg->int32() % max_val;
    self->mBody->set_val(i, val);
  }

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// FuncVectObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef FuncVect_methods[] = {
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
  {"max_val", (PyCFunction)FuncVect_max_val, METH_NOARGS,
   PyDoc_STR("return max_val() (none)")},
  {"input_size", (PyCFunction)FuncVect_input_size, METH_NOARGS,
   PyDoc_STR("return input_size() (none)")},
  {"val", (PyCFunction)FuncVect_val, METH_VARARGS,
   PyDoc_STR("return val() (uint)")},
  {"set_val", (PyCFunction)FuncVect_set_val, METH_VARARGS,
   PyDoc_STR("set_val() (uint, uint)")},
  {"set_random_val", (PyCFunction)FuncVect_set_random_val, METH_VARARGS,
   PyDoc_STR("set_random_val(RandGen)")},
  {NULL, NULL, 0, NULL} // end-marker
};


END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// FuncVectObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyFuncVect_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "igf.FuncVect",                // tp_name
  sizeof(FuncVectObject),          // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)FuncVect_dealloc,    // tp_dealloc
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
  "Function Vector",             // tp_doc

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
  FuncVect_methods,                // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)FuncVect_init,         // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)FuncVect_new,           // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と FuncVect の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief FuncVect から Python オブジェクトを生成する．
// @param[in] obj FuncVect オブジェクト
// @return 変換した Python オブジェクトを返す．
PyObject*
PyFuncVect_FromFuncVect(const FuncVect& obj)
{
  FuncVectObject* py_obj = FuncVect_new(&PyFuncVect_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  ymuint max_val = obj.max_val();
  ymuint input_size = obj.input_size();
  py_obj->mBody = new FuncVect(input_size, max_val);
  for (ymuint i = 0; i < input_size; ++ i) {
    py_obj->mBody->set_val(i, obj.val(i));
  }

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief PyObject から FuncVect へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return FuncVect へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
FuncVect*
PyFuncVect_AsFuncVectPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyFuncVect_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "igf.FuncVect is expected");
    return NULL;
  }

  // 強制的にキャスト
  FuncVectObject* my_obj = (FuncVectObject*)py_obj;

  return my_obj->mBody;
}

// FuncVectObject 関係の初期化を行う．
void
FuncVectObject_init(PyObject* m)
{
  if ( PyType_Ready(&PyFuncVect_Type) < 0 ) {
    return;
  }

  // タイプモジュールの登録を行う．
  PyModule_AddObject(m, "FuncVect", (PyObject*)&PyFuncVect_Type);
}

END_NAMESPACE_YM_IGF

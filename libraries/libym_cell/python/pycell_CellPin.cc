
/// @file pycell_CellPin.cc
/// @brief CellPin の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/CellPin.h"
#include "PyInputPin.h"
#include "PyOutputPin.h"
#include "PyInoutPin.h"
#include "PyInternalPin.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// 定数オブジェクト
PyObject* kStrInput = NULL;
PyObject* kStrOutput = NULL;
PyObject* kStrInout = NULL;
PyObject* kStrInternal = NULL;

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellPin を表す型
struct CellPinObject
{
  // Python のお約束
  PyObject_HEAD

  // PyPin
  PyPin* mPin;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellPinObject を開放する関数
void
CellPin_dealloc(CellPinObject* self)
{
  delete self->mPin;

  PyObject_Del(self);
}

// id 関数
PyObject*
CellPin_id(CellPinObject* self,
	   PyObject* args)
{
  PyObject* result = self->mPin->id();

  Py_INCREF(result);
  return result;
}

// name 関数
PyObject*
CellPin_name(CellPinObject* self,
	     PyObject* args)
{
  PyObject* result = self->mPin->name();

  Py_INCREF(result);
  return result;
}

// direction 関数
PyObject*
CellPin_direction(CellPinObject* self,
		  PyObject* args)
{
  PyObject* result = NULL;
  switch ( self->mPin->pin()->direction() ) {
  case CellPin::kDirInput:    result = kStrInput; break;
  case CellPin::kDirOutput:   result = kStrOutput; break;
  case CellPin::kDirInout:    result = kStrInout; break;
  case CellPin::kDirInternal: result = kStrInternal; break;
  }

  Py_INCREF(result);
  return result;
}

// input_id 関数
PyObject*
CellPin_input_id(CellPinObject* self,
		 PyObject* args)
{
  PyObject* result = self->mPin->input_id();

  Py_INCREF(result);
  return result;
}

// capacitance 関数
PyObject*
CellPin_capacitance(CellPinObject* self,
		    PyObject* args)
{
  PyObject* result = self->mPin->capacitance();

  Py_INCREF(result);
  return result;
}

// rise_capacitance 関数
PyObject*
CellPin_rise_capacitance(CellPinObject* self,
			 PyObject* args)
{
  PyObject* result = self->mPin->rise_capacitance();

  Py_INCREF(result);
  return result;
}

// fall_capacitance 関数
PyObject*
CellPin_fall_capacitance(CellPinObject* self,
			 PyObject* args)
{
  PyObject* result = self->mPin->fall_capacitance();

  Py_INCREF(result);
  return result;
}

// output_id 関数
PyObject*
CellPin_output_id(CellPinObject* self,
		  PyObject* args)
{
  PyObject* result = self->mPin->output_id();

  Py_INCREF(result);
  return result;
}

// function 関数
PyObject*
CellPin_function(CellPinObject* self,
		 PyObject* args)
{
  PyObject* result = self->mPin->function();

  Py_INCREF(result);
  return result;
}

// three_state 関数
PyObject*
CellPin_three_state(CellPinObject* self,
		    PyObject* args)
{
  PyObject* result = self->mPin->three_state();

  Py_INCREF(result);
  return result;
}

// max_fanout 関数
PyObject*
CellPin_max_fanout(CellPinObject* self,
		   PyObject* args)
{
  PyObject* result = self->mPin->max_fanout();

  Py_INCREF(result);
  return result;
}

// min_fanout 関数
PyObject*
CellPin_min_fanout(CellPinObject* self,
		   PyObject* args)
{
  PyObject* result = self->mPin->min_fanout();

  Py_INCREF(result);
  return result;
}

// max_capacitance 関数
PyObject*
CellPin_max_capacitance(CellPinObject* self,
			PyObject* args)
{
  PyObject* result = self->mPin->max_capacitance();

  Py_INCREF(result);
  return result;
}

// min_capacitance 関数
PyObject*
CellPin_min_capacitance(CellPinObject* self,
			PyObject* args)
{
  PyObject* result = self->mPin->min_capacitance();

  Py_INCREF(result);
  return result;
}

// max_transition 関数
PyObject*
CellPin_max_transition(CellPinObject*  self,
		       PyObject* args)
{
  PyObject* result = self->mPin->max_transition();

  Py_INCREF(result);
  return result;
}

// min_transition 関数
PyObject*
CellPin_min_transition(CellPinObject*  self,
		       PyObject* args)
{
  PyObject* result = self->mPin->min_transition();

  Py_INCREF(result);
  return result;
}

// internal_id 関数
PyObject*
CellPin_internal_id(CellPinObject* self,
		    PyObject* args)
{
  PyObject* result = self->mPin->internal_id();

  Py_INCREF(result);
  return result;
}


//////////////////////////////////////////////////////////////////////
// CellPinObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellPin_methods[] = {
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
  {"id", (PyCFunction)CellPin_id, METH_NOARGS,
   PyDoc_STR("return ID")},
  {"name", (PyCFunction)CellPin_name, METH_NOARGS,
   PyDoc_STR("return name")},
  {"direction", (PyCFunction)CellPin_direction, METH_NOARGS,
   PyDoc_STR("return direction(pin-type)")},

  {"input_id", (PyCFunction)CellPin_input_id, METH_NOARGS,
   PyDoc_STR("return input ID")},
  {"capacitance", (PyCFunction)CellPin_capacitance, METH_NOARGS,
   PyDoc_STR("return capacitance")},
  {"rise_capacitance", (PyCFunction)CellPin_rise_capacitance, METH_NOARGS,
   PyDoc_STR("return rise capacitance")},
  {"fall_capacitance", (PyCFunction)CellPin_fall_capacitance, METH_NOARGS,
   PyDoc_STR("return fall capacitance")},

  {"output_id", (PyCFunction)CellPin_output_id, METH_NOARGS,
   PyDoc_STR("return output ID")},
  {"function", (PyCFunction)CellPin_function, METH_NOARGS,
   PyDoc_STR("return function expression")},
  {"three_state", (PyCFunction)CellPin_three_state, METH_NOARGS,
   PyDoc_STR("return tristate condition")},
  {"max_fanout", (PyCFunction)CellPin_max_fanout, METH_NOARGS,
   PyDoc_STR("return max fanout")},
  {"min_fanout", (PyCFunction)CellPin_min_fanout, METH_NOARGS,
   PyDoc_STR("return min fanout")},
  {"max_capacitance", (PyCFunction)CellPin_max_capacitance, METH_NOARGS,
   PyDoc_STR("return max capacitance")},
  {"min_capacitance", (PyCFunction)CellPin_min_capacitance, METH_NOARGS,
   PyDoc_STR("return min capacitance")},
  {"max_transition", (PyCFunction)CellPin_max_transition, METH_NOARGS,
   PyDoc_STR("return max transition time")},
  {"min_transition", (PyCFunction)CellPin_min_transition, METH_NOARGS,
   PyDoc_STR("return min transition time")},

  {"internal_id", (PyCFunction)CellPin_internal_id, METH_NOARGS,
   PyDoc_STR("return internal ID")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellPinObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellPin_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellPin",           // tp_name
  sizeof(CellPinObject),        // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellPin_dealloc,  // tp_dealloc
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
  "Cell Pin",                   // tp_doc

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
  CellPin_methods,              // tp_methods
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
// PyObject と CellPin の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief CellPin から PyObject を作る．
// @param[in] pin CellPin へのポインタ
// @return pin を表す PyObject
PyObject*
PyCellPin_FromCellPin(const CellPin* pin)
{
  CellPinObject* self = PyObject_New(CellPinObject, &PyCellPin_Type);
  if ( self == NULL ) {
    return NULL;
  }

  PyPin* pypin = NULL;
  switch ( pin->direction() ){
  case CellPin::kDirInput:    pypin = new PyInputPin(pin); break;
  case CellPin::kDirOutput:   pypin = new PyOutputPin(pin); break;
  case CellPin::kDirInout:    pypin = new PyInoutPin(pin); break;
  case CellPin::kDirInternal: pypin = new PyInternalPin(pin); break;
  }
  self->mPin = pypin;

  Py_INCREF(self);
  return (PyObject*)self;
}

// @brief PyObject から CellPin へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return CellPin へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
const CellPin*
PyCellPin_AsCellPinPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellPin_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellPin is expected");
    return NULL;
  }

  // 強制的にキャスト
  CellPinObject* my_obj = (CellPinObject*)py_obj;

  return my_obj->mPin->pin();
}

// CellPinObject 関係の初期化を行う．
void
CellPinObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellPin_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellPin", (PyObject*)&PyCellPin_Type);
}

END_NAMESPACE_YM

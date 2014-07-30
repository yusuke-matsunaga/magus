
/// @file pycell_CellPin.cc
/// @brief CellPin の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_YmCell/pycell.h"
#include "ym_YmCell/CellPin.h"
#include "ym_YmCell/CellCapacitance.h"
#include "ym_YmCell/CellTime.h"
#include "ym_YmLogic/pylogic.h"
#include "ym_YmLogic/Expr.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellPin を表す型
struct CellPinObject
{
  // Python のお約束
  PyObject_HEAD

  // CellPin
  const CellPin* mPin;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellPinObject を開放する関数
void
CellPin_dealloc(CellPinObject* self)
{
  PyObject_Del(self);
}

// id 関数
PyObject*
CellPin_id(CellPinObject* self,
	   PyObject* args)
{
  return PyObject_FromYmuint32(self->mPin->pin_id());
}

// name 関数
PyObject*
CellPin_name(CellPinObject* self,
	     PyObject* args)
{
  return PyObject_FromString(self->mPin->name());
}

// direction 関数
PyObject*
CellPin_direction(CellPinObject* self,
		  PyObject* args)
{
  return PyCellPinDir_FromCellPinDir(self->mPin->direction());
}

// input_id 関数
PyObject*
CellPin_input_id(CellPinObject* self,
		 PyObject* args)
{
  return PyObject_FromYmuint32(self->mPin->input_id());
}

// capacitance 関数
PyObject*
CellPin_capacitance(CellPinObject* self,
		    PyObject* args)
{
  return PyCellCapacitance_FromCellCapacitance(self->mPin->capacitance());
}

// rise_capacitance 関数
PyObject*
CellPin_rise_capacitance(CellPinObject* self,
			 PyObject* args)
{
  return PyCellCapacitance_FromCellCapacitance(self->mPin->rise_capacitance());
}

// fall_capacitance 関数
PyObject*
CellPin_fall_capacitance(CellPinObject* self,
			 PyObject* args)
{
  return PyCellCapacitance_FromCellCapacitance(self->mPin->fall_capacitance());
}

// output_id 関数
PyObject*
CellPin_output_id(CellPinObject* self,
		  PyObject* args)
{
  return PyObject_FromYmuint32(self->mPin->output_id());
}

// has_function 関数
PyObject*
CellPin_has_function(CellPinObject* self,
		     PyObject* args)
{
  return PyObject_FromBool(self->mPin->has_function());
}

// function 関数
PyObject*
CellPin_function(CellPinObject* self,
		 PyObject* args)
{
  if ( !self->mPin->has_function() ) {
    PyErr_SetString(PyExc_ValueError, "No function.");
    return NULL;
  }
  return PyExpr_FromExpr(self->mPin->function());
}

// has_three_state 関数
PyObject*
CellPin_has_three_state(CellPinObject* self,
			PyObject* args)
{
  return PyObject_FromBool(self->mPin->has_three_state());
}

// three_state 関数
PyObject*
CellPin_three_state(CellPinObject* self,
		    PyObject* args)
{
  if ( !self->mPin->has_three_state() ) {
    PyErr_SetString(PyExc_ValueError, "No three state condition");
    return NULL;
  }
  return PyExpr_FromExpr(self->mPin->three_state());
}

// max_fanout 関数
PyObject*
CellPin_max_fanout(CellPinObject* self,
		   PyObject* args)
{
  return PyCellCapacitance_FromCellCapacitance(self->mPin->max_fanout());
}

// min_fanout 関数
PyObject*
CellPin_min_fanout(CellPinObject* self,
		   PyObject* args)
{
  return PyCellCapacitance_FromCellCapacitance(self->mPin->min_fanout());
}

// max_capacitance 関数
PyObject*
CellPin_max_capacitance(CellPinObject* self,
			PyObject* args)
{
  return PyCellCapacitance_FromCellCapacitance(self->mPin->max_capacitance());
}

// min_capacitance 関数
PyObject*
CellPin_min_capacitance(CellPinObject* self,
			PyObject* args)
{
  return PyCellCapacitance_FromCellCapacitance(self->mPin->min_capacitance());
}

// max_transition 関数
PyObject*
CellPin_max_transition(CellPinObject*  self,
		       PyObject* args)
{
  return PyCellTime_FromCellTime(self->mPin->max_transition());
}

// min_transition 関数
PyObject*
CellPin_min_transition(CellPinObject*  self,
		       PyObject* args)
{
  return PyCellTime_FromCellTime(self->mPin->min_transition());
}

// internal_id 関数
PyObject*
CellPin_internal_id(CellPinObject* self,
		    PyObject* args)
{
  return PyObject_FromYmuint32(self->mPin->internal_id());
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
  {"has_function", (PyCFunction)CellPin_has_function, METH_NOARGS,
   PyDoc_STR("return True if having a function expression")},
  {"has_three_state", (PyCFunction)CellPin_has_three_state, METH_NOARGS,
   PyDoc_STR("return True if having a tristate condition")},
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

  self->mPin = pin;

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

  return my_obj->mPin;
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

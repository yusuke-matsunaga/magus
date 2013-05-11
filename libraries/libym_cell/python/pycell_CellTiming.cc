
/// @file pycell_CellTiming.cc
/// @brief CellTiming の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/CellTiming.h"
#include "ym_logic/LogExpr.h"
#include "ym_logic/LogExprWriter.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellTiming を表す型
struct CellTimingObject
{
  // Python のお約束
  PyObject_HEAD

  // CellTiming
  const CellTiming* mTiming;

  // 立ち上がり遷移遅延テーブル
  PyObject* mRiseTransition;

  // 立ち下がり遷移遅延テーブル
  PyObject* mFallTransition;

  // 立ち上がり伝搬遅延テーブル
  PyObject* mRisePropagation;

  // 立ち下がり伝搬遅延テーブル
  PyObject* mFallPropagation;

  // 立ち上がりセル遅延テーブル
  PyObject* mCellRise;

  // 立ち下がりセル遅延テーブル
  PyObject* mCellFall;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellTimingObject を開放する関数
void
CellTiming_dealloc(CellTimingObject* self)
{
  Py_XDECREF(self->mRiseTransition);
  Py_XDECREF(self->mFallTransition);
  Py_XDECREF(self->mRisePropagation);
  Py_XDECREF(self->mFallPropagation);
  Py_XDECREF(self->mCellRise);
  Py_XDECREF(self->mCellFall);

  PyObject_Del(self);
}

// type 関数
PyObject*
CellTiming_type(CellTimingObject* self,
		PyObject* args)
{
  return PyCellTimingType_FromCellTimingType(self->mTiming->type());
}

// timing_cond 関数
PyObject*
CellTiming_timing_cond(CellTimingObject* self,
		       PyObject* args)
{
  LogExprWriter lew;
  string func_str = lew.dump_string(self->mTiming->timing_cond());
  return PyObject_FromString(func_str);
}

// intrinsic_rise 関数
PyObject*
CellTiming_intrinsic_rise(CellTimingObject* self,
			  PyObject* args)
{
  return PyCellTime_FromCellTime(self->mTiming->intrinsic_rise());
}

// intrinsic_fall 関数
PyObject*
CellTiming_intrinsic_fall(CellTimingObject* self,
			  PyObject* args)
{
  return PyCellTime_FromCellTime(self->mTiming->intrinsic_fall());
}

// slope_rise 関数
PyObject*
CellTiming_slope_rise(CellTimingObject* self,
		      PyObject* args)
{
  return PyCellTime_FromCellTime(self->mTiming->slope_rise());
}

// slope_fall 関数
PyObject*
CellTiming_slope_fall(CellTimingObject* self,
		      PyObject* args)
{
  return PyCellTime_FromCellTime(self->mTiming->slope_fall());
}

// rise_resistance 関数
PyObject*
CellTiming_rise_resistance(CellTimingObject* self,
			   PyObject* args)
{
  return PyCellResistance_FromCellResistance(self->mTiming->rise_resistance());
}

// fall_resistance 関数
PyObject*
CellTiming_fall_resistance(CellTimingObject* self,
			   PyObject* args)
{
  return PyCellResistance_FromCellResistance(self->mTiming->fall_resistance());
}

// rise_pin_resistance 関数
PyObject*
CellTiming_rise_pin_resistance(CellTimingObject* self,
			       PyObject* args)
{
  return PyCellResistance_FromCellResistance(self->mTiming->rise_pin_resistance());
}

// fall_pin_resistance 関数
PyObject*
CellTiming_fall_pin_resistance(CellTimingObject* self,
			       PyObject* args)
{
  return PyCellResistance_FromCellResistance(self->mTiming->fall_pin_resistance());
}

// rise_delay_intercept 関数
PyObject*
CellTiming_rise_delay_intercept(CellTimingObject* self,
				PyObject* args)
{
  return PyCellTime_FromCellTime(self->mTiming->rise_delay_intercept());
}

// fall_delay_intercept 関数
PyObject*
CellTiming_fall_delay_intercept(CellTimingObject* self,
				PyObject* args)
{
  return PyCellTime_FromCellTime(self->mTiming->fall_delay_intercept());
}

// rise_transition 関数
PyObject*
CellTiming_rise_transition(CellTimingObject* self,
			   PyObject* args)
{
  PyObject* result = self->mRiseTransition;

  if ( result == NULL ) {
    PyErr_SetString(PyExc_TypeError, "No LUT for rise_transition");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// fall_transition 関数
PyObject*
CellTiming_fall_transition(CellTimingObject* self,
			   PyObject* args)
{
  PyObject* result = self->mFallTransition;

  if ( result == NULL ) {
    PyErr_SetString(PyExc_TypeError, "No LUT for fall_transition");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// rise_propagation 関数
PyObject*
CellTiming_rise_propagation(CellTimingObject* self,
			    PyObject* args)
{
  PyObject* result = self->mRisePropagation;

  if ( result == NULL ) {
    PyErr_SetString(PyExc_TypeError, "No LUT for rise_propagation");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// fall_propagation 関数
PyObject*
CellTiming_fall_propagation(CellTimingObject* self,
			    PyObject* args)
{
  PyObject* result = self->mFallPropagation;

  if ( result == NULL ) {
    PyErr_SetString(PyExc_TypeError, "No LUT for fall_propagation");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// cell_rise 関数
PyObject*
CellTiming_cell_rise(CellTimingObject* self,
		     PyObject* args)
{
  PyObject* result = self->mCellRise;

  if ( result == NULL ) {
    PyErr_SetString(PyExc_TypeError, "No LUT for cell_rise");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// cell_fall 関数
PyObject*
CellTiming_cell_fall(CellTimingObject* self,
		     PyObject* args)
{
  PyObject* result = self->mCellFall;

  if ( result == NULL ) {
    PyErr_SetString(PyExc_TypeError, "No LUT for cell_fall");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}


//////////////////////////////////////////////////////////////////////
// CellTimingObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellTiming_methods[] = {
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

  {"type", (PyCFunction)CellTiming_type, METH_NOARGS,
   PyDoc_STR("return timing type")},
  {"timing_cond", (PyCFunction)CellTiming_timing_cond, METH_NOARGS,
   PyDoc_STR("return timing condition")},
  {"intrinsic_rise", (PyCFunction)CellTiming_intrinsic_rise, METH_NOARGS,
   PyDoc_STR("return 'intrinsic rise'")},
  {"intrinsic_fall", (PyCFunction)CellTiming_intrinsic_fall, METH_NOARGS,
   PyDoc_STR("return 'intrinsic fall'")},
  {"slope_rise", (PyCFunction)CellTiming_slope_rise, METH_NOARGS,
   PyDoc_STR("return 'slope rise'")},
  {"slope_fall", (PyCFunction)CellTiming_slope_fall, METH_NOARGS,
   PyDoc_STR("return 'slope fall'")},
  {"rise_resistance", (PyCFunction)CellTiming_rise_resistance, METH_NOARGS,
   PyDoc_STR("return 'rise resistance'")},
  {"fall_resistance", (PyCFunction)CellTiming_fall_resistance, METH_NOARGS,
   PyDoc_STR("return 'fall resistance'")},
  {"rise_pin_resistance", (PyCFunction)CellTiming_rise_pin_resistance, METH_NOARGS,
   PyDoc_STR("return 'rise pin resistance'")},
  {"fall_pin_resistance", (PyCFunction)CellTiming_fall_pin_resistance, METH_NOARGS,
   PyDoc_STR("return 'fall pin resistance'")},
  {"rise_delay_intercept", (PyCFunction)CellTiming_rise_delay_intercept, METH_NOARGS,
   PyDoc_STR("return 'rise delay intercept'")},
  {"fall_delay_intercept", (PyCFunction)CellTiming_fall_delay_intercept, METH_NOARGS,
   PyDoc_STR("return 'fall delay intercept'")},
  {"rise_transition", (PyCFunction)CellTiming_rise_transition, METH_NOARGS,
   PyDoc_STR("return 'rise transition'")},
  {"fall_transition", (PyCFunction)CellTiming_fall_transition, METH_NOARGS,
   PyDoc_STR("return 'fall transition'")},
  {"rise_propagation", (PyCFunction)CellTiming_rise_propagation, METH_NOARGS,
   PyDoc_STR("return 'rise propagation'")},
  {"fall_propagation", (PyCFunction)CellTiming_fall_propagation, METH_NOARGS,
   PyDoc_STR("return 'fall propagation'")},
  {"cell_rise", (PyCFunction)CellTiming_cell_rise, METH_NOARGS,
   PyDoc_STR("return 'cell rise'")},
  {"cell_fall", (PyCFunction)CellTiming_cell_fall, METH_NOARGS,
   PyDoc_STR("return 'cell fall'")},

  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellTimingObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellTiming_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellTiming",        // tp_name
  sizeof(CellTimingObject),     // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellTiming_dealloc,     // tp_dealloc
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
  "Cell Timing",                // tp_doc

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
  CellTiming_methods,           // tp_methods
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
// PyObject と CellTiming の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief CellTiming から PyObject を作る．
// @param[in] timing CellTiming へのポインタ
// @return timing を表す PyObject
PyObject*
PyCellTiming_FromCellTiming(const CellTiming* timing)
{
  CellTimingObject* self = PyObject_New(CellTimingObject, &PyCellTiming_Type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mTiming = timing;

  if ( timing->rise_transition() != NULL ) {
    assert_cond( timing->fall_transition() != NULL, __FILE__, __LINE__);
    self->mRiseTransition = PyCellLut_FromCellLut(timing->rise_transition());
    self->mFallTransition = PyCellLut_FromCellLut(timing->fall_transition());
    if ( timing->rise_propagation() != NULL ) {
      assert_cond( timing->fall_propagation() != NULL, __FILE__, __LINE__);
      assert_cond( timing->cell_rise() == NULL, __FILE__, __LINE__);
      assert_cond( timing->cell_fall() == NULL, __FILE__, __LINE__);
      self->mRisePropagation = PyCellLut_FromCellLut(timing->rise_propagation());
      self->mFallPropagation = PyCellLut_FromCellLut(timing->fall_propagation());
      self->mCellRise = NULL;
      self->mCellFall = NULL;
    }
    else {
      assert_cond( timing->cell_rise() != NULL, __FILE__, __LINE__);
      assert_cond( timing->cell_fall() != NULL, __FILE__, __LINE__);
      assert_cond( timing->fall_propagation() == NULL, __FILE__, __LINE__);
      self->mCellRise = PyCellLut_FromCellLut(timing->cell_rise());
      self->mCellFall = PyCellLut_FromCellLut(timing->cell_fall());
      self->mRisePropagation = NULL;
      self->mFallPropagation = NULL;
    }
  }
  else {
    self->mRiseTransition = NULL;
    self->mFallTransition = NULL;
    self->mRisePropagation = NULL;
    self->mFallPropagation = NULL;
    self->mCellRise = NULL;
    self->mCellFall = NULL;
  }

  Py_INCREF(self);
  return (PyObject*)self;
}

// @brief PyObject から CellTiming へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return CellTiming へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
const CellTiming*
PyCellTiming_AsCellTimingPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellTiming_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellTiming is expected.");
    return NULL;
  }

  // 強制的にキャスト
  CellTimingObject* my_obj = (CellTimingObject*)py_obj;

  return my_obj->mTiming;
}

// CellTiming 関係の初期化を行なう．
void
CellTimingObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellTiming_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellTiming", (PyObject*)&PyCellTiming_Type);
}

END_NAMESPACE_YM

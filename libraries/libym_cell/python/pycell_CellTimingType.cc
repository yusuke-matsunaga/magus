
/// @file pycell_CellTimingType.cc
/// @brief tCellTimingType の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_YmCell/pycell.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// tCellTimingType を表す型
struct CellTimingTypeObject
{
  // Python のお約束
  PyObject_HEAD

  // 実際の値
  tCellTimingType mVal;

};

// kCellTimingCombinational の実体
CellTimingTypeObject kCellTimingCombinationalStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingCombinational
};

// kCellTimingCombinationalRise の実体
CellTimingTypeObject kCellTimingCombinationalRiseStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingCombinationalRise
};

// kCellTimingCombinationalFall の実体
CellTimingTypeObject kCellTimingCombinationalFallStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingCombinationalFall
};

// kCellTimingThreeStateEnable の実体
CellTimingTypeObject kCellTimingThreeStateEnableStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingThreeStateEnable
};

// kCellTimingThreeStateDisable の実体
CellTimingTypeObject kCellTimingThreeStateDisableStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingThreeStateDisable
};

// kCellTimingThreeStateEnableRise の実体
CellTimingTypeObject kCellTimingThreeStateEnableRiseStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingThreeStateEnableRise
};

// kCellTimingThreeStateEnableFall の実体
CellTimingTypeObject kCellTimingThreeStateEnableFallStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingThreeStateEnableFall
};

// kCellTimingThreeStateDisableRise の実体
CellTimingTypeObject kCellTimingThreeStateDisableRiseStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingThreeStateDisableRise
};

// kCellTimingThreeStateDisableFall の実体
CellTimingTypeObject kCellTimingThreeStateDisableFallStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingThreeStateDisableFall
};

// kCellTimingRisingEdge の実体
CellTimingTypeObject kCellTimingRisingEdgeStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingRisingEdge
};

// kCellTimingFallingEdge の実体
CellTimingTypeObject kCellTimingFallingEdgeStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingFallingEdge
};

// kCellTimingPreset の実体
CellTimingTypeObject kCellTimingPresetStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingPreset
};

// kCellTimingClear の実体
CellTimingTypeObject kCellTimingClearStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingClear
};

// kCellTimingHoldRising の実体
CellTimingTypeObject kCellTimingHoldRisingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingHoldRising
};

// kCellTimingHoldFalling の実体
CellTimingTypeObject kCellTimingHoldFallingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingHoldFalling
};

// kCellTimingSetupRising の実体
CellTimingTypeObject kCellTimingSetupRisingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingSetupRising
};

// kCellTimingSetupFalling の実体
CellTimingTypeObject kCellTimingSetupFallingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingSetupFalling
};

// kCellTimingRecoveryRising の実体
CellTimingTypeObject kCellTimingRecoveryRisingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingRecoveryRising
};

// kCellTimingRecoveryFalling の実体
CellTimingTypeObject kCellTimingRecoveryFallingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingRecoveryFalling
};

// kCellTimingSkewRising の実体
CellTimingTypeObject kCellTimingSkewRisingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingSkewRising
};

// kCellTimingSkewFalling の実体
CellTimingTypeObject kCellTimingSkewFallingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingSkewFalling
};

// kCellTimingRemovalRising の実体
CellTimingTypeObject kCellTimingRemovalRisingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingRemovalRising
};

// kCellTimingRemovalFalling の実体
CellTimingTypeObject kCellTimingRemovalFallingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingRemovalFalling
};

// kCellTimingNonSeqSetupRising の実体
CellTimingTypeObject kCellTimingNonSeqSetupRisingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingNonSeqSetupRising
};

// kCellTimingNonSeqSetupFalling の実体
CellTimingTypeObject kCellTimingNonSeqSetupFallingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingNonSeqSetupFalling
};

// kCellTimingNonSeqHoldRising の実体
CellTimingTypeObject kCellTimingNonSeqHoldRisingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingNonSeqHoldRising
};

// kCellTimingNonSeqHoldFalling の実体
CellTimingTypeObject kCellTimingNonSeqHoldFallingStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingNonSeqHoldFalling
};

// kCellTimingNochangeHighHigh の実体
CellTimingTypeObject kCellTimingNochangeHighHighStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingNochangeHighHigh
};

// kCellTimingNochangeHighLow の実体
CellTimingTypeObject kCellTimingNochangeHighLowStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingNochangeHighLow
};

// kCellTimingNochangeLowHigh の実体
CellTimingTypeObject kCellTimingNochangeLowHighStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingNochangeLowHigh
};

// kCellTimingNochangeLowLow の実体
CellTimingTypeObject kCellTimingNochangeLowLowStruct = {
  PyObject_HEAD_INIT(&PyCellTimingType_Type)
  kCellTimingNochangeLowLow
};

// repr 用の文字列オブジェクト
PyObject* kCellTimingCombinationalString         = NULL;
PyObject* kCellTimingCombinationalRiseString     = NULL;
PyObject* kCellTimingCombinationalFallString     = NULL;
PyObject* kCellTimingThreeStateEnableString      = NULL;
PyObject* kCellTimingThreeStateDisableString     = NULL;
PyObject* kCellTimingThreeStateEnableRiseString  = NULL;
PyObject* kCellTimingThreeStateEnableFallString  = NULL;
PyObject* kCellTimingThreeStateDisableRiseString = NULL;
PyObject* kCellTimingThreeStateDisableFallString = NULL;
PyObject* kCellTimingRisingEdgeString            = NULL;
PyObject* kCellTimingFallingEdgeString           = NULL;
PyObject* kCellTimingPresetString                = NULL;
PyObject* kCellTimingClearString                 = NULL;
PyObject* kCellTimingHoldRisingString            = NULL;
PyObject* kCellTimingHoldFallingString           = NULL;
PyObject* kCellTimingSetupRisingString           = NULL;
PyObject* kCellTimingSetupFallingString          = NULL;
PyObject* kCellTimingRecoveryRisingString        = NULL;
PyObject* kCellTimingRecoveryFallingString       = NULL;
PyObject* kCellTimingSkewRisingString            = NULL;
PyObject* kCellTimingSkewFallingString           = NULL;
PyObject* kCellTimingRemovalRisingString         = NULL;
PyObject* kCellTimingRemovalFallingString        = NULL;
PyObject* kCellTimingNonSeqSetupRisingString     = NULL;
PyObject* kCellTimingNonSeqSetupFallingString    = NULL;
PyObject* kCellTimingNonSeqHoldRisingString      = NULL;
PyObject* kCellTimingNonSeqHoldFallingString     = NULL;
PyObject* kCellTimingNochangeHighHighString      = NULL;
PyObject* kCellTimingNochangeHighLowString       = NULL;
PyObject* kCellTimingNochangeLowHighString       = NULL;
PyObject* kCellTimingNochangeLowLowString        = NULL;

// 個々の定数を表す文字列
const char* kCellTimingCombinationalName         = "comibinational";
const char* kCellTimingCombinationalRiseName     = "combinational_rise";
const char* kCellTimingCombinationalFallName     = "combinational_fall";
const char* kCellTimingThreeStateEnableName      = "three_state_enable";
const char* kCellTimingThreeStateDisableName     = "three_state_disable";
const char* kCellTimingThreeStateEnableRiseName  = "three_state_enable_rise";
const char* kCellTimingThreeStateEnableFallName  = "three_state_enable_fall";
const char* kCellTimingThreeStateDisableRiseName = "three_state_disable_rise";
const char* kCellTimingThreeStateDisableFallName = "three_state_disable_fall";
const char* kCellTimingRisingEdgeName            = "rising_edge";
const char* kCellTimingFallingEdgeName           = "falling_edge";
const char* kCellTimingPresetName                = "preset";
const char* kCellTimingClearName                 = "clear";
const char* kCellTimingHoldRisingName            = "hold_rising";
const char* kCellTimingHoldFallingName           = "hold_falling";
const char* kCellTimingSetupRisingName           = "setup_rising";
const char* kCellTimingSetupFallingName          = "setup_falling";
const char* kCellTimingRecoveryRisingName        = "recovery_rising";
const char* kCellTimingRecoveryFallingName       = "recovery_falling";
const char* kCellTimingSkewRisingName            = "skew_rising";
const char* kCellTimingSkewFallingName           = "skew_falling";
const char* kCellTimingRemovalRisingName         = "removal_rising";
const char* kCellTimingRemovalFallingName        = "removal_falling";
const char* kCellTimingNonSeqSetupRisingName     = "non_seq_setup_rising";
const char* kCellTimingNonSeqSetupFallingName    = "non_seq_setup_falling";
const char* kCellTimingNonSeqHoldRisingName      = "non_seq_hold_rising";
const char* kCellTimingNonSeqHoldFallingName     = "non_seq_hold_falling";
const char* kCellTimingNochangeHighHighName      = "nochange_high_high";
const char* kCellTimingNochangeHighLowName       = "nochange_high_low";
const char* kCellTimingNochangeLowHighName       = "nochange_low_high";
const char* kCellTimingNochangeLowLowName        = "nochange_low_low";


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellTimingTypeObject の生成関数
// 実際には既存のオブジェクトを返す．
CellTimingTypeObject*
CellTimingType_new(PyTypeObject* type,
		   PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }

  return (CellTimingTypeObject*)PyCellTimingType_FromString(str);
}

// CellTimingTypeObject を開放する関数
void
CellTimingType_dealloc(CellTimingTypeObject* self)
{
  PyObject_Del(self);
}

// repr 関数
PyObject*
CellTimingType_repr(CellTimingTypeObject* self)
{
  PyObject* result = NULL;
  switch ( self->mVal ) {
  case kCellTimingCombinational:         result = kCellTimingCombinationalString; break;
  case kCellTimingCombinationalRise:     result = kCellTimingCombinationalRiseString; break;
  case kCellTimingCombinationalFall:     result = kCellTimingCombinationalFallString; break;
  case kCellTimingThreeStateEnable:      result = kCellTimingThreeStateEnableString; break;
  case kCellTimingThreeStateDisable:     result = kCellTimingThreeStateDisableString; break;
  case kCellTimingThreeStateEnableRise:  result = kCellTimingThreeStateEnableRiseString; break;
  case kCellTimingThreeStateEnableFall:  result = kCellTimingThreeStateEnableFallString; break;
  case kCellTimingThreeStateDisableRise: result = kCellTimingThreeStateDisableRiseString; break;
  case kCellTimingThreeStateDisableFall: result = kCellTimingThreeStateDisableFallString; break;
  case kCellTimingRisingEdge:            result = kCellTimingRisingEdgeString; break;
  case kCellTimingFallingEdge:           result = kCellTimingFallingEdgeString; break;
  case kCellTimingPreset:                result = kCellTimingPresetString; break;
  case kCellTimingClear:                 result = kCellTimingClearString; break;
  case kCellTimingHoldRising:            result = kCellTimingHoldRisingString; break;
  case kCellTimingHoldFalling:           result = kCellTimingHoldFallingString; break;
  case kCellTimingSetupRising:           result = kCellTimingSetupRisingString; break;
  case kCellTimingSetupFalling:          result = kCellTimingSetupFallingString; break;
  case kCellTimingRecoveryRising:        result = kCellTimingRecoveryRisingString; break;
  case kCellTimingRecoveryFalling:       result = kCellTimingRecoveryFallingString; break;
  case kCellTimingSkewRising:            result = kCellTimingSkewRisingString; break;
  case kCellTimingSkewFalling:           result = kCellTimingSkewFallingString; break;
  case kCellTimingRemovalRising:         result = kCellTimingRemovalRisingString; break;
  case kCellTimingRemovalFalling:        result = kCellTimingRemovalFallingString; break;
  case kCellTimingNonSeqSetupRising:     result = kCellTimingNonSeqSetupRisingString; break;
  case kCellTimingNonSeqSetupFalling:    result = kCellTimingNonSeqSetupFallingString; break;
  case kCellTimingNonSeqHoldRising:      result = kCellTimingNonSeqHoldRisingString; break;
  case kCellTimingNonSeqHoldFalling:     result = kCellTimingNonSeqHoldFallingString; break;
  case kCellTimingNochangeHighHigh:      result = kCellTimingNochangeHighHighString; break;
  case kCellTimingNochangeHighLow:       result = kCellTimingNochangeHighLowString; break;
  case kCellTimingNochangeLowHigh:       result = kCellTimingNochangeLowHighString; break;
  case kCellTimingNochangeLowLow:        result = kCellTimingNochangeLowLowString; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

//////////////////////////////////////////////////////////////////////
// CellTimingTypeObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellTimingType_methods[] = {
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
// CellTimingTypeObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellTimingType_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.CellTimingType",              // tp_name
  sizeof(CellTimingTypeObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellTimingType_dealloc,     // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)CellTimingType_repr,          // tp_repr

  // Method suites for standard classes
  0,                               // tp_as_number
  0,                               // tp_as_sequence
  0,                               // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                     // tp_hash
  (ternaryfunc)0,                  // tp_call
  (reprfunc)0,                     // tp_str
  (getattrofunc)0,                 // tp_getattro
  (setattrofunc)0,                 // tp_setattro

  // Functions to access object as input/output buffer
  0,                               // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,              // tp_flags

  // Documentation string
  "Timing Type",                 // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                 // tp_traverse

  // delete references to contained objects
  (inquiry)0,                      // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                  // tp_richcompare

  // weak reference enabler
  (long)0,                         // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                  // tp_iter
  (iternextfunc)0,                 // tp_iternext

  // Attribute descriptor and subclassing stuff
  CellTimingType_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)0,                     // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)CellTimingType_new,            // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と tCellTimingType の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief tCellTimingType から CellTimingTypeObject を生成する．
// @param[in] val tCellTimingType の値
PyObject*
PyCellTimingType_FromCellTimingType(tCellTimingType val)
{
  PyObject* result = NULL;
  switch ( val ) {
  case kCellTimingCombinational:         result = Py_kCellTimingCombinational; break;
  case kCellTimingCombinationalRise:     result = Py_kCellTimingCombinationalRise; break;
  case kCellTimingCombinationalFall:     result = Py_kCellTimingCombinationalFall; break;
  case kCellTimingThreeStateEnable:      result = Py_kCellTimingThreeStateEnable; break;
  case kCellTimingThreeStateDisable:     result = Py_kCellTimingThreeStateDisable; break;
  case kCellTimingThreeStateEnableRise:  result = Py_kCellTimingThreeStateEnableRise; break;
  case kCellTimingThreeStateEnableFall:  result = Py_kCellTimingThreeStateEnableFall; break;
  case kCellTimingThreeStateDisableRise: result = Py_kCellTimingThreeStateDisableRise; break;
  case kCellTimingThreeStateDisableFall: result = Py_kCellTimingThreeStateDisableFall; break;
  case kCellTimingRisingEdge:            result = Py_kCellTimingRisingEdge; break;
  case kCellTimingFallingEdge:           result = Py_kCellTimingFallingEdge; break;
  case kCellTimingPreset:                result = Py_kCellTimingPreset; break;
  case kCellTimingClear:                 result = Py_kCellTimingClear; break;
  case kCellTimingHoldRising:            result = Py_kCellTimingHoldRising; break;
  case kCellTimingHoldFalling:           result = Py_kCellTimingHoldFalling; break;
  case kCellTimingSetupRising:           result = Py_kCellTimingSetupRising; break;
  case kCellTimingSetupFalling:          result = Py_kCellTimingSetupFalling; break;
  case kCellTimingRecoveryRising:        result = Py_kCellTimingRecoveryRising; break;
  case kCellTimingRecoveryFalling:       result = Py_kCellTimingRecoveryFalling; break;
  case kCellTimingSkewRising:            result = Py_kCellTimingSkewRising; break;
  case kCellTimingSkewFalling:           result = Py_kCellTimingSkewFalling; break;
  case kCellTimingRemovalRising:         result = Py_kCellTimingRemovalRising; break;
  case kCellTimingRemovalFalling:        result = Py_kCellTimingRemovalFalling; break;
  case kCellTimingNonSeqSetupRising:     result = Py_kCellTimingNonSeqSetupRising; break;
  case kCellTimingNonSeqSetupFalling:    result = Py_kCellTimingNonSeqSetupFalling; break;
  case kCellTimingNonSeqHoldRising:      result = Py_kCellTimingNonSeqHoldRising; break;
  case kCellTimingNonSeqHoldFalling:     result = Py_kCellTimingNonSeqHoldFalling; break;
  case kCellTimingNochangeHighHigh:      result = Py_kCellTimingNochangeHighHigh; break;
  case kCellTimingNochangeHighLow:       result = Py_kCellTimingNochangeHighLow; break;
  case kCellTimingNochangeLowHigh:       result = Py_kCellTimingNochangeLowHigh; break;
  case kCellTimingNochangeLowLow:        result = Py_kCellTimingNochangeLowLow; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// @brief 文字列から CellTimingTypeObject を生成する．
// @param[in] str 文字列
PyObject*
PyCellTimingType_FromString(const char* str)
{
  PyObject* result = NULL;
  if ( str == NULL ) {
    // デフォルト値
    result = Py_kCellTimingCombinational;
  }
  else if ( strcmp(str, kCellTimingCombinationalName) == 0 ) {
    result = Py_kCellTimingCombinational;
  }
  else if ( strcmp(str, kCellTimingCombinationalRiseName) == 0 ) {
    result = Py_kCellTimingCombinationalRise;
  }
  else if ( strcmp(str, kCellTimingCombinationalFallName) == 0 ) {
    result = Py_kCellTimingCombinationalFall;
  }
  else if ( strcmp(str, kCellTimingThreeStateEnableName) == 0 ) {
    result = Py_kCellTimingThreeStateEnable;
  }
  else if ( strcmp(str, kCellTimingThreeStateDisableName) == 0 ) {
    result = Py_kCellTimingThreeStateDisable;
  }
  else if ( strcmp(str, kCellTimingThreeStateEnableRiseName) == 0 ) {
    result = Py_kCellTimingThreeStateEnableRise;
  }
  else if ( strcmp(str, kCellTimingThreeStateEnableFallName) == 0 ) {
    result = Py_kCellTimingThreeStateEnableFall;
  }
  else if ( strcmp(str, kCellTimingThreeStateDisableRiseName) == 0 ) {
    result = Py_kCellTimingThreeStateDisableRise;
  }
  else if ( strcmp(str, kCellTimingThreeStateDisableFallName) == 0 ) {
    result = Py_kCellTimingThreeStateDisableFall;
  }
  else if ( strcmp(str, kCellTimingRisingEdgeName) == 0 ) {
    result = Py_kCellTimingRisingEdge;
  }
  else if ( strcmp(str, kCellTimingFallingEdgeName) == 0 ) {
    result = Py_kCellTimingFallingEdge;
  }
  else if ( strcmp(str, kCellTimingPresetName) == 0 ) {
    result = Py_kCellTimingPreset;
  }
  else if ( strcmp(str, kCellTimingClearName) == 0 ) {
    result = Py_kCellTimingClear;
  }
  else if ( strcmp(str, kCellTimingHoldRisingName) == 0 ) {
    result = Py_kCellTimingHoldRising;
  }
  else if ( strcmp(str, kCellTimingHoldFallingName) == 0 ) {
    result = Py_kCellTimingHoldFalling;
  }
  else if ( strcmp(str, kCellTimingSetupRisingName) == 0 ) {
    result = Py_kCellTimingSetupRising;
  }
  else if ( strcmp(str, kCellTimingSetupFallingName) == 0 ) {
    result = Py_kCellTimingSetupFalling;
  }
  else if ( strcmp(str, kCellTimingRecoveryRisingName) == 0 ) {
    result = Py_kCellTimingRecoveryRising;
  }
  else if ( strcmp(str, kCellTimingRecoveryFallingName) == 0 ) {
    result = Py_kCellTimingRecoveryFalling;
  }
  else if ( strcmp(str, kCellTimingSkewRisingName) == 0 ) {
    result = Py_kCellTimingSkewRising;
  }
  else if ( strcmp(str, kCellTimingSkewFallingName) == 0 ) {
    result = Py_kCellTimingSkewFalling;
  }
  else if ( strcmp(str, kCellTimingRemovalRisingName) == 0 ) {
    result = Py_kCellTimingRemovalRising;
  }
  else if ( strcmp(str, kCellTimingRemovalFallingName) == 0 ) {
    result = Py_kCellTimingRemovalFalling;
  }
  else if ( strcmp(str, kCellTimingNonSeqSetupRisingName) == 0 ) {
    result = Py_kCellTimingNonSeqSetupRising;
  }
  else if ( strcmp(str, kCellTimingNonSeqSetupFallingName) == 0 ) {
    result = Py_kCellTimingNonSeqSetupFalling;
  }
  else if ( strcmp(str, kCellTimingNonSeqHoldRisingName) == 0 ) {
    result = Py_kCellTimingNonSeqHoldRising;
  }
  else if ( strcmp(str, kCellTimingNonSeqHoldFallingName) == 0 ) {
    result = Py_kCellTimingNonSeqHoldFalling;
  }
  else if ( strcmp(str, kCellTimingNochangeHighHighName) == 0 ) {
    result = Py_kCellTimingNochangeHighHigh;
  }
  else if ( strcmp(str, kCellTimingNochangeHighLowName) == 0 ) {
    result = Py_kCellTimingNochangeHighLow;
  }
  else if ( strcmp(str, kCellTimingNochangeLowHighName) == 0 ) {
    result = Py_kCellTimingNochangeLowHigh;
  }
  else if ( strcmp(str, kCellTimingNochangeLowLowName) == 0 ) {
    result = Py_kCellTimingNochangeLowLow;
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "Illegal string for tCellTimingType");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から tCellTimingType を取り出す．
// @param[in] py_obj Python オブジェクト
// @return tCellTimingType を返す．
// @note 変換が失敗したら TypeError を送出し，kCellTimingCombinational を返す．
tCellTimingType
PyCellTimingType_AsCellTimingType(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellTimingType_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellTimingType is expected");
    return kCellTimingCombinational;
  }

  // 強制的にキャスト
  CellTimingTypeObject* my_obj = (CellTimingTypeObject*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// CellTimingTypeObject の外部変数
//////////////////////////////////////////////////////////////////////

// kCellTimingCombinational を表すオブジェクト
PyObject* Py_kCellTimingCombinational         = NULL;

// kCellTimingCombinationalRise を表すオブジェクト
PyObject* Py_kCellTimingCombinationalRise     = NULL;

// kCellTimingCombinationalFall を表すオブジェクト
PyObject* Py_kCellTimingCombinationalFall     = NULL;

// kCellTimingThreeStateEnable を表すオブジェクト
PyObject* Py_kCellTimingThreeStateEnable      = NULL;

// kCellTimingThreeStateDisable を表すオブジェクト
PyObject* Py_kCellTimingThreeStateDisable     = NULL;

// kCellTimingThreeStateEnableRise を表すオブジェクト
PyObject* Py_kCellTimingThreeStateEnableRise  = NULL;

// kCellTimingThreeStateEnableFall を表すオブジェクト
PyObject* Py_kCellTimingThreeStateEnableFall  = NULL;

// kCellTimingThreeStateDisableRise を表すオブジェクト
PyObject* Py_kCellTimingThreeStateDisableRise = NULL;

// kCellTimingThreeStateDisableFall を表すオブジェクト
PyObject* Py_kCellTimingThreeStateDisableFall = NULL;

// kCellTimingRisingEdge を表すオブジェクト
PyObject* Py_kCellTimingRisingEdge            = NULL;

// kCellTimingFallingEdge を表すオブジェクト
PyObject* Py_kCellTimingFallingEdge           = NULL;

// kCellTimingPreset を表すオブジェクト
PyObject* Py_kCellTimingPreset                = NULL;

// kCellTimingClear を表すオブジェクト
PyObject* Py_kCellTimingClear                 = NULL;

// kCellTimingHoldRising を表すオブジェクト
PyObject* Py_kCellTimingHoldRising            = NULL;

// kCellTimingHoldFalling を表すオブジェクト
PyObject* Py_kCellTimingHoldFalling           = NULL;

// kCellTimingSetupRising を表すオブジェクト
PyObject* Py_kCellTimingSetupRising           = NULL;

// kCellTimingSetupFalling を表すオブジェクト
PyObject* Py_kCellTimingSetupFalling          = NULL;

// kCellTimingRecoveryRising を表すオブジェクト
PyObject* Py_kCellTimingRecoveryRising        = NULL;

// kCellTimingRecoveryFalling を表すオブジェクト
PyObject* Py_kCellTimingRecoveryFalling       = NULL;

// kCellTimingSkewRising を表すオブジェクト
PyObject* Py_kCellTimingSkewRising            = NULL;

// kCellTimingSkewFalling を表すオブジェクト
PyObject* Py_kCellTimingSkewFalling           = NULL;

// kCellTimingRemovalRising を表すオブジェクト
PyObject* Py_kCellTimingRemovalRising         = NULL;

// kCellTimingRemovalFalling を表すオブジェクト
PyObject* Py_kCellTimingRemovalFalling        = NULL;

// kCellTimingNonSeqSetupRising を表すオブジェクト
PyObject* Py_kCellTimingNonSeqSetupRising     = NULL;

// kCellTimingNonSeqSetupFalling を表すオブジェクト
PyObject* Py_kCellTimingNonSeqSetupFalling    = NULL;

// kCellTimingNonSeqHoldRising を表すオブジェクト
PyObject* Py_kCellTimingNonSeqHoldRising      = NULL;

// kCellTimingNonSeqHoldFalling を表すオブジェクト
PyObject* Py_kCellTimingNonSeqHoldFalling     = NULL;

// kCellTimingNochangeHighHigh を表すオブジェクト
PyObject* Py_kCellTimingNochangeHighHigh      = NULL;

// kCellTimingNochangeHighLow を表すオブジェクト
PyObject* Py_kCellTimingNochangeHighLow       = NULL;

// kCellTimingNochangeLowHigh を表すオブジェクト
PyObject* Py_kCellTimingNochangeLowHigh       = NULL;

// kCellTimingNochangeLowLow を表すオブジェクト
PyObject* Py_kCellTimingNochangeLowLow        = NULL;


BEGIN_NONAMESPACE

// tCellTimingType の定数を設定する関数
void
obj_set(CellTimingTypeObject& my_obj,
	PyObject*& py_obj,
	PyObject* module,
	const char* name)
{
  py_obj = (PyObject*)&my_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

END_NONAMESPACE

// CellTimingTypeObject 関係の初期化を行う．
void
CellTimingTypeObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellTimingType_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "CellTimingType", (PyObject*)&PyCellTimingType_Type);

  // 定数オブジェクトの生成と登録
  obj_set(kCellTimingCombinationalStruct,
          Py_kCellTimingCombinational, m,
          "kCellTimingCombinational");

  obj_set(kCellTimingCombinationalRiseStruct,
          Py_kCellTimingCombinationalRise, m,
          "kCellTimingCombinationalRise");

  obj_set(kCellTimingCombinationalFallStruct,
          Py_kCellTimingCombinationalFall, m,
          "kCellTimingCombinationalFall");

  obj_set(kCellTimingThreeStateEnableStruct,
          Py_kCellTimingThreeStateEnable, m,
          "kCellTimingThreeStateEnable");

  obj_set(kCellTimingThreeStateDisableStruct,
          Py_kCellTimingThreeStateDisable, m,
          "kCellTimingThreeStateDisable");

  obj_set(kCellTimingThreeStateEnableRiseStruct,
          Py_kCellTimingThreeStateEnableRise, m,
          "kCellTimingThreeStateEnableRise");

  obj_set(kCellTimingThreeStateEnableFallStruct,
          Py_kCellTimingThreeStateEnableFall, m,
          "kCellTimingThreeStateEnableFall");

  obj_set(kCellTimingThreeStateDisableRiseStruct,
          Py_kCellTimingThreeStateDisableRise, m,
          "kCellTimingThreeStateDisableRise");

  obj_set(kCellTimingThreeStateDisableFallStruct,
          Py_kCellTimingThreeStateDisableFall, m,
          "kCellTimingThreeStateDisableFall");

  obj_set(kCellTimingRisingEdgeStruct,
          Py_kCellTimingRisingEdge, m,
          "kCellTimingRisingEdge");

  obj_set(kCellTimingFallingEdgeStruct,
          Py_kCellTimingFallingEdge, m,
          "kCellTimingFallingEdge");

  obj_set(kCellTimingPresetStruct,
          Py_kCellTimingPreset, m,
          "kCellTimingPreset");

  obj_set(kCellTimingClearStruct,
          Py_kCellTimingClear, m,
          "kCellTimingClear");

  obj_set(kCellTimingHoldRisingStruct,
          Py_kCellTimingHoldRising, m,
          "kCellTimingHoldRising");

  obj_set(kCellTimingHoldFallingStruct,
          Py_kCellTimingHoldFalling, m,
          "kCellTimingHoldFalling");

  obj_set(kCellTimingSetupRisingStruct,
          Py_kCellTimingSetupRising, m,
          "kCellTimingSetupRising");

  obj_set(kCellTimingSetupFallingStruct,
          Py_kCellTimingSetupFalling, m,
          "kCellTimingSetupFalling");

  obj_set(kCellTimingRecoveryRisingStruct,
          Py_kCellTimingRecoveryRising, m,
          "kCellTimingRecoveryRising");

  obj_set(kCellTimingRecoveryFallingStruct,
          Py_kCellTimingRecoveryFalling, m,
          "kCellTimingRecoveryFalling");

  obj_set(kCellTimingSkewRisingStruct,
          Py_kCellTimingSkewRising, m,
          "kCellTimingSkewRising");

  obj_set(kCellTimingSkewFallingStruct,
          Py_kCellTimingSkewFalling, m,
          "kCellTimingSkewFalling");

  obj_set(kCellTimingRemovalRisingStruct,
          Py_kCellTimingRemovalRising, m,
          "kCellTimingRemovalRising");

  obj_set(kCellTimingRemovalFallingStruct,
          Py_kCellTimingRemovalFalling, m,
          "kCellTimingRemovalFalling");

  obj_set(kCellTimingNonSeqSetupRisingStruct,
          Py_kCellTimingNonSeqSetupRising, m,
          "kCellTimingNonSeqSetupRising");

  obj_set(kCellTimingNonSeqSetupFallingStruct,
          Py_kCellTimingNonSeqSetupFalling, m,
          "kCellTimingNonSeqSetupFalling");

  obj_set(kCellTimingNonSeqHoldRisingStruct,
          Py_kCellTimingNonSeqHoldRising, m,
          "kCellTimingNonSeqHoldRising");

  obj_set(kCellTimingNonSeqHoldFallingStruct,
          Py_kCellTimingNonSeqHoldFalling, m,
          "kCellTimingNonSeqHoldFalling");

  obj_set(kCellTimingNochangeHighHighStruct,
          Py_kCellTimingNochangeHighHigh, m,
          "kCellTimingNochangeHighHigh");

  obj_set(kCellTimingNochangeHighLowStruct,
          Py_kCellTimingNochangeHighLow, m,
          "kCellTimingNochangeHighLow");

  obj_set(kCellTimingNochangeLowHighStruct,
          Py_kCellTimingNochangeLowHigh, m,
          "kCellTimingNochangeLowHigh");

  obj_set(kCellTimingNochangeLowLowStruct,
          Py_kCellTimingNochangeLowLow, m,
          "kCellTimingNochangeLowLow");

  // 定数オブジェクト用の文字列オブジェクトの生成
  kCellTimingCombinationalString         = PyString_FromString(kCellTimingCombinationalName);
  kCellTimingCombinationalRiseString     = PyString_FromString(kCellTimingCombinationalRiseName);
  kCellTimingCombinationalFallString     = PyString_FromString(kCellTimingCombinationalFallName);
  kCellTimingThreeStateEnableString      = PyString_FromString(kCellTimingThreeStateEnableName);
  kCellTimingThreeStateDisableString     = PyString_FromString(kCellTimingThreeStateDisableName);
  kCellTimingThreeStateEnableRiseString  = PyString_FromString(kCellTimingThreeStateEnableRiseName);
  kCellTimingThreeStateEnableFallString  = PyString_FromString(kCellTimingThreeStateEnableFallName);
  kCellTimingThreeStateDisableRiseString = PyString_FromString(kCellTimingThreeStateDisableRiseName);
  kCellTimingThreeStateDisableFallString = PyString_FromString(kCellTimingThreeStateDisableFallName);
  kCellTimingRisingEdgeString            = PyString_FromString(kCellTimingRisingEdgeName);
  kCellTimingFallingEdgeString           = PyString_FromString(kCellTimingFallingEdgeName);
  kCellTimingPresetString                = PyString_FromString(kCellTimingPresetName);
  kCellTimingClearString                 = PyString_FromString(kCellTimingClearName);
  kCellTimingHoldRisingString            = PyString_FromString(kCellTimingHoldRisingName);
  kCellTimingHoldFallingString           = PyString_FromString(kCellTimingHoldFallingName);
  kCellTimingSetupRisingString           = PyString_FromString(kCellTimingSetupRisingName);
  kCellTimingSetupFallingString          = PyString_FromString(kCellTimingSetupFallingName);
  kCellTimingRecoveryRisingString        = PyString_FromString(kCellTimingRecoveryRisingName);
  kCellTimingRecoveryFallingString       = PyString_FromString(kCellTimingRecoveryFallingName);
  kCellTimingSkewRisingString            = PyString_FromString(kCellTimingSkewRisingName);
  kCellTimingSkewFallingString           = PyString_FromString(kCellTimingSkewFallingName);
  kCellTimingRemovalRisingString         = PyString_FromString(kCellTimingRemovalRisingName);
  kCellTimingRemovalFallingString        = PyString_FromString(kCellTimingRemovalFallingName);
  kCellTimingNonSeqSetupRisingString     = PyString_FromString(kCellTimingNonSeqSetupRisingName);
  kCellTimingNonSeqSetupFallingString    = PyString_FromString(kCellTimingNonSeqSetupFallingName);
  kCellTimingNonSeqHoldRisingString      = PyString_FromString(kCellTimingNonSeqHoldRisingName);
  kCellTimingNonSeqHoldFallingString     = PyString_FromString(kCellTimingNonSeqHoldFallingName);
  kCellTimingNochangeHighHighString      = PyString_FromString(kCellTimingNochangeHighHighName);
  kCellTimingNochangeHighLowString       = PyString_FromString(kCellTimingNochangeHighLowName);
  kCellTimingNochangeLowHighString       = PyString_FromString(kCellTimingNochangeLowHighName);
  kCellTimingNochangeLowLowString        = PyString_FromString(kCellTimingNochangeLowLowName);

}

END_NAMESPACE_YM

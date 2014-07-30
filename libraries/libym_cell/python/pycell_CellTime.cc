
/// @file pycell_CellTime.cc
/// @brief CellTime の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_YmCell/pycell.h"
#include "ym_YmCell/CellTime.h"
#include "ym_YmUtils/FileODO.h"
#include "ym_YmUtils/FileIDO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// CellTimeObject の外部変数
//////////////////////////////////////////////////////////////////////

// 無限大を表す定数
PyObject* Py_kCellTimeInf = NULL;


BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellTime を表す型
struct CellTimeObject
{
  // Python のお約束
  PyObject_HEAD

  // CellTime の本体
  double mVal;

};

// Py_kCellTimeInf の本体
CellTimeObject Py_kCellTimeInfStruct = {
  PyObject_HEAD_INIT(&PyCellTime_Type)
  DBL_MAX
};

// Py_kCellTimeInf 用の文字列オブジェクト
PyObject* Py_kCellTimeInfString = NULL;


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellTimeObject の生成関数
CellTimeObject*
CellTime_new(PyTypeObject* type)
{
  CellTimeObject* self = PyObject_New(CellTimeObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // とりあえず 0.0 で初期化しておく
  self->mVal = 0.0;

  return self;
}

// CellTimeObject を開放する関数
void
CellTime_dealloc(CellTimeObject* self)
{
  PyObject_Del(self);
}

// 初期化関数
int
CellTime_init(CellTimeObject* self,
	      PyObject* args)
{
  // args をパーズして初期化を行なう．
  // エラーが起きたらエラーメッセージをセットして -1 を返す．
  // 正常に終了したら 0 を返す．

  double val = 0.0;
  if ( !PyArg_ParseTuple(args, "|d", &val) ) {
    return -1;
  }

  self->mVal = val;

  return 0;
}

// repr 関数
PyObject*
CellTime_repr(CellTimeObject* self)
{
  if ( self->mVal == DBL_MAX ) {
    Py_INCREF(Py_kCellTimeInfString);
    return Py_kCellTimeInfString;
  }

  ostringstream buf;
  buf << self->mVal;
  return PyObject_FromString(buf.str());
}

// 比較関数
int
CellTime_compare(CellTimeObject* left,
		 CellTimeObject* right)
{
  double diff = left->mVal - right->mVal;
  if ( diff < 0.0 ) {
    return -1;
  }
  if ( diff > 0.0 ) {
    return 1;
  }
  return 0;
}

// value 関数
PyObject*
CellTime_value(CellTimeObject* self,
	       PyObject* args)
{
  return PyObject_FromDouble(self->mVal);
}

// add 関数
PyObject*
CellTime_add(PyObject* left,
	     PyObject* right)
{
  if ( !PyCellTime_Check(left) || !PyCellTime_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "both parameters must be cell_lib.Time");
    return NULL;
  }

  double val1 = PyCellTime_AsDouble(left);
  double val2 = PyCellTime_AsDouble(right);

  return PyCellTime_FromDouble(val1 + val2);
}

// sub 関数
PyObject*
CellTime_sub(PyObject* left,
	     PyObject* right)
{
  if ( !PyCellTime_Check(left) || !PyCellTime_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "both parameters must be cell_lib.Time");
    return NULL;
  }

  double val1 = PyCellTime_AsDouble(left);
  double val2 = PyCellTime_AsDouble(right);

  return PyCellTime_FromDouble(val1 - val2);
}

// inplace add 関数
PyObject*
CellTime_iadd(PyObject* left,
	      PyObject* right)
{
  if ( !PyCellTime_Check(left) || !PyCellTime_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "both parameters must be cell_lib.Time");
    return NULL;
  }

  CellTimeObject* obj1 = (CellTimeObject*)left;
  CellTimeObject* obj2 = (CellTimeObject*)right;
  obj1->mVal += obj2->mVal;

  Py_INCREF(left);
  return left;
}

// inplace sub 関数
PyObject*
CellTime_isub(PyObject* left,
	      PyObject* right)
{
  if ( !PyCellTime_Check(left) || !PyCellTime_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "both parameters must be cell_lib.Time");
    return NULL;
  }

  CellTimeObject* obj1 = (CellTimeObject*)left;
  CellTimeObject* obj2 = (CellTimeObject*)right;
  obj1->mVal += obj2->mVal;

  Py_INCREF(left);
  return left;
}


//////////////////////////////////////////////////////////////////////
// CellTimeObject の NumberMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PyNumberMethods CellTime_nbmethods = {
  (binaryfunc)CellTime_add,    // nb_add
  (binaryfunc)CellTime_sub,    // nb_subtract
  (binaryfunc)0,               // nb_multiply
  (binaryfunc)0,               // nb_divide
  (binaryfunc)0,               // nb_remainder
  (binaryfunc)0,               // nb_divmod
  (ternaryfunc)0,              // nb_power
  (unaryfunc)0,                // nb_negative
  (unaryfunc)0,                // nb_positive
  (unaryfunc)0,                // nb_absolute
  (inquiry)0,                  // nb_nonzero
  (unaryfunc)0,                // nb_invert
  (binaryfunc)0,               // nb_lshift
  (binaryfunc)0,               // nb_rshift
  (binaryfunc)0,               // nb_and
  (binaryfunc)0,               // nb_xor
  (binaryfunc)0,               // nb_or
  (coercion)0,                 // nb_coerce
  (unaryfunc)0,                // nb_int
  (unaryfunc)0,                // nb_long
  (unaryfunc)0,                // nb_float
  (unaryfunc)0,                // nb_oct
  (unaryfunc)0,                // nb_hex

  // Added in release 2.0
  (binaryfunc)CellTime_iadd,   // nb_inplace_add
  (binaryfunc)CellTime_isub,   // nb_inplace_subtract
  (binaryfunc)0,               // nb_inplace_multiply
  (binaryfunc)0,               // nb_inplace_divide
  (binaryfunc)0,               // nb_inplace_remainder
  (ternaryfunc)0,              // nb_inplace_power
  (binaryfunc)0,               // nb_inplace_lshift
  (binaryfunc)0,               // nb_inplace_rshift
  (binaryfunc)0,               // nb_inplace_and
  (binaryfunc)0,               // nb_inplace_xor
  (binaryfunc)0,               // nb_inplace_or

  // Added in release 2.2
  (binaryfunc)0,               // nb_floor_divide
  (binaryfunc)0,               // nb_true_divide
  (binaryfunc)0,               // nb_inplace_floor_divide
  (binaryfunc)0,               // nb_inplace_true_divide

  // Added in release 2.5
  (unaryfunc)0                 // nb_index
};


//////////////////////////////////////////////////////////////////////
// CellTimeObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellTime_methods[] = {
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
  {"value", (PyCFunction)CellTime_value, METH_NOARGS,
   PyDoc_STR("return double value (NONE)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellTimeObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellTime_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.Time",                     // tp_name
  sizeof(CellTimeObject),          // tp_basicsize
  (int)0,                          // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellTime_dealloc,    // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)CellTime_compare,       // tp_compare
  (reprfunc)CellTime_repr,         // tp_repr

  // Method suites for standard classes
  &CellTime_nbmethods,             // tp_as_number
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
  "Cell Time",                     // tp_doc

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
  CellTime_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)CellTime_init,         // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)CellTime_new,           // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と CellTime の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief CellTime から CellTimeObject を生成する．
// @param[in] obj CellTime オブジェクト
PyObject*
PyCellTime_FromCellTime(const CellTime& obj)
{
  CellTimeObject* py_obj = CellTime_new(&PyCellTime_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mVal = obj.value();

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief double から CellTimeObject を生成する．
// @param[in] val 値
PyObject*
PyCellTime_FromDouble(double val)
{
  CellTimeObject* py_obj = CellTime_new(&PyCellTime_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mVal = val;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief PyObject から CellTime を取り出す．
// @param[in] py_obj Python オブジェクト
// @return CellTime を返す．
// @note 変換が失敗したら TypeError を送出し，CellTime(0.0) を返す．
CellTime
PyCellTime_AsCellTime(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellTime_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.Time is expected");
    return CellTime(0.0);
  }

  // 強制的にキャスト
  CellTimeObject* my_obj = (CellTimeObject*)py_obj;

  return CellTime(my_obj->mVal);
}

// @brief PyObject から double を取り出す．
// @param[in] py_obj Python オブジェクト
// @return double を返す．
// @note 変換が失敗したら TypeError を送出し，0.0 を返す．
double
PyCellTime_AsDouble(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellTime_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.Time is expected");
    return 0.0;
  }

  // 強制的にキャスト
  CellTimeObject* my_obj = (CellTimeObject*)py_obj;

  return my_obj->mVal;
}

// CellTimeObject 関係の初期化を行う．
void
CellTimeObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellTime_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Time", (PyObject*)&PyCellTime_Type);

  // 定数オブジェクトの生成
  Py_kCellTimeInf = (PyObject*)&Py_kCellTimeInfStruct;
  Py_XINCREF(Py_kCellTimeInf);
  PyModule_AddObject(m, "kTimeInf", Py_kCellTimeInf);

  // 定数オブジェクト用の文字列オブジェクトの生成
  Py_kCellTimeInfString = PyString_InternFromString("infinity");
  Py_INCREF(Py_kCellTimeInfString);
}

END_NAMESPACE_YM

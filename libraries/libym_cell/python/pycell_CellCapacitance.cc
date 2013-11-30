
/// @file pycell_CellCapacitance.cc
/// @brief CellCapacitance の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/CellCapacitance.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// CellCapacitanceObject の外部変数
//////////////////////////////////////////////////////////////////////

// 無限大を表す定数
PyObject* Py_kCellCapacitanceInf = NULL;


BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellCapacitance を表す型
struct CellCapacitanceObject
{
  // Python のお約束
  PyObject_HEAD

  // CellCapacitance の本体
  double mVal;

};

// Py_kCellCapacitanceInf の本体
CellCapacitanceObject Py_kCellCapacitanceInfStruct = {
  PyObject_HEAD_INIT(&PyCellCapacitance_Type)
  DBL_MAX
};

// Py_kCellCapacitanceInf 用の文字列オブジェクト
PyObject* Py_kCellCapacitanceInfString = NULL;


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellCapacitanceObject の生成関数
CellCapacitanceObject*
CellCapacitance_new(PyTypeObject* type)
{
  CellCapacitanceObject* self = PyObject_New(CellCapacitanceObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // とりあえず 0.0 で初期化しておく
  self->mVal = 0.0;

  return self;
}

// CellCapacitanceObject を開放する関数
void
CellCapacitance_dealloc(CellCapacitanceObject* self)
{
  PyObject_Del(self);
}

// 初期化関数
int
CellCapacitance_init(CellCapacitanceObject* self,
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
CellCapacitance_repr(CellCapacitanceObject* self)
{
  if ( self->mVal == DBL_MAX ) {
    Py_INCREF(Py_kCellCapacitanceInfString);
    return Py_kCellCapacitanceInfString;
  }

  ostringstream buf;
  buf << self->mVal;
  return PyObject_FromString(buf.str());
}

// 比較関数
int
CellCapacitance_compare(CellCapacitanceObject* left,
			CellCapacitanceObject* right)
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
CellCapacitance_value(CellCapacitanceObject* self,
		      PyObject* args)
{
  return PyObject_FromDouble(self->mVal);
}

// add 関数
PyObject*
CellCapacitance_add(PyObject* left,
		    PyObject* right)
{
  if ( !PyCellCapacitance_Check(left) ||
       !PyCellCapacitance_Check(right) ) {
    PyErr_SetString(PyExc_TypeError,
		    "both parameters must be cell_lib.Capacitance");
    return NULL;
  }

  CellCapacitanceObject* obj1 = (CellCapacitanceObject*)left;
  CellCapacitanceObject* obj2 = (CellCapacitanceObject*)right;

  return PyCellCapacitance_FromDouble(obj1->mVal + obj2->mVal);
}

// sub 関数
PyObject*
CellCapacitance_sub(PyObject* left,
		    PyObject* right)
{
  if ( !PyCellCapacitance_Check(left) ||
       !PyCellCapacitance_Check(right) ) {
    PyErr_SetString(PyExc_TypeError,
		    "both parameters must be cell_lib.Capacitance");
    return NULL;
  }

  CellCapacitanceObject* obj1 = (CellCapacitanceObject*)left;
  CellCapacitanceObject* obj2 = (CellCapacitanceObject*)right;

  return PyCellCapacitance_FromDouble(obj1->mVal - obj2->mVal);
}

// inplace add 関数
PyObject*
CellCapacitance_iadd(PyObject* left,
		     PyObject* right)
{
  if ( !PyCellCapacitance_Check(left) ||
       !PyCellCapacitance_Check(right) ) {
    PyErr_SetString(PyExc_TypeError,
		    "both parameters must be cell_lib.Capacitance");
    return NULL;
  }

  CellCapacitanceObject* obj1 = (CellCapacitanceObject*)left;
  CellCapacitanceObject* obj2 = (CellCapacitanceObject*)right;

  obj1->mVal += obj2->mVal;

  Py_INCREF(left);
  return left;
}

// inplace sub 関数
PyObject*
CellCapacitance_isub(PyObject* left,
	      PyObject* right)
{
  if ( !PyCellCapacitance_Check(left) ||
       !PyCellCapacitance_Check(right) ) {
    PyErr_SetString(PyExc_TypeError,
		    "both parameters must be cell_lib.Capacitance");
    return NULL;
  }

  CellCapacitanceObject* obj1 = (CellCapacitanceObject*)left;
  CellCapacitanceObject* obj2 = (CellCapacitanceObject*)right;

  obj1->mVal += obj2->mVal;

  Py_INCREF(left);
  return left;
}


//////////////////////////////////////////////////////////////////////
// CellCapacitanceObject の NumberMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PyNumberMethods CellCapacitance_nbmethods = {
  (binaryfunc)CellCapacitance_add,    // nb_add
  (binaryfunc)CellCapacitance_sub,    // nb_subtract
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
  (binaryfunc)CellCapacitance_iadd,   // nb_inplace_add
  (binaryfunc)CellCapacitance_isub,   // nb_inplace_subtract
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
// CellCapacitanceObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellCapacitance_methods[] = {
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
  {"value", (PyCFunction)CellCapacitance_value, METH_NOARGS,
   PyDoc_STR("return double value (NONE)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellCapacitanceObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellCapacitance_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.Capacitance",                     // tp_name
  sizeof(CellCapacitanceObject),          // tp_basicsize
  (int)0,                                 // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellCapacitance_dealloc,    // tp_dealloc
  (printfunc)0,                           // tp_print
  (getattrfunc)0,                         // tp_getattr
  (setattrfunc)0,                         // tp_setattr
  (cmpfunc)CellCapacitance_compare,       // tp_compare
  (reprfunc)CellCapacitance_repr,         // tp_repr

  // Method suites for standard classes
  &CellCapacitance_nbmethods,             // tp_as_number
  0,                                      // tp_as_sequence
  0,                                      // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                            // tp_hash
  (ternaryfunc)0,                         // tp_call
  (reprfunc)0,                            // tp_str
  (getattrofunc)0,                        // tp_getattro
  (setattrofunc)0,                        // tp_setattro

  // Functions to access object as input/output buffer
  0,                                      // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,                     // tp_flags

  // Documentation string
  "Cell Capacitance",                     // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                        // tp_traverse

  // delete references to contained objects
  (inquiry)0,                             // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                         // tp_richcompare

  // weak reference enabler
  (long)0,                                // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                         // tp_iter
  (iternextfunc)0,                        // tp_iternext

  // Attribute descriptor and subclassing stuff
  CellCapacitance_methods,                // tp_methods
  0,                                      // tp_members
  0,                                      // tp_getset
  (struct _typeobject*)0,                 // tp_base
  (PyObject*)0,                           // tp_dict
  (descrgetfunc)0,                        // tp_descr_get
  (descrsetfunc)0,                        // tp_descr_set
  (long)0,                                // tp_dictoffset
  (initproc)CellCapacitance_init,         // tp_init
  (allocfunc)0,                           // tp_alloc
  (newfunc)CellCapacitance_new,           // tp_new
  (freefunc)0,                            // tp_free
  (inquiry)0,                             // tp_is_gc

  (PyObject*)0,                           // tp_bases
  (PyObject*)0,                           // tp_mro (method resolution order)
  (PyObject*)0,                           // tp_cache
  (PyObject*)0,                           // tp_subclasses
  (PyObject*)0                            // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と CellCapacitance の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief CellCapacitance から CellCapacitanceObject を生成する．
// @param[in] obj CellCapacitance オブジェクト
PyObject*
PyCellCapacitance_FromCellCapacitance(const CellCapacitance& obj)
{
  CellCapacitanceObject* py_obj = CellCapacitance_new(&PyCellCapacitance_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mVal = obj.value();

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief double から CellCapacitanceObject を生成する．
// @param[in] val 値
PyObject*
PyCellCapacitance_FromDouble(double val)
{
  CellCapacitanceObject* py_obj = CellCapacitance_new(&PyCellCapacitance_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mVal = val;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief PyObject から CellCapacitance を取り出す．
// @param[in] py_obj Python オブジェクト
// @return CellCapacitance を返す．
// @note 変換が失敗したら TypeError を送出し，CellCapacitance(0.0) を返す．
CellCapacitance
PyCellCapacitance_AsCellCapacitance(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellCapacitance_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.Capacitance is expected");
    return CellCapacitance(0.0);
  }

  // 強制的にキャスト
  CellCapacitanceObject* my_obj = (CellCapacitanceObject*)py_obj;

  return CellCapacitance(my_obj->mVal);
}

// @brief PyObject から double を取り出す．
// @param[in] py_obj Python オブジェクト
// @return doubleを返す．
// @note 変換が失敗したら TypeError を送出し，0.0 を返す．
double
PyCellCapacitance_AsDouble(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellCapacitance_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.Capacitance is expected");
    return 0.0;
  }

  // 強制的にキャスト
  CellCapacitanceObject* my_obj = (CellCapacitanceObject*)py_obj;

  return my_obj->mVal;
}

// CellCapacitanceObject 関係の初期化を行う．
void
CellCapacitanceObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellCapacitance_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Capacitance", (PyObject*)&PyCellCapacitance_Type);

  // 定数オブジェクトの生成
  Py_kCellCapacitanceInf = (PyObject*)&Py_kCellCapacitanceInfStruct;
  Py_XINCREF(Py_kCellCapacitanceInf);
  PyModule_AddObject(m, "kCapacitanceInf", Py_kCellCapacitanceInf);

  // 定数オブジェクト用の文字列オブジェクトの生成
  Py_kCellCapacitanceInfString = PyString_InternFromString("infinity");
  Py_INCREF(Py_kCellCapacitanceInfString);
}

END_NAMESPACE_YM

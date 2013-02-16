
/// @file pylogic_TvFunc.cc
/// @brief TvFunc の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/pylogic.h"
#include "ym_logic/TvFunc.h"
#include "ym_utils/pyutils.h"
#include "ym_utils/FileBinI.h"
#include "ym_utils/FileBinO.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// TvFunc を表す型
struct TvFuncObject
{
  // Python のお約束
  PyObject_HEAD

  // TvFunc の本体
  TvFunc* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// TvFuncObject の生成関数
TvFuncObject*
TvFunc_new(PyTypeObject* type)
{
  TvFuncObject* self = PyObject_New(TvFuncObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ここでは NULL にしておく．
  self->mBody = NULL;

  return self;
}

// TvFuncObject を開放する関数
void
TvFunc_dealloc(TvFuncObject* self)
{
  // TvFunc の開放を行なう．
  delete self->mBody;

  PyObject_Del(self);
}

// 初期化関数
int
TvFunc_init(TvFuncObject* self,
	    PyObject* args)
{
  // args をパーズして初期化を行なう．
  // エラーが起きたらエラーメッセージをセットして -1 を返す．
  // 正常に終了したら 0 を返す．

  ymuint n = PyTuple_GET_SIZE(args);
  if ( n > 1 ) {
    PyErr_SetString(PyExc_TypeError, "parameter must be int or list of int");
    return -1;
  }

  if ( self->mBody != NULL ) {
    delete self->mBody;
  }

  if ( n == 0 ) {
    self->mBody = new TvFunc();
  }
  else {
    PyObject* obj = PyTuple_GET_ITEM(args, 0);
    if ( PyInt_Check(obj) ) {
      ymuint ni;
      if ( !PyArg_ParseTuple(args, "k", &ni) ) {
	return -1;
      }
      self->mBody = new TvFunc(ni);
    }
    else if ( PyList_Check(obj) ) {
      ymuint n = PyList_GET_SIZE(obj);
      ymuint ni = 0;
      ymuint x = n;
      while ( (x & 1U) == 0U ) {
	x >>= 1;
	++ ni;
      }
      if ( x != 1U ) {
	PyErr_SetString(PyExc_TypeError, "the size of the list must be 2^n");
	return -1;
      }
      vector<int> values(n);
      for (ymuint i = 0; i < n; ++ i) {
	PyObject* item = PyList_GET_ITEM(obj, i);
	values[i] = PyInt_AsLong(item);
      }
      self->mBody = new TvFunc(ni, values);
    }
    else {
      PyErr_SetString(PyExc_TypeError, "parameter must be int or list of int");
      return -1;
    }
  }

  return 0;
}

// repr 関数
PyObject*
TvFunc_repr(TvFuncObject* self)
{
  ostringstream buf;
  self->mBody->print(buf, 16);
  return conv_to_pyobject(buf.str());
}

// const_zero 関数
PyObject*
TvFunc_const_zero(PyTypeObject* type,
		  PyObject* args)
{
  ymuint ni;
  if ( !PyArg_ParseTuple(args, "k", &ni) ) {
    return NULL;
  }

  return PyTvFunc_FromTvFunc(TvFunc::const_zero(ni));
}

// const_one 関数
PyObject*
TvFunc_const_one(PyTypeObject* type,
		 PyObject* args)
{
  ymuint ni;
  if ( !PyArg_ParseTuple(args, "k", &ni) ) {
    return NULL;
  }

  return PyTvFunc_FromTvFunc(TvFunc::const_one(ni));
}

// posi_literal 関数
PyObject*
TvFunc_posi_literal(PyTypeObject* type,
		    PyObject* args)
{
  ymuint ni;
  PyObject* obj1;
  if ( !PyArg_ParseTuple(args, "kO!", &ni,
			 &PyVarId_Type, &obj1) ) {
    return NULL;
  }
  VarId vid;
  if ( !conv_from_pyobject(obj1, vid) ) {
    return NULL;
  }

  return PyTvFunc_FromTvFunc(TvFunc::posi_literal(ni, vid));
}

// nega_literal 関数
PyObject*
TvFunc_nega_literal(PyTypeObject* type,
		    PyObject* args)
{
  ymuint ni;
  PyObject* obj1;
  if ( !PyArg_ParseTuple(args, "kO!", &ni,
			 &PyVarId_Type, &obj1) ) {
    return NULL;
  }
  VarId vid;
  if ( !conv_from_pyobject(obj1, vid) ) {
    return NULL;
  }

  return PyTvFunc_FromTvFunc(TvFunc::nega_literal(ni, vid));
}

// ni 関数
PyObject*
TvFunc_ni(TvFuncObject* self,
	  PyObject* args)
{
  return conv_to_pyobject(self->mBody->ni());
}

// value 関数
PyObject*
TvFunc_value(TvFuncObject* self,
	     PyObject* args)
{
  ymuint pos;
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return NULL;
  }

  ymuint ni = self->mBody->ni();
  if ( pos >= (1U << ni) ) {
    PyErr_SetString(PyExc_ValueError, "parameter must be less then 2^ni()");
    return NULL;
  }

  return conv_to_pyobject(self->mBody->value(pos));
}

// count_zero 関数
PyObject*
TvFunc_count_zero(TvFuncObject* self,
		  PyObject* args)
{
  return conv_to_pyobject(self->mBody->count_zero());
}

// count_one 関数
PyObject*
TvFunc_count_one(TvFuncObject* self,
		 PyObject* args)
{
  return conv_to_pyobject(self->mBody->count_one());
}

// walsh_0 関数
PyObject*
TvFunc_walsh_0(TvFuncObject* self,
	       PyObject* args)
{
  return conv_to_pyobject(self->mBody->walsh_0());
}

// walsh_1 関数
PyObject*
TvFunc_walsh_1(TvFuncObject* self,
	       PyObject* args)
{
  PyObject* obj;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyVarId_Type, &obj) ) {
    return NULL;
  }
  VarId vid;
  if ( !conv_from_pyobject(obj, vid) ) {
    return NULL;
  }

  return conv_to_pyobject(self->mBody->walsh_1(vid));
}

// walsh_2 関数
PyObject*
TvFunc_walsh_2(TvFuncObject* self,
	       PyObject* args)
{
  PyObject* obj1;
  PyObject* obj2;
  if ( !PyArg_ParseTuple(args, "O!O!",
			 &PyVarId_Type, &obj1,
			 &PyVarId_Type, &obj2) ) {
    return NULL;
  }
  VarId vid1;
  if ( !conv_from_pyobject(obj1, vid1) ) {
    return NULL;
  }
  VarId vid2;
  if ( !conv_from_pyobject(obj2, vid2) ) {
    return NULL;
  }

  return conv_to_pyobject(self->mBody->walsh_2(vid1, vid2));
}

// check_sup 関数
PyObject*
TvFunc_check_sup(TvFuncObject* self,
		 PyObject* args)
{
  PyObject* obj;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyVarId_Type, &obj) ) {
    return NULL;
  }
  VarId vid;
  if ( !conv_from_pyobject(obj, vid) ) {
    return NULL;
  }

  return conv_to_pyobject(self->mBody->check_sup(vid));
}

// check_sym 関数
PyObject*
TvFunc_check_sym(TvFuncObject* self,
		 PyObject* args)
{
  PyObject* obj1;
  PyObject* obj2;
  PyObject* obj3;
  if ( !PyArg_ParseTuple(args, "O!O!|O!",
			 &PyVarId_Type, &obj1,
			 &PyVarId_Type, &obj2,
			 &PyPol_Type, &obj3) ) {
    return NULL;
  }
  VarId vid1;
  if ( !conv_from_pyobject(obj1, vid1) ) {
    return NULL;
  }
  VarId vid2;
  if ( !conv_from_pyobject(obj2, vid2) ) {
    return NULL;
  }
  tPol pol = kPolPosi;
  if ( obj3 != NULL ) {
    if ( !conv_from_pyobject(obj3, pol) ) {
      return NULL;
    }
  }

  return conv_to_pyobject(self->mBody->check_sym(vid1, vid2, pol));
}

// ハッシュ関数
long
TvFunc_hash(TvFuncObject* self)
{
  return self->mBody->hash();
}

// dump 関数
PyObject*
TvFunc_dump(TvFuncObject* self,
	    PyObject* args)
{
  FileBinO* bp = parse_FileBinO(args);
  if ( bp == NULL ) {
    return NULL;
  }

  self->mBody->dump(*bp);

  Py_INCREF(Py_None);
  return Py_None;
}

// restore 関数
PyObject*
TvFunc_restore(TvFuncObject* self,
	       PyObject* args)
{
  FileBinI* bp = parse_FileBinI(args);
  if ( bp == NULL ) {
    return NULL;
  }

  self->mBody->restore(*bp);

  Py_INCREF(Py_None);
  return Py_None;
}

// inplace cofactor 関数
PyObject*
TvFunc_icofactor(TvFuncObject* self,
		 PyObject* args)
{
  PyObject* obj1;
  PyObject* obj2;
  if ( !PyArg_ParseTuple(args, "O!O!",
			 &PyVarId_Type, &obj1,
			 &PyPol_Type, &obj2) ) {
    return NULL;
  }
  VarId vid1;
  if ( !conv_from_pyobject(obj1, vid1) ) {
    return NULL;
  }
  tPol pol = kPolPosi;
  if ( obj2 != NULL ) {
    if ( !conv_from_pyobject(obj2, pol) ) {
      return NULL;
    }
  }

  self->mBody->set_cofactor(vid1, pol);

  Py_INCREF(Py_None);
  return Py_None;
}

// cofactor 関数
PyObject*
TvFunc_cofactor(TvFuncObject* self,
		PyObject* args)
{
  PyObject* obj1;
  PyObject* obj2;
  if ( !PyArg_ParseTuple(args, "O!O!",
			 &PyVarId_Type, &obj1,
			 &PyPol_Type, &obj2) ) {
    return NULL;
  }
  VarId vid1;
  if ( !conv_from_pyobject(obj1, vid1) ) {
    return NULL;
  }
  tPol pol = kPolPosi;
  if ( obj2 != NULL ) {
    if ( !conv_from_pyobject(obj2, pol) ) {
      return NULL;
    }
  }

  return PyTvFunc_FromTvFunc(self->mBody->cofactor(vid1, pol));
}

// xform 関数
PyObject*
TvFunc_xform(TvFuncObject* self,
	     PyObject* args)
{
#if 0
  PyObject* obj1;
  if ( !PyArg_ParseTuple(args, "O!", &NpnMapType, &obj1) ) {
    return NULL;
  }
  NpnMap npnmap;
  if ( !conv_from_pyobject(obj1, npnmap) ) {
    return NULL;
  }

  return PyTvFunc_FromTvFunc(self->mBody->xform(npnmap));
#else
  return NULL;
#endif
}

// invert 関数
PyObject*
TvFunc_invert(PyObject* left)
{
  if ( TvFuncObject_Check(left) ) {
    TvFuncObject* obj = (TvFuncObject*)left;
    return PyTvFunc_FromTvFunc(~(*obj->mBody));
  }
  PyErr_SetString(PyExc_TypeError, "parameter must be logic.TvFunc");
  return NULL;
}

// negate 関数
PyObject*
TvFunc_negate(PyObject* left)
{
  if ( TvFuncObject_Check(left) ) {
    TvFuncObject* obj = (TvFuncObject*)left;
    obj->mBody->negate();
    Py_INCREF(Py_None);
    return Py_None;
  }
  PyErr_SetString(PyExc_TypeError, "parameter must be logic.TvFunc");
  return NULL;
}

// and 関数
PyObject*
TvFunc_and(PyObject* left,
	   PyObject* right)
{
  if ( TvFuncObject_Check(left) && TvFuncObject_Check(right) ) {
    TvFuncObject* obj1 = (TvFuncObject*)left;
    TvFuncObject* obj2 = (TvFuncObject*)right;
    return PyTvFunc_FromTvFunc(*obj1->mBody & *obj2->mBody);
  }
  PyErr_SetString(PyExc_TypeError, "parameter must be logic.TvFunc");
  return NULL;
}

// or 関数
PyObject*
TvFunc_or(PyObject* left,
	  PyObject* right)
{
  if ( TvFuncObject_Check(left) && TvFuncObject_Check(right) ) {
    TvFuncObject* obj1 = (TvFuncObject*)left;
    TvFuncObject* obj2 = (TvFuncObject*)right;
    return PyTvFunc_FromTvFunc(*obj1->mBody | *obj2->mBody);
  }
  PyErr_SetString(PyExc_TypeError, "parameter must be logic.TvFunc");
  return NULL;
}

// xor 関数
PyObject*
TvFunc_xor(PyObject* left,
	   PyObject* right)
{
  if ( TvFuncObject_Check(left) && TvFuncObject_Check(right) ) {
    TvFuncObject* obj1 = (TvFuncObject*)left;
    TvFuncObject* obj2 = (TvFuncObject*)right;
    return PyTvFunc_FromTvFunc(*obj1->mBody ^ *obj2->mBody);
  }
  PyErr_SetString(PyExc_TypeError, "parameter must be logic.TvFunc");
  return NULL;
}

// inplace and 関数
PyObject*
TvFunc_iand(PyObject* left,
	    PyObject* right)
{
  if ( TvFuncObject_Check(left) && TvFuncObject_Check(right) ) {
    TvFuncObject* obj1 = (TvFuncObject*)left;
    TvFuncObject* obj2 = (TvFuncObject*)right;
    (*obj1->mBody) &= *obj2->mBody;
    Py_INCREF(left);
    return left;
  }
  PyErr_SetString(PyExc_TypeError, "parameter must be logic.TvFunc");
  return NULL;
}

// inplace or 関数
PyObject*
TvFunc_ior(PyObject* left,
	   PyObject* right)
{
  if ( TvFuncObject_Check(left) && TvFuncObject_Check(right) ) {
    TvFuncObject* obj1 = (TvFuncObject*)left;
    TvFuncObject* obj2 = (TvFuncObject*)right;
    (*obj1->mBody) |= *obj2->mBody;
    Py_INCREF(left);
    return left;
  }
  PyErr_SetString(PyExc_TypeError, "parameter must be logic.TvFunc");
  return NULL;
}

// inplace xor 関数
PyObject*
TvFunc_ixor(PyObject* left,
	    PyObject* right)
{
  if ( TvFuncObject_Check(left) && TvFuncObject_Check(right) ) {
    TvFuncObject* obj1 = (TvFuncObject*)left;
    TvFuncObject* obj2 = (TvFuncObject*)right;
    (*obj1->mBody) ^= *obj2->mBody;
    Py_INCREF(left);
    return left;
  }
  PyErr_SetString(PyExc_TypeError, "parameter must be logic.TvFunc");
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// TvFuncObject の NumberMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PyNumberMethods TvFunc_nbmethods = {
  (binaryfunc)0,               // nb_add
  (binaryfunc)0,               // nb_subtract
  (binaryfunc)0,               // nb_multiply
  (binaryfunc)0,               // nb_divide
  (binaryfunc)0,               // nb_remainder
  (binaryfunc)0,               // nb_divmod
  (ternaryfunc)0,              // nb_power
  (unaryfunc)0,                // nb_negative
  (unaryfunc)0,                // nb_positive
  (unaryfunc)0,                // nb_absolute
  (inquiry)0,                  // nb_nonzero
  (unaryfunc)TvFunc_invert,    // nb_invert
  (binaryfunc)0,               // nb_lshift
  (binaryfunc)0,               // nb_rshift
  (binaryfunc)TvFunc_and,      // nb_and
  (binaryfunc)TvFunc_xor,      // nb_xor
  (binaryfunc)TvFunc_or,       // nb_or
  (coercion)0,                 // nb_coerce
  (unaryfunc)0,                // nb_int
  (unaryfunc)0,                // nb_long
  (unaryfunc)0,                // nb_float
  (unaryfunc)0,                // nb_oct
  (unaryfunc)0,                // nb_hex

  // Added in release 2.0
  (binaryfunc)0,               // nb_inplace_add
  (binaryfunc)0,               // nb_inplace_subtract
  (binaryfunc)0,               // nb_inplace_multiply
  (binaryfunc)0,               // nb_inplace_divide
  (binaryfunc)0,               // nb_inplace_remainder
  (ternaryfunc)0,              // nb_inplace_power
  (binaryfunc)0,               // nb_inplace_lshift
  (binaryfunc)0,               // nb_inplace_rshift
  (binaryfunc)TvFunc_iand,     // nb_inplace_and
  (binaryfunc)TvFunc_ixor,     // nb_inplace_xor
  (binaryfunc)TvFunc_ior,      // nb_inplace_or

  // Added in release 2.2
  (binaryfunc)0,               // nb_floor_divide
  (binaryfunc)0,               // nb_true_divide
  (binaryfunc)0,               // nb_inplace_floor_divide
  (binaryfunc)0,               // nb_inplace_true_divide

  // Added in release 2.5
  (unaryfunc)0                 // nb_index
};


//////////////////////////////////////////////////////////////////////
// TvFuncObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef TvFunc_methods[] = {
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
  {"const_zero", (PyCFunction)TvFunc_const_zero, METH_STATIC | METH_VARARGS,
   PyDoc_STR("return constant 0 (int)")},
  {"const_one", (PyCFunction)TvFunc_const_one, METH_STATIC | METH_VARARGS,
   PyDoc_STR("return constant 1 (int)")},
  {"posi_literal", (PyCFunction)TvFunc_posi_literal, METH_STATIC | METH_VARARGS,
   PyDoc_STR("return positive literal (int, VarId)")},
  {"nega_literal", (PyCFunction)TvFunc_nega_literal, METH_STATIC | METH_VARARGS,
   PyDoc_STR("return negative literal (int, VarId)")},
  {"negate", (PyCFunction)TvFunc_negate, METH_NOARGS,
   PyDoc_STR("inplace invert (NONE)")},
  {"icofactor", (PyCFunction)TvFunc_icofactor, METH_VARARGS,
   PyDoc_STR("inplace cofactor (VarId, Pol)")},
  {"cofactor", (PyCFunction)TvFunc_icofactor, METH_VARARGS,
   PyDoc_STR("cofactor (VarId, Pol)")},
  {"xform", (PyCFunction)TvFunc_xform, METH_VARARGS,
   PyDoc_STR("NPN transform (NpnMap)")},
  {"ni", (PyCFunction)TvFunc_ni, METH_NOARGS,
   PyDoc_STR("return input number (NONE")},
  {"value", (PyCFunction)TvFunc_value, METH_VARARGS,
   PyDoc_STR("return truth value (int)")},
  {"count_zero", (PyCFunction)TvFunc_count_zero, METH_NOARGS,
   PyDoc_STR("return 0's count (NONE)")},
  {"count_one", (PyCFunction)TvFunc_count_one, METH_NOARGS,
   PyDoc_STR("return 1's count (NONE)")},
  {"walsh_0", (PyCFunction)TvFunc_walsh_0, METH_NOARGS,
   PyDoc_STR("return Walsh 0th coeffecient (NONE)")},
  {"walsh_1", (PyCFunction)TvFunc_walsh_1, METH_VARARGS,
   PyDoc_STR("return Walsh 1st coeffecient (VarId)")},
  {"walsh_2", (PyCFunction)TvFunc_walsh_2, METH_VARARGS,
   PyDoc_STR("return Walsh 2nd coeffecient (VarId, Varid)")},
  {"check_sup", (PyCFunction)TvFunc_check_sup, METH_VARARGS,
   PyDoc_STR("check support (VarId)")},
  {"check_sym", (PyCFunction)TvFunc_check_sup, METH_VARARGS,
   PyDoc_STR("check symmetry (VarId, VarId, Pol)")},
  {"dump", (PyCFunction)TvFunc_dump, METH_VARARGS,
   PyDoc_STR("dump (FileBinO)")},
  {"restore", (PyCFunction)TvFunc_restore, METH_VARARGS,
   PyDoc_STR("restore (FileBinI)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// TvFuncObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyTvFunc_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logic.TvFunc",                // tp_name
  sizeof(TvFuncObject),          // tp_basicsize
  (int)0,                        // tp_itemsize

  // Methods to implement standard operations

  (destructor)TvFunc_dealloc,    // tp_dealloc
  (printfunc)0,                  // tp_print
  (getattrfunc)0,                // tp_getattr
  (setattrfunc)0,                // tp_setattr
  (cmpfunc)0,                    // tp_compare
  (reprfunc)TvFunc_repr,         // tp_repr

  // Method suites for standard classes
  &TvFunc_nbmethods,             // tp_as_number
  0,                             // tp_as_sequence
  0,                             // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)TvFunc_hash,         // tp_hash
  (ternaryfunc)0,                // tp_call
  (reprfunc)0,                   // tp_str
  (getattrofunc)0,               // tp_getattro
  (setattrofunc)0,               // tp_setattro

  // Functions to access object as input/output buffer
  0,                             // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,            // tp_flags

  // Documentation string
  "truth vector",                // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,               // tp_traverse

  // delete references to contained objects
  (inquiry)0,                    // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                // tp_richcompare

  // weak reference enabler
  (long)0,                       // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                // tp_iter
  (iternextfunc)0,               // tp_iternext

  // Attribute descriptor and subclassing stuff
  TvFunc_methods,                // tp_methods
  0,                             // tp_members
  0,                             // tp_getset
  (struct _typeobject*)0,        // tp_base
  (PyObject*)0,                  // tp_dict
  (descrgetfunc)0,               // tp_descr_get
  (descrsetfunc)0,               // tp_descr_set
  (long)0,                       // tp_dictoffset
  (initproc)TvFunc_init,         // tp_init
  (allocfunc)0,                  // tp_alloc
  (newfunc)TvFunc_new,           // tp_new
  (freefunc)0,                   // tp_free
  (inquiry)0,                    // tp_is_gc

  (PyObject*)0,                  // tp_bases
  (PyObject*)0,                  // tp_mro (method resolution order)
  (PyObject*)0,                  // tp_cache
  (PyObject*)0,                  // tp_subclasses
  (PyObject*)0                   // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と TvFunc の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から TvFunc を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj TvFunc を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が TvFuncObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   TvFunc& obj)
{
  // 型のチェック
  if ( !TvFuncObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  TvFuncObject* my_obj = (TvFuncObject*)py_obj;

  obj = *my_obj->mBody;

  return true;
}

// @brief TvFunc から PyObject を生成する．
// @param[in] obj TvFunc オブジェクト
PyObject*
PyTvFunc_FromTvFunc(const TvFunc& obj)
{
  TvFuncObject* py_obj = TvFunc_new(&PyTvFunc_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mBody = new TvFunc(obj);

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// TvFuncObject 関係の初期化を行う．
void
TvFuncObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyTvFunc_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "TvFunc", (PyObject*)&PyTvFunc_Type);
}

END_NAMESPACE_YM

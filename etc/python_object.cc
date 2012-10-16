
/// @file pyutils_@FOO@.cc
/// @brief @FOO@ の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/@FOO@.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// @FOO@ を表す型
struct @FOO@Object
{
  // Python のお約束
  PyObject_HEAD

  // @FOO@ の本体
  @FOO@* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// @FOO@Object の生成関数
@FOO@Object*
@FOO@_new(PyTypeObject* type)
{
  @FOO@Object* self = PyObject_New(@FOO@Object, type);
  if ( self == NULL ) {
    return NULL;
  }

  // @FOO@ の生成を行なう．
  self->mBody = new @FOO@();

  return self;
}

// @FOO@Object を開放する関数
void
@FOO@_dealloc(@FOO@Object* self)
{
  // @FOO@ の開放を行なう．
  delete self->mBody;

  PyObject_Del(self);
}

// 初期化関数
int
@FOO@_init(@FOO@Object* self,
	   PyObject* args)
{
  // args をパーズして初期化を行なう．
  // エラーが起きたらエラーメッセージをセットして -1 を返す．
  // 正常に終了したら 0 を返す．

  return 0;
}

// XXX 関数
PyObject*
@FOO@_XXX(@FOO@Object* self,
	  PyObject* args)
{
  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// @FOO@Object の NumberMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PyNumberMethods @FOO@_nbmethods = {
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
  (binaryfunc)0,               // nb_inplace_add
  (binaryfunc)0,               // nb_inplace_subtract
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
// @FOO@Object の MappingMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PyMappingMethods @FOO@_mpmethods = {
  (lenfunc)0,                  // mp_length
  (binaryfunc)0,               // mp_subscript
  (objobjargproc)0             // mp_ass_subscript
};


//////////////////////////////////////////////////////////////////////
// @FOO@Object の SequenceMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PySequenceMethods @FOO@_sqmethods = {
  (lenfunc)0,                  // sq_length
  (binaryfunc)0,               // sq_concat
  (ssizeargfunc)0,             // sq_repeat
  (ssizeargfunc)0,             // sq_item
  (ssizeobjeargproc)0,         // sq_ass_item
  (objobjproc)0,               // sq_contains
  (binaryfunc)0,               // sq_inplace_concat
  (ssizeargfunc)0              // sq_inplace_repeat
};


//////////////////////////////////////////////////////////////////////
// @FOO@Object のバッファオブジェクト構造体の定義
//////////////////////////////////////////////////////////////////////
PyBufferProcs @FOO@_bfprocs = {
  (getreadbufferproc)0,        // bf_getreadbuffer
  (getwritebufferproc)0,       // bf_getwritebuffer
  (getsegcountproc)0,          // bf_getsegcount
  (getcharbufferproc)0         // bf_getcharbuffer
};


//////////////////////////////////////////////////////////////////////
// @FOO@Object のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef @FOO@_methods[] = {
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
// @FOO@Object 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject @FOO@Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "@FOO@",                      // tp_name
  sizeof(@FOO@Object),          // tp_basicsize
  (int)0,                       // tp_itemsize

  // Methods to implement standard operations

  (destructor)@FOO@_dealloc,    // tp_dealloc
  (printfunc)0,                 // tp_print
  (getattrfunc)0,               // tp_getattr
  (setattrfunc)0,               // tp_setattr
  (cmpfunc)0,                   // tp_compare
  (reprfunc)0,                  // tp_repr

  // Method suites for standard classes
  &@FOO@_nbmethods,             // tp_as_number
  &@FOO@_sqmethods,             // tp_as_sequence
  &@FOO@_mpmethods,             // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                  // tp_hash
  (ternaryfunc)0,               // tp_call
  (reprfunc)0,                  // tp_str
  (getattrofunc)0,              // tp_getattro
  (setattrofunc)0,              // tp_setattro

  // Functions to access object as input/output buffer
  &@FOO@_bfprocs,               // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,           // tp_flags

  // Documentation string
  "documentation string",       // tp_doc

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
  @FOO@_methods,                // tp_methods
  @FOO@_members,                // tp_members
  @FOO@_getset,                 // tp_getset
  (struct _typeobject*)0,       // tp_base
  (PyObject*)0,                 // tp_dict
  (descrgetfunc)0,              // tp_descr_get
  (descrsetfunc)0,              // tp_descr_set
  (long)0,                      // tp_dictoffset
  (initproc)@FOO@_init,         // tp_init
  (allocfunc)0,                 // tp_alloc
  (newfunc)@FOO@_new,           // tp_new
  (freefunc)0,                  // tp_free
  (inquiry)0,                   // tp_is_gc

  (PyObject*)0,                 // tp_bases
  (PyObject*)0,                 // tp_mro (method resolution order)
  (PyObject*)0,                 // tp_cache
  (PyObject*)0,                 // tp_subclasses
  (PyObject*)0                  // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と @FOO@ の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から @FOO@ を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj @FOO@ を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が @FOO@Object ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   @FOO@& obj)
{
  // 型のチェック
  if ( !@FOO@Object_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  @FOO@Object* my_obj = (@FOO@Object*)py_obj;

  obj = *my_obj->mBody;

  return true;
}

// @brief @FOO@ から PyObject を生成する．
// @param[in] obj @FOO@ オブジェクト
PyObject*
conv_to_pyobject(const @FOO@& obj)
{
  @FOO@Object* py_obj = @FOO@_new(&@FOO@Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mBody = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @FOO@Object 関係の初期化を行う．
void
@FOO@Object_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&@FOO@Type) < 0 ) {
    return NULL;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "@FOO@", (PyObject*)&@FOO@Type);
}

END_NAMESPACE_YM_PYTHON


/// @file pycell_%%TYPE_NAME%%.cc
/// @brief %%CTYPE_NAME%% の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// %%CTYPE_NAME%% を表す型
struct %%TYPE_NAME%%Object
{
  // Python のお約束
  PyObject_HEAD

  // 実際の値
  %%CTYPE_NAME%% mVal;

};

%%%PART_A%%%


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// %%TYPE_NAME%%Object の生成関数
// 実際には既存のオブジェクトを返す．
%%TYPE_NAME%%Object*
%%TYPE_NAME%%_new(PyTypeObject* type,
		   PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return NULL;
  }

  return (%%TYPE_NAME%%Object*)Py%%TYPE_NAME%%_FromString(str);
}

// %%TYPE_NAME%%Object を開放する関数
void
%%TYPE_NAME%%_dealloc(%%TYPE_NAME%%Object* self)
{
  PyObject_Del(self);
}

// repr 関数
PyObject*
%%TYPE_NAME%%_repr(%%TYPE_NAME%%Object* self)
{
  PyObject* result = NULL;
  switch ( self->mVal ) {
%%%PART_B%%%
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

//////////////////////////////////////////////////////////////////////
// %%TYPE_NAME%%Object のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef %%TYPE_NAME%%_methods[] = {
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
// %%TYPE_NAME%%Object 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject Py%%TYPE_NAME%%_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.%%TYPE_NAME%%",              // tp_name
  sizeof(%%TYPE_NAME%%Object),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)%%TYPE_NAME%%_dealloc,     // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)%%TYPE_NAME%%_repr,          // tp_repr

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
  "%%TYPE_DOC%%",                 // tp_doc

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
  %%TYPE_NAME%%_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)0,                     // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)%%TYPE_NAME%%_new,            // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と %%CTYPE_NAME%% の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief %%CTYPE_NAME%% から %%TYPE_NAME%%Object を生成する．
// @param[in] technology %%CTYPE_NAME%% の値
PyObject*
Py%%TYPE_NAME%%_From%%TYPE_NAME%%(%%CTYPE_NAME%% technology)
{
  PyObject* result = NULL;
  switch ( technology ) {
%%%PART_C%%%
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// @brief 文字列から %%TYPE_NAME%%Object を生成する．
// @param[in] str 文字列
PyObject*
Py%%TYPE_NAME%%_FromString(const char* str)
{
  PyObject* result = NULL;
  if ( str == NULL ) {
    // デフォルト値
    result = Py_%%DEFAULT_VAL%%
  }
%%%PART_D%%%
  else {
    PyErr_SetString(PyExc_ValueError,
		    "Illegal string for %%CTYPE_NAME%%");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から %%CTYPE_NAME%% を取り出す．
// @param[in] py_obj Python オブジェクト
// @return %%CTYPE_NAME%% を返す．
// @note 変換が失敗したら TypeError を送出し，%%DEFAULT_VAL%% を返す．
%%CTYPE_NAME%%
Py%%TYPE_NAME%%_As%%TYPE_NAME%%(PyObject* py_obj)
{
  // 型のチェック
  if ( !Py%%TYPE_NAME%%_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.%%TYPE_NAME%% is expected");
    return %%DEFAULT_VAL%%;
  }

  // 強制的にキャスト
  %%TYPE_NAME%%Object* my_obj = (%%TYPE_NAME%%Object*)py_obj;

  return my_obj->mVal;
}


//////////////////////////////////////////////////////////////////////
// %%TYPE_NAME%%Object の外部変数
//////////////////////////////////////////////////////////////////////

%%%PART_E%%%


BEGIN_NONAMESPACE

// %%CTYPE_NAME%% の定数を設定する関数
void
obj_set(%%TYPE_NAME%%Object& my_obj,
	PyObject*& py_obj,
	PyObject* module,
	const char* name)
{
  py_obj = (PyObject*)&my_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

END_NONAMESPACE

// %%TYPE_NAME%%Object 関係の初期化を行う．
void
%%TYPE_NAME%%Object_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&Py%%TYPE_NAME%%_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "%%TYPE_NAME%%", (PyObject*)&Py%%TYPE_NAME%%_Type);

  // 定数オブジェクトの生成と登録
%%%PART_F%%%

  // 定数オブジェクト用の文字列オブジェクトの生成
%%%PART_G%%%

}

END_NAMESPACE_YM

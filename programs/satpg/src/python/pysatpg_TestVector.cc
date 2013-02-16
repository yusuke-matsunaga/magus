
/// @file pysatpg_TestVector.cc
/// @brief TestVector の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "pysatpg.h"
#include "TestVector.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// TestVector を表す型
struct TestVectorObject
{
  // Python のお約束
  PyObject_HEAD

  // TestVector のポインタ
  TestVector* mPtr;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////


// TestVectorObject の生成関数
TestVectorObject*
TestVector_new(PyTypeObject* type)
{
  TestVectorObject* self = PyObject_New(TestVectorObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ここではポインタを NULL にしておく．
  self->mPtr = NULL;

  return self;
}

// TestVectorObject を開放する関数
void
TestVector_dealloc(TestVector* self)
{
  // TestVector は開放しない．

  PyObject_Del(self);
}

// 初期化関数
int
TestVector_init(TestVectorObject* self,
		PyObject* args)
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// TestVectorObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef TestVector_methods[] = {
  {NULL, NULL, 0, NULL}
};


END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// TestVector 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyTestVector_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "satpg.TestVector",             // tp_name
  sizeof(TestVectorObject),       // tp_basicsize
  0,                              // tp_itemsize
  /* methods */
  (destructor)TestVector_dealloc, // tp_dealloc
  0,                              // tp_print
  0,                              // tp_getattr
  0,                              // tp_setattr
  0,                              // tp_compare
  0,                              // tp_repr
  0,                              // tp_as_number
  0,                              // tp_as_sequence
  0,                              // tp_as_mapping
  0,                              // tp_hash
  0,                              // tp_call
  (reprfunc)0,                    // tp_str
  0,                              // tp_getattro
  0,                              // tp_setattro
  0,                              // tp_as_buffer
  Py_TPFLAGS_DEFAULT,             // tp_flags
  "test vector",                  // tp_doc
  0,                              // tp_traverse
  0,                              // tp_clear
  0,                              // tp_richcompare
  0,                              // tp_weaklistoffset
  0,                              // tp_iter
  0,                              // tp_iternext
  TestVector_methods,             // tp_methods
  0,                              // tp_members
  0,                              // tp_getset
  0,                              // tp_base
  0,                              // tp_dict
  0,                              // tp_descr_get
  0,                              // tp_descr_set
  0,                              // tp_dictoffset
  (initproc)TestVector_init,      // tp_init
  0,                              // tp_alloc
  (newfunc)TestVector_new,        // tp_new
  0,                              // tp_free
  0,                              // tp_is_gc
};


//////////////////////////////////////////////////////////////////////
// PyObject と TestVector 間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から TestVector を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] pobj TestVector を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が TestVectorObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   TestVector*& pobj)
{
  // 型のチェック
  if ( !TestVectorObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  TestVectorObject* fileloc_obj = (TestVectorObject*)py_obj;

  pobj = fileloc_obj->mPtr;

  return true;
}

// @brief TestVector から PyObject を生成する．
// @param[in] obj TestVector オブジェクト
PyObject*
PyTestVector_FromTestVector(TestVector* obj)
{
  TestVectorObject* py_obj = TestVector_new(&PyTestVector_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mPtr = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// TestVectorObject 関係の初期化を行う．
void
TestVectorObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyTestVector_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "TestVector", (PyObject*)&PyTestVector_Type);
}


END_NAMESPACE_YM_SATPG

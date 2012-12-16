
/// @file pysatpg_FaultMgr.cc
/// @brief FaultMgr の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "pysatpg.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_PYSATPG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// FaultMgr を表す型
struct FaultMgrObject
{
  // Python のお約束
  PyObject_HEAD

  // FaultMgr のポインタ
  FaultMgr* mPtr;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////


// FaultMgrObject の生成関数
FaultMgrObject*
FaultMgr_new(PyTypeObject* type)
{
  FaultMgrObject* self = PyObject_New(FaultMgrObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ここではポインタを NULL にしておく．
  self->mPtr = NULL;

  return self;
}

// FaultMgrObject を開放する関数
void
FaultMgr_dealloc(FaultMgr* self)
{
  // FaultMgr は開放しない．

  PyObject_Del(self);
}

// 初期化関数
int
FaultMgr_init(FaultMgrObject* self,
	      PyObject* args)
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// FaultMgrObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef FaultMgr_methods[] = {
  {NULL, NULL, 0, NULL}
};


END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// FaultMgr 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject FaultMgrType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "satpg.FaultMgr",             // tp_name
  sizeof(FaultMgrObject),       // tp_basicsize
  0,                            // tp_itemsize
  /* methods */
  (destructor)FaultMgr_dealloc, // tp_dealloc
  0,                            // tp_print
  0,                            // tp_getattr
  0,                            // tp_setattr
  0,                            // tp_compare
  0,                            // tp_repr
  0,                            // tp_as_number
  0,                            // tp_as_sequence
  0,                            // tp_as_mapping
  0,                            // tp_hash
  0,                            // tp_call
  (reprfunc)0,                  // tp_str
  0,                            // tp_getattro
  0,                            // tp_setattro
  0,                            // tp_as_buffer
  Py_TPFLAGS_DEFAULT,           // tp_flags
  "fault manager",              // tp_doc
  0,                            // tp_traverse
  0,                            // tp_clear
  0,                            // tp_richcompare
  0,                            // tp_weaklistoffset
  0,                            // tp_iter
  0,                            // tp_iternext
  FaultMgr_methods,             // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)FaultMgr_init,      // tp_init
  0,                            // tp_alloc
  (newfunc)FaultMgr_new,        // tp_new
  0,                            // tp_free
  0,                            // tp_is_gc
};


//////////////////////////////////////////////////////////////////////
// PyObject と FaultMgr 間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から FaultMgr を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] pobj FaultMgr を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が FaultMgrObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   FaultMgr*& pobj)
{
  // 型のチェック
  if ( !FaultMgrObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  FaultMgrObject* fileloc_obj = (FaultMgrObject*)py_obj;

  pobj = fileloc_obj->mPtr;

  return true;
}

// @brief FaultMgr から PyObject を生成する．
// @param[in] obj FaultMgr オブジェクト
PyObject*
FaultMgr_FromFaultMgr(FaultMgr* obj)
{
  FaultMgrObject* py_obj = FaultMgr_new(&FaultMgrType);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mPtr = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// FaultMgrObject 関係の初期化を行う．
void
FaultMgrObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&FaultMgrType) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "FaultMgr", (PyObject*)&FaultMgrType);
}


END_NAMESPACE_YM_PYSATPG

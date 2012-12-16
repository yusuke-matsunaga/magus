
/// @file pysatpg_SaFault.cc
/// @brief SaFault の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "pysatpg.h"
#include "SaFault.h"


BEGIN_NAMESPACE_YM_PYSATPG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// SaFault を表す型
struct SaFaultObject
{
  // Python のお約束
  PyObject_HEAD

  // SaFault のポインタ
  SaFault* mPtr;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////


// SaFaultObject の生成関数
SaFaultObject*
SaFault_new(PyTypeObject* type)
{
  SaFaultObject* self = PyObject_New(SaFaultObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ここではポインタを NULL にしておく．
  self->mPtr = NULL;

  return self;
}

// SaFaultObject を開放する関数
void
SaFault_dealloc(SaFaultObject* self)
{
  // SaFault は開放しない．

  PyObject_Del(self);
}

// 初期化関数
int
SaFault_init(SaFaultObject* self,
	     PyObject* args)
{
  return 0;
}

// str 関数
PyObject*
SaFault_str(SaFaultObject* self)
{
  using namespace nsYm::nsPython;

  SaFault* f = self->mPtr;

  string str;
  if ( f != NULL ) {
    str = f->str();
  }
  else {
    str = "NONE";
  }
  return conv_to_pyobject(str);
}

// is_valid 関数
PyObject*
SaFault_is_valid(SaFaultObject* self,
		 PyObject* args)
{
  using namespace nsYm::nsPython;

  bool is_valid = (self->mPtr != NULL);
  return conv_to_pyobject(is_valid);
}


//////////////////////////////////////////////////////////////////////
// SaFaultObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef SaFault_methods[] = {
  {"is_valid", (PyCFunction)SaFault_is_valid, METH_NOARGS,
   PyDoc_STR("return true if valid (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE



//////////////////////////////////////////////////////////////////////
// SaFaultObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject SaFaultType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "satpg.SaFault",             // tp_name
  sizeof(SaFaultObject),       // tp_basicsize
  0,                           // tp_itemsize
  /* methods */
  (destructor)SaFault_dealloc, // tp_dealloc
  0,                           // tp_print
  0,                           // tp_getattr
  0,                           // tp_setattr
  0,                           // tp_compare
  0,                           // tp_repr
  0,                           // tp_as_number
  0,                           // tp_as_sequence
  0,                           // tp_as_mapping
  0,                           // tp_hash
  0,                           // tp_call
  (reprfunc)SaFault_str,       // tp_str
  0,                           // tp_getattro
  0,                           // tp_setattro
  0,                           // tp_as_buffer
  Py_TPFLAGS_DEFAULT,          // tp_flags
  "stuck-at fault",            // tp_doc
  0,                           // tp_traverse
  0,                           // tp_clear
  0,                           // tp_richcompare
  0,                           // tp_weaklistoffset
  0,                           // tp_iter
  0,                           // tp_iternext
  SaFault_methods,             // tp_methods
  0,                           // tp_members
  0,                           // tp_getset
  0,                           // tp_base
  0,                           // tp_dict
  0,                           // tp_descr_get
  0,                           // tp_descr_set
  0,                           // tp_dictoffset
  (initproc)SaFault_init,      // tp_init
  0,                           // tp_alloc
  (newfunc)SaFault_new,        // tp_new
  0,                           // tp_free
  0,                           // tp_is_gc
};


//////////////////////////////////////////////////////////////////////
// PyObject と SaFault 間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から SaFault を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] pobj SaFault を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が SaFaultObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   SaFault*& pobj)
{
  // 型のチェック
  if ( !SaFaultObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  SaFaultObject* fileloc_obj = (SaFaultObject*)py_obj;

  pobj = fileloc_obj->mPtr;

  return true;
}

// @brief SaFault から PyObject を生成する．
// @param[in] obj SaFault オブジェクト
PyObject*
SaFault_FromSaFault(SaFault* obj)
{
  SaFaultObject* py_obj = SaFault_new(&SaFaultType);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mPtr = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// SaFaultObject 関係の初期化を行う．
void
SaFaultObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&SaFaultType) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "SaFault", (PyObject*)&SaFaultType);
}

END_NAMESPACE_YM_PYSATPG

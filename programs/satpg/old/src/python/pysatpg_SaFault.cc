
/// @file pysatpg_SaFault.cc
/// @brief SaFault の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "pysatpg.h"
#include "SaFault.h"
#include "ym_YmNetworks/TgNode.h"


BEGIN_NAMESPACE_YM_SATPG

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

  // 故障の ID をキーにして故障の PyObject を収めたハッシュ表
  hash_map<ymuint, PyObject*>* mFaultMap;

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
  SaFault* f = self->mPtr;

  string str;
  if ( f != NULL ) {
    str = f->str();
  }
  else {
    str = "NONE";
  }
  return PyObject_FromString(str);
}

// id 関数
PyObject*
SaFault_id(SaFaultObject* self,
	   PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->id());
}

// node 関数
PyObject*
SaFault_node(SaFaultObject* self,
	     PyObject* args)
{
  const TgNode* node = self->mPtr->node();
  return PyObject_FromYmuint32(node->gid());
}

// source_node 関数
PyObject*
SaFault_source_node(SaFaultObject* self,
		    PyObject* args)
{
  const TgNode* node = self->mPtr->source_node();
  if ( node == NULL ) {
    PyErr_SetString(PyExc_ValueError,
		    "not an input fault");
    return NULL;
  }

  return PyObject_FromYmuint32(node->gid());
}

// is_input_fault 関数
PyObject*
SaFault_is_input_fault(SaFaultObject* self,
		       PyObject* args)
{
  return PyObject_FromBool(self->mPtr->is_input_fault());
}

// is_output_fault 関数
PyObject*
SaFault_is_output_fault(SaFaultObject* self,
			PyObject* args)
{
  return PyObject_FromBool(self->mPtr->is_output_fault());
}

// pos 関数
PyObject*
SaFault_pos(SaFaultObject* self,
	    PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->pos());
}

// val 関数
PyObject*
SaFault_val(SaFaultObject* self,
	    PyObject* args)
{
  return PyObject_FromYmint32(self->mPtr->val());
}

// val3 関数
PyObject*
SaFault_val3(SaFaultObject* self,
	     PyObject* args)
{
  return PyVal3_FromVal3(self->mPtr->val3());
}

// status 関数を返す．
PyObject*
SaFault_status(SaFaultObject* self,
	       PyObject* args)
{
  return PyFaultStatus_FromFaultStatus(self->mPtr->status());
}

// pat_num 関数を返す．
PyObject*
SaFault_pat_num(SaFaultObject* self,
		PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->pat_num());
}

// pat 関数
PyObject*
SaFault_pat(SaFaultObject* self,
	    PyObject* args)
{
  ymuint pos;
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return NULL;
  }
  return PyTestVector_FromTestVector(self->mPtr->pat(pos));
}


//////////////////////////////////////////////////////////////////////
// SaFaultObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef SaFault_methods[] = {
  {"id", (PyCFunction)SaFault_id, METH_NOARGS,
   PyDoc_STR("return ID number (NONE)")},
  {"node_id", (PyCFunction)SaFault_node, METH_NOARGS,
   PyDoc_STR("return node (NONE)")},
  {"source_node_id", (PyCFunction)SaFault_source_node, METH_NOARGS,
   PyDoc_STR("return source node (NONE)")},
  {"is_input_fault", (PyCFunction)SaFault_is_input_fault, METH_NOARGS,
   PyDoc_STR("check if input fault (NONE)")},
  {"is_output_fault", (PyCFunction)SaFault_is_output_fault, METH_NOARGS,
   PyDoc_STR("check if output fault (NONE)")},
  {"pos", (PyCFunction)SaFault_pos, METH_NOARGS,
   PyDoc_STR("return fault's input position (NONE)")},
  {"val", (PyCFunction)SaFault_val, METH_NOARGS,
   PyDoc_STR("return fault's value (NONE)")},
  {"val3", (PyCFunction)SaFault_val3, METH_NOARGS,
   PyDoc_STR("return fault's value in ternary mode (NONE)")},
  {"status", (PyCFunction)SaFault_status, METH_NOARGS,
   PyDoc_STR("return fault's status (NONE)")},
  {"pat_num", (PyCFunction)SaFault_pat_num, METH_NOARGS,
   PyDoc_STR("return pattern number (NONE)")},
  {"pat", (PyCFunction)SaFault_pat, METH_VARARGS,
   PyDoc_STR("return test pattern (int)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// SaFaultObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PySaFault_Type = {
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

// @brief SaFault から PyObject を生成する．
// @param[in] obj SaFault オブジェクト
PyObject*
PySaFault_FromSaFault(SaFault* obj)
{
  SaFaultObject* py_obj = SaFault_new(&PySaFault_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mPtr = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief PyObject から SaFault へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return SaFault へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
SaFault*
PySaFault_AsSaFaultPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PySaFault_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "satpg.SaFault is expected");
    return NULL;
  }

  // 強制的にキャスト
  SaFaultObject* my_obj = (SaFaultObject*)py_obj;

  return my_obj->mPtr;
}

// SaFaultObject 関係の初期化を行う．
void
SaFaultObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PySaFault_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "SaFault", (PyObject*)&PySaFault_Type);
}

END_NAMESPACE_YM_SATPG

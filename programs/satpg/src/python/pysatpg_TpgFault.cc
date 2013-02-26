
/// @file pysatpg_TpgFault.cc
/// @brief TpgFault の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "pysatpg.h"
#include "TpgFault.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// TpgFault を表す型
struct TpgFaultObject
{
  // Python のお約束
  PyObject_HEAD

  // TpgFault のポインタ
  TpgFault* mPtr;

  // 故障の ID をキーにして故障の PyObject を収めたハッシュ表
  hash_map<ymuint, PyObject*>* mFaultMap;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////


// TpgFaultObject の生成関数
TpgFaultObject*
TpgFault_new(PyTypeObject* type)
{
  TpgFaultObject* self = PyObject_New(TpgFaultObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // ここではポインタを NULL にしておく．
  self->mPtr = NULL;

  return self;
}

// TpgFaultObject を開放する関数
void
TpgFault_dealloc(TpgFaultObject* self)
{
  // TpgFault は開放しない．

  PyObject_Del(self);
}

// 初期化関数
int
TpgFault_init(TpgFaultObject* self,
	      PyObject* args)
{
  return 0;
}

// str 関数
PyObject*
TpgFault_str(TpgFaultObject* self)
{
  TpgFault* f = self->mPtr;

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
TpgFault_id(TpgFaultObject* self,
	    PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->id());
}

// node 関数
PyObject*
TpgFault_node(TpgFaultObject* self,
	      PyObject* args)
{
  const TpgNode* node = self->mPtr->node();
  return PyObject_FromYmuint32(node->id());
}

// source_node 関数
PyObject*
TpgFault_source_node(TpgFaultObject* self,
		     PyObject* args)
{
  const TpgNode* node = self->mPtr->source_node();
  if ( node == NULL ) {
    PyErr_SetString(PyExc_ValueError,
		    "not an input fault");
    return NULL;
  }

  return PyObject_FromYmuint32(node->id());
}

// is_input_fault 関数
PyObject*
TpgFault_is_input_fault(TpgFaultObject* self,
			PyObject* args)
{
  return PyObject_FromBool(self->mPtr->is_input_fault());
}

// is_output_fault 関数
PyObject*
TpgFault_is_output_fault(TpgFaultObject* self,
			 PyObject* args)
{
  return PyObject_FromBool(self->mPtr->is_output_fault());
}

// pos 関数
PyObject*
TpgFault_pos(TpgFaultObject* self,
	     PyObject* args)
{
  return PyObject_FromYmuint32(self->mPtr->pos());
}

// val 関数
PyObject*
TpgFault_val(TpgFaultObject* self,
	     PyObject* args)
{
  return PyObject_FromYmint32(self->mPtr->val());
}

// val3 関数
PyObject*
TpgFault_val3(TpgFaultObject* self,
	      PyObject* args)
{
  return PyVal3_FromVal3(self->mPtr->val3());
}

// status 関数を返す．
PyObject*
TpgFault_status(TpgFaultObject* self,
		PyObject* args)
{
  return PyFaultStatus_FromFaultStatus(self->mPtr->status());
}


//////////////////////////////////////////////////////////////////////
// TpgFaultObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef TpgFault_methods[] = {
  {"id", (PyCFunction)TpgFault_id, METH_NOARGS,
   PyDoc_STR("return ID number (NONE)")},
  {"node_id", (PyCFunction)TpgFault_node, METH_NOARGS,
   PyDoc_STR("return node (NONE)")},
  {"source_node_id", (PyCFunction)TpgFault_source_node, METH_NOARGS,
   PyDoc_STR("return source node (NONE)")},
  {"is_input_fault", (PyCFunction)TpgFault_is_input_fault, METH_NOARGS,
   PyDoc_STR("check if input fault (NONE)")},
  {"is_output_fault", (PyCFunction)TpgFault_is_output_fault, METH_NOARGS,
   PyDoc_STR("check if output fault (NONE)")},
  {"pos", (PyCFunction)TpgFault_pos, METH_NOARGS,
   PyDoc_STR("return fault's input position (NONE)")},
  {"val", (PyCFunction)TpgFault_val, METH_NOARGS,
   PyDoc_STR("return fault's value (NONE)")},
  {"val3", (PyCFunction)TpgFault_val3, METH_NOARGS,
   PyDoc_STR("return fault's value in ternary mode (NONE)")},
  {"status", (PyCFunction)TpgFault_status, METH_NOARGS,
   PyDoc_STR("return fault's status (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// TpgFaultObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyTpgFault_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "satpg.TpgFault",             // tp_name
  sizeof(TpgFaultObject),       // tp_basicsize
  0,                           // tp_itemsize
  /* methods */
  (destructor)TpgFault_dealloc, // tp_dealloc
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
  (reprfunc)TpgFault_str,       // tp_str
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
  TpgFault_methods,             // tp_methods
  0,                           // tp_members
  0,                           // tp_getset
  0,                           // tp_base
  0,                           // tp_dict
  0,                           // tp_descr_get
  0,                           // tp_descr_set
  0,                           // tp_dictoffset
  (initproc)TpgFault_init,      // tp_init
  0,                           // tp_alloc
  (newfunc)TpgFault_new,        // tp_new
  0,                           // tp_free
  0,                           // tp_is_gc
};


//////////////////////////////////////////////////////////////////////
// PyObject と TpgFault 間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief TpgFault から PyObject を生成する．
// @param[in] obj TpgFault オブジェクト
PyObject*
PyTpgFault_FromTpgFault(TpgFault* obj)
{
  TpgFaultObject* py_obj = TpgFault_new(&PyTpgFault_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mPtr = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief PyObject から TpgFault へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return TpgFault へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
TpgFault*
PyTpgFault_AsTpgFaultPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyTpgFault_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "satpg.TpgFault is expected");
    return NULL;
  }

  // 強制的にキャスト
  TpgFaultObject* my_obj = (TpgFaultObject*)py_obj;

  return my_obj->mPtr;
}

// TpgFaultObject 関係の初期化を行う．
void
TpgFaultObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyTpgFault_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "TpgFault", (PyObject*)&PyTpgFault_Type);
}

END_NAMESPACE_YM_SATPG


/// @file pylogic_Aig.cc
/// @brief Aig の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/pylogic.h"
#include "ym_logic/Aig.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// Aig を表す型
struct AigObject
{
  // Python のお約束
  PyObject_HEAD

  // Aig の本体
  Aig mAig;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// AigObject の生成関数
AigObject*
Aig_new(PyTypeObject* type)
{
  AigObject* self = PyObject_New(AigObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // Aig を生成する．
  // 実はコンストラクタを強制的に呼んでいる．
  new (&self->mAig) Aig();

  return self;
}

// AigObject を開放する関数
void
Aig_dealloc(AigObject* self)
{
  // Aig に関してはなにも必要ない．

  PyObject_Del(self);
}

// 初期化関数
int
Aig_init(AigObject* self,
	 PyObject* args)
{
  // なにもしない．
  return 0;
}

// ハッシュ関数
long
Aig_hash(AigObject* self)
{
  return self->mAig.hash_func();
}

// negate 関数
PyObject*
Aig_negate(AigObject* self,
	   PyObject* args)
{
  return PyAig_FromAig(~self->mAig);
}

// normalize 関数
PyObject*
Aig_normalize(AigObject* self,
	      PyObject* args)
{
  return PyAig_FromAig(self->mAig.normalize());
}

// node_id 関数
PyObject*
Aig_node_id(AigObject* self,
	    PyObject* args)
{
  return conv_to_pyobject(self->mAig.node_id());
}

// inv 関数
PyObject*
Aig_inv(AigObject* self,
	PyObject* args)
{
  return conv_to_pyobject(self->mAig.inv());
}

// is_zero 関数
PyObject*
Aig_is_zero(AigObject* self,
	    PyObject* args)
{
  return conv_to_pyobject(self->mAig.is_zero());
}

// is_one 関数
PyObject*
Aig_is_one(AigObject* self,
	   PyObject* args)
{
  return conv_to_pyobject(self->mAig.is_one());
}

// is_const 関数
PyObject*
Aig_is_const(AigObject* self,
	     PyObject* args)
{
  return conv_to_pyobject(self->mAig.is_const());
}

// is_input 関数
PyObject*
Aig_is_input(AigObject* self,
	     PyObject* args)
{
  return conv_to_pyobject(self->mAig.is_input());
}

// input_id 関数
PyObject*
Aig_input_id(AigObject* self,
	     PyObject* args)
{
  return PyVarId_FromVarId(self->mAig.input_id());
}

// is_and 関数
PyObject*
Aig_is_and(AigObject* self,
	   PyObject* args)
{
  return conv_to_pyobject(self->mAig.is_and());
}

// fanin 関数
PyObject*
Aig_fanin(AigObject* self,
	  PyObject* args)
{
  ymuint pos;
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return NULL;
  }

  return PyAig_FromAig(self->mAig.fanin(pos));
}

// fanin0 関数
PyObject*
Aig_fanin0(AigObject* self,
	   PyObject* args)
{
  return PyAig_FromAig(self->mAig.fanin0());
}

// fanin1 関数
PyObject*
Aig_fanin1(AigObject* self,
	   PyObject* args)
{
  return PyAig_FromAig(self->mAig.fanin1());
}


// AigObject のメソッドテーブル
PyMethodDef Aig_methods[] = {
  {"negate", (PyCFunction)Aig_negate, METH_NOARGS,
   PyDoc_STR("return negated AIG (NONE)")},
  {"normalize", (PyCFunction)Aig_normalize, METH_NOARGS,
   PyDoc_STR("return normalized AIG (NONE)")},
  {"node_id", (PyCFunction)Aig_node_id, METH_NOARGS,
   PyDoc_STR("return Node ID (NONE)")},
  {"inv", (PyCFunction)Aig_inv, METH_NOARGS,
   PyDoc_STR("return INV flag (NONE)")},
  {"is_zero", (PyCFunction)Aig_is_zero, METH_NOARGS,
   PyDoc_STR("return true if representing constant 0 (NONE)")},
  {"is_one", (PyCFunction)Aig_is_one, METH_NOARGS,
   PyDoc_STR("return true if representing constant 1 (NONE)")},
  {"is_const", (PyCFunction)Aig_is_const, METH_NOARGS,
   PyDoc_STR("return true if representing constant (NONE)")},
  {"is_input", (PyCFunction)Aig_is_input, METH_NOARGS,
   PyDoc_STR("return true if representing INPUT (NONE)")},
  {"input_id", (PyCFunction)Aig_input_id, METH_NOARGS,
   PyDoc_STR("return INPUT ID (NONE)")},
  {"is_and", (PyCFunction)Aig_is_and, METH_NOARGS,
   PyDoc_STR("return true if representing AND (NONE)")},
  {"fanin", (PyCFunction)Aig_fanin, METH_NOARGS,
   PyDoc_STR("return fanin's AIG (uint)")},
  {"fanin0", (PyCFunction)Aig_fanin0, METH_NOARGS,
   PyDoc_STR("return fanin0's AIG (NONE)")},
  {"fanin1", (PyCFunction)Aig_fanin1, METH_NOARGS,
   PyDoc_STR("return fanin1's AIG (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


// AigObject 用のタイプオブジェクト
PyTypeObject PyAig_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logic.Aig",            /*tp_name*/
  sizeof(AigObject),      /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)Aig_dealloc,    /*tp_dealloc*/
  0,                          /*tp_print*/
  0,                          /*tp_getattr*/
  0,                          /*tp_setattr*/
  0,                          /*tp_compare*/
  0,                          /*tp_repr*/
  0,                          /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  (hashfunc)Aig_hash,         /*tp_hash*/
  0,                          /*tp_call*/
  0,                          /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "AIG(And-Inverter-Graph)",  /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  Aig_methods,                /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)Aig_init,         /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)Aig_new,           /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

// @brief PyObject から Aig を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj Aig を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が PolObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   Aig& obj)
{
  // 型のチェック
  if ( !AigObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  AigObject* aig_obj = (AigObject*)py_obj;

  obj = aig_obj->mAig;

  return true;
}

// @brief Aig から PyObject を生成する．
// @param[in] obj Aig オブジェクト
PyObject*
PyAig_FromAig(const Aig& obj)
{
  AigObject* aig_obj = Aig_new(&PyAig_Type);
  if ( aig_obj == NULL ) {
    return NULL;
  }

  aig_obj->mAig = obj;

  Py_INCREF(aig_obj);
  return (PyObject*)aig_obj;
}

// AigObject 関係の初期化を行う．
void
AigObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyAig_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Aig", (PyObject*)&PyAig_Type);
}

END_NAMESPACE_YM

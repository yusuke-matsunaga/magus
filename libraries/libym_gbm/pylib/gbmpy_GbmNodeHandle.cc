
/// @file gbmpy_GbmNodeHandle.cc
/// @brief GbmNodeHandle の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "gbmpy.h"
#include "ym_gbm/GbmNodeHandle.h"


BEGIN_NAMESPACE_GBMPY

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// GbmNodeHandle を表す型
struct GbmNodeHandleObject
{
  // Python のお約束
  PyObject_HEAD

  // GbmNodeHandle の本体
  GbmNodeHandle mHandle;

};


//////////////////////////////////////////////////////////////////////
// Ptyhon 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// GbmNodeHandleObject 用の生成関数
GbmNodeHandleObject*
GbmNodeHandle_new(PyTypeObject* type)
{
  GbmNodeHandleObject* self = PyObject_New(GbmNodeHandleObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // GbmNodeHandle のコンストラクタを起動
  new (&self->mHandle) GbmNodeHandle();

  return self;
}

// GbmNodeHandleObject を解放する関数
void
GbmNodeHandle_dealloc(GbmNodeHandleObject* self)
{
  PyObject_Del(self);
}

// 初期化関数
int
GbmNodeHandle_init(GbmNodeHandleObject* self,
		   PyObject* args)
{
  return 0;
}

// str 関数
PyObject*
GbmNodeHandle_str(GbmNodeHandleObject* self)
{
  return Py_BuildValue("s", "gbm");
}

// set 関数
PyObject*
GbmNodeHandle_set(GbmNodeHandleObject* self,
		  PyObject* args)
{
  ymuint id;
  int tmp = 0;
  if ( !PyArg_ParseTuple(args, "I|B", &id, &tmp) ) {
    return NULL;
  }
  bool inv = static_cast<bool>(tmp);
  self->mHandle.set(id, inv);

  Py_INCREF(Py_None);
  return Py_None;
}

// negate 関数
PyObject*
GbmNodeHandle_negate(GbmNodeHandleObject* self,
		     PyObject* args)
{
  self->mHandle.negate();

  Py_INCREF(Py_None);
  return Py_None;
}

// id 関数
PyObject*
GbmNodeHandle_id(GbmNodeHandleObject* self,
		 PyObject* args)
{
  ymuint id = self->mHandle.id();
  return Py_BuildValue("I", id);
}

// inv 関数
PyObject*
GbmNodeHandle_inv(GbmNodeHandleObject* self,
		  PyObject* args)
{
  bool inv = self->mHandle.inv();
  return Py_BuildValue("B", inv);
}

// data 関数
PyObject*
GbmNodeHandle_data(GbmNodeHandleObject* self,
		   PyObject* args)
{
  ymuint data = self->mHandle.data();
  return Py_BuildValue("I", data);
}

// GbmObject のメソッドテーブル
PyMethodDef GbmNodeHandle_methods[] = {
  {"set", (PyCFunction)GbmNodeHandle_set, METH_VARARGS,
   PyDoc_STR("set GbmNodeHandle")},
  {"negate", (PyCFunction)GbmNodeHandle_negate, METH_NOARGS,
   PyDoc_STR("negate GbmNodeHandle")},
  {"id", (PyCFunction)GbmNodeHandle_id, METH_NOARGS,
   PyDoc_STR("return GbmNodeHandle::id()")},
  {"inv", (PyCFunction)GbmNodeHandle_inv, METH_NOARGS,
   PyDoc_STR("return GbmNodeHandle::inv()")},
  {"data", (PyCFunction)GbmNodeHandle_data, METH_NOARGS,
   PyDoc_STR("return GbmNodeHandle::data()")},
  { NULL, NULL, 0, NULL }
};

END_NONAMESPACE

// GbmNodeHandleObject 用のタイプオブジェクト
PyTypeObject GbmNodeHandleType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "gbmpy.GbmNodeHandle",       /*tp_name*/
  sizeof(GbmNodeHandleObject),/*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)GbmNodeHandle_dealloc,/*tp_dealloc*/
  0,                          /*tp_print*/
  0,                          /*tp_getattr*/
  0,                          /*tp_setattr*/
  0,                          /*tp_compare*/
  0,                          /*tp_repr*/
  0,                          /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  0,                          /*tp_hash*/
  0,                          /*tp_call*/
  (reprfunc)GbmNodeHandle_str,/*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "GbmNodeHandle",            /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  GbmNodeHandle_methods,      /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)GbmNodeHandle_init, /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)GbmNodeHandle_new, /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

// @brief PyObject から GbmNodeHandle を取り出す．
// @param[in] obj Python オブジェクト
// @param[out] handle GbmNodeHandle を格納する変数
// @retval true 変換を成功した．
// @retval false 変換が失敗した．obj が GbmNodeHandleObject ではなかった．
bool
covn_from_pyobject(PyObject* obj,
		   GbmNodeHandle& handle)
{
  if ( !GbmNodeHandleObject_Check(obj) ) {
    return false;
  }
  GbmNodeHandleObject* gbm_obj = (GbmNodeHandleObject*)obj;
  handle = gbm_obj->mHandle;

  return true;
}

// @brief GbmNodeHandle から PyObject を生成する．
// @param[in] handle GbmNodeHandle
PyObject*
conv_to_pyobject(GbmNodeHandle handle)
{
  GbmNodeHandleObject* obj = GbmNodeHandle_new(&GbmNodeHandleType);
  if ( obj == NULL ) {
    return NULL;
  }

  obj->mHandle = handle;

  Py_INCREF(obj);
  return (PyObject*)obj;
}

END_NAMESPACE_GBMPY

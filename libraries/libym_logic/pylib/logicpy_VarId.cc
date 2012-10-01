
/// @file logicpy_VarId.cc
/// @brief VarId の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "logicpy.h"
#include "ym_logic/VarId.h"


BEGIN_NAMESPACE_LOGICPY

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// VarId を表す型
struct VarIdObject
{
  // Python のお約束
  PyObject_HEAD

  // VarId の本体
  VarId mVarId;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// VarIdObject の生成関数
VarIdObject*
VarId_new(PyTypeObject* type)
{
  VarIdObject* self = PyObject_New(VarIdObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // VarId 自体の生成は自動的に行われる．

  return self;
}

// VarIdObject を開放する関数
void
VarId_dealloc(VarIdObject* self)
{
  // VarId は開放の必要がない．

  PyObject_Del(self);
}

// 初期化関数
int
VarId_init(VarIdObject* self,
	   PyObject* args)
{
  // なにもしない．
  return 0;
}

// str 関数
PyObject*
VarId_str(VarIdObject* self)
{
  ostringstream buf;
  buf << "V#" << self->mVarId.val();
  return Py_BuildValue("s", buf.str().c_str());
}

// val 関数
PyObject*
VarId_val(VarIdObject* self)
{
  return Py_BuildValue("I", self->mVarId.val());
}

// VarIdObject のメソッドテーブル
PyMethodDef VarId_methods[] = {
  {"val", (PyCFunction)VarId_val, METH_NOARGS,
   PyDoc_STR("return VarId::val()")},
  { NULL, NULL, 0, NULL }
};

END_NONAMESPACE


// VarIdObject 用のタイプオブジェクト
PyTypeObject VarIdType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logicpy.VarId",            /*tp_name*/
  sizeof(VarIdObject),        /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)VarId_dealloc,  /*tp_dealloc*/
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
  (reprfunc)VarId_str,        /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "ID for variable",          /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  VarId_methods,              /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)VarId_init,       /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)VarId_new,         /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

// @brief PyObject から VarId を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj VarId を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が VarIdObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   VarId& obj)
{
  // 型のチェック
  if ( !VarIdObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  VarIdObject* varid_obj = (VarIdObject*)py_obj;

  obj = varid_obj->mVarId;

  return true;
}

// @brief VarId から PyObject を生成する．
// @param[in] obj VarId オブジェクト
PyObject*
conv_to_pyobject(VarId obj)
{
  VarIdObject* varid_obj = VarId_new(&VarIdType);
  if ( varid_obj == NULL ) {
    return NULL;
  }

  varid_obj->mVarId = obj;

  Py_INCREF(varid_obj);
  return (PyObject*)varid_obj;
}

END_NAMESPACE_LOGICPY

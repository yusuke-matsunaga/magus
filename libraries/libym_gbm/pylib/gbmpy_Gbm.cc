
/// @file gbmpy_Gbm.cc
/// @brief Gbm の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "gbmpy.h"
#include "ym_gbm/Gbm.h"


BEGIN_NAMESPACE_GBMPY

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// Gbm を表す型
struct GbmObject
{
  // Python のお約束
  PyObject_HEAD

  // Gbm の本体
  Gbm* mGbm;

};


//////////////////////////////////////////////////////////////////////
// Ptyhon 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// GbmObject 用の生成関数
GbmObject*
Gbm_new(PyTypeObject* type)
{
  GbmObject* self = PyObject_New(GbmObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mGbm = NULL;

  return self;
}

// GbmObject を解放する関数
void
Gbm_dealloc(GbmObject* self)
{
  delete self->mGbm;
  PyObject_Del(self);
}

// 初期化関数
int
Gbm_init(GbmObject* self,
	 PyObject* args)
{
  return 0;
}

// str 関数
PyObject*
Gbm_str(GbmObject* self)
{
  return Py_BuildValue("s", "gbm");
}

// GbmObject のメソッドテーブル
PyMethodDef Gbm_methods[] = {

};

END_NONAMESPACE

// GbmObject 用のタイプオブジェクト
PyTypeObject GbmType = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "gbmpy.Gbm",                /*tp_name*/
    sizeof(GbmObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Gbm_dealloc,    /*tp_dealloc*/
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
    (reprfunc)Gbm_str,          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,         /*tp_flags*/
    "General Boolean Matcher",  /*tp_doc*/
    0,                          /*tp_traverse*/
    0,                          /*tp_clear*/
    0,                          /*tp_richcompare*/
    0,                          /*tp_weaklistoffset*/
    0,                          /*tp_iter*/
    0,                          /*tp_iternext*/
    Gbm_methods,                /*tp_methods*/
    0,                          /*tp_members*/
    0,                          /*tp_getset*/
    0,                          /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    (initproc)Gbm_init,         /*tp_init*/
    0,                          /*tp_alloc*/
    (newfunc)Gbm_new,           /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};

// @brief PyObject から Gbm を取り出す．
// @param[in] obj Python オブジェクト
// @param[out] pgbm Gbm オブジェクトを格納するポインタ
// @retval true 変換を成功した．
// @retval false 変換が失敗した．obj が GbmObject ではなかった．
bool
covn_from_pyobject(PyObject* obj,
		   Gbm* pgbm)
{
  if ( !GbmObject_Check(obj) ) {
    return false;
  }
  GbmObject* gbm_obj = (GbmObject*)obj;
  pgbm = gbm_obj->mGbm;

  return true;
}

// @brief Gbm から PyObject を生成する．
// @param[in] node GbmNode
PyObject*
conv_to_pyobject(Gbm& gbm)
{
  GbmObject* obj = Gbm_new(&GbmType);
  if ( obj == NULL ) {
    return NULL;
  }

  obj->mGbm = &gbm;

  Py_INCREF(obj);
  return (PyObject*)obj;
}

END_NAMESPACE_GBMPY

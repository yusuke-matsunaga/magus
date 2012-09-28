
/// @file gbmpy_GbmNode.cc
/// @brief GbmNode の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "gbmpy.h"
#include "ym_gbm/GbmNode.h"


BEGIN_NAMESPACE_GBMPY

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// GbmNode を表す型
struct GbmNodeObject
{
  // Python のお約束
  PyObject_HEAD

  // GbmNode の本体
  GbmNode* mNode;

};


//////////////////////////////////////////////////////////////////////
// Ptyhon 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// GbmNodeObject 用の生成関数
GbmNodeObject*
GbmNode_new(PyTypeObject* type)
{
  GbmNodeObject* self = PyObject_New(GbmNodeObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mNode = NULL;

  return self;
}

// GbmNodeObject を解放する関数
void
GbmNode_dealloc(GbmNodeObject* self)
{
  PyObject_Del(self);
}

// 初期化関数
int
GbmNode_init(GbmNodeObject* self,
	     PyObject* args)
{
  return 0;
}

// str 関数
PyObject*
GbmNode_str(GbmNodeObject* self)
{
  return Py_BuildValue("s", "");
}

// GbmNodeObject のメソッドテーブル
PyMethodDef GbmNode_methods[] = {

};

END_NONAMESPACE

// GbmNodeObject 用のタイプオブジェクト
PyTypeObject GbmNodeType = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "gbmpy.GbmNode",            /*tp_name*/
    sizeof(GbmNodeObject),      /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)GbmNode_dealloc, /*tp_dealloc*/
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
    (reprfunc)GbmNode_str,      /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,         /*tp_flags*/
    "GbmNode",                  /*tp_doc*/
    0,                          /*tp_traverse*/
    0,                          /*tp_clear*/
    0,                          /*tp_richcompare*/
    0,                          /*tp_weaklistoffset*/
    0,                          /*tp_iter*/
    0,                          /*tp_iternext*/
    GbmNode_methods,            /*tp_methods*/
    0,                          /*tp_members*/
    0,                          /*tp_getset*/
    0,                          /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    (initproc)GbmNode_init,     /*tp_init*/
    0,                          /*tp_alloc*/
    (newfunc)GbmNode_new,       /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};

// @brief PyObject から GbmNode を取り出す．
// @param[in] obj Python オブジェクト
// @param[out] node GbmNode オブジェクトを格納するポインタ
// @retval true 変換を成功した．
// @retval false 変換が失敗した．obj が GbmObject ではなかった．
bool
covn_from_pyobject(PyObject* obj,
		   GbmNode*& node)
{
  if ( !GbmNodeObject_Check(obj) ) {
    return false;
  }
  GbmNodeObject* gbmnode_obj = (GbmNodeObject*)obj;
  node = gbmnode_obj->mNode;

  return true;
}

END_NAMESPACE_GBMPY

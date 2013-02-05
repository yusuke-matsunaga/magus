
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

  // 故障の ID をキーにして故障の PyObject を収めたハッシュ表
  hash_map<ymuint, PyObject*>* mFaultMap;

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

  self->mPtr = new FaultMgr();
  self->mFaultMap = NULL;

  return self;
}

// FaultMgrObject を開放する関数
void
FaultMgr_dealloc(FaultMgrObject* self)
{
  delete self->mPtr;

  delete self->mFaultMap;

  PyObject_Del(self);
}

// 初期化関数
int
FaultMgr_init(FaultMgrObject* self,
	      PyObject* args)
{
  // なにもしない．
  return 0;
}

// 故障に対応する Python オブジェクトを得る．
PyObject*
fobject(SaFault* f,
	hash_map<ymuint, PyObject*>& fmap)
{
  hash_map<ymuint, PyObject*>::iterator p = fmap.find(f->id());
  if ( p == fmap.end() ) {
    PyObject* obj = SaFault_FromSaFault(f);
    fmap.insert(make_pair(f->id(), obj));
    return obj;
  }
  else {
    return p->second;
  }
}

// 故障のリストを表す Python オブジェクトを得る．
PyObject*
flist(const vector<SaFault*>& fault_list,
      hash_map<ymuint, PyObject*>& fmap)
{
  ymuint n = fault_list.size();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    SaFault* f = fault_list[i];
    PyObject* obj1 = fobject(f, fmap);
    PyList_SetItem(list_obj, i, obj1);
  }
  return list_obj;
}

// 全ての故障のリストを得る．
PyObject*
FaultMgr_all_list(FaultMgrObject* self,
		  PyObject* args)
{
  return flist(self->mPtr->all_list(), *self->mFaultMap);
}

// 全ての代表故障のリストを得る．
PyObject*
FaultMgr_rep_list(FaultMgrObject* self,
		  PyObject* args)
{
  return flist(self->mPtr->all_rep_list(), *self->mFaultMap);
}

// 検出済みの代表故障のリストを得る．
PyObject*
FaultMgr_det_list(FaultMgrObject* self,
		  PyObject* args)
{
  return flist(self->mPtr->det_list(), *self->mFaultMap);
}

// 未検出の代表故障のリストを得る．
PyObject*
FaultMgr_remain_list(FaultMgrObject* self,
		     PyObject* args)
{
  return flist(self->mPtr->remain_list(), *self->mFaultMap);
}

// 検出不能故障のリストを得る．
PyObject*
FaultMgr_untest_list(FaultMgrObject* self,
		     PyObject* args)
{
  return flist(self->mPtr->untest_list(), *self->mFaultMap);
}

// 代表故障を取り出す．
PyObject*
FaultMgr_find_rep_fault(FaultMgrObject* self,
			PyObject* args)
{
}

// 出力の故障を取り出す．
PyObject*
FaultMgr_find_ofault(FaultMgrObject* self,
		     PyObject* args)
{
}

// 入力の故障を取り出す．
PyObject*
FaultMgr_find_ifault(FaultMgrObject* self,
		     PyObject* args)
{
}

// クリアする．
PyObject*
FaultMgr_clear(FaultMgrObject* self,
	       PyObject* args)
{
  self->mPtr->clear();
  self->mFaultMap->clear();


}

// 故障の状態を変更する．
PyObject*
FaultMgr_set_status(FaultMgrObject* self,
		    PyObject* args)
{
}


//////////////////////////////////////////////////////////////////////
// FaultMgrObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef FaultMgr_methods[] = {
  {"all_list", (PyCFunction)FaultMgr_all_list, METH_NOARGS,
   PyDoc_STR("return list of all faults (NONE)")},
  {"rep_list", (PyCFunction)FaultMgr_rep_list, METH_NOARGS,
   PyDoc_STR("return list of representative faults (NONE)")},
  {"det_list", (PyCFunction)FaultMgr_det_list, METH_NOARGS,
   PyDoc_STR("return list of detected faults (NONE)")},
  {"remain_list", (PyCFunction)FaultMgr_remain_list, METH_NOARGS,
   PyDoc_STR("return list of remain faults (NONE)")},
  {"untest_list", (PyCFunction)FaultMgr_untest_list, METH_NOARGS,
   PyDoc_STR("return list of untestable faults (NONE)")},
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

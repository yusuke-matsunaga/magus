
/// @file logicpy_Bdd.cc
/// @brief Bdd の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "logicpy.h"
#include "ym_logic/Bdd.h"
#include "ym_logic/BddLitSet.h"
#include "ym_logic/BddVarSet.h"


BEGIN_NAMESPACE_LOGICPY

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// Bdd を表す型
struct BddObject
{
  // Python のお約束
  PyObject_HEAD

  // Bdd の本体
  Bdd* mBdd;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// BddObject の生成関数
BddObject*
Bdd_new(PyTypeObject* type)
{
  BddObject* self = PyObject_New(BddObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // Bdd を生成する．
  self->mBdd = new Bdd;

  return self;
}

// BddObject を開放する関数
void
Bdd_dealloc(BddObject* self)
{
  // Bdd を開放する
  delete self->mBdd;

  PyObject_Del(self);
}

// 初期化関数
int
Bdd_init(BddObject* self,
	 PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "|O!", &BddMgrType, &obj) ) {
    return NULL;
  }

  if ( self->mBdd != NULL ) {
    delete self->mBdd;
  }

  if ( obj != NULL ) {
    BddMgr* mgr_p;
    if ( !conv_from_pyobject(obj, mgr_p) ) {
      return NULL;
    }
    self->mBdd = new Bdd(*mgr_p);
  }
  else {
    self->mBdd = new Bdd;
  }

  return 0;
}

// ハッシュ関数
long
Bdd_hash(BddObject* self)
{
  return self->mBdd->hash();
}

// is_zero 関数
PyObject*
Bdd_is_zero(BddObject* self,
	    PyObject* args)
{
  return Py_BuildValue("b", self->mBdd->is_zero());
}

// is_one 関数
PyObject*
Bdd_is_one(BddObject* self,
	   PyObject* args)
{
  return Py_BuildValue("b", self->mBdd->is_one());
}

// is_const 関数
PyObject*
Bdd_is_const(BddObject* self,
	     PyObject* args)
{
  return Py_BuildValue("b", self->mBdd->is_const());
}

// is_overflow 関数
PyObject*
Bdd_is_overflow(BddObject* self,
		PyObject* args)
{
  return Py_BuildValue("b", self->mBdd->is_overflow());
}

// is_error 関数
PyObject*
Bdd_is_error(BddObject* self,
	     PyObject* args)
{
  return Py_BuildValue("b", self->mBdd->is_error());
}

// is_invalid 関数
PyObject*
Bdd_is_invalid(BddObject* self,
	       PyObject* args)
{
  return Py_BuildValue("b", self->mBdd->is_invalid());
}

// is_leaf 関数
PyObject*
Bdd_is_leaf(BddObject* self,
	    PyObject* args)
{
  return Py_BuildValue("b", self->mBdd->is_leaf());
}

// is_posi_cube 関数
PyObject*
Bdd_is_posi_cube(BddObject* self,
		 PyObject* args)
{
  return Py_BuildValue("b", self->mBdd->is_posi_cube());
}

// is_cube 関数
PyObject*
Bdd_is_cube(BddObject* self,
	    PyObject* args)
{
  return Py_BuildValue("b", self->mBdd->is_cube());
}

// check_symmetry 関数
PyObject*
Bdd_check_symmetry(BddObject* self,
		   PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  PyObject* obj3 = NULL;
  if ( !PyArg_ParseTuple(args, "O!O!|O!", &VarIdType, &obj1, &VarIdType, &obj2, &PolType, &obj3) ) {
    return NULL;
  }

  VarId vid1;
  if ( !conv_from_pyobject(obj1, vid1) ) {
    return NULL;
  }

  VarId vid2;
  if ( !conv_from_pyobject(obj2, vid2) ) {
    return NULL;
  }

  tPol pol = kPolPosi;
  if ( obj3 != NULL ) {
    if ( !conv_from_pyobject(obj3, pol) ) {
      return NULL;
    }
  }

  return Py_BuildValue("b", self->mBdd->check_symmetry(vid1, vid2, pol));
}

// set_zero 関数
PyObject*
Bdd_set_zero(BddObject* self,
	     PyObject* args)
{
  self->mBdd->set_zero();

  Py_INCREF(Py_None);
  return Py_None;
}

// set_one 関数
PyObject*
Bdd_set_one(BddObject* self,
	    PyObject* args)
{
  self->mBdd->set_one();

  Py_INCREF(Py_None);
  return Py_None;
}

// set_overflow 関数
PyObject*
Bdd_set_overflow(BddObject* self,
		 PyObject* args)
{
  self->mBdd->set_overflow();

  Py_INCREF(Py_None);
  return Py_None;
}

// set_error 関数
PyObject*
Bdd_set_error(BddObject* self,
	      PyObject* args)
{
  self->mBdd->set_error();

  Py_INCREF(Py_None);
  return Py_None;
}

// invert 関数
PyObject*
Bdd_invert(BddObject* self,
	   PyObject* args)
{
  return conv_to_pyobject(~(*self->mBdd));
}

// root_decomp 関数
PyObject*
Bdd_root_decomp(BddObject* self,
		PyObject* args)
{
  Bdd f0;
  Bdd f1;
  VarId vid = self->mBdd->root_decomp(f0, f1);
  PyObject* obj1 = conv_to_pyobject(vid);
  PyObject* obj2 = conv_to_pyobject(f0);
  PyObject* obj3 = conv_to_pyobject(f1);
  return Py_BuildValue("OOO", obj1, obj2, obj3);
}

// root_var 関数
PyObject*
Bdd_root_var(BddObject* self,
	     PyObject* args)
{
  return conv_to_pyobject(self->mBdd->root_var());
}

// edge0 関数
PyObject*
Bdd_edge0(BddObject* self,
	  PyObject* args)
{
  return conv_to_pyobject(self->mBdd->edge0());
}

// edge1 関数
PyObject*
Bdd_edge1(BddObject* self,
	  PyObject* args)
{
  return conv_to_pyobject(self->mBdd->edge1());
}

// cofactor 関数
// Literal か (VarId, Pol) を引数にとる．
PyObject*
Bdd_cofactor(BddObject* self,
	     PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  if ( !PyArg_ParseTuple(args, "O|O", &obj1, &obj2) ) {
    return NULL;
  }

  Literal lit;
  if ( obj2 != NULL ) {
    // obj1 は VarId, obj2 は Pol でなければならない．
    VarId vid;
    if ( !conv_from_pyobject(obj1, vid) ) {
      PyErr_SetString(ErrorObject, "must be logic.VarId");
      return NULL;
    }
    tPol pol;
    if ( !conv_from_pyobject(obj2, pol) ) {
      PyErr_SetString(ErrorObject, "must be logic.Pol");
      return NULL;
    }
    lit.set(vid, pol);
  }
  else if ( !conv_from_pyobject(obj1, lit) ) {
    PyErr_SetString(ErrorObject, "must be logic.Literal");
    return NULL;
  }

  return conv_to_pyobject(self->mBdd->cofactor(lit));
}

// xor_moment 関数
// VarId を引数にとる．
PyObject*
Bdd_xor_moment(BddObject* self,
	       PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &VarIdType, &obj) ) {
    return NULL;
  }

  VarId vid;
  if ( !conv_from_pyobject(obj, vid) ) {
    return NULL;
  }

  return conv_to_pyobject(self->mBdd->xor_moment(vid));
}

// SCC 関数
PyObject*
Bdd_SCC(BddObject* self,
	PyObject* args)
{
  return conv_to_pyobject(self->mBdd->SCC());
}

// compose 関数
PyObject*
Bdd_compose(BddObject* self,
	    PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  if ( !PyArg_ParseTuple(args, "O!O!", &VarIdType, &obj1, &BddType, &obj2) ) {
    return NULL;
  }

  VarId vid;
  if ( !conv_from_pyobject(obj1, vid) ) {
    return NULL;
  }

  Bdd sub_bdd;
  if ( !conv_from_pyobject(obj2, sub_bdd) ) {
    return NULL;
  }

  return conv_to_pyobject(self->mBdd->compose(vid, sub_bdd));
}

// multi_compose 関数
PyObject*
Bdd_multi_compose(BddObject* self,
		  PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &PyDict_Type, &obj) ) {
    return NULL;
  }

  VarBddMap comp_map;
  PyObject* item_list = PyDict_Items(obj);
  ymuint n = PyList_GET_SIZE(item_list);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* item = PyList_GET_ITEM(item_list, i);
    PyObject* vid_obj = NULL;
    PyObject* sub_obj = NULL;
    if ( !PyArg_ParseTuple(item, "O!O!", &VarIdType, &vid_obj, &BddType, &sub_obj) ) {
      return NULL;
    }

    VarId vid;
    if ( !conv_from_pyobject(vid_obj, vid) ) {
      return NULL;
    }

    Bdd sub_func;
    if ( !conv_from_pyobject(sub_obj, sub_func) ) {
      return NULL;
    }
    comp_map.insert(make_pair(vid, sub_func));
  }

  return conv_to_pyobject(self->mBdd->compose(comp_map));

}

// remap_var 関数
PyObject*
Bdd_remap_var(BddObject* self,
	      PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &PyDict_Type, &obj) ) {
    return NULL;
  }

  VarVarMap var_map;
  PyObject* item_list = PyDict_Items(obj);
  ymuint n = PyList_GET_SIZE(item_list);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* item = PyList_GET_ITEM(item_list, i);
    PyObject* obj1 = NULL;
    PyObject* obj2 = NULL;
    if ( !PyArg_ParseTuple(item, "O!O!", &VarIdType, &obj1, &VarIdType, &obj2) ) {
      return NULL;
    }

    VarId vid;
    if ( !conv_from_pyobject(obj1, vid) ) {
      return NULL;
    }

    VarId new_vid;
    if ( !conv_from_pyobject(obj2, new_vid) ) {
      return NULL;
    }
    var_map.insert(make_pair(vid, new_vid));
  }

  return conv_to_pyobject(self->mBdd->remap_var(var_map));
}

// esmooth 関数
PyObject*
Bdd_esmooth(BddObject* self,
	    PyObject* args)
{
  BddVarSet vset(*self->mBdd);
  if ( PyTuple_GET_SIZE(args) == 1 && PyList_Check(PyTuple_GET_ITEM(args, 0)) ) {
    PyObject* list_obj = PyTuple_GET_ITEM(args, 0);
    ymuint n = PyList_GET_SIZE(list_obj);
    for (ymuint i = 0; i < n; ++ i) {
      PyObject* obj = PyList_GET_ITEM(list_obj, i);
      PyObject* vid_obj = NULL;
      if ( !PyArg_ParseTuple(obj, "O!", &VarIdType, &vid_obj) ) {
	return NULL;
      }

      VarId vid;
      if ( !conv_from_pyobject(vid_obj, vid) ) {
	return NULL;
      }
    }
  }
  else {
    ymuint n = PyTuple_GET_SIZE(args);
    for (ymuint i = 0; i < n; ++ i) {
      PyObject* obj = PyTuple_GET_ITEM(args, i);
      PyObject* vid_obj = NULL;
      if ( !PyArg_ParseTuple(obj, "O!", &VarIdType, &vid_obj) ) {
	return NULL;
      }

      VarId vid;
      if ( !conv_from_pyobject(vid_obj, vid) ) {
	return NULL;
      }

    }
  }

  return conv_to_pyobject(self->mBdd->esmooth(vset));
}

// asmooth 関数
PyObject*
Bdd_asmooth(BddObject* self,
	    PyObject* args)
{
}

// push_down 関数
PyObject*
Bdd_push_down(BddObject* self,
	      PyObject* args)
{
  ymuint x_level;
  ymuint y_level;
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "kk|O!", &x_level, &y_level, &PolType, &obj) ) {
    return NULL;
  }

  tPol pol = kPolPosi;
  if ( obj != NULL ) {
    if ( !conv_from_pyobject(obj, pol) ) {
      return NULL;
    }
  }

  return conv_to_pyobject(self->mBdd->push_down(x_level, y_level, pol));
}

// sop 関数
PyObject*
Bdd_sop(BddObject* self,
	PyObject* args)
{
  return conv_to_pyobject(self->mBdd->sop());
}

// node_count 関数
PyObject*
Bdd_node_count(BddObject* self,
	       PyObject* args)
{
  return Py_BuildValue("K", self->mBdd->node_count());
}

// density 関数
PyObject*
Bdd_density(BddObject* self,
	    PyObject* args)
{
  ymuint nvar;
  if ( !PyArg_ParseTuple(args, "k", &nvar) ) {
    return NULL;
  }

  return Py_BuildValue("d", self->mBdd->density(nvar));
}

// onepath 関数
PyObject*
Bdd_onepath(BddObject* self,
	    PyObject* args)
{
  BddLitSet bdd_litset = self->mBdd->onepath();
  // これを Literal のタプルに変換
}

// shortest_onepath 関数
PyObject*
Bdd_shortest_onepath(BddObject* self,
		     PyObject* args)
{
  BddLitSet bdd_litset = self->mBdd->shortest_onepath();
  // これを Literal のタプルに変換
}

// shortest_onpath_len 関数
PyObject*
Bdd_shortest_onepath_len(BddObject* self,
			 PyObject* args)
{
  return Py_BuildValue("k", self->mBdd->shortest_onepath_len());
}

// support 関数
PyObject*
Bdd_support(BddObject* self,
	    PyObject* args)
{
}

// print 関数
PyObject*
Bdd_print(BddObject* self,
	  PyObject* args)
{
}

// BddObject のメソッドテーブル
PyMethodDef Bdd_methods[] = {
  {"is_zero", (PyCFunction)Bdd_is_zero, METH_NOARGS,
   PyDoc_STR("check if constant 0 (NONE)")},
  {"is_one", (PyCFunction)Bdd_is_one, METH_NOARGS,
   PyDoc_STR("check if constant 1 (NONE)")},
  {"is_overflow", (PyCFunction)Bdd_is_overflow, METH_NOARGS,
   PyDoc_STR("check if overflow (NONE)")},
  {"is_error", (PyCFunction)Bdd_is_error, METH_NOARGS,
   PyDoc_STR("check if error (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE

// BddObject 用のタイプオブジェクト
PyTypeObject BddType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logic.Bdd",               /*tp_name*/
  sizeof(BddObject),         /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)Bdd_dealloc,    /*tp_dealloc*/
  0,                          /*tp_print*/
  0,                          /*tp_getattr*/
  0,                          /*tp_setattr*/
  0,                          /*tp_compare*/
  0,                          /*tp_repr*/
  0,                          /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  (hashfunc)Bdd_hash,         /*tp_hash*/
  0,                          /*tp_call*/
  0,                          /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "BDD",                      /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  Bdd_methods,                /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)Bdd_init,         /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)Bdd_new,           /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

// @brief PyObject から Bdd を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj Bdd を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が BddObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   Bdd& obj)
{
  // 型のチェック
  if ( !BddObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  BddObject* bdd_obj = (BddObject*)py_obj;

  obj = *(bdd_obj->mBdd);

  return true;
}

// @brief Bdd から PyObject を生成する．
// @param[in] obj Bdd オブジェクト
PyObject*
conv_to_pyobject(const Bdd& obj)
{
  BddObject* bdd_obj = Bdd_new(&BddType);
  if ( bdd_obj == NULL ) {
    return NULL;
  }

  *bdd_obj->mBdd = obj;

  Py_INCREF(bdd_obj);
  return (PyObject*)bdd_obj;
}

END_NAMESPACE_LOGICPY

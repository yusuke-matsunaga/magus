
/// @file pylogic_Bdd.cc
/// @brief Bdd の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/pylogic.h"
#include "ym_utils/pyutils.h"
#include "ym_logic/Bdd.h"
#include "ym_logic/BddLitSet.h"
#include "ym_logic/BddVarSet.h"
#include "ym_utils/FileBinI.h"
#include "ym_utils/FileBinO.h"


BEGIN_NAMESPACE_YM

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
  if ( !PyArg_ParseTuple(args, "|O!",
			 &PyBddMgr_Type, &obj) ) {
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

// inv 演算
PyObject*
Bdd_inv(PyObject* left)
{
  if ( BddObject_Check(left) ) {
    BddObject* obj1 = (BddObject*)left;
    return PyBdd_FromBdd(~(*obj1->mBdd));
  }
  PyErr_SetString(PyExc_TypeError, "logic.Bdd is expected");
  return NULL;
}

// and 演算
PyObject*
Bdd_and(PyObject* left,
	PyObject* right)
{
  if ( BddObject_Check(left) && BddObject_Check(right) ) {
    BddObject* obj1 = (BddObject*)left;
    BddObject* obj2 = (BddObject*)right;
    return PyBdd_FromBdd(*obj1->mBdd & *obj2->mBdd);
  }
  PyErr_SetString(PyExc_TypeError, "logic.Bdd is expected");
  return NULL;
}

// or 演算
PyObject*
Bdd_or(PyObject* left,
       PyObject* right)
{
  if ( BddObject_Check(left) && BddObject_Check(right) ) {
    BddObject* obj1 = (BddObject*)left;
    BddObject* obj2 = (BddObject*)right;
    return PyBdd_FromBdd(*obj1->mBdd | *obj2->mBdd);
  }
  PyErr_SetString(PyExc_TypeError, "logic.Bdd is expected");
  return NULL;
}

// xor 演算
PyObject*
Bdd_xor(PyObject* left,
	PyObject* right)
{
  if ( BddObject_Check(left) && BddObject_Check(right) ) {
    BddObject* obj1 = (BddObject*)left;
    BddObject* obj2 = (BddObject*)right;
    return PyBdd_FromBdd(*obj1->mBdd ^ *obj2->mBdd);
  }
  PyErr_SetString(PyExc_TypeError, "logic.Bdd is expected");
  return NULL;
}

// inplace and 演算
PyObject*
Bdd_iand(PyObject* left,
	 PyObject* right)
{
  if ( BddObject_Check(left) && BddObject_Check(right) ) {
    BddObject* obj1 = (BddObject*)left;
    BddObject* obj2 = (BddObject*)right;
    *obj1->mBdd &= *obj2->mBdd;
    Py_INCREF(left);
    return left;
  }
  PyErr_SetString(PyExc_TypeError, "logic.Bdd is expected");
  return NULL;
}

// inplace xor 演算
PyObject*
Bdd_ixor(PyObject* left,
	 PyObject* right)
{
  if ( BddObject_Check(left) && BddObject_Check(right) ) {
    BddObject* obj1 = (BddObject*)left;
    BddObject* obj2 = (BddObject*)right;
    *obj1->mBdd ^= *obj2->mBdd;
    Py_INCREF(left);
    return left;
  }
  PyErr_SetString(PyExc_TypeError, "logic.Bdd is expected");
  return NULL;
}

// inplace or 演算
PyObject*
Bdd_ior(PyObject* left,
	PyObject* right)
{
  if ( BddObject_Check(left) && BddObject_Check(right) ) {
    BddObject* obj1 = (BddObject*)left;
    BddObject* obj2 = (BddObject*)right;
    *obj1->mBdd |= *obj2->mBdd;
    Py_INCREF(left);
    return left;
  }
  PyErr_SetString(PyExc_TypeError, "logic.Bdd is expected");
  return NULL;
}

// is_zero 関数
PyObject*
Bdd_is_zero(BddObject* self,
	    PyObject* args)
{
  return conv_to_pyobject(self->mBdd->is_zero());
}

// is_one 関数
PyObject*
Bdd_is_one(BddObject* self,
	   PyObject* args)
{
  return conv_to_pyobject(self->mBdd->is_one());
}

// is_const 関数
PyObject*
Bdd_is_const(BddObject* self,
	     PyObject* args)
{
  return conv_to_pyobject(self->mBdd->is_const());
}

// is_overflow 関数
PyObject*
Bdd_is_overflow(BddObject* self,
		PyObject* args)
{
  return conv_to_pyobject(self->mBdd->is_overflow());
}

// is_error 関数
PyObject*
Bdd_is_error(BddObject* self,
	     PyObject* args)
{
  return conv_to_pyobject(self->mBdd->is_error());
}

// is_invalid 関数
PyObject*
Bdd_is_invalid(BddObject* self,
	       PyObject* args)
{
  return conv_to_pyobject(self->mBdd->is_invalid());
}

// is_leaf 関数
PyObject*
Bdd_is_leaf(BddObject* self,
	    PyObject* args)
{
  return conv_to_pyobject(self->mBdd->is_leaf());
}

// is_posi_cube 関数
PyObject*
Bdd_is_posi_cube(BddObject* self,
		 PyObject* args)
{
  return conv_to_pyobject(self->mBdd->is_posi_cube());
}

// is_cube 関数
PyObject*
Bdd_is_cube(BddObject* self,
	    PyObject* args)
{
  return conv_to_pyobject(self->mBdd->is_cube());
}

// check_symmetry 関数
PyObject*
Bdd_check_symmetry(BddObject* self,
		   PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  PyObject* obj3 = NULL;
  if ( !PyArg_ParseTuple(args, "O!O!|O!",
			 &PyVarId_Type, &obj1,
			 &PyVarId_Type, &obj2,
			 &PyPol_Type, &obj3) ) {
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

  bool stat = self->mBdd->check_symmetry(vid1, vid2, pol);
  return conv_to_pyobject(stat);
}

// root_decomp 関数
PyObject*
Bdd_root_decomp(BddObject* self,
		PyObject* args)
{
  Bdd f0;
  Bdd f1;
  VarId vid = self->mBdd->root_decomp(f0, f1);
  PyObject* obj1 = PyVarId_FromVarId(vid);
  PyObject* obj2 = PyBdd_FromBdd(f0);
  PyObject* obj3 = PyBdd_FromBdd(f1);
  return Py_BuildValue("OOO", obj1, obj2, obj3);
}

// root_var 関数
PyObject*
Bdd_root_var(BddObject* self,
	     PyObject* args)
{
  return PyVarId_FromVarId(self->mBdd->root_var());
}

// edge0 関数
PyObject*
Bdd_edge0(BddObject* self,
	  PyObject* args)
{
  return PyBdd_FromBdd(self->mBdd->edge0());
}

// edge1 関数
PyObject*
Bdd_edge1(BddObject* self,
	  PyObject* args)
{
  return PyBdd_FromBdd(self->mBdd->edge1());
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
      PyErr_SetString(PyExc_TypeError, "must be logic.VarId");
      return NULL;
    }
    tPol pol;
    if ( !conv_from_pyobject(obj2, pol) ) {
      PyErr_SetString(PyExc_TypeError, "must be logic.Pol");
      return NULL;
    }
    lit.set(vid, pol);
  }
  else if ( !conv_from_pyobject(obj1, lit) ) {
    PyErr_SetString(PyExc_TypeError, "must be logic.Literal");
    return NULL;
  }

  return PyBdd_FromBdd(self->mBdd->cofactor(lit));
}

// xor_moment 関数
// VarId を引数にとる．
PyObject*
Bdd_xor_moment(BddObject* self,
	       PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyVarId_Type, &obj) ) {
    return NULL;
  }

  VarId vid;
  if ( !conv_from_pyobject(obj, vid) ) {
    return NULL;
  }

  return PyBdd_FromBdd(self->mBdd->xor_moment(vid));
}

// SCC 関数
PyObject*
Bdd_SCC(BddObject* self,
	PyObject* args)
{
  return PyBdd_FromBdd(self->mBdd->SCC());
}

// compose 関数
PyObject*
Bdd_compose(BddObject* self,
	    PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  if ( !PyArg_ParseTuple(args, "O!O!",
			 &PyVarId_Type, &obj1,
			 &PyBdd_Type, &obj2) ) {
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

  return PyBdd_FromBdd(self->mBdd->compose(vid, sub_bdd));
}

// multi_compose 関数
PyObject*
Bdd_multi_compose(BddObject* self,
		  PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyDict_Type, &obj) ) {
    return NULL;
  }

  VarBddMap comp_map;
  PyObject* item_list = PyDict_Items(obj);
  ymuint n = PyList_GET_SIZE(item_list);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* item = PyList_GET_ITEM(item_list, i);
    PyObject* vid_obj = NULL;
    PyObject* sub_obj = NULL;
    if ( !PyArg_ParseTuple(item, "O!O!",
			   &PyVarId_Type, &vid_obj,
			   &PyBdd_Type, &sub_obj) ) {
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

  return PyBdd_FromBdd(self->mBdd->compose(comp_map));

}

// remap_var 関数
PyObject*
Bdd_remap_var(BddObject* self,
	      PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyDict_Type, &obj) ) {
    return NULL;
  }

  VarVarMap var_map;
  PyObject* item_list = PyDict_Items(obj);
  ymuint n = PyList_GET_SIZE(item_list);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* item = PyList_GET_ITEM(item_list, i);
    PyObject* obj1 = NULL;
    PyObject* obj2 = NULL;
    if ( !PyArg_ParseTuple(item, "O!O!",
			   &PyVarId_Type, &obj1,
			   &PyVarId_Type, &obj2) ) {
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

  return PyBdd_FromBdd(self->mBdd->remap_var(var_map));
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
      if ( !PyArg_ParseTuple(obj, "O!",
			     &PyVarId_Type, &vid_obj) ) {
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
      if ( !PyArg_ParseTuple(obj, "O!",
			     &PyVarId_Type, &vid_obj) ) {
	return NULL;
      }

      VarId vid;
      if ( !conv_from_pyobject(vid_obj, vid) ) {
	return NULL;
      }

    }
  }

  return PyBdd_FromBdd(self->mBdd->esmooth(vset));
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
  if ( !PyArg_ParseTuple(args, "kk|O!", &x_level, &y_level,
			 &PyPol_Type, &obj) ) {
    return NULL;
  }

  tPol pol = kPolPosi;
  if ( obj != NULL ) {
    if ( !conv_from_pyobject(obj, pol) ) {
      return NULL;
    }
  }

  return PyBdd_FromBdd(self->mBdd->push_down(x_level, y_level, pol));
}

// sop 関数
PyObject*
Bdd_sop(BddObject* self,
	PyObject* args)
{
  return PyLogExpr_FromLogExpr(self->mBdd->sop());
}

// node_count 関数
PyObject*
Bdd_node_count(BddObject* self,
	       PyObject* args)
{
  return conv_to_pyobject(self->mBdd->node_count());
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

  return conv_to_pyobject(self->mBdd->density(nvar));
}

// onepath 関数
PyObject*
Bdd_onepath(BddObject* self,
	    PyObject* args)
{
  BddLitSet bdd_litset = self->mBdd->onepath();
  ymuint n = bdd_litset.size();
  PyObject* ans_list = PyList_New(n);
  ymuint i = 0;
  for (BddLitSet::iterator p = bdd_litset.begin();
       p != bdd_litset.end(); ++ p, ++ i) {
    Literal lit = *p;
    PyObject* obj = PyLiteral_FromLiteral(lit);
    PyList_SetItem(ans_list, i, obj);
  }

  return ans_list;
}

// shortest_onepath 関数
PyObject*
Bdd_shortest_onepath(BddObject* self,
		     PyObject* args)
{
  BddLitSet bdd_litset = self->mBdd->shortest_onepath();
  ymuint n = bdd_litset.size();
  PyObject* ans_list = PyList_New(n);
  ymuint i = 0;
  for (BddLitSet::iterator p = bdd_litset.begin();
       p != bdd_litset.end(); ++ p, ++ i) {
    Literal lit = *p;
    PyObject* obj = PyLiteral_FromLiteral(lit);
    PyList_SetItem(ans_list, i, obj);
  }

  return ans_list;
}

// shortest_onpath_len 関数
PyObject*
Bdd_shortest_onepath_len(BddObject* self,
			 PyObject* args)
{
  return conv_to_pyobject(self->mBdd->shortest_onepath_len());
}

// support 関数
PyObject*
Bdd_support(BddObject* self,
	    PyObject* args)
{
  VarVector sup;
  self->mBdd->support(sup);
  ymuint n = sup.size();
  PyObject* ans_list = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    VarId vid = sup[i];
    PyObject* obj = PyVarId_FromVarId(vid);
    PyList_SetItem(ans_list, i, obj);
  }

  return ans_list;
}

// print 関数
PyObject*
Bdd_print(BddObject* self,
	  PyObject* args)
{
  // 手抜き
  self->mBdd->print(cout);

  Py_INCREF(Py_None);
  return Py_None;
}

// dump 関数
PyObject*
Bdd_dump(BddObject* self,
	 PyObject* args)
{
  FileBinO* bp = parse_FileBinO(args);
  if ( bp == NULL ) {
    return NULL;
  }

  self->mBdd->dump(*bp);

  Py_INCREF(Py_None);
  return Py_None;
}

// restore 関数
PyObject*
Bdd_restore(BddObject* self,
	    PyObject* args)
{
  FileBinI* bp = parse_FileBinI(args);
  if ( bp == NULL ) {
    return NULL;
  }

  self->mBdd->restore(*bp);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// BddObject の NumberMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PyNumberMethods Bdd_nbmethods = {
  (binaryfunc)0,               // nb_add
  (binaryfunc)0,               // nb_subtract
  (binaryfunc)0,               // nb_multiply
  (binaryfunc)0,               // nb_divide
  (binaryfunc)0,               // nb_remainder
  (binaryfunc)0,               // nb_divmod
  (ternaryfunc)0,              // nb_power
  (unaryfunc)0,                // nb_negative
  (unaryfunc)0,                // nb_positive
  (unaryfunc)0,                // nb_absolute
  (inquiry)0,                  // nb_nonzero
  (unaryfunc)Bdd_inv,          // nb_invert
  (binaryfunc)0,               // nb_lshift
  (binaryfunc)0,               // nb_rshift
  (binaryfunc)Bdd_and,         // nb_and
  (binaryfunc)Bdd_xor,         // nb_xor
  (binaryfunc)Bdd_or,          // nb_or
  (coercion)0,                 // nb_coerce
  (unaryfunc)0,                // nb_int
  (unaryfunc)0,                // nb_long
  (unaryfunc)0,                // nb_float
  (unaryfunc)0,                // nb_oct
  (unaryfunc)0,                // nb_hex

  // Added in release 2.0
  (binaryfunc)0,               // nb_inplace_add
  (binaryfunc)0,               // nb_inplace_subtract
  (binaryfunc)0,               // nb_inplace_multiply
  (binaryfunc)0,               // nb_inplace_divide
  (binaryfunc)0,               // nb_inplace_remainder
  (ternaryfunc)0,              // nb_inplace_power
  (binaryfunc)0,               // nb_inplace_lshift
  (binaryfunc)0,               // nb_inplace_rshift
  (binaryfunc)Bdd_iand,        // nb_inplace_and
  (binaryfunc)Bdd_ixor,        // nb_inplace_xor
  (binaryfunc)Bdd_ior,         // nb_inplace_or

  // Added in release 2.2
  (binaryfunc)0,               // nb_floor_divide
  (binaryfunc)0,               // nb_true_divide
  (binaryfunc)0,               // nb_inplace_floor_divide
  (binaryfunc)0,               // nb_inplace_true_divide

  // Added in release 2.5
  (unaryfunc)0                 // nb_index
};


//////////////////////////////////////////////////////////////////////
// BddObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef Bdd_methods[] = {
  {"is_zero", (PyCFunction)Bdd_is_zero, METH_NOARGS,
   PyDoc_STR("check if constant 0 (NONE)")},
  {"is_one", (PyCFunction)Bdd_is_one, METH_NOARGS,
   PyDoc_STR("check if constant 1 (NONE)")},
  {"is_const", (PyCFunction)Bdd_is_const, METH_NOARGS,
   PyDoc_STR("check if constant (NONE)")},
  {"is_overflow", (PyCFunction)Bdd_is_overflow, METH_NOARGS,
   PyDoc_STR("check if overflow (NONE)")},
  {"is_error", (PyCFunction)Bdd_is_error, METH_NOARGS,
   PyDoc_STR("check if error (NONE)")},
  {"is_invalid", (PyCFunction)Bdd_is_invalid, METH_NOARGS,
   PyDoc_STR("check if invalide (overflow or error) (NONE)")},
  {"is_leaf", (PyCFunction)Bdd_is_leaf, METH_NOARGS,
   PyDoc_STR("check if leaf (NONE)")},
  {"is_posi_cube", (PyCFunction)Bdd_is_posi_cube, METH_NOARGS,
   PyDoc_STR("check if representing a potivie cube function (NONE)")},
  {"is_cube", (PyCFunction)Bdd_is_cube, METH_NOARGS,
   PyDoc_STR("check if representing a cube function (NONE)")},
  {"check_symmetry", (PyCFunction)Bdd_check_symmetry, METH_VARARGS,
   PyDoc_STR("check symmetry (VarId, VarId, ?Pol)")},
  {"root_decomp", (PyCFunction)Bdd_root_decomp, METH_NOARGS,
   PyDoc_STR("decompose on root node (NONE)")},
  {"root_var", (PyCFunction)Bdd_root_var, METH_NOARGS,
   PyDoc_STR("return root variable (NONE)")},
  {"edge0", (PyCFunction)Bdd_edge0, METH_NOARGS,
   PyDoc_STR("return root's edge0 function (NONE)")},
  {"edge1", (PyCFunction)Bdd_edge1, METH_NOARGS,
   PyDoc_STR("return root's edge1 function (NONE)")},
  {"cofactor", (PyCFunction)Bdd_cofactor, METH_VARARGS,
   PyDoc_STR("return cofactor (VarId, Pol)")},
  {"xor_moment", (PyCFunction)Bdd_xor_moment, METH_VARARGS,
   PyDoc_STR("return XOR mement coefficient (VarId)")},
  {"SCC", (PyCFunction)Bdd_SCC, METH_NOARGS,
   PyDoc_STR("return the smallest cube containg F (SCC(F)) (NONE)")},
  {"compose", (PyCFunction)Bdd_compose, METH_VARARGS,
   PyDoc_STR("do composition operation (VarId, Bdd)")},
  {"multi_compose", (PyCFunction)Bdd_multi_compose, METH_VARARGS,
   PyDoc_STR("do multi-variable composition operation (dictionary of (VarId: Bdd))")},
  {"remap_var", (PyCFunction)Bdd_remap_var, METH_VARARGS,
   PyDoc_STR("do remapping variables (dictionary of (VarId : VarId))")},
  {"esmooth", (PyCFunction)Bdd_esmooth, METH_VARARGS,
   PyDoc_STR("do existential smoothing (list of VarId)")},
  {"asmooth", (PyCFunction)Bdd_asmooth, METH_VARARGS,
   PyDoc_STR("do universal (all) smoothing (list of VarId)")},
  {"push_down", (PyCFunction)Bdd_push_down, METH_VARARGS,
   PyDoc_STR("do push-down operation (uint, uint, ?Pol)")},
  {"sop", (PyCFunction)Bdd_sop, METH_NOARGS,
   PyDoc_STR("return SOP expression (NONE)")},
  {"node_count", (PyCFunction)Bdd_node_count, METH_NOARGS,
   PyDoc_STR("return node count (NONE)")},
  {"onepath", (PyCFunction)Bdd_onepath, METH_NOARGS,
   PyDoc_STR("return one-path (NONE)")},
  {"shortest_onepath", (PyCFunction)Bdd_shortest_onepath, METH_NOARGS,
   PyDoc_STR("return the shortest path (NONE)")},
  {"shortest_onepath_len", (PyCFunction)Bdd_shortest_onepath_len, METH_NOARGS,
   PyDoc_STR("return the length of the shortest path (NONE)")},
  {"support", (PyCFunction)Bdd_onepath, METH_NOARGS,
   PyDoc_STR("return the support set (NONE)")},
  {"print", (PyCFunction)Bdd_print, METH_NOARGS,
   PyDoc_STR("print out the contents (NONE)")},
  {"dump", (PyCFunction)Bdd_dump, METH_VARARGS,
   PyDoc_STR("dump (FileBinO)")},
  {"restore", (PyCFunction)Bdd_restore, METH_VARARGS,
   PyDoc_STR("restore (FileBinI)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE

// BddObject 用のタイプオブジェクト
PyTypeObject PyBdd_Type = {
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
  &Bdd_nbmethods,             /*tp_as_number*/
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
PyBdd_FromBdd(const Bdd& obj)
{
  BddObject* bdd_obj = Bdd_new(&PyBdd_Type);
  if ( bdd_obj == NULL ) {
    return NULL;
  }

  *bdd_obj->mBdd = obj;

  Py_INCREF(bdd_obj);
  return (PyObject*)bdd_obj;
}

// BddObject 関係の初期化
void
BddObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyBdd_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Bdd", (PyObject*)&PyBdd_Type);
}

END_NAMESPACE_YM

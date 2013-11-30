
/// @file pylogic_LogExpr.cc
/// @brief LogExpr の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/pylogic.h"
#include "ym_logic/LogExpr.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// LogExpr を表す型
struct LogExprObject
{
  // Python のお約束
  PyObject_HEAD

  // LogExpr の本体
  LogExpr* mLogExpr;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// LogExprObject の生成関数
LogExprObject*
LogExpr_new(PyTypeObject* type)
{
  LogExprObject* self = PyObject_New(LogExprObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // LogExpr を生成する．
  self->mLogExpr = new LogExpr();

  return self;
}

// LogExprObject を開放する関数
void
LogExpr_dealloc(LogExprObject* self)
{
  // LogExpr を開放する．
  delete self->mLogExpr;

  PyObject_Del(self);
}

// 初期化関数
int
LogExpr_init(LogExprObject* self,
	     PyObject* args)
{
  char* str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &str) ) {
    return -1;
  }

  if ( str != NULL ) {
    istringstream is(str);
    string err_s;
    if ( !self->mLogExpr->read_from_stream(is, err_s) ) {
      PyErr_SetString(PyExc_ValueError, err_s.c_str());
      return -1;
    }
  }

  return 0;
}

// str 関数
PyObject*
LogExpr_str(LogExprObject* self)
{
  ostringstream buf;
  buf << *self->mLogExpr;
  return PyObject_FromString(buf.str());
}

// inv 関数
PyObject*
LogExpr_inv(PyObject* left)
{
  if ( !PyLogExpr_Check(left) ) {
    PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
    return NULL;
  }

  LogExpr* expr_p = PyLogExpr_AsLogExprPtr(left);

  return PyLogExpr_FromLogExpr(~(*expr_p));
}

// and 関数
PyObject*
LogExpr_and(PyObject* left,
	    PyObject* right)
{
  if ( !PyLogExpr_Check(left) || !PyLogExpr_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
    return NULL;
  }

  LogExpr* expr1_p = PyLogExpr_AsLogExprPtr(left);
  LogExpr* expr2_p = PyLogExpr_AsLogExprPtr(right);

  return PyLogExpr_FromLogExpr(*expr1_p & *expr2_p);
}

// or 関数
PyObject*
LogExpr_or(PyObject* left,
	   PyObject* right)
{
  if ( !PyLogExpr_Check(left) || !PyLogExpr_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
    return NULL;
  }

  LogExpr* expr1_p = PyLogExpr_AsLogExprPtr(left);
  LogExpr* expr2_p = PyLogExpr_AsLogExprPtr(right);

  return PyLogExpr_FromLogExpr(*expr1_p | *expr2_p);
}

// xor 関数
PyObject*
LogExpr_xor(PyObject* left,
	    PyObject* right)
{
  if ( !PyLogExpr_Check(left) || !PyLogExpr_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
    return NULL;
  }

  LogExpr* expr1_p = PyLogExpr_AsLogExprPtr(left);
  LogExpr* expr2_p = PyLogExpr_AsLogExprPtr(right);

  return PyLogExpr_FromLogExpr(*expr1_p ^ *expr2_p);
}

// inplace and 関数
PyObject*
LogExpr_iand(PyObject* left,
	     PyObject* right)
{
  if ( !PyLogExpr_Check(left) || !PyLogExpr_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
    return NULL;
  }

  LogExpr* expr1_p = PyLogExpr_AsLogExprPtr(left);
  LogExpr* expr2_p = PyLogExpr_AsLogExprPtr(right);

  *expr1_p &= *expr2_p;

  Py_INCREF(left);
  return (PyObject*)left;
}

// inplace xor 関数
PyObject*
LogExpr_ixor(PyObject* left,
	     PyObject* right)
{
  if ( !PyLogExpr_Check(left) || !PyLogExpr_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
    return NULL;
  }

  LogExpr* expr1_p = PyLogExpr_AsLogExprPtr(left);
  LogExpr* expr2_p = PyLogExpr_AsLogExprPtr(right);

  *expr1_p ^= *expr2_p;

  Py_INCREF(left);
  return (PyObject*)left;
}

// inplace or 関数
PyObject*
LogExpr_ior(PyObject* left,
	    PyObject* right)
{
  if ( !PyLogExpr_Check(left) || !PyLogExpr_Check(right) ) {
    PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
    return NULL;
  }

  LogExpr* expr1_p = PyLogExpr_AsLogExprPtr(left);
  LogExpr* expr2_p = PyLogExpr_AsLogExprPtr(right);

  *expr1_p |= *expr2_p;

  Py_INCREF(left);
  return (PyObject*)left;
}

// make_zero 関数
PyObject*
LogExpr_make_zero(PyTypeObject* type_obj,
		  PyObject* args)
{
  return PyLogExpr_FromLogExpr(LogExpr::make_zero());
}

// make_one 関数
PyObject*
LogExpr_make_one(PyTypeObject* type_obj,
		 PyObject* args)
{
  return PyLogExpr_FromLogExpr(LogExpr::make_one());
}

// make_literal 関数
// LiteralType のオブジェクトを一つか VarId と Pol を引数にとる．
PyObject*
LogExpr_make_literal(PyTypeObject* type_obj,
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
    if ( !PyVarId_Check(obj1) || !PyPol_Check(obj2) ) {
      PyErr_SetString(PyExc_TypeError, "(logic.VarId, logic.Pol) is expected");
      return NULL;
    }

    VarId vid = PyVarId_AsVarId(obj1);
    tPol pol = PyPol_AsPol(obj2);
    lit.set(vid, pol);
  }
  else {
    if ( PyVarId_Check(obj1) ) {
      VarId vid = PyVarId_AsVarId(obj1);
      lit.set(vid, kPolPosi);
    }
    else if ( PyLiteral_Check(obj1) ) {
      lit = PyLiteral_AsLiteral(obj1);
    }
    else {
      PyErr_SetString(PyExc_TypeError,
		      "logic.Literal or logic.VarId are expected");
      return NULL;
    }
  }

  return PyLogExpr_FromLogExpr(LogExpr::make_literal(lit));
}

// make_posiliteral 関数
// VarIdType のオブジェクトを一つ引数にとる．
PyObject*
LogExpr_make_posiliteral(PyTypeObject* type_obj,
			 PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyVarId_Type, &obj) ) {
    return NULL;
  }

  VarId vid = PyVarId_AsVarId(obj);

  return PyLogExpr_FromLogExpr(LogExpr::make_posiliteral(vid));
}

// make_negaliteral 関数
// VarIdType のオブジェクトを一つ引数にとる．
PyObject*
LogExpr_make_negaliteral(PyTypeObject* type_obj,
			 PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyVarId_Type, &obj) ) {
    return NULL;
  }

  VarId vid = PyVarId_AsVarId(obj);

  return PyLogExpr_FromLogExpr(LogExpr::make_negaliteral(vid));
}

// make_and 関数
// PyLogExpr_Type のオブジェクトのタプルを一つ引数にとる．
PyObject*
LogExpr_make_and(PyTypeObject* type_obj,
		 PyObject* args)
{
  PyObject* arg_list = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &PyList_Type, &arg_list) ) {
    return NULL;
  }
  ymuint n = PyList_GET_SIZE(arg_list);
  LogExprVector child_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* chd_obj = PyList_GET_ITEM(arg_list, i);
    if ( !PyLogExpr_Check(chd_obj) ) {
      PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
      return NULL;
    }

    LogExpr* chd_expr_p = PyLogExpr_AsLogExprPtr(chd_obj);
    child_list[i] = *chd_expr_p;
  }
  return PyLogExpr_FromLogExpr(LogExpr::make_and(child_list));
}

// make_or 関数
// PyLogExpr_Type のオブジェクトのタプルを一つ引数にとる．
PyObject*
LogExpr_make_or(PyTypeObject* type_obj,
		PyObject* args)
{
  PyObject* arg_list = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &PyList_Type, &arg_list) ) {
    return NULL;
  }
  ymuint n = PyList_GET_SIZE(arg_list);
  LogExprVector child_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* chd_obj = PyList_GET_ITEM(arg_list, i);
    if ( !PyLogExpr_Check(chd_obj) ) {
      PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
      return NULL;
    }

    LogExpr* chd_expr_p = PyLogExpr_AsLogExprPtr(chd_obj);
    child_list[i] = *chd_expr_p;
  }
  return PyLogExpr_FromLogExpr(LogExpr::make_or(child_list));
}

// make_xor 関数
// PyLogExpr_Type のオブジェクトのタプルを一つ引数にとる．
PyObject*
LogExpr_make_xor(PyTypeObject* type_obj,
		 PyObject* args)
{
  PyObject* arg_list = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &PyList_Type, &arg_list) ) {
    return NULL;
  }
  ymuint n = PyList_GET_SIZE(arg_list);
  LogExprVector child_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* chd_obj = PyList_GET_ITEM(arg_list, i);
    if ( !PyLogExpr_Check(chd_obj) ) {
      PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
      return NULL;
    }

    LogExpr* chd_expr_p = PyLogExpr_AsLogExprPtr(chd_obj);
    child_list[i] = *chd_expr_p;
  }
  return PyLogExpr_FromLogExpr(LogExpr::make_xor(child_list));
}

// compose 関数
// VarIdType と PyLogExpr_Type のオブジェクトを引数にとる．
PyObject*
LogExpr_compose(LogExprObject* self,
		PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  if ( !PyArg_ParseTuple(args, "O!O!",
			 &PyVarId_Type, &obj1,
			 &PyLogExpr_Type, &obj2) ) {
    return NULL;
  }

  VarId vid = PyVarId_AsVarId(obj1);
  LogExpr* sub_expr_p = PyLogExpr_AsLogExprPtr(obj2);

  return PyLogExpr_FromLogExpr(self->mLogExpr->compose(vid, *sub_expr_p));
}

// multi_compose 関数
// VarIdType:LogExpr の連想配列を引数にとる．
PyObject*
LogExpr_multi_compose(LogExprObject* self,
		      PyObject* args)
{
  PyObject* obj = NULL;
  if ( !PyArg_ParseTuple(args, "O!", &PyDict_Type, &obj) ) {
    return NULL;
  }

  VarLogExprMap comp_map;
  PyObject* item_list = PyDict_Items(obj);
  ymuint n = PyList_GET_SIZE(item_list);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* item = PyList_GET_ITEM(item_list, i);
    PyObject* vid_obj = NULL;
    PyObject* sub_obj = NULL;
    if ( !PyArg_ParseTuple(item, "O!O!",
			   &PyVarId_Type, &vid_obj,
			   &PyLogExpr_Type, &sub_obj) ) {
      return NULL;
    }

    VarId vid = PyVarId_AsVarId(vid_obj);
    LogExpr* sub_expr_p = PyLogExpr_AsLogExprPtr(sub_obj);
    comp_map.insert(make_pair(vid, *sub_expr_p));
  }

  return PyLogExpr_FromLogExpr(self->mLogExpr->compose(comp_map));
}

// remap_var 関数
// VarIdType:VarIdType の連想配列を引数にとる．
PyObject*
LogExpr_remap_var(LogExprObject* self,
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
    if ( !PyArg_ParseTuple(item, "O!O!",
			   &PyVarId_Type, &obj1,
			   &PyVarId_Type, &obj2) ) {
      return NULL;
    }

    VarId vid = PyVarId_AsVarId(obj1);
    VarId new_vid = PyVarId_AsVarId(obj2);
    var_map.insert(make_pair(vid, new_vid));
  }

  return PyLogExpr_FromLogExpr(self->mLogExpr->remap_var(var_map));
}

// simplify 関数
// 引数をとらない．
// 返り値もない．
PyObject*
LogExpr_simplify(LogExprObject* self,
		 PyObject* args)
{
  self->mLogExpr->simplify();

  Py_INCREF(Py_None);
  return Py_None;
}

// eval 関数
// 入力値を表す ulong のタプルとマスクを表す ulong を引数にとる．
// マスクが省略された場合は All 1 を仮定する．
// 評価結果の ulong を返す．
PyObject*
LogExpr_eval(LogExprObject* self,
	     PyObject* args)
{
  PyObject* obj1 = NULL;
  ymulong mask = ~0UL;
  if ( !PyArg_ParseTuple(args, "O!|k",
			 &PyTuple_Type, &obj1, &mask) ) {
    return NULL;
  }
  ymuint n = PyTuple_GET_SIZE(obj1);
  vector<ymulong> vals(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* val_obj = PyTuple_GET_ITEM(obj1, i);
    ymulong val;
    if ( !PyArg_ParseTuple(val_obj, "k", &val) ) {
      return NULL;
    }
    vals[i] = val;
  }
  ymulong val = self->mLogExpr->eval(vals, mask);

  return PyObject_FromYmuint64(val);
}

// is_zero 関数
PyObject*
LogExpr_is_zero(LogExprObject* self,
		PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_zero());
}

// is_one 関数
PyObject*
LogExpr_is_one(LogExprObject* self,
	       PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_one());
}

// is_constant 関数
PyObject*
LogExpr_is_constant(LogExprObject* self,
		    PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_constant());
}

// is_posiliteral 関数
PyObject*
LogExpr_is_posiliteral(LogExprObject* self,
		       PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_posiliteral());
}

// is_negaliteral 関数
PyObject*
LogExpr_is_negaliteral(LogExprObject* self,
		       PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_negaliteral());
}

// is_literal 関数
PyObject*
LogExpr_is_literal(LogExprObject* self,
		       PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_literal());
}

// varid 関数
PyObject*
LogExpr_varid(LogExprObject* self,
	      PyObject* args)
{
  return PyVarId_FromVarId(self->mLogExpr->varid());
}

// is_and 関数
PyObject*
LogExpr_is_and(LogExprObject* self,
	       PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_and());
}

// is_or 関数
PyObject*
LogExpr_is_or(LogExprObject* self,
	      PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_or());
}

// is_xor 関数
PyObject*
LogExpr_is_xor(LogExprObject* self,
	       PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_xor());
}

// is_op 関数
PyObject*
LogExpr_is_op(LogExprObject* self,
	      PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_op());
}

// child_num 関数
PyObject*
LogExpr_child_num(LogExprObject* self,
		  PyObject* args)
{
  return PyObject_FromYmuint32(self->mLogExpr->child_num());
}

// child 関数
// 符号なし整数を一つ引数としてとる．
PyObject*
LogExpr_child(LogExprObject* self,
	      PyObject* args)
{
  ymuint pos;
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return NULL;
  }

  LogExpr child = self->mLogExpr->child(pos);

  return PyLogExpr_FromLogExpr(child);
}

// child_list 関数
PyObject*
LogExpr_child_list(LogExprObject* self,
		   PyObject* args)
{
  ymuint n = self->mLogExpr->child_num();
  PyObject* ans_list = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    LogExpr child = self->mLogExpr->child(i);
    PyObject* obj = PyLogExpr_FromLogExpr(child);
    PyList_SetItem(ans_list, i, obj);
  }

  return ans_list;
}

// is_simple 関数
PyObject*
LogExpr_is_simple(LogExprObject* self,
		  PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_simple());
}

// is_simple_and 関数
PyObject*
LogExpr_is_simple_and(LogExprObject* self,
		      PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_simple_and());
}

// is_simple_or 関数
PyObject*
LogExpr_is_simple_or(LogExprObject* self,
		     PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_simple_or());
}

// is_simple_xor 関数
PyObject*
LogExpr_is_simple_xor(LogExprObject* self,
		      PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_simple_xor());
}

// is_sop 関数
PyObject*
LogExpr_is_sop(LogExprObject* self,
	       PyObject* args)
{
  return PyObject_FromBool(self->mLogExpr->is_sop());
}

// litnum 関数
// VarId と Pol を引数にとる．
// どちらも省略可能
PyObject*
LogExpr_litnum(LogExprObject* self,
	       PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  if ( !PyArg_ParseTuple(args, "|O!O!",
			 &PyVarId_Type, &obj1,
			 &PyPol_Type, &obj2) ) {
    return NULL;
  }

  ymuint litnum = 0;
  if ( obj1 == NULL ) {
    litnum = self->mLogExpr->litnum();
  }
  else {
    // obj1 は VarId でなければならない．
    if ( !PyVarId_Check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "logic.VarId is expected");
      return NULL;
    }

    VarId varid = PyVarId_AsVarId(obj1);
    if ( obj2 == NULL ) {
      litnum = self->mLogExpr->litnum(varid);
    }
    else {
      // obj2 は Pol でなければならない．
      if ( !PyPol_Check(obj2) ) {
	PyErr_SetString(PyExc_TypeError, "logic.Pol is expected");
	return NULL;
      }

      tPol pol = PyPol_AsPol(obj2);
      litnum = self->mLogExpr->litnum(varid, pol);
    }
  }

  return PyObject_FromYmuint32(litnum);
}

// input_size 関数
PyObject*
LogExpr_input_size(LogExprObject* self,
		   PyObject* args)
{
  return PyObject_FromYmuint32(self->mLogExpr->input_size());
}

// sop_cubenum 関数
PyObject*
LogExpr_sop_cubenum(LogExprObject* self,
		    PyObject* args)
{
  return PyObject_FromYmuint32(self->mLogExpr->sop_cubenum());
}

// sop_litnum 関数
// VarId と Pol を引数にとる．
// どちらも省略可能
PyObject*
LogExpr_sop_litnum(LogExprObject* self,
		   PyObject* args)
{
  PyObject* obj1 = NULL;
  PyObject* obj2 = NULL;
  if ( !PyArg_ParseTuple(args, "|O!O!",
			 &PyVarId_Type, &obj1,
			 &PyPol_Type, &obj2) ) {
    return NULL;
  }

  ymuint litnum = 0;
  if ( obj1 == NULL ) {
    litnum = self->mLogExpr->sop_litnum();
  }
  else {
    // obj1 は VarId でなければならない．
    if ( !PyVarId_Check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "logic.VarId is expected");
      return NULL;
    }

    VarId varid = PyVarId_AsVarId(obj1);
    if ( obj2 == NULL ) {
      litnum = self->mLogExpr->sop_litnum(varid);
    }
    else {
      // obj2 は Pol でなければならない．
      if ( !PyPol_Check(obj2) ) {
	PyErr_SetString(PyExc_TypeError, "logic.Pol is expected");
	return NULL;
      }

      tPol pol = PyPol_AsPol(obj2);
      litnum = self->mLogExpr->sop_litnum(varid, pol);
    }
  }

  return PyObject_FromYmuint32(litnum);
}


//////////////////////////////////////////////////////////////////////
// LogExprObject の NumberMethods 構造体の定義
//////////////////////////////////////////////////////////////////////
PyNumberMethods LogExpr_nbmethods = {
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
  (unaryfunc)LogExpr_inv,      // nb_invert
  (binaryfunc)0,               // nb_lshift
  (binaryfunc)0,               // nb_rshift
  (binaryfunc)LogExpr_and,     // nb_and
  (binaryfunc)LogExpr_xor,     // nb_xor
  (binaryfunc)LogExpr_or,      // nb_or
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
  (binaryfunc)LogExpr_iand,    // nb_inplace_and
  (binaryfunc)LogExpr_ixor,    // nb_inplace_xor
  (binaryfunc)LogExpr_ior,     // nb_inplace_or

  // Added in release 2.2
  (binaryfunc)0,               // nb_floor_divide
  (binaryfunc)0,               // nb_true_divide
  (binaryfunc)0,               // nb_inplace_floor_divide
  (binaryfunc)0,               // nb_inplace_true_divide

  // Added in release 2.5
  (unaryfunc)0                 // nb_index
};


// LogExprObject のメソッドテーブル
PyMethodDef LogExpr_methods[] = {
  {"make_zero", (PyCFunction)LogExpr_make_zero, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make constant 0 (NONE)")},
  {"make_one", (PyCFunction)LogExpr_make_one, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make constant 1 (NONE)")},
  {"make_literal", (PyCFunction)LogExpr_make_literal, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make literal (Literal)")},
  {"make_posiliteral", (PyCFunction)LogExpr_make_posiliteral,
   METH_STATIC | METH_VARARGS,
   PyDoc_STR("make positive literal (VarId)")},
  {"make_negaliteral", (PyCFunction)LogExpr_make_negaliteral,
   METH_STATIC | METH_VARARGS,
   PyDoc_STR("make negative literal (VarId)")},
  {"make_and", (PyCFunction)LogExpr_make_and, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make and (list of LogExpr)")},
  {"make_or", (PyCFunction)LogExpr_make_or, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make or (list of LogExpr)")},
  {"make_xor", (PyCFunction)LogExpr_make_xor, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make xor (list of LogExpr)")},
  {"compose", (PyCFunction)LogExpr_compose, METH_VARARGS,
   PyDoc_STR("compose(vid, sub_expr) (VarId, LogExpr)")},
  {"multi_compose", (PyCFunction)LogExpr_multi_compose, METH_VARARGS,
   PyDoc_STR("multi_compose( dictionay of (VarId, sub_expr) )")},
  {"remap_var", (PyCFunction)LogExpr_remap_var, METH_VARARGS,
   PyDoc_STR("remap variables( dictionay of (VarId, VarId) )")},
  {"simplify", (PyCFunction)LogExpr_simplify, METH_NOARGS,
   PyDoc_STR("simplify (NONE)")},
  {"eval", (PyCFunction)LogExpr_eval, METH_VARARGS,
   PyDoc_STR("evaluate bitvector ( tuple of ulong, ulong )")},
  {"is_zero", (PyCFunction)LogExpr_is_zero, METH_NOARGS,
   PyDoc_STR("return true if constant 0 (NONE)")},
  {"is_one", (PyCFunction)LogExpr_is_one, METH_NOARGS,
   PyDoc_STR("return true if constant 1 (NONE)")},
  {"is_constant", (PyCFunction)LogExpr_is_constant, METH_NOARGS,
   PyDoc_STR("return true if constant (NONE)")},
  {"is_posiliteral", (PyCFunction)LogExpr_is_posiliteral, METH_NOARGS,
   PyDoc_STR("return true if positive literal (NONE)")},
  {"is_negaliteral", (PyCFunction)LogExpr_is_posiliteral, METH_NOARGS,
   PyDoc_STR("return true if negative literal (NONE)")},
  {"is_literal", (PyCFunction)LogExpr_is_posiliteral, METH_NOARGS,
   PyDoc_STR("return true if literal (NONE)")},
  {"varid", (PyCFunction)LogExpr_varid, METH_NOARGS,
   PyDoc_STR("return VarId (NONE)")},
  {"is_and", (PyCFunction)LogExpr_is_and, METH_NOARGS,
   PyDoc_STR("return true if the root is AND (NONE)")},
  {"is_or", (PyCFunction)LogExpr_is_or, METH_NOARGS,
   PyDoc_STR("return true if the root is OR (NONE)")},
  {"is_xor", (PyCFunction)LogExpr_is_xor, METH_NOARGS,
   PyDoc_STR("return true if the root is XOR (NONE)")},
  {"is_op", (PyCFunction)LogExpr_is_op, METH_NOARGS,
   PyDoc_STR("return true if the root is OP (NONE)")},
  {"child_num", (PyCFunction)LogExpr_child_num, METH_NOARGS,
   PyDoc_STR("return the number of children (NONE)")},
  {"child", (PyCFunction)LogExpr_child, METH_VARARGS,
   PyDoc_STR("return the n'th child (uint)")},
  {"child_list", (PyCFunction)LogExpr_child_list, METH_NOARGS,
   PyDoc_STR("return the children list (NONE)")},
  {"is_simple", (PyCFunction)LogExpr_is_simple, METH_NOARGS,
   PyDoc_STR("return true if simple expression (NONE)")},
  {"is_simple_and", (PyCFunction)LogExpr_is_simple_and, METH_NOARGS,
   PyDoc_STR("return true if simple AND expression (NONE)")},
  {"is_simple_or", (PyCFunction)LogExpr_is_simple_or, METH_NOARGS,
   PyDoc_STR("return true if simple OR expression (NONE)")},
  {"is_simple_xor", (PyCFunction)LogExpr_is_simple_xor, METH_NOARGS,
   PyDoc_STR("return true if simple XOR expression (NONE)")},
  {"is_sop", (PyCFunction)LogExpr_is_sop, METH_NOARGS,
   PyDoc_STR("return true if SOP expression (NONE)")},
  {"litnum", (PyCFunction)LogExpr_litnum, METH_VARARGS,
   PyDoc_STR("return the number of literals ( ?VarId, ?Pol)")},
  {"input_size", (PyCFunction)LogExpr_input_size, METH_NOARGS,
   PyDoc_STR("return the maximum number of input variables (NONE)")},
  {"sop_cubenum", (PyCFunction)LogExpr_sop_cubenum, METH_NOARGS,
   PyDoc_STR("return the number of cubes in SOP form (NONE)")},
  {"sop_litnum", (PyCFunction)LogExpr_sop_cubenum, METH_VARARGS,
   PyDoc_STR("return the number of literals in SOP form ( ?VarId, ?Pol )")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


// LogExprObject 用のタイプオブジェクト
PyTypeObject PyLogExpr_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logic.LogExpr",            /*tp_name*/
  sizeof(LogExprObject),      /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)LogExpr_dealloc,    /*tp_dealloc*/
  0,                          /*tp_print*/
  0,                          /*tp_getattr*/
  0,                          /*tp_setattr*/
  0,                          /*tp_compare*/
  0,                          /*tp_repr*/
  &LogExpr_nbmethods,            /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  0,                          /*tp_hash*/
  0,                          /*tp_call*/
  (reprfunc)LogExpr_str,          /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "Logic Expression",                 /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  LogExpr_methods,                /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)LogExpr_init,         /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)LogExpr_new,           /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

// @brief LogExpr から PyObject を生成する．
// @param[in] obj LogExpr オブジェクト
PyObject*
PyLogExpr_FromLogExpr(const LogExpr& obj)
{
  LogExprObject* logexpr_obj = LogExpr_new(&PyLogExpr_Type);
  if ( logexpr_obj == NULL ) {
    return NULL;
  }

  *logexpr_obj->mLogExpr = obj;

  Py_INCREF(logexpr_obj);
  return (PyObject*)logexpr_obj;
}

// @brief PyObject から LogExpr へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return LogExpr へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
LogExpr*
PyLogExpr_AsLogExprPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyLogExpr_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "logic.LogExpr is expected");
    return NULL;
  }

  // 強制的にキャスト
  LogExprObject* logexpr_obj = (LogExprObject*)py_obj;

  return logexpr_obj->mLogExpr;
}

// LogExprObject 関係の初期化
void
LogExprObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyLogExpr_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "LogExpr", (PyObject*)&PyLogExpr_Type);
}

END_NAMESPACE_YM

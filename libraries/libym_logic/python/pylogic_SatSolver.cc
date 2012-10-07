
/// @file pylogic_SatSolver.cc
/// @brief SatSolver の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/pylogic.h"
#include "ym_logic/SatSolver.h"
#include "ym_logic/SatStats.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// SatSolver を表す型
struct SatSolverObject
{
  // Python のお約束
  PyObject_HEAD

  // SatSolver の本体
  SatSolver* mSolver;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// SatSolverObject の生成関数
SatSolverObject*
SatSolver_new(PyTypeObject* type)
{
  SatSolverObject* self = PyObject_New(SatSolverObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mSolver = NULL;

  return self;
}


// SatSolver を開放する関数
void
SatSolver_dealloc(SatSolverObject* self)
{
  delete self->mSolver;

  PyObject_Del(self);
}

// 初期化関数
int
SatSolver_init(SatSolverObject* self,
	       PyObject* args)
{
  char* type_s = NULL;
  char* option_s = NULL;
  char* file_s = NULL;
  if ( !PyArg_ParseTuple(args, "|sss", &type_s, &option_s, &file_s) ) {
    return -1;
  }

  string type;
  if ( type_s != NULL ) {
    type = type_s;
  }

  string option;
  if ( option_s != NULL ) {
    option = option_s;
  }

  // 今は outp は無視

  if ( self->mSolver != NULL ) {
    delete self->mSolver;
  }

  self->mSolver = new SatSolver(type, option);

  return 0;
}

// sane 関数
PyObject*
SatSolver_sane(SatSolverObject* self,
	       PyObject* args)
{
  return conv_to_pyobject(self->mSolver->sane());
}

// new_var 関数
PyObject*
SatSolver_new_var(SatSolverObject* self,
		  PyObject* args)
{
  return conv_to_pyobject(self->mSolver->new_var());
}

// add_clause 関数
PyObject*
SatSolver_add_clause(SatSolverObject* self,
		     PyObject* args)
{
  vector<Literal> lits;
  ymuint n = PyTuple_GET_SIZE(args);
  if ( n == 1 ) {
    PyObject* obj0 = PyTuple_GET_ITEM(args, 0);
    if ( !PyList_Check(obj0) ) {
      PyErr_SetString(ErrorObject, "list of Literal is expected");
      return NULL;
    }
    n = PyList_GET_SIZE(obj0);
    lits.resize(n);
    for (ymuint i = 0; i < n; ++ i) {
      PyObject* obj = PyList_GET_ITEM(obj0, i);
      Literal lit;
      if ( !conv_from_pyobject(obj, lit) ) {
	return NULL;
      }
      lits[i] = lit;
    }
  }
  else {
    lits.resize(n);
    for (ymuint i = 0; i < n; ++ i) {
      PyObject* obj = PyTuple_GET_ITEM(args, i);
      Literal lit;
      if ( !conv_from_pyobject(obj, lit) ) {
	return NULL;
      }
      lits[i] = lit;
    }
  }

  self->mSolver->add_clause(lits);

  Py_INCREF(Py_None);
  return Py_None;
}

// solve 関数
PyObject*
SatSolver_solve(SatSolverObject* self,
		PyObject* args)
{
  vector<Bool3> model;
  Bool3 stat;

  if ( args != NULL ) {
    if ( PyTuple_GET_SIZE(args) != 1 ||
	 !PyList_Check(PyTuple_GET_ITEM(args, 0)) ) {
      PyErr_SetString(ErrorObject, "list of Literal is expected");
      return NULL;
    }
    PyObject* list_obj = PyTuple_GET_ITEM(args, 0);
    ymuint n = PyList_GET_SIZE(list_obj);
    vector<Literal> assumptions(n);
    for (ymuint i = 0; i < n; ++ i) {
      PyObject* item = PyList_GET_ITEM(list_obj, i);
      Literal lit;
      if ( !conv_from_pyobject(item, lit) ) {
	return NULL;
      }
      assumptions[i] = lit;
    }
    stat = self->mSolver->solve(assumptions, model);
  }
  else {
    stat = self->mSolver->solve(model);
  }

  ymuint n = model.size();
  PyObject* model_obj = PyTuple_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* val_obj = conv_to_pyobject(model[i]);
    PyTuple_SetItem(model_obj, i, val_obj);
  }

  PyObject* stat_obj = conv_to_pyobject(stat);
  return Py_BuildValue("(OO)", stat_obj, model_obj);
}


BEGIN_NONAMESPACE

inline
PyObject*
conv_to_pyobject(ymuint val)
{
  return conv_to_pyobject(val);
}

inline
PyObject*
conv_to_pyobject(ymuint64 val)
{
  return conv_to_pyobject(val);
}

END_NONAMESPACE

// get_stats 関数
PyObject*
SatSolver_get_stats(SatSolverObject* self,
		    PyObject* args)
{
  SatStats stats;
  self->mSolver->get_stats(stats);

  // 結果を Python の dictionary に格納する．
  PyObject* dict_obj = PyDict_New();

  PyDict_SetItemString(dict_obj, "restart", conv_to_pyobject(stats.mRestart));
  PyDict_SetItemString(dict_obj, "variable_num", conv_to_pyobject(stats.mVarNum));
  PyDict_SetItemString(dict_obj, "constr_clause_num", conv_to_pyobject(stats.mConstrClauseNum));
  PyDict_SetItemString(dict_obj, "constr_literal_num", conv_to_pyobject(stats.mConstrLitNum));
  PyDict_SetItemString(dict_obj, "learnt_clause_num", conv_to_pyobject(stats.mLearntClauseNum));
  PyDict_SetItemString(dict_obj, "learnt_literal_num", conv_to_pyobject(stats.mLearntLitNum));
  PyDict_SetItemString(dict_obj, "conflict_num", conv_to_pyobject(stats.mConflictNum));
  PyDict_SetItemString(dict_obj, "decision_num", conv_to_pyobject(stats.mDecisionNum));
  PyDict_SetItemString(dict_obj, "propagation_num", conv_to_pyobject(stats.mPropagationNum));
  PyDict_SetItemString(dict_obj, "conflict_limit", conv_to_pyobject(stats.mConflictLimit));
  PyDict_SetItemString(dict_obj, "learnt_limit", conv_to_pyobject(stats.mLearntLimit));

  return dict_obj;
}

// variable_num 関数
PyObject*
SatSolver_variable_num(SatSolverObject* self,
		       PyObject* args)
{
  return conv_to_pyobject(self->mSolver->variable_num());
}

// clause_num 関数
PyObject*
SatSolver_clause_num(SatSolverObject* self,
		     PyObject* args)
{
  return conv_to_pyobject(self->mSolver->clause_num());
}

// literal_num 関数
PyObject*
SatSolver_literal_num(SatSolverObject* self,
		      PyObject* args)
{
  return conv_to_pyobject(self->mSolver->literal_num());
}

// set_max_conflict 関数
PyObject*
SatSolver_set_max_conflict(SatSolverObject* self,
			   PyObject* args)
{
  ymuint64 val;
  if ( !PyArg_ParseTuple(args, "K", &val) ) {
    return NULL;
  }

  return conv_to_pyobject(self->mSolver->set_max_conflict(val));
}

// timer_on 関数
PyObject*
SatSolver_timer_on(SatSolverObject* self,
		   PyObject* args)
{
  bool enable;
  if ( !PyArg_ParseTuple(args, "b", &enable) ) {
    return NULL;
  }

  self->mSolver->timer_on(enable);

  Py_INCREF(Py_None);
  return Py_None;
}


//  SatSolver のメソッドテーブル
PyMethodDef SatSolver_methods[] = {
  {"sane", (PyCFunction)SatSolver_sane, METH_NOARGS,
   PyDoc_STR("return true if solver is sane (NONE)")},
  {"new_var", (PyCFunction)SatSolver_new_var, METH_NOARGS,
   PyDoc_STR("add new variable. return VarId")},
  {"add_clause", (PyCFunction)SatSolver_add_clause, METH_VARARGS,
   PyDoc_STR("add new clause. (list of Literal)")},
  {"solve", (PyCFunction)SatSolver_solve, METH_VARARGS,
   PyDoc_STR("solve the SAT problem (?list of Literal)")},
  {"get_stats", (PyCFunction)SatSolver_get_stats, METH_NOARGS,
   PyDoc_STR("return current statistics (NONE)")},
  {"variable_num", (PyCFunction)SatSolver_variable_num, METH_NOARGS,
   PyDoc_STR("return the number of variables. (NONE)")},
  {"clause_num", (PyCFunction)SatSolver_clause_num, METH_NOARGS,
   PyDoc_STR("return the number of clauses. (NONE)")},
  {"literal_num", (PyCFunction)SatSolver_literal_num, METH_NOARGS,
   PyDoc_STR("return the number of literals. (NONE)")},
  {"set_max_conflict", (PyCFunction)SatSolver_set_max_conflict, METH_NOARGS,
   PyDoc_STR("set the maximum conflict limit. (ulonglong)")},
  {"timer_on", (PyCFunction)SatSolver_timer_on, METH_NOARGS,
   PyDoc_STR("enable timer (Bool)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


// SatSolverObject 用のタイプオブジェクト
PyTypeObject SatSolverType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "logic.SatSolver",            /*tp_name*/
  sizeof(SatSolverObject),      /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)SatSolver_dealloc,    /*tp_dealloc*/
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
  0,                          /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "BDD manager",              /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  SatSolver_methods,             /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)SatSolver_init,      /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)SatSolver_new,        /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

// @brief PyObject から SatSolver を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj_p SatSolver を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が SatSolverObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   SatSolver* obj_p)
{
  // 型のチェック
  if ( !SatSolverObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  SatSolverObject* satsolver_obj = (SatSolverObject*)py_obj;

  obj_p = satsolver_obj->mSolver;

  return true;
}

END_NAMESPACE_YM_PYTHON

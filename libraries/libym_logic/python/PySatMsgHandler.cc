
/// @file PySatMsgHandler.cc
/// @brief PySatMsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "PySatMsgHandler.h"
#include "ym_logic/SatStats.h"
#include "ym_utils/pyutils.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PySatMsgHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PySatMsgHandler::PySatMsgHandler(PyObject* func_obj) :
  mFuncObj(func_obj)
{
  assert_cond( PyCallable_Check(func_obj), __FILE__, __LINE__);
  Py_INCREF(func_obj);
}

// @brief デストラクタ
PySatMsgHandler::~PySatMsgHandler()
{
  Py_DECREF(mFuncObj);
}

// @brief 内部状態の出力を行う仮想関数
// @param[in] stats SAT ソルバの内部状態
void
PySatMsgHandler::operator()(const SatStats& stats)
{
  // stats を Python の dictionary に格納する．
  PyObject* dict_obj = PyDict_New();

  PyDict_SetItemString(dict_obj, "restart",            PyObject_FromYmuint64(stats.mRestart));
  PyDict_SetItemString(dict_obj, "variable_num",       PyObject_FromYmuint64(stats.mVarNum));
  PyDict_SetItemString(dict_obj, "constr_clause_num",  PyObject_FromYmuint64(stats.mConstrClauseNum));
  PyDict_SetItemString(dict_obj, "constr_literal_num", PyObject_FromYmuint64(stats.mConstrLitNum));
  PyDict_SetItemString(dict_obj, "learnt_clause_num",  PyObject_FromYmuint64(stats.mLearntClauseNum));
  PyDict_SetItemString(dict_obj, "learnt_literal_num", PyObject_FromYmuint64(stats.mLearntLitNum));
  PyDict_SetItemString(dict_obj, "conflict_num",       PyObject_FromYmuint64(stats.mConflictNum));
  PyDict_SetItemString(dict_obj, "decision_num",       PyObject_FromYmuint64(stats.mDecisionNum));
  PyDict_SetItemString(dict_obj, "propagation_num",    PyObject_FromYmuint64(stats.mPropagationNum));
  PyDict_SetItemString(dict_obj, "conflict_limit",     PyObject_FromYmuint64(stats.mConflictLimit));
  PyDict_SetItemString(dict_obj, "learnt_limit",       PyObject_FromYmuint64(stats.mLearntLimit));
  PyDict_SetItemString(dict_obj, "time",               PyUSTime_FromUSTime(stats.mTime));

  PyObject* args = Py_BuildValue("O", dict_obj);

  // Python の関数を呼び出す．
  PyObject* result = PyObject_CallObject(mFuncObj, args);

  Py_DECREF(args);

  if ( result == NULL ) {
    // コールバック中でエラーが起こった．
    // けどこの関数はエラーを送出できない．
    PyErr_Clear();
  }
  else {
    // どちらにせよ結果は使わない．
    Py_DECREF(result);
  }
}

END_NAMESPACE_YM

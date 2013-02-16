
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

  PyDict_SetItemString(dict_obj, "restart",            conv_to_pyobject(stats.mRestart));
  PyDict_SetItemString(dict_obj, "variable_num",       conv_to_pyobject(stats.mVarNum));
  PyDict_SetItemString(dict_obj, "constr_clause_num",  conv_to_pyobject(stats.mConstrClauseNum));
  PyDict_SetItemString(dict_obj, "constr_literal_num", conv_to_pyobject(stats.mConstrLitNum));
  PyDict_SetItemString(dict_obj, "learnt_clause_num",  conv_to_pyobject(stats.mLearntClauseNum));
  PyDict_SetItemString(dict_obj, "learnt_literal_num", conv_to_pyobject(stats.mLearntLitNum));
  PyDict_SetItemString(dict_obj, "conflict_num",       conv_to_pyobject(stats.mConflictNum));
  PyDict_SetItemString(dict_obj, "decision_num",       conv_to_pyobject(stats.mDecisionNum));
  PyDict_SetItemString(dict_obj, "propagation_num",    conv_to_pyobject(stats.mPropagationNum));
  PyDict_SetItemString(dict_obj, "conflict_limit",     conv_to_pyobject(stats.mConflictLimit));
  PyDict_SetItemString(dict_obj, "learnt_limit",       conv_to_pyobject(stats.mLearntLimit));
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

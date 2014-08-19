
/// @file PySatMsgHandler.cc
/// @brief PySatMsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "PySatMsgHandler.h"
#include "ym_YmLogic/SatStats.h"
#include "ym_YmUtils/pyutils.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// @brief SatStats の内容を Python の dictionary オブジェクトに変換する．
// @param[in] stats SAT ソルバの内部状態
// @return stats の内容を表す Python オブジェクト
PyObject*
PyObject_from_SatStats(const SatStats& stats)
{
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

  return args;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス PySatMsgHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PySatMsgHandler::PySatMsgHandler(PyObject* func_obj) :
  mFuncObj(func_obj)
{
  assert_cond( PyCallable_Check(func_obj), __FILE__, __LINE__);
  Py_INCREF(func_obj);

  mPrintHeaderObj = Py_BuildValue("s", "print_header");

  mPrintMessageObj = Py_BuildValue("s", "print_message");

  mPrintFooterObj = Py_BuildValue("s", "print_footer");
}

// @brief デストラクタ
PySatMsgHandler::~PySatMsgHandler()
{
  Py_DECREF(mFuncObj);
  Py_DECREF(mPrintHeaderObj);
  Py_DECREF(mPrintMessageObj);
  Py_DECREF(mPrintFooterObj);
}

// @brief ヘッダの出力
void
PySatMsgHandler::print_header()
{
  // mFuncObj.print_header() を呼び出す．
  PyObject* result = PyObject_CallMethodObjArgs(mFuncObj, mPrintHeaderObj, NULL);

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

// @brief メッセージの出力を行う仮想関数
// @param[in] stats SAT ソルバの内部状態
void
PySatMsgHandler::print_message(const SatStats& stats)
{
  // stats を Python オブジェクトに格納する．
  PyObject* args = PyObject_from_SatStats(stats);

  // mFuncObj.print_message(stats) を呼び出す．
  PyObject* result = PyObject_CallMethodObjArgs(mFuncObj, mPrintMessageObj, args, NULL);

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

// @brief フッタの出力
// @param[in] stats SAT ソルバの内部状態
void
PySatMsgHandler::print_footer(const SatStats& stats)
{
  // stats を Python オブジェクトに格納する．
  PyObject* args = PyObject_from_SatStats(stats);

  // mFuncObj.print_footer(stats) を呼び出す．
  PyObject* result = PyObject_CallMethodObjArgs(mFuncObj, mPrintFooterObj, args, NULL);

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

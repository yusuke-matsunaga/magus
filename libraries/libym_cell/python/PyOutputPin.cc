
/// @file PyOutputPin.cc
/// @brief PyOutputPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyOutputPin.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyOutputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyOutputPin::PyOutputPin(const CellPin* pin) :
  PyPin(pin)
{
  assert_cond( pin->is_output(), __FILE__, __LINE__);
  mOutputId = PyObject_FromYmuint32(pin->output_id());
  mFunction = NULL;
  mThreeState = NULL;
  mMaxFanout = PyObject_FromDouble(pin->max_fanout());
  mMinFanout = PyObject_FromDouble(pin->min_fanout());
  mMaxCapacitance = PyObject_FromDouble(pin->max_capacitance());
  mMinCapacitance = PyObject_FromDouble(pin->min_capacitance());
  mMaxTransition = PyObject_FromDouble(pin->max_transition());
  mMinTransition = PyObject_FromDouble(pin->min_transition());
}

// @brief デストラクタ
PyOutputPin::~PyOutputPin()
{
  Py_DEFREF(mOutputId);
  Py_DEFREF(mFunction);
  Py_DEFREF(mThreeState);
  Py_DEFREF(mMaxFanout);
  Py_DEFREF(mMinFanout);
  Py_DEFREF(mMaxCapacitance);
  Py_DEFREF(mMinCapacitance);
  Py_DEFREF(mMaxTransition);
  Py_DEFREF(mMinTransition);
}

// @brief 出力ピン番号を返す．
PyObject*
PyOutputPin::output_id()
{
  return mOutputId;
}

// @brief 機能を表す論理式を返す．
PyObject*
PyOutputPin::function()
{
  return mFunction;
}

// @brief three_state 論理式を返す．
PyObject*
PyOutputPin::three_state()
{
  return mThreeState;
}

// @brief 最大ファンアウト容量を返す．
PyObject*
PyOutputPin::max_fanout()
{
  return mMaxFanout;
}

// @brief 最小ファンアウト容量を返す．
PyObject*
PyOutputPin::min_fanout()
{
  return mMinFanout;
}

// @brief 最大負荷容量を返す．
PyObject*
PyOutputPin::max_capacitance()
{
  return mMaxCapacitance;
}

// @brief 最小負荷容量を返す．
PyObject*
PyOutputPin::min_capacitance()
{
  return mMinCapacitance;
}

// @brief 最大遷移時間を返す．
PyObject*
PyOutputPin::max_transition()
{
  return mMaxTransition;
}

// @brief 最小遷移時間を返す．
PyObject*
PyOutputPin::min_transition()
{
  return mMinTransition;
}

END_NAMESPACE_YM

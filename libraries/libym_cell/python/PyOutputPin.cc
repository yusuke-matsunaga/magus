
/// @file PyOutputPin.cc
/// @brief PyOutputPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyOutputPin.h"
#include "ym_cell/CellPin.h"


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
  mMaxFanout = PyCellCapacitance_FromCellCapacitance(pin->max_fanout());
  mMinFanout = PyCellCapacitance_FromCellCapacitance(pin->min_fanout());
  mMaxCapacitance = PyCellCapacitance_FromCellCapacitance(pin->max_capacitance());
  mMinCapacitance = PyCellCapacitance_FromCellCapacitance(pin->min_capacitance());
  mMaxTransition = PyCellTime_FromCellTime(pin->max_transition());
  mMinTransition = PyCellTime_FromCellTime(pin->min_transition());
}

// @brief デストラクタ
PyOutputPin::~PyOutputPin()
{
  Py_DECREF(mOutputId);
  Py_DECREF(mFunction);
  Py_DECREF(mThreeState);
  Py_DECREF(mMaxFanout);
  Py_DECREF(mMinFanout);
  Py_DECREF(mMaxCapacitance);
  Py_DECREF(mMinCapacitance);
  Py_DECREF(mMaxTransition);
  Py_DECREF(mMinTransition);
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

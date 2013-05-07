
/// @file PyOutputPin.cc
/// @brief PyOutputPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyOutputPin.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellCapacitance.h"
#include "ym_cell/CellTime.h"
#include "ym_logic/LogExpr.h"
#include "ym_logic/LogExprWriter.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyOutputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyOutputPin::PyOutputPin(const CellPin* pin) :
  PyPin(pin)
{
  assert_cond( pin->is_output(), __FILE__, __LINE__);

  LogExprWriter lew;

  string func_str;
  if ( pin->has_function() ) {
    func_str = lew.dump_string(pin->function());
  }
  mFunction = PyObject_FromString(func_str);

  string tristate_str;
  if ( pin->has_three_state() ) {
    tristate_str = lew.dump_string(pin->three_state());
  }
  mThreeState = PyObject_FromString(tristate_str);

}

// @brief デストラクタ
PyOutputPin::~PyOutputPin()
{
  Py_DECREF(mFunction);
  Py_DECREF(mThreeState);
}

// @brief 出力ピン番号を返す．
PyObject*
PyOutputPin::output_id()
{
  return PyObject_FromYmuint32(pin()->output_id());
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
  return PyCellCapacitance_FromCellCapacitance(pin()->max_fanout());
}

// @brief 最小ファンアウト容量を返す．
PyObject*
PyOutputPin::min_fanout()
{
  return PyCellCapacitance_FromCellCapacitance(pin()->min_fanout());
}

// @brief 最大負荷容量を返す．
PyObject*
PyOutputPin::max_capacitance()
{
  return PyCellCapacitance_FromCellCapacitance(pin()->max_capacitance());
}

// @brief 最小負荷容量を返す．
PyObject*
PyOutputPin::min_capacitance()
{
  return PyCellCapacitance_FromCellCapacitance(pin()->min_capacitance());
}

// @brief 最大遷移時間を返す．
PyObject*
PyOutputPin::max_transition()
{
  return PyCellTime_FromCellTime(pin()->max_transition());
}

// @brief 最小遷移時間を返す．
PyObject*
PyOutputPin::min_transition()
{
  return PyCellTime_FromCellTime(pin()->min_transition());
}

END_NAMESPACE_YM

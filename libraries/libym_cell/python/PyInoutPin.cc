
/// @file PyInoutPin.cc
/// @brief PyInoutPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyInoutPin.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellCapacitance.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyInoutPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyInoutPin::PyInoutPin(const CellPin* pin) :
  PyOutputPin(pin)
{
  assert_cond( pin->is_inout(), __FILE__, __LINE__);
}

// @brief デストラクタ
PyInoutPin::~PyInoutPin()
{
}

// @brief 入力ピン番号を得る．
PyObject*
PyInoutPin::input_id()
{
  return PyObject_FromYmuint32(pin()->input_id());
}

// @brief 負荷容量を返す．
PyObject*
PyInoutPin::capacitance()
{
  return PyCellCapacitance_FromCellCapacitance(pin()->capacitance());
}

// @brief 立ち上がりの負荷容量を返す．
PyObject*
PyInoutPin::rise_capacitance()
{
  return PyCellCapacitance_FromCellCapacitance(pin()->rise_capacitance());
}

// @brief 立ち下がりの負荷容量を返す．
PyObject*
PyInoutPin::fall_capacitance()
{
  return PyCellCapacitance_FromCellCapacitance(pin()->fall_capacitance());
}

END_NAMESPACE_YM

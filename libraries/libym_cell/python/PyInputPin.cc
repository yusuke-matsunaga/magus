
/// @file PyInputPin.cc
/// @brief PyInputPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyInputPin.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyInputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyInputPin::PyInputPin(const CellPin* pin) :
  PyPin(pin)
{
  assert_cond( pin->is_input(), __FILE__, __LINE__);
  mInputId = PyObject_FromYmuint32(pin->input_id());
  mCapacitance = PyCellCapacitance_FromCellCapacitance(pin->capacitance());
  mRiseCapacitance = PyCellCapacitance_FromCellCapacitance(pin->rise_capacitance());
  mFallCapacitance = PyCellCapacitance_FromCellCapacitance(pin->fall_capacitance());
}

// @brief デストラクタ
PyInputPin::~PyInputPin()
{
  Py_DECREF(mInputId);
  Py_DECREF(mCapacitance);
  Py_DECREF(mRiseCapacitance);
  Py_DECREF(mFallCapacitance);
}

// @brief 入力ピン番号を得る．
PyObject*
PyInputPin::input_id()
{
  return mInputId;
}

// @brief 負荷容量を返す．
PyObject*
PyInputPin::capacitance()
{
  return mCapacitance;
}

// @brief 立ち上がりの負荷容量を返す．
PyObject*
PyInputPin::rise_capacitance()
{
  return mRiseCapacitance;
}

// @brief 立ち下がりの負荷容量を返す．
PyObject*
PyInputPin::fall_capacitance()
{
  return mFallCapacitance;
}

END_NAMESPACE_YM

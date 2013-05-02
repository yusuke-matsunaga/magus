
/// @file PyInoutPin.cc
/// @brief PyInoutPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyInoutPin.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyInoutPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyInoutPin::PyInoutPin(const CellPin* pin) :
  PyOutputPin(pin)
{
  assert_cond( pin->is_inout(), __FILE__, __LINE__);
  mInputId = PyObject_FromYmuint32(pin->input_id());
  mCapacitance = PyCellCapacitance_FromCellCapacitance(pin->capacitance());
  mRiseCapacitance = PyCellCapacitance_FromCellCapacitance(pin->rise_capacitance());
  mFallCapacitance = PyCellCapacitance_FromCellCapacitance(pin->fall_capacitance());
}

// @brief デストラクタ
PyInoutPin::~PyInoutPin()
{
  Py_DECREF(mInputId);
  Py_DECREF(mCapacitance);
  Py_DECREF(mRiseCapacitance);
  Py_DECREF(mFallCapacitance);
}

// @brief 入力ピン番号を得る．
PyObject*
PyInoutPin::input_id()
{
  return mInputId;
}

// @brief 負荷容量を返す．
PyObject*
PyInoutPin::capacitance()
{
  return mCapacitance;
}

// @brief 立ち上がりの負荷容量を返す．
PyObject*
PyInoutPin::rise_capacitance()
{
  return mRiseCapacitance;
}

// @brief 立ち下がりの負荷容量を返す．
PyObject*
PyInoutPin::fall_capacitance()
{
  return mFallCapacitance;
}

END_NAMESPACE_YM

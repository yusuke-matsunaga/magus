
/// @file PyCell.cc
/// @brief PyCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyCell.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM

/// @brief CellPin から PyObject を作る．
/// @param[in] pin CellPin へのポインタ
/// @return cell を表す PyObject
extern
PyObject*
PyCellPin_FromCellPin(const CellPin* pin);

/// @brief CellTiming から PyObject を作る．
/// @param[in] timing CellTiming へのポインタ
/// @return timing を表す PyObject
extern
PyObject*
PyCellTiming_FromCellTiming(const CellTiming* timing);


//////////////////////////////////////////////////////////////////////
// クラス PyCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyCell::PyCell(const Cell* cell) :
  mCell(cell)
{
  mGroup = NULL;

  ymuint np = cell->pin_num();
  mPinArray = new PyObject*[np];
  for (ymuint i = 0; i < np; ++ i) {
    const CellPin* pin = cell->pin(i);
    mPinArray[i] = PyCellPin_FromCellPin(pin);
  }

  ymuint nt = cell->timing_num();
  mTimingArray = new PyObject*[nt];
  for (ymuint i = 0; i < nt; ++ i) {
    const CellTiming* timing = cell->timing(i);
    mTimingArray[i] = PyCellTiming_FromCellTiming(timing);
  }
}

// @brief デストラクタ
PyCell::~PyCell()
{
  ymuint np = mCell->pin_num();
  for (ymuint i = 0; i < np; ++ i) {
    Py_DECREF(mPinArray[i]);
  }
  delete [] mPinArray;

  ymuint nt = mCell->timing_num();
  for (ymuint i = 0; i < nt; ++ i) {
    Py_DECREF(mTimingArray[i]);
  }

  Py_DECREF(mGroup);
}

// @brief セルグループを設定する．
void
PyCell::set_group(PyObject* group)
{
  mGroup = group;
  Py_INCREF(mGroup);
}

// @brief ピン番号からピンを得る．
PyObject*
PyCell::pin(ymuint pin_id)
{
  assert_cond( pin_id < mCell->pin_num(), __FILE__, __LINE__);
  return mPinArray[pin_id];
}

// @brief タイミング番号からタイミングを得る．
PyObject*
PyCell::timing(ymuint t_id)
{
  assert_cond( t_id < mCell->timing_num(), __FILE__, __LINE__);
  return mTimingArray[t_id];
}

END_NAMESPACE_YM

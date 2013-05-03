
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

//////////////////////////////////////////////////////////////////////
// クラス PyCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyCell::PyCell(const Cell* cell)
{
  mCell = cell;
  mId = PyObject_FromYmuint32(cell->id());
  mName = PyObject_FromString(cell->name());
  mArea = PyCellArea_FromCellArea(cell->area());

  ymuint np = cell->pin_num();
  mPinArray = new PyObject*[np];
  for (ymuint i = 0; i < np; ++ i) {
    const CellPin* pin = cell->pin(i);
    mPinArray[i] = PyCellPin_FromCellPin(pin);
  }

  mGroup = NULL;
}

// @brief デストラクタ
PyCell::~PyCell()
{
  Py_DECREF(mId);
  Py_DECREF(mName);
  Py_DECREF(mArea);

  ymuint np = mCell->pin_num();
  for (ymuint i = 0; i < np; ++ i) {
    Py_DECREF(mPinArray[i]);
  }
  delete [] mPinArray;

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
  return mPinArray[pin_id];
}

END_NAMESPACE_YM


/// @file PyCell.cc
/// @brief PyCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyCell.h"


BEGIN_NAMESPACE_YM

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
    PyObject* obj = NULL;
    switch ( pin->direction() ) {
    case CellPin::kDirInput:    obj = new PyInput(pin); break;
    case CellPin::kDirOutput:   obj = new PyOutput(pin); break;
    case CellPin::kDirInout:    obj = new PyInout(pin); break;
    case CellPin::kDirInternal: obj = new PyInternal(pin); break;
    }
    mPinArray[i] = obj;
  }
}

// @brief デストラクタ
PyCell::~PyCell()
{
  Py_DECREF(mId);
  Py_DECREF(mName);
  Py_DECREF(mArea);
  ymuint np = cell->pin_num();
  for (ymuint i = 0; i < np; ++ i) {
    Py_DECREF(mPinArray[i]);
  }
  delete [] mPinArray;
}

// @brief ピン番号からピンを得る．
PyObject*
PyCell::pin(ymuint pin_id)
{
  return mPinArray[pin_id];
}

// @brief 名前からピンを得る．
PyObject*
PyCell::pin(const string& name)
{
}

END_NAMESPACE_YM

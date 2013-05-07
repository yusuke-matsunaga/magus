
/// @file PyInternalPin.cc
/// @brief PyInternalPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyInternalPin.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyInternalPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyInternalPin::PyInternalPin(const CellPin* pin) :
  PyPin(pin)
{
  assert_cond(pin->is_internal(), __FILE__, __LINE__);
}

// @brief デストラクタ
PyInternalPin::~PyInternalPin()
{
}

// @brief 内部ピン番号を返す．
PyObject*
PyInternalPin::internal_id()
{
  return PyObject_FromYmuint32(pin()->internal_id());
}

END_NAMESPACE_YM

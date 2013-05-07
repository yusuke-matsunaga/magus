
/// @file PyPin.cc
/// @brief PyPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyPin.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyPin::PyPin(const CellPin* pin) :
  mPin(pin)
{
}

// @brief デストラクタ
PyPin::~PyPin()
{
}

// @brief ピン番号を得る．
PyObject*
PyPin::id()
{
  return PyObject_FromYmuint32(mPin->pin_id());
}

// @brief ピン名を得る．
PyObject*
PyPin::name()
{
  return PyObject_FromString(mPin->name());
}

// @brief 入力ピン番号を得る．
PyObject*
PyPin::input_id()
{
  return NULL;
}

// @brief 負荷容量を返す．
PyObject*
PyPin::capacitance()
{
  return NULL;
}

// @brief 立ち上がりの負荷容量を返す．
PyObject*
PyPin::rise_capacitance()
{
  return NULL;
}

// @brief 立ち下がりの負荷容量を返す．
PyObject*
PyPin::fall_capacitance()
{
  return NULL;
}

// @brief 出力ピン番号を返す．
PyObject*
PyPin::output_id()
{
  return NULL;
}

// @brief 機能を表す論理式を返す．
PyObject*
PyPin::function()
{
  return NULL;
}

// @brief three_state 論理式を返す．
PyObject*
PyPin::three_state()
{
  return NULL;
}

// @brief 最大ファンアウト容量を返す．
PyObject*
PyPin::max_fanout()
{
  return NULL;
}

// @brief 最小ファンアウト容量を返す．
PyObject*
PyPin::min_fanout()
{
  return NULL;
}

// @brief 最大負荷容量を返す．
PyObject*
PyPin::max_capacitance()
{
  return NULL;
}

// @brief 最小負荷容量を返す．
PyObject*
PyPin::min_capacitance()
{
  return NULL;
}

// @brief 最大遷移時間を返す．
PyObject*
PyPin::max_transition()
{
  return NULL;
}

// @brief 最小遷移時間を返す．
PyObject*
PyPin::min_transition()
{
  return NULL;
}

// @brief 内部ピン番号を返す．
PyObject*
PyPin::internal_id()
{
  return NULL;
}

END_NAMESPACE_YM

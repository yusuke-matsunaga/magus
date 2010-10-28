
/// @file libym_cell/mislib/MislibPin.cc
/// @brief MislibPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MislibPin.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス MislibPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibPin::MislibPin()
{
}

// @brief デストラクタ
MislibPin::~MislibPin()
{
}

// @brief ピン名を返す．
string
MislibPin::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibInputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibInputPin::MislibInputPin()
{
}

// @brief デストラクタ
MislibInputPin::~MislibInputPin()
{
}

// @brief 方向を返す．
tCellDirection
MislibInputPin::direction() const
{
  return kDirInput;
}

// @brief 負荷容量を返す．
CellCapacitance
MislibInputPin::capacitance() const
{
  return mCapacitance;
}

// @brief 立ち上がり時の負荷容量を返す．
CellCapacitance
MislibInputPin::rise_capacitance() const
{
  return mCapacitance;
}

// @brief 立ち下がり時の負荷容量を返す．
CellCapacitance
MislibInputPin::fall_capacitance() const
{
  return mCapacitance;
}

// @brief 最大ファンアウト容量を返す．
CellCapacitance
MislibInputPin::max_fanout() const
{
  return CellCapacitance::infty();
}

// @brief 最小ファンアウト容量を返す．
CellCapacitance
MislibInputPin::min_fanout() const
{
  return CellCapacitance(0.0);
}

// @brief 最大負荷容量を返す．
CellCapacitance
MislibInputPin::max_capacitance() const
{
  return CellCapacitance::infty();
}

// @brief 最小負荷容量を返す．
CellCapacitance
MislibInputPin::min_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 最大遷移時間を返す．
CellTime
MislibInputPin::max_transition() const
{
  return CellTime::infty();
}

// @brief 最小遷移時間を返す．
CellTime
MislibInputPin::min_transition() const
{
  return CellTime(0.0);
}


//////////////////////////////////////////////////////////////////////
// クラス MislibOutputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibOutputPin::MislibOutputPin()
{
}

// @brief デストラクタ
MislibOutputPin::~MislibOutputPin()
{
}

// @brief 方向を返す．
tCellDirection
MislibOutputPin::direction() const
{
  return kDirInput;
}

// @brief 負荷容量を返す．
CellCapacitance
MislibOutputPin::capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 立ち上がり時の負荷容量を返す．
CellCapacitance
MislibOutputPin::rise_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 立ち下がり時の負荷容量を返す．
CellCapacitance
MislibOutputPin::fall_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 最大ファンアウト容量を返す．
CellCapacitance
MislibOutputPin::max_fanout() const
{
  return CellCapacitance::infty();
}

// @brief 最小ファンアウト容量を返す．
CellCapacitance
MislibOutputPin::min_fanout() const
{
  return CellCapacitance(0.0);
}

// @brief 最大負荷容量を返す．
CellCapacitance
MislibOutputPin::max_capacitance() const
{
  return CellCapacitance::infty();
}

// @brief 最小負荷容量を返す．
CellCapacitance
MislibOutputPin::min_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 最大遷移時間を返す．
CellTime
MislibOutputPin::max_transition() const
{
  return CellTime::infty();
}

// @brief 最小遷移時間を返す．
CellTime
MislibOutputPin::min_transition() const
{
  return CellTime(0.0);
}

END_NAMESPACE_YM_CELL


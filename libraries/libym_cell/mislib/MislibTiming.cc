
/// @file libym_cell/mislib/MislibTiming.h
/// @brief MislibTiming の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MislibTiming.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス MislibTiming
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibTiming::MislibTiming()
{
}

// @brief デストラクタ
MislibTiming::~MislibTiming()
{
}

// @brief 型の取得
tCellTimingType
MislibTiming::type() const
{
  return kTimingCombinational;
}

// @brief 立ち上がり固有遅延の取得
CellTime
MislibTiming::intrinsic_rise() const
{
  return mIntrinsic[0];
}

// @brief 立ち下がり固有遅延の取得
CellTime
MislibTiming::intrinsic_fall() const
{
  return mIntrinsic[1];
}

// @brief 立ち上がりスロープ遅延の取得
CellTime
MislibTiming::slope_rise() const
{
  return CellTime(0.0);
}

// @brief 立ち下がりスロープ遅延の取得
CellTime
MislibTiming::slope_fall() const
{
  return CellTime(0.0);
}

// @brief 立ち上がり遷移遅延の取得
CellResistance
MislibTiming::rise_resistance() const
{
  return mResistance[0];
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
MislibTiming::fall_resistance() const
{
  return mResistance[1];
}

// @brief 立ち上がり遷移遅延の取得
CellResistance
MislibTiming::rise_pin_resistance() const
{
  return CellResistance(0.0);
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
MislibTiming::fall_pin_resistance() const
{
  return CellResistance(0.0);
}

// @brief 立ち上がり？？？
CellTime
MislibTiming::rise_delay_intercept() const
{
  return CellTime(0.0);
}

// @brief 立ち下がり？？？
CellTime
MislibTiming::fall_delay_intercept() const
{
  return CellTime(0.0);
}

// @brief 立ち上がり遷移遅延テーブルの取得
const CellLut*
MislibTiming::rise_transition() const
{
  return NULL;
}

// @brief 立ち下がり遷移遅延テーブルの取得
const CellLut*
MislibTiming::fall_transition() const
{
  return NULL;
}

// @brief 立ち上がり伝搬遅延テーブルの取得
const CellLut*
MislibTiming::rise_propagation() const
{
  return NULL;
}

// @brief 立ち下がり伝搬遅延テーブルの取得
const CellLut*
MislibTiming::fall_propagation() const
{
  return NULL;
}

// @brief 立ち上がりセル遅延テーブルの取得
const CellLut*
MislibTiming::cell_rise() const
{
  return NULL;
}

// @brief 立ち下がりセル遅延テーブルの取得
const CellLut*
MislibTiming::cell_fall() const
{
  return NULL;
}

END_NAMESPACE_YM_CELL


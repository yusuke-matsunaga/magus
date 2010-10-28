
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

/// @brief 関連する終点ピンの取得
const CellPin*
MislibTiming::opin() const
{
  return mOpin;
}

// @brief 条件の取得
tCellTimingSense
MislibTiming::timing_sense() const
{
}

// @brief 型の取得
tCellTimingType
MislibTiming::timing_type() const
{
}

// @brief 立ち上がり固有遅延の取得
CellTime
MislibTiming::intrinsic_rise() const
{
}

// @brief 立ち下がり固有遅延の取得
CellTime
MislibTiming::intrinsic_fall() const
{
}

// @brief 立ち上がりスロープ遅延の取得
CellTime
MislibTiming::slope_rise() const
{
}

// @brief 立ち下がりスロープ遅延の取得
CellTime
MislibTiming::slope_fall() const
{
}

// @brief 立ち上がり遷移遅延の取得
CellResistance
MislibTiming::rise_resistance() const
{
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
MislibTiming::fall_resistance() const
{
}

// @brief 立ち上がり遷移遅延の取得
CellResistance
MislibTiming::rise_pin_resistance() const
{
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
MislibTiming::fall_pin_resistance() const
{
}

// @brief 立ち上がり？？？
CellTime
MislibTiming::rise_delay_intercept() const
{
}

// @brief 立ち下がり？？？
CellTime
MislibTiming::fall_delay_intercept() const
{
}

// @brief 立ち上がり遷移遅延テーブルの取得
const CellLut*
MislibTiming::rise_transition() const
{
}

// @brief 立ち下がり遷移遅延テーブルの取得
const CellLut*
MislibTiming::fall_transition() const
{
}

// @brief 立ち上がり伝搬遅延テーブルの取得
const CellLut*
MislibTiming::rise_propagation() const
{
}

// @brief 立ち下がり伝搬遅延テーブルの取得
const CellLut*
MislibTiming::fall_propagation() const
{
}

// @brief 立ち上がりセル遅延テーブルの取得
const CellLut*
MislibTiming::cell_rise() const
{
}

// @brief 立ち下がりセル遅延テーブルの取得
const CellLut*
MislibTiming::cell_fall() const
{
}


//////////////////////////////////////////////////////////////////////
// クラス MislibTiming1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibTiming1::MislibTiming1()
{
}

// @brief デストラクタ
MislibTiming1::~MislibTiming1()
{
}

// @brief 関連する開始ピン数の取得
ymuint
MislibTiming1::ipin_num() const
{
  return 1;
}

// @brief 関連する開始ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < ipin_num() )
const CellPin*
MislibTiming1::ipin(ymuint pos) const
{
  if ( pos == 0 ) {
    return mIpin;
  }
  else {
    return NULL;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MislibTiming2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibTiming2::MislibTiming2() :
  mIpinNum(0U),
  mIpinArray(NULL)
{
}

// @brief デストラクタ
MislibTiming2::~MislibTiming2()
{
  delete [] mIpinArray;
}

// @brief 関連する開始ピン数の取得
ymuint
MislibTiming2::ipin_num() const
{
  return mIpinNum;
}

// @brief 関連する開始ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < ipin_num() )
const CellPin*
MislibTiming2::ipin(ymuint pos) const
{
  return mIpinArray[pos];
}

END_NAMESPACE_YM_CELL


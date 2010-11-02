
/// @file libym_cell/CiPin.cc
/// @brief CiPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CiPin.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiPin::CiPin()
{
}

// @brief デストラクタ
CiPin::~CiPin()
{
}

// @brief ピン番号を返す．
ymuint
CiPin::id() const
{
  return mId;
}

// @brief ピン名を返す．
string
CiPin::name() const
{
  return mName;
}

// @brief 負荷容量を返す．
CellCapacitance
CiPin::capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 立ち上がり時の負荷容量を返す．
CellCapacitance
CiPin::rise_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 立ち下がり時の負荷容量を返す．
CellCapacitance
CiPin::fall_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 機能を表す論理式を返す．
LogExpr
CiPin::function() const
{
  return LogExpr::make_zero();
}

// @brief 最大ファンアウト容量を返す．
CellCapacitance
CiPin::max_fanout() const
{
  return CellCapacitance(0.0);
}

// @brief 最小ファンアウト容量を返す．
CellCapacitance
CiPin::min_fanout() const
{
  return CellCapacitance(0.0);
}

// @brief 最大負荷容量を返す．
CellCapacitance
CiPin::max_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 最小負荷容量を返す．
CellCapacitance
CiPin::min_capacitance() const
{
  return CellCapacitance(0.0);
}

// @brief 最大遷移時間を返す．
CellTime
CiPin::max_transition() const
{
  return CellTime(0.0);
}

// @brief 最小遷移時間を返す．
CellTime
CiPin::min_transition() const
{
  return CellTime(0.0);
}

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] timing_sense タイミング情報の摘要条件
// @return 条件に合致するタイミング情報を返す．
// @note なければ NULL を返す．
const CellTiming*
CiPin::timing(ymuint ipos,
	      tCellTimingSense sense) const
{
  return NULL;
}

// @brief 出力ピン(入出力ピン)の関数を設定する．
// @param[in] function 関数を表す論理式
void
CiPin::set_function(const LogExpr& function)
{
}

// @brief 出力ピン(入出力ピン)のタイミング情報を設定する．
// @param[in] pin_id 入力ピンのピン番号
// @param[in] sense タイミング情報の適用条件
// @param[in] timing 設定するタイミング情報
void
CiPin::set_timing(ymuint pin_id,
		  tCellTimingSense sense,
		  const CellTiming* timing)
{
}


//////////////////////////////////////////////////////////////////////
// クラス CiInputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiInputPin::CiInputPin()
{
}

// @brief デストラクタ
CiInputPin::~CiInputPin()
{
}

// @brief 方向を返す．
tCellDirection
CiInputPin::direction() const
{
  return kDirInput;
}

// @brief 負荷容量を返す．
CellCapacitance
CiInputPin::capacitance() const
{
  return mCapacitance;
}

// @brief 立ち上がり時の負荷容量を返す．
CellCapacitance
CiInputPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief 立ち下がり時の負荷容量を返す．
CellCapacitance
CiInputPin::fall_capacitance() const
{
  return mFallCapacitance;
}


//////////////////////////////////////////////////////////////////////
// クラス CiOutputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiOutputPin::CiOutputPin()
{
}

// @brief デストラクタ
CiOutputPin::~CiOutputPin()
{
}

// @brief 方向を返す．
tCellDirection
CiOutputPin::direction() const
{
  return kDirOutput;
}

// @brief 機能を表す論理式を返す．
LogExpr
CiOutputPin::function() const
{
  return mFunction;
}

// @brief 最大ファンアウト容量を返す．
CellCapacitance
CiOutputPin::max_fanout() const
{
  return mMaxFanout;
}

// @brief 最小ファンアウト容量を返す．
CellCapacitance
CiOutputPin::min_fanout() const
{
  return mMinFanout;
}

// @brief 最大負荷容量を返す．
CellCapacitance
CiOutputPin::max_capacitance() const
{
  return mMaxCapacitance;
}

// @brief 最小負荷容量を返す．
CellCapacitance
CiOutputPin::min_capacitance() const
{
  return mMinCapacitance;
}

// @brief 最大遷移時間を返す．
CellTime
CiOutputPin::max_transition() const
{
  return mMaxTransition;;
}

// @brief 最小遷移時間を返す．
CellTime
CiOutputPin::min_transition() const
{
  return mMinTransition;
}

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] timing_sense タイミング情報の摘要条件
// @return 条件に合致するタイミング情報を返す．
// @note なければ NULL を返す．
const CellTiming*
CiOutputPin::timing(ymuint ipos,
		    tCellTimingSense sense) const
{
  ymuint offset = 0;
  if ( sense == kSenseNegaUnate ) {
    offset = 1;
  }
  return mTimingArray[ipos * 2 + offset];
}

// @brief 出力ピン(入出力ピン)の関数を設定する．
// @param[in] function 関数を表す論理式
void
CiOutputPin::set_function(const LogExpr& function)
{
  mFunction = function;
}

// @brief 出力ピン(入出力ピン)のタイミング情報を設定する．
// @param[in] pin_id 入力ピンのピン番号
// @param[in] sense タイミング情報の適用条件
// @param[in] timing 設定するタイミング情報
void
CiOutputPin::set_timing(ymuint pin_id,
			tCellTimingSense sense,
			const CellTiming* timing)
{
  switch ( sense ) {
  case kSensePosiUnate:
    mTimingArray[pin_id * 2 + 0] = timing;
    break;

  case kSenseNegaUnate:
    mTimingArray[pin_id * 2 + 1] = timing;
    break;

  case kSenseNonUnate:
    mTimingArray[pin_id * 2 + 0] = timing;
    mTimingArray[pin_id * 2 + 1] = timing;
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス CiInoutPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiInoutPin::CiInoutPin()
{
}

// @brief デストラクタ
CiInoutPin::~CiInoutPin()
{
}

// @brief 方向を返す．
tCellDirection
CiInoutPin::direction() const
{
  return kDirInput;
}

// @brief 負荷容量を返す．
CellCapacitance
CiInoutPin::capacitance() const
{
  return mCapacitance;
}

// @brief 立ち上がり時の負荷容量を返す．
CellCapacitance
CiInoutPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief 立ち下がり時の負荷容量を返す．
CellCapacitance
CiInoutPin::fall_capacitance() const
{
  return mFallCapacitance;
}

END_NAMESPACE_YM_CELL

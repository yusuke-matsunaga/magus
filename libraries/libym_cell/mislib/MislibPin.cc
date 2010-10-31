
/// @file libym_cell/mislib/MislibPin.cc
/// @brief MislibPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MislibPin.h"
#include "MislibTiming.h"


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

// @brief 機能を表す論理式を返す．
LogExpr
MislibInputPin::function() const
{
  return LogExpr();
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

// @brief タイミング情報の取得
// @param[in] ipos 入力ピン番号
// @param[out] timing_list タイミング情報を納めるベクタ
// @return 条件に合致するタイミング情報の数を返す．
ymuint
MislibInputPin::timing(ymuint ipos,
		       vector<const CellTiming*>& timing_list) const
{
  timing_list.clear();
  return 0;
}

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] timing_sense タイミング情報の摘要条件
// @return 条件に合致するタイミング情報を返す．
// @note なければ NULL を返す．
const CellTiming*
MislibInputPin::timing(ymuint ipos,
		       tCellTimingSense timing_sense) const
{
  return NULL;
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
  return kDirOutput;
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

// @brief 機能を表す論理式を返す．
LogExpr
MislibOutputPin::function() const
{
  return mFunction;
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

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] sense タイミング情報の摘要条件
// @return 条件に合致するタイミング情報を返す．
// @note なければ NULL を返す．
const CellTiming*
MislibOutputPin::timing(ymuint ipos,
			tCellTimingSense sense) const
{
  if ( ipos == 0 ) return NULL;
  if ( sense == kSensePosiUnate ) {
    return mTimingArray[(ipos - 1) * 2];
  }
  else if ( sense == kSenseNegaUnate ) {
    return mTimingArray[(ipos - 1) * 2 + 1];
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  return NULL;
}

END_NAMESPACE_YM_CELL


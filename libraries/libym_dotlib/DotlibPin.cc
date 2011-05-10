
/// @file libym_dotlib/DotlibPin.cc
/// @brief DotlibPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibPin.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibPin::DotlibPin()
{
  mBitWidth = NULL;
  mCapacitance = NULL;
  mClock = NULL;
  mDirection = NULL;
  mFallCapacitance = NULL;
  mFanoutLoad = NULL;
  mFunction = NULL;
  mInternalNode = NULL;
  mMaxCapacitance = NULL;
  mMaxFanout = NULL;
  mMaxTransition = NULL;
  mMinCapacitance = NULL;
  mMinFanout = NULL;
  mMinTransition = NULL;
  mPinFuncType = NULL;
  mRiseCapacitance = NULL;
}

// @brief デストラクタ
DotlibPin::~DotlibPin()
{
}

// @brief 名前を返す．
ShString
DotlibPin::name() const
{
  return mName;
}

// @brief "bit_width" を返す．
const DotlibNode*
DotlibPin::bit_width() const
{
  return mBitWidth;
}

// @brief "capacitance" を返す．
const DotlibNode*
DotlibPin::capacitance() const
{
  return mCapacitance;
}

// @brief "clock" を返す．
const DotlibNode*
DotlibPin::clock() const
{
  return mClock;
}

// @brief "direction" を返す．
const DotlibNode*
DotlibPin::direction() const
{
  return mDirection;
}

// @brief "fall_capacitance" を返す．
const DotlibNode*
DotlibPin::fall_capacitance() const
{
  return mFallCapacitance;
}

// @brief "fanout_load" を返す．
const DotlibNode*
DotlibPin::fanout_load() const
{
  return mFanoutLoad;
}

// @brief "function" を返す．
const DotlibNode*
DotlibPin::function() const
{
  return mFunction;
}

// @brief "internal_node" を返す．
const DotlibNode*
DotlibPin::internal_node() const
{
  return mInternalNode;
}

// @brief "max_capacitance" を返す．
const DotlibNode*
DotlibPin::max_capacitance() const
{
  return mMaxCapacitance;
}

// @brief "max_fanout" を返す．
const DotlibNode*
DotlibPin::max_fanout() const
{
  return mMaxFanout;
}

// @brief "max_transition" を返す．
const DotlibNode*
DotlibPin::max_transition() const
{
  return mMaxTransition;
}

// @brief "min_capacitance" を返す．
const DotlibNode*
DotlibPin::min_capacitance() const
{
  return mMinCapacitance;
}

// @brief "min_fanout" を返す．
const DotlibNode*
DotlibPin::min_fanout() const
{
  return mMinFanout;
}

// @brief "min_transition" を返す．
const DotlibNode*
DotlibPin::min_transition() const
{
  return mMinTransition;
}

// @brief "pin_func_type" を返す．
const DotlibNode*
DotlibPin::pin_func_type() const
{
  return mPinFuncType;
}

// @brief "rise_capacitance" を返す．
const DotlibNode*
DotlibPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief "timing" グループのリストを得る．
const list<const DotlibNode*>&
DotlibPin::timing_list() const
{
  return mTimingList;
}

END_NAMESPACE_YM_DOTLIB

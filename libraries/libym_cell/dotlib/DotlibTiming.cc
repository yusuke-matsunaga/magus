
/// @file DotlibTiming.cc
/// @brief DotlibTiming の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibTiming.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibTiming
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibTiming::DotlibTiming()
{
}

// @brief デストラクタ
DotlibTiming::~DotlibTiming()
{
}

// @brief 内容を初期化する．
void
DotlibTiming::init()
{
  mFallResistance = NULL;
  mRiseResistance = NULL;
  mIntrinsicFall = NULL;
  mIntrinsicRise = NULL;
  mRelatedBusEquivalent = NULL;
  mRelatedBusPins = NULL;
  mRelatedPin = NULL;
  mSlopeFall = NULL;
  mSlopeRise = NULL;
  mTimingSense = NULL;
  mTimingType = NULL;
  mWhen = NULL;
  mWhenStart = NULL;
  mWhenEnd = NULL;
  mFallDelayIntercept = NULL;
  mRiseDelayIntercept = NULL;
  mFallPinResistance = NULL;
  mRisePinResistance = NULL;
  mCellDegradation = NULL;
  mCellFall = NULL;
  mCellRise = NULL;
  mFallConstraint = NULL;
  mRiseConstraint = NULL;
  mFallPropagation = NULL;
  mRisePropagation = NULL;
  mFallTransition = NULL;
  mRiseTransition = NULL;
  mRetainingFall = NULL;
  mRetainingRise = NULL;
  mRetainFallSlew = NULL;
  mRetainRiseSlew = NULL;
}

// @brief "fall_resistance" を返す．
const DotlibNode*
DotlibTiming::fall_resistance() const
{
  return mFallResistance;
}

// @brief "rise_resistance" を返す．
const DotlibNode*
DotlibTiming::rise_resistance() const
{
  return mRiseResistance;
}

// @brief "intrinsic_fall" を返す．
const DotlibNode*
DotlibTiming::intrinsic_fall() const
{
  return mIntrinsicFall;
}

// @brief "intrinsic_rise" を返す．
const DotlibNode*
DotlibTiming::intrinsic_rise() const
{
  return mIntrinsicRise;
}

// @brief "related_bus_equivalent" を返す．
const DotlibNode*
DotlibTiming::related_bus_equivalent() const
{
  return mRelatedBusEquivalent;
}

// @brief "related_bus_pins" を返す．
const DotlibNode*
DotlibTiming::related_bus_pins() const
{
  return mRelatedBusPins;
}

// @brief "related_pin" を返す．
const DotlibNode*
DotlibTiming::related_pin() const
{
  return mRelatedPin;
}

// @brief "slope_fall" を返す．
const DotlibNode*
DotlibTiming::slope_fall() const
{
  return mSlopeFall;
}

// @brief "slope_rise" を返す．
const DotlibNode*
DotlibTiming::slope_rise() const
{
  return mSlopeRise;
}

// @brief "timing_sense" を返す．
const DotlibNode*
DotlibTiming::timing_sense() const
{
  return mTimingSense;
}

// @brief "timing_type" を返す．
const DotlibNode*
DotlibTiming::timing_type() const
{
  return mTimingType;
}

// @brief "when" を返す．
const DotlibNode*
DotlibTiming::when() const
{
  return mWhen;
}

// @brief "when_start" を返す．
const DotlibNode*
DotlibTiming::when_start() const
{
  return mWhenStart;
}

// @brief "when_end" を返す．
const DotlibNode*
DotlibTiming::when_end() const
{
  return mWhenEnd;
}

// @brief "fall_delay_intercept" を返す．
const DotlibNode*
DotlibTiming::fall_delay_intercept() const
{
  return mFallDelayIntercept;
}

// @brief "rise_delay_intercept" を返す．
const DotlibNode*
DotlibTiming::rise_delay_intercept() const
{
  return mRiseDelayIntercept;
}

// @brief "fall_pin_resistance" を返す．
const DotlibNode*
DotlibTiming::fall_pin_resistance() const
{
  return mFallPinResistance;
}

// @brief "rise_pin_resistance" を返す．
const DotlibNode*
DotlibTiming::rise_pin_resistance() const
{
  return mRisePinResistance;
}

// @brief "cell_degradation" を返す．
const DotlibNode*
DotlibTiming::cell_degradation() const
{
  return mCellDegradation;
}

// @brief "cell_fall" を返す．
const DotlibNode*
DotlibTiming::cell_fall() const
{
  return mCellFall;
}

// @brief "cell_rise" を返す．
const DotlibNode*
DotlibTiming::cell_rise() const
{
  return mCellRise;
}

// @brief "fall_constraint" を返す．
const DotlibNode*
DotlibTiming::fall_constraint() const
{
  return mFallConstraint;
}

// @brief "rise_constraint" を返す．
const DotlibNode*
DotlibTiming::rise_constraint() const
{
  return mRiseConstraint;
}

// @brief "fall_propagation" を返す．
const DotlibNode*
DotlibTiming::fall_propagation() const
{
  return mFallPropagation;
}

// @brief "rise_propagation" を返す．
const DotlibNode*
DotlibTiming::rise_propagation() const
{
  return mRisePropagation;
}

// @brief "fall_transition" を返す．
const DotlibNode*
DotlibTiming::fall_transition() const
{
  return mFallTransition;
}

// @brief "rise_transition" を返す．
const DotlibNode*
DotlibTiming::rise_transition() const
{
  return mRiseTransition;
}

// @brief "retaining_fall" を返す．
const DotlibNode*
DotlibTiming::retaining_fall() const
{
  return mRetainingFall;
}

// @brief "retaining_rise" を返す．
const DotlibNode*
DotlibTiming::retaining_rise() const
{
  return mRetainingRise;
}

// @brief "retain_fall_slew" を返す．
const DotlibNode*
DotlibTiming::retain_fall_slew() const
{
  return mRetainFallSlew;
}

// @brief "retain_rise_slew" を返す．
const DotlibNode*
DotlibTiming::retain_rise_slew() const
{
  return mRetainRiseSlew;
}

END_NAMESPACE_YM_DOTLIB

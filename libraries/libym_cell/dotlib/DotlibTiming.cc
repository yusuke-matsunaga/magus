
/// @file DotlibTiming.cc
/// @brief DotlibTiming の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibTiming.h"
#include "DotlibAttr.h"


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

// @brief 内容をセットする．
bool
DotlibTiming::set_data(const DotlibNode* timing_node)
{
  init();

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

  // 属性を内部のハッシュに登録する．
  for (const DotlibAttr* attr = timing_node->attr_top();
       attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    add(attr_name, attr_value);
  }

  // 'related_pin' を取り出す．
  if ( !get_singleton_or_null("related_pin", mRelatedPin) ) {
    return false;
  }

  // 'related_bus_pins' を取り出す．
  if ( !get_singleton_or_null("related_bus_pins", mRelatedBusPins) ) {
    return false;
  }

  // 'related_bus_equivalent' を取り出す．
  if ( !get_singleton_or_null("related_bus_equivalent", mRelatedBusEquivalent) ) {
    return false;
  }

  // 'timing_sense' を取り出す．
  if ( !get_singleton_or_null("timing_sense", mTimingSense) ) {
    return false;
  }

  // 'timing_type' を取り出す．
  if ( !get_singleton_or_null("timing_type", mTimingType) ) {
    return false;
  }

  // 'when' を取り出す．
  if ( !get_singleton_or_null("when", mWhen) ) {
    return false;
  }

  // 'when_start' を取り出す．
  if ( !get_singleton_or_null("when_start", mWhenStart) ) {
    return false;
  }

  // 'when_end' を取り出す．
  if ( !get_singleton_or_null("when_end", mWhenEnd) ) {
    return false;
  }

  // 'rise_resistance' を取り出す．
  if ( !get_singleton_or_null("rise_resistance", mRiseResistance) ) {
    return false;
  }

  // 'fall_resistance' を取り出す．
  if ( !get_singleton_or_null("fall_resistance", mFallResistance) ) {
    return false;
  }

  // 'intrinsic_rise' を取り出す．
  if ( !get_singleton_or_null("intrinsic_rise", mIntrinsicRise) ) {
    return false;
  }

  // 'intrinsic_fall' を取り出す．
  if ( !get_singleton_or_null("intrinsic_fall", mIntrinsicFall) ) {
    return false;
  }

  // 'slope_rise' を取り出す．
  if ( !get_singleton_or_null("slope_rise", mSlopeRise) ) {
    return false;
  }

  // 'slope_fall' を取り出す．
  if ( !get_singleton_or_null("slope_fall", mSlopeFall) ) {
    return false;
  }

  // 'rise_delay_intercept' を取り出す．
  if ( !get_singleton_or_null("rise_delay_intercept", mRiseDelayIntercept) ) {
    return false;
  }

  // 'fall_delay_intercept' を取り出す．
  if ( !get_singleton_or_null("fall_delay_intercept", mFallDelayIntercept) ) {
    return false;
  }

  // 'rise_pin_resistance' を取り出す．
  if ( !get_singleton_or_null("rise_pin_resistance", mRisePinResistance) ) {
    return false;
  }

  // 'fall_pin_resistance' を取り出す．
  if ( !get_singleton_or_null("fall_pin_resistance", mFallPinResistance) ) {
    return false;
  }

  // 'cell_degradation' を取り出す．
  if ( !get_singleton_or_null("cell_degradation", mCellDegradation) ) {
    return false;
  }

  // 'cell_rise' を取り出す．
  if ( !get_singleton_or_null("cell_rise", mCellRise) ) {
    return false;
  }

  // 'cell_fall' を取り出す．
  if ( !get_singleton_or_null("cell_fall", mCellFall) ) {
    return false;
  }

  // 'rise_constraint' を取り出す．
  if ( !get_singleton_or_null("rise_constraint", mRiseConstraint) ) {
    return false;
  }

  // 'fall_constraint' を取り出す．
  if ( !get_singleton_or_null("fall_constraint", mFallConstraint) ) {
    return false;
  }

  // 'rise_propagation' を取り出す．
  if ( !get_singleton_or_null("rise_propagation", mRisePropagation) ) {
    return false;
  }

  // 'fall_propagation' を取り出す．
  if ( !get_singleton_or_null("fall_propagation", mFallPropagation) ) {
    return false;
  }

  // 'rise_transition' を取り出す．
  if ( !get_singleton_or_null("rise_transition", mRiseTransition) ) {
    return false;
  }

  // 'fall_transition' を取り出す．
  if ( !get_singleton_or_null("fall_transition", mFallTransition) ) {
    return false;
  }

  // 'retaining_rise' を取り出す．
  if ( !get_singleton_or_null("retaining_rise", mRetainingRise) ) {
    return false;
  }

  // 'retaining_fall' を取り出す．
  if ( !get_singleton_or_null("retaining_fall", mRetainingFall) ) {
    return false;
  }

  // 'retain_rise_slew' を取り出す．
  if ( !get_singleton_or_null("retain_rise_slew", mRetainRiseSlew) ) {
    return false;
  }

  // 'retain_fall_slew' を取り出す．
  if ( !get_singleton_or_null("retain_fall_slew", mRetainFallSlew) ) {
    return false;
  }

  return true;
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

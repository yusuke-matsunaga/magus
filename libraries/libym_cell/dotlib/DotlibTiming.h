#ifndef DOTLIB_DOTLIBTIMING_H
#define DOTLIB_DOTLIBTIMING_H

/// @file dotlib/DotlibTiming.h
/// @brief DolibTiming のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibAttrMap.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibTiming DotlibTiming.h "DotlibTiming.h"
/// @brief DotlibNode の木から取り出したタイミング情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibTiming :
  public DotlibAttrMap
{
  friend class DotlibNode;

public:

  /// @brief コンストラクタ
  DotlibTiming();

  /// @brief デストラクタ
  ~DotlibTiming();


public:

  /// @brief 内容を初期化する．
  void
  init();

  /// @brief "fall_resistance" を返す．
  const DotlibNode*
  fall_resistance() const;

  /// @brief "rise_resistance" を返す．
  const DotlibNode*
  rise_resistance() const;

  /// @brief "intrinsic_fall" を返す．
  const DotlibNode*
  intrinsic_fall() const;

  /// @brief "intrinsic_rise" を返す．
  const DotlibNode*
  intrinsic_rise() const;

  /// @brief "related_bus_equivalent" を返す．
  const DotlibNode*
  related_bus_equivalent() const;

  /// @brief "related_bus_pins" を返す．
  const DotlibNode*
  related_bus_pins() const;

  /// @brief "related_pin" を返す．
  const DotlibNode*
  related_pin() const;

  /// @brief "slope_fall" を返す．
  const DotlibNode*
  slope_fall() const;

  /// @brief "slope_rise" を返す．
  const DotlibNode*
  slope_rise() const;

  /// @brief "timing_sense" を返す．
  const DotlibNode*
  timing_sense() const;

  /// @brief "timing_type" を返す．
  const DotlibNode*
  timing_type() const;

  /// @brief "when" を返す．
  const DotlibNode*
  when() const;

  /// @brief "when_start" を返す．
  const DotlibNode*
  when_start() const;

  /// @brief "when_end" を返す．
  const DotlibNode*
  when_end() const;

  /// @brief "fall_delay_intercept" を返す．
  const DotlibNode*
  fall_delay_intercept() const;

  /// @brief "rise_delay_intercept" を返す．
  const DotlibNode*
  rise_delay_intercept() const;

  /// @brief "fall_pin_resistance" を返す．
  const DotlibNode*
  fall_pin_resistance() const;

  /// @brief "rise_pin_resistance" を返す．
  const DotlibNode*
  rise_pin_resistance() const;

  /// @brief "cell_degradation" を返す．
  const DotlibNode*
  cell_degradation() const;

  /// @brief "cell_fall" を返す．
  const DotlibNode*
  cell_fall() const;

  /// @brief "cell_rise" を返す．
  const DotlibNode*
  cell_rise() const;

  /// @brief "fall_constraint" を返す．
  const DotlibNode*
  fall_constraint() const;

  /// @brief "rise_constraint" を返す．
  const DotlibNode*
  rise_constraint() const;

  /// @brief "fall_propagation" を返す．
  const DotlibNode*
  fall_propagation() const;

  /// @brief "rise_propagation" を返す．
  const DotlibNode*
  rise_propagation() const;

  /// @brief "fall_transition" を返す．
  const DotlibNode*
  fall_transition() const;

  /// @brief "rise_transition" を返す．
  const DotlibNode*
  rise_transition() const;

  /// @brief "retaining_fall" を返す．
  const DotlibNode*
  retaining_fall() const;

  /// @brief "retaining_rise" を返す．
  const DotlibNode*
  retaining_rise() const;

  /// @brief "retain_fall_slew" を返す．
  const DotlibNode*
  retain_fall_slew() const;

  /// @brief "retain_rise_slew" を返す．
  const DotlibNode*
  retain_rise_slew() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // fall_resistance
  const DotlibNode* mFallResistance;

  // rise_resistance
  const DotlibNode* mRiseResistance;

  // intrinsic_fall
  const DotlibNode* mIntrinsicFall;

  // intrinsic_rise
  const DotlibNode* mIntrinsicRise;

  // related_bus_equivalent
  const DotlibNode* mRelatedBusEquivalent;

  // related_bus_pins
  const DotlibNode* mRelatedBusPins;

  // related_pin
  const DotlibNode* mRelatedPin;

  // slope_fall
  const DotlibNode* mSlopeFall;

  // slope_rise
  const DotlibNode* mSlopeRise;

  // timing_sense
  const DotlibNode* mTimingSense;

  // timing_type
  const DotlibNode* mTimingType;

  // when
  const DotlibNode* mWhen;

  // when_start
  const DotlibNode* mWhenStart;

  // when_end
  const DotlibNode* mWhenEnd;

  // fall_delay_intercept
  const DotlibNode* mFallDelayIntercept;

  // rise_delay_intercept
  const DotlibNode* mRiseDelayIntercept;

  // fall_pin_resistance
  const DotlibNode* mFallPinResistance;

  // rise_pin_resistance
  const DotlibNode* mRisePinResistance;

  // cell_degradation
  const DotlibNode* mCellDegradation;

  // cell_fall
  const DotlibNode* mCellFall;

  // cell_rise
  const DotlibNode* mCellRise;

  // fall_constraint
  const DotlibNode* mFallConstraint;

  // rise_constraint
  const DotlibNode* mRiseConstraint;

  // fall_propagation
  const DotlibNode* mFallPropagation;

  // rise_propagation
  const DotlibNode* mRisePropagation;

  // fall_transition
  const DotlibNode* mFallTransition;

  // rise_transition
  const DotlibNode* mRiseTransition;

  // retaining_fall
  const DotlibNode* mRetainingFall;

  // retaining_rise
  const DotlibNode* mRetainingRise;

  // retain_fall_slew
  const DotlibNode* mRetainFallSlew;

  // retain_rise_slew
  const DotlibNode* mRetainRiseSlew;

};

END_NAMESPACE_YM_DOTLIB

#endif // DOTLIB_DOTLIBTIMING_H

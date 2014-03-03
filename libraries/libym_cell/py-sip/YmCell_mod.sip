
/// @file YmCell_mod.sip
/// @brief YmCell モジュールの sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

%Import YmUtils/YmUtils_mod.sip
%Import YmLogic/YmLogic_mod.sip

%Module YmCell

%Include CellArea.sip
%Include CellCapacitance.sip
%Include CellResistance.sip
%Include CellTime.sip

%Include CellClass.sip
%Include CellGroup.sip
%Include Cell.sip
%Include CellPin.sip
%Include CellBus.sip
%Include CellBusType.sip
%Include CellBundle.sip
%Include CellTiming.sip
%Include CellLut.sip
%Include CellLutTemplate.sip

%Include CellLibrary.sip
%Include CellPatGraph.sip

%Include CellMislibReader.sip
%Include CellDotlibReader.sip


//////////////////////////////////////////////////////////////////////
/// @brief テクノロジを表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellTechnology {
  kCellTechCmos,
  kCellTechFpga
};


//////////////////////////////////////////////////////////////////////
/// @brief 遅延モデルを表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellDelayModel {
  kCellDelayGenericCmos,
  kCellDelayTableLookup,
  kCellDelayPiecewiseCmos,
  kCellDelayCmos2,
  kCellDelayDcm
};


//////////////////////////////////////////////////////////////////////
/// @brief ピンの方向を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellPinDirection {
  kCellPinInput,
  kCellPinOutput,
  kCellPinInout,
  kCellPinInternal
};


//////////////////////////////////////////////////////////////////////
/// @brief タイミングの条件を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellTimingSense {
  kCellPosiUnate,
  kCellNegaUnate,
  kCellNonUnate
};


/////////////////////////////////////////////////////////////////////
/// @brief タイミング情報の型を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellTimingType {
  kCellTimingCombinational,
  kCellTimingCombinationalRise,
  kCellTimingCombinationalFall,

  kCellTimingThreeStateEnable,
  kCellTimingThreeStateDisable,
  kCellTimingThreeStateEnableRise,
  kCellTimingThreeStateEnableFall,
  kCellTimingThreeStateDisableRise,
  kCellTimingThreeStateDisableFall,

  kCellTimingRisingEdge,
  kCellTimingFallingEdge,

  kCellTimingPreset,
  kCellTimingClear,

  kCellTimingHoldRising,
  kCellTimingHoldFalling,

  kCellTimingSetupRising,
  kCellTimingSetupFalling,

  kCellTimingRecoveryRising,
  kCellTimingRecoveryFalling,

  kCellTimingSkewRising,
  kCellTimingSkewFalling,

  kCellTimingRemovalRising,
  kCellTimingRemovalFalling,

  kCellTimingNonSeqSetupRising,
  kCellTimingNonSeqSetupFalling,
  kCellTimingNonSeqHoldRising,
  kCellTimingNonSeqHoldFalling,

  kCellTimingNochangeHighHigh,
  kCellTimingNochangeHighLow,
  kCellTimingNochangeLowHigh,
  kCellTimingNochangeLowLow
};


//////////////////////////////////////////////////////////////////////
/// @brief ルックアップテーブルの変数の型
//////////////////////////////////////////////////////////////////////
enum tCellVarType {
  kCellVarInputNetTransition,

  kCellVarTotalOutputNetCapacitance,
  kCellVarOutputNetLength,
  kCellVarOutputNetWireCap,
  kCellVarOutputNetPinCap,

  kCellVarRelatedOutTotalOutputNetCapacitance,
  kCellVarRelatedOutOutputNetLength,
  kCellVarRelatedOutOutputNetWireCap,
  kCellVarRelatedOutOutputNetPinCap,

  kCellVarConstrainedPinTransition,

  kCellVarRelatedPinTransition,

  kCellVarNone
};


//////////////////////////////////////////////////////////////////////
/// @brief パタングラフのノードの型
//////////////////////////////////////////////////////////////////////
enum tCellPatType {
  kCellPatInput,
  kCellPatAnd,
  kCellPatXor
};

%ModuleHeaderCode
#include "cell/cell_nsdef.h"

using nsYm::tCellTechnology;
using nsYm::kCellTechCmos;
using nsYm::kCellTechFpga;

using nsYm::tCellDelayModel;
using nsYm::kCellDelayGenericCmos;
using nsYm::kCellDelayTableLookup;
using nsYm::kCellDelayPiecewiseCmos;
using nsYm::kCellDelayCmos2;
using nsYm::kCellDelayDcm;

using nsYm::tCellPinDirection;
using nsYm::kCellPinInput;
using nsYm::kCellPinOutput;
using nsYm::kCellPinInout;
using nsYm::kCellPinInternal;

using nsYm::tCellTimingSense;
using nsYm::kCellPosiUnate;
using nsYm::kCellNegaUnate;
using nsYm::kCellNonUnate;

using nsYm::tCellTimingType;
using nsYm::kCellTimingCombinational;
using nsYm::kCellTimingCombinationalRise;
using nsYm::kCellTimingCombinationalFall;
using nsYm::kCellTimingThreeStateEnable;
using nsYm::kCellTimingThreeStateDisable;
using nsYm::kCellTimingThreeStateEnableRise;
using nsYm::kCellTimingThreeStateEnableFall;
using nsYm::kCellTimingThreeStateDisableRise;
using nsYm::kCellTimingThreeStateDisableFall;
using nsYm::kCellTimingRisingEdge;
using nsYm::kCellTimingFallingEdge;
using nsYm::kCellTimingPreset;
using nsYm::kCellTimingClear;
using nsYm::kCellTimingHoldRising;
using nsYm::kCellTimingHoldFalling;
using nsYm::kCellTimingSetupRising;
using nsYm::kCellTimingSetupFalling;
using nsYm::kCellTimingRecoveryRising;
using nsYm::kCellTimingRecoveryFalling;
using nsYm::kCellTimingSkewRising;
using nsYm::kCellTimingSkewFalling;
using nsYm::kCellTimingRemovalRising;
using nsYm::kCellTimingRemovalFalling;
using nsYm::kCellTimingNonSeqSetupRising;
using nsYm::kCellTimingNonSeqSetupFalling;
using nsYm::kCellTimingNonSeqHoldRising;
using nsYm::kCellTimingNonSeqHoldFalling;
using nsYm::kCellTimingNochangeHighHigh;
using nsYm::kCellTimingNochangeHighLow;
using nsYm::kCellTimingNochangeLowHigh;
using nsYm::kCellTimingNochangeLowLow;

using nsYm::tCellVarType;
using nsYm::kCellVarInputNetTransition;
using nsYm::kCellVarTotalOutputNetCapacitance;
using nsYm::kCellVarOutputNetLength;
using nsYm::kCellVarOutputNetWireCap;
using nsYm::kCellVarOutputNetPinCap;
using nsYm::kCellVarRelatedOutTotalOutputNetCapacitance;
using nsYm::kCellVarRelatedOutOutputNetLength;
using nsYm::kCellVarRelatedOutOutputNetWireCap;
using nsYm::kCellVarRelatedOutOutputNetPinCap;
using nsYm::kCellVarConstrainedPinTransition;
using nsYm::kCellVarRelatedPinTransition;
using nsYm::kCellVarNone;

using nsYm::tCellPatType;
using nsYm::kCellPatInput;
using nsYm::kCellPatAnd;
using nsYm::kCellPatXor;

%End

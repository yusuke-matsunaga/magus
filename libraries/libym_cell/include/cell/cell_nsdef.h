#ifndef CELL_CELL_NSDEF_H
#define CELL_CELL_NSDEF_H

/// @file cell/cell_nsdef.h
/// @brief libym_cell 用の名前空間の定義
///
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup Cell セルライブラリ
///
/// セルライブラリを扱うためのクラスライブラリ

#include "ymtools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief cell の名前空間の開始
#define BEGIN_NAMESPACE_YM_CELL \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsCell)

/// @brief cell の名前空間の終了
#define END_NAMESPACE_YM_CELL \
END_NAMESPACE(nsCell) \
END_NAMESPACE_YM

/// @namespace nsYm::nsCell
/// @brief セルライブラリ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラスの前方参照
//////////////////////////////////////////////////////////////////////

class CellArea;
class CellTime;
class CellCapacitance;
class CellResistance;

class CellLibrary;
class Cell;
class CellPin;
class CellBusType;
class CellBus;
class CellBundle;
class CellTiming;
class CellLutTemplate;
class CellLut;

class CellReader;
class CellMislibReader;
class CellDotlibReader;

class CellDumper;
class CellRestorer;

class CellGroup;
class CellClass;
class CellFFInfo;
class CellLatchInfo;
class CellPatGraph;
class CellPat2Graph;


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
  kCellPosiUnate = 1,
  kCellNegaUnate = 2,
  kCellNonUnate  = 3
};


/////////////////////////////////////////////////////////////////////
/// @brief タイミング情報の型を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tCellTimingType {
  kCellTimingCombinational          = 0,
  kCellTimingCombinationalRise      = 1,
  kCellTimingCombinationalFall      = 2,

  kCellTimingThreeStateEnable       = 3,
  kCellTimingThreeStateDisable      = 4,
  kCellTimingThreeStateEnableRise   = 5,
  kCellTimingThreeStateEnableFall   = 6,
  kCellTimingThreeStateDisableRise  = 7,
  kCellTimingThreeStateDisableFall  = 8,

  kCellTimingRisingEdge             = 9,
  kCellTimingFallingEdge            = 10,

  kCellTimingPreset                 = 11,
  kCellTimingClear                  = 12,

  kCellTimingHoldRising             = 13,
  kCellTimingHoldFalling            = 14,

  kCellTimingSetupRising            = 15,
  kCellTimingSetupFalling           = 16,

  kCellTimingRecoveryRising         = 17,
  kCellTimingRecoveryFalling        = 18,

  kCellTimingSkewRising             = 19,
  kCellTimingSkewFalling            = 20,

  kCellTimingRemovalRising          = 21,
  kCellTimingRemovalFalling         = 22,

  kCellTimingNonSeqSetupRising      = 23,
  kCellTimingNonSeqSetupFalling     = 24,
  kCellTimingNonSeqHoldRising       = 25,
  kCellTimingNonSeqHoldFalling      = 26,

  kCellTimingNochangeHighHigh       = 27,
  kCellTimingNochangeHighLow        = 28,
  kCellTimingNochangeLowHigh        = 29,
  kCellTimingNochangeLowLow         = 30
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
  kCellPatInput = 0,
  kCellPatAnd   = 2,
  kCellPatXor   = 3
};


/// @brief ストリーム出力演算子
/// @param[in] s 出力先のストリーム
/// @param[in] delay_model 遅延モデル
/// @return s を返す．
ostream&
operator<<(ostream& s,
	   tCellDelayModel delay_model);

/// @brief ストリーム出力演算子
/// @param[in] s 出力先のストリーム
/// @param[in] timing_sense タイミングセンス
/// @return s を返す．
ostream&
operator<<(ostream& s,
	   tCellTimingSense timing_sense);


/// @brief ストリーム出力演算子
/// @param[in] s 出力先のストリーム
/// @param[in] timing_type タイミング条件
/// @return s を返す．
ostream&
operator<<(ostream& s,
	   tCellTimingType timing_type);

/// @brief ストリーム出力演算子
/// @param[in] s 出力先のストリーム
/// @param[in] var_type 変数の型
/// @return s を返す．
ostream&
operator<<(ostream& s,
	   tCellVarType var_type);


END_NAMESPACE_YM_CELL


BEGIN_NAMESPACE_YM

using nsCell::CellArea;
using nsCell::CellTime;
using nsCell::CellCapacitance;
using nsCell::CellResistance;

using nsCell::CellLibrary;
using nsCell::Cell;
using nsCell::CellPin;
using nsCell::CellBusType;
using nsCell::CellBus;
using nsCell::CellBundle;
using nsCell::CellTiming;
using nsCell::CellLutTemplate;
using nsCell::CellLut;

using nsCell::CellReader;
using nsCell::CellMislibReader;
using nsCell::CellDotlibReader;

using nsCell::CellDumper;
using nsCell::CellRestorer;

using nsCell::CellGroup;
using nsCell::CellClass;
using nsCell::CellFFInfo;
using nsCell::CellLatchInfo;
using nsCell::CellPatGraph;

using nsCell::tCellTechnology;
using nsCell::kCellTechCmos;
using nsCell::kCellTechFpga;

using nsCell::tCellDelayModel;
using nsCell::kCellDelayGenericCmos;
using nsCell::kCellDelayTableLookup;
using nsCell::kCellDelayPiecewiseCmos;
using nsCell::kCellDelayCmos2;
using nsCell::kCellDelayDcm;

using nsCell::tCellPinDirection;
using nsCell::kCellPinInput;
using nsCell::kCellPinOutput;
using nsCell::kCellPinInout;
using nsCell::kCellPinInternal;

using nsCell::tCellTimingSense;
using nsCell::kCellPosiUnate;
using nsCell::kCellNegaUnate;
using nsCell::kCellNonUnate;

using nsCell::tCellTimingType;
using nsCell::kCellTimingCombinational;
using nsCell::kCellTimingCombinationalRise;
using nsCell::kCellTimingCombinationalFall;
using nsCell::kCellTimingThreeStateEnable;
using nsCell::kCellTimingThreeStateDisable;
using nsCell::kCellTimingThreeStateEnableRise;
using nsCell::kCellTimingThreeStateEnableFall;
using nsCell::kCellTimingThreeStateDisableRise;
using nsCell::kCellTimingThreeStateDisableFall;
using nsCell::kCellTimingRisingEdge;
using nsCell::kCellTimingFallingEdge;
using nsCell::kCellTimingPreset;
using nsCell::kCellTimingClear;
using nsCell::kCellTimingHoldRising;
using nsCell::kCellTimingHoldFalling;
using nsCell::kCellTimingSetupRising;
using nsCell::kCellTimingSetupFalling;
using nsCell::kCellTimingRecoveryRising;
using nsCell::kCellTimingRecoveryFalling;
using nsCell::kCellTimingSkewRising;
using nsCell::kCellTimingSkewFalling;
using nsCell::kCellTimingRemovalRising;
using nsCell::kCellTimingRemovalFalling;
using nsCell::kCellTimingNonSeqSetupRising;
using nsCell::kCellTimingNonSeqSetupFalling;
using nsCell::kCellTimingNonSeqHoldRising;
using nsCell::kCellTimingNonSeqHoldFalling;
using nsCell::kCellTimingNochangeHighHigh;
using nsCell::kCellTimingNochangeHighLow;
using nsCell::kCellTimingNochangeLowHigh;
using nsCell::kCellTimingNochangeLowLow;

using nsCell::tCellVarType;
using nsCell::kCellVarInputNetTransition;
using nsCell::kCellVarTotalOutputNetCapacitance;
using nsCell::kCellVarOutputNetLength;
using nsCell::kCellVarOutputNetWireCap;
using nsCell::kCellVarOutputNetPinCap;
using nsCell::kCellVarRelatedOutTotalOutputNetCapacitance;
using nsCell::kCellVarRelatedOutOutputNetLength;
using nsCell::kCellVarRelatedOutOutputNetWireCap;
using nsCell::kCellVarRelatedOutOutputNetPinCap;
using nsCell::kCellVarConstrainedPinTransition;
using nsCell::kCellVarRelatedPinTransition;
using nsCell::kCellVarNone;

using nsCell::tCellPatType;
using nsCell::kCellPatInput;
using nsCell::kCellPatAnd;
using nsCell::kCellPatXor;

END_NAMESPACE_YM

#endif // CELL_CELL_NSDEF_H

#ifndef YM_CELL_CELL_NSDEF_H
#define YM_CELL_CELL_NSDEF_H

/// @file ym_cell/cell_nsdef.h
/// @brief libym_cell 用の名前空間の定義
///
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup CellGroup セルライブラリ
///
/// セルライブラリを扱うためのクラスライブラリ

#include "ymtools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief ym_cell の名前空間の開始
#define BEGIN_NAMESPACE_YM_CELL \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsCell)

/// @brief ym_cell の名前空間の終了
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
  kVarInputNetTransition,

  kVarTotalOutputNetCapacitance,
  kVarOutputNetLength,
  kVarOutputNetWireCap,
  kVarOutputNetPinCap,

  kVarRelatedOutTotalOutputNetCapacitance,
  kVarRelatedOutOutputNetLength,
  kVarRelatedOutOutputNetWireCap,
  kVarRelatedOutOutputNetPinCap,

  kVarConstrainedPinTransition,

  kVarRelatedPinTransition,

  kVarNone
};


//////////////////////////////////////////////////////////////////////
/// @brief パタングラフのノードの型
//////////////////////////////////////////////////////////////////////
enum tCellPatType {
  kCellPatInput = 0,
  kCellPatAnd   = 2,
  kCellPatXor   = 3
};


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

using nsCell::tCellTimingSense;
using nsCell::tCellTimingType;
using nsCell::tCellPatType;
using nsCell::kCellPatInput;
using nsCell::kCellPatAnd;
using nsCell::kCellPatXor;

END_NAMESPACE_YM

#endif // YM_CELL_CELL_NSDEF_H

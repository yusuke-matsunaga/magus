
/// @file libym_cell/mislib/MislibLibrary.cc
/// @brief MislibLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MislibLibrary.h"
#include "MislibCell.h"
#include "MislibPin.h"
#include "MislibTiming.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス MislibLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibLibrary::MislibLibrary() :
  mAlloc(4096),
  mCellNum(0U),
  mCellArray(NULL)
{
}

// @brief デストラクタ
MislibLibrary::~MislibLibrary()
{
}

// @brief 名前の取得
string
MislibLibrary::name() const
{
  return mName;
}

// @brief テクノロジの取得
tCellTechnology
MislibLibrary::technology() const
{
  return kTechAsic;
}

// @brief 遅延モデルの取得
tCellDelayModel
MislibLibrary::delay_model() const
{
  return kDelayGeneric;
}

// @brief バス命名規則の取得
string
MislibLibrary::bus_naming_style() const
{
  return string();
}

// @brief 日付情報の取得
string
MislibLibrary::date() const
{
  return string();
}

// @brief リビジョン情報の取得
string
MislibLibrary::revision() const
{
  return string();
}

// @brief コメント情報の取得
string
MislibLibrary::comment() const
{
  return string();
}

// @brief 時間単位の取得
string
MislibLibrary::time_unit() const
{
  return "ns";
}

// @brief 電圧単位の取得
string
MislibLibrary::voltage_unit() const
{
  return "V";
}

// @brief 電流単位の取得
string
MislibLibrary::current_unit() const
{
  return "mA";
}

// @brief 抵抗単位の取得
string
MislibLibrary::pulling_resistance_unit() const
{
  return "Ohm";
}

// @brief 容量単位の取得
string
MislibLibrary::capacitive_load_unit() const
{
  return "pF";
}

// @brief 電力単位の取得
string
MislibLibrary::leakage_power_unit() const
{
  return "uW";
}

// @brief ルックアップテーブルのテンプレートの取得
// @param[in] name テンプレート名
// @note なければ NULL を返す．
const CellLutTemplate*
MislibLibrary::lu_table_template(const char* name) const
{
  return NULL;
}

// @brief バスタイプの取得
// @param[in] name バスタイプ名
// @note なければ NULL を返す．
const CellBusType*
MislibLibrary::bus_type(const char* name) const
{
  return NULL;
}

// @brief このライブラリの持つセル数の取得
ymuint
MislibLibrary::cell_num() const
{
  return mCellNum;
}

// @brief セルの取得
// @param[in] pos 位置番号( 0 <= pos < cell_num() )
const Cell*
MislibLibrary::cell(ymuint pos) const
{
  return &mCellArray[pos];
}

// @brief 名前からのセルの取得
const Cell*
MislibLibrary::cell(const char* name) const
{
#warning "TODO: not yet implemented"
  return NULL;
}

// @brief セル数を設定する．
// @param[in] num 設定する値
void
MislibLibrary::set_cell_num(ymuint num)
{
  assert_cond( mCellNum == 0, __FILE__, __LINE__);

  mCellNum = num;

  void* p = mAlloc.get_memory(sizeof(MislibCell) * num);
  mCellArray = new (p) MislibCell[num];
}

// @brief セルの内容を設定する．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
// @param[in] name 名前
// @param[in] area 面積
// @param[in] opin_name 出力ピンの名前
// @param[in] opin_expr 出力ピンの論理式
// @param[in] ni 入力数
void
MislibLibrary::set_cell(ymuint pos,
			ShString name,
			CellArea area,
			ShString opin_name,
			const LogExpr& opin_expr,
			ymuint ni)
{
  MislibCell& cell = mCellArray[pos];

  cell.mName = name;
  cell.mArea = area;

  cell.mInputPinNum = ni;
  void *p = mAlloc.get_memory(sizeof(MislibInputPin) * ni);
  cell.mInputPinArray = new (p) MislibInputPin[ni];

  cell.mOutputPin.mName = opin_name;
  cell.mOutputPin.mFunction = opin_expr;

  void* q = mAlloc.get_memory(sizeof(CellTiming*) * ni * 2);
  cell.mOutputPin.mTimingArray = new (q) CellTiming*[ni * 2];
  for (ymuint i = 0; i < ni; ++ i) {
    cell.mOutputPin.mTimingArray[i * 2 + 0] = NULL;
    cell.mOutputPin.mTimingArray[i * 2 + 1] = NULL;
  }
}

// @brief セルの入力ピンの内容を設定する．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
// @param[in] ipin_pos 入力ピン番号
// @param[in] ipin_name 入力ピン名
// @param[in] ipin_load 入力ピンの負荷容量
void
MislibLibrary::set_cell_ipin(ymuint pos,
			     ymuint ipin_pos,
			     ShString ipin_name,
			     CellCapacitance ipin_load)
{
  MislibCell& cell = mCellArray[pos];
  MislibInputPin& pin = cell.mInputPinArray[ipin_pos];

  pin.mName = ipin_name;
  pin.mCapacitance = ipin_load;
}

// @brief セルのタイミング情報を設定する．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
// @param[in] ipin_pos 入力ピン番号
// @param[in] sense タイミング条件
// @param[in] rise_intrinsic 立ち上がり固有遅延
// @param[in] rise_resistance 立ち上がり負荷依存係数
// @param[in] fall_intrinsic 立ち下がり固有遅延
// @param[in] fall_resistance 立ち下がり負荷依存係数
void
MislibLibrary::set_cell_timing(ymuint pos,
			       ymuint ipin_pos,
			       tCellTimingSense sense,
			       CellTime rise_intrinsic,
			       CellResistance rise_resistance,
			       CellTime fall_intrinsic,
			       CellResistance fall_resistance)
{
  MislibCell& cell = mCellArray[pos];
  MislibOutputPin& pin = cell.mOutputPin;

  void* p = mAlloc.get_memory(sizeof(MislibTiming));
  MislibTiming* timing = new (p) MislibTiming;
  timing->mIntrinsic[0] = rise_intrinsic;
  timing->mIntrinsic[1] = fall_intrinsic;
  timing->mResistance[0] = rise_resistance;
  timing->mResistance[1] = fall_resistance;

  switch ( sense ) {
  case kSensePosiUnate:
    pin.mTimingArray[ipin_pos * 2 + 0] = timing;
    break;

  case kSenseNegaUnate:
    pin.mTimingArray[ipin_pos * 2 + 1] = timing;
    break;

  case kSenseNonUnate:
    pin.mTimingArray[ipin_pos * 2 + 0] = timing;
    pin.mTimingArray[ipin_pos * 2 + 1] = timing;
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

END_NAMESPACE_YM_CELL



/// @file libym_cell/CiLibrary.cc
/// @brief CiLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CiLibrary.h"
#include "CiCell.h"
#include "CiPin.h"
#include "CiTiming.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
CiLibrary::CiLibrary(const string& name) :
  mAlloc(4096),
  mName(name)
{
}

// @brief デストラクタ
CiLibrary::~CiLibrary()
{
}

// @brief 名前の取得
string
CiLibrary::name() const
{
  return mName;
}

// @brief テクノロジの取得
// このクラスは常に kTechAsic を返す．
tCellTechnology
CiLibrary::technology() const
{
  return kTechAsic;
}

// @brief 遅延モデルの取得
// 返り値は
// - kDelayGeneric
// - kDelayPiecewise
// - kDelayNonlinear
// のいずれか
tCellDelayModel
CiLibrary::delay_model() const
{
  return kDelayGeneric;
}

// @brief バス命名規則の取得
string
CiLibrary::bus_naming_style() const
{
  return mBusNamingStyle;
}

// @brief 日付情報の取得
string
CiLibrary::date() const
{
  return mDate;
}

/// @brief リビジョン情報の取得
string
CiLibrary::revision() const
{
  return mRevision;
}

// @brief コメント情報の取得
string
CiLibrary::comment() const
{
  return mComment;
}

// @brief 時間単位の取得
string
CiLibrary::time_unit() const
{
  return mTimeUnit;
}

// @brief 電圧単位の取得
string
CiLibrary::voltage_unit() const
{
  return mVoltageUnit;
}

// @brief 電流単位の取得
string
CiLibrary::current_unit() const
{
  return mCurrentUnit;
}

// @brief 抵抗単位の取得
string
CiLibrary::pulling_resistance_unit() const
{
  return mPullingResistanceUnit;
}

// @brief 容量単位の取得
string
CiLibrary::capacitive_load_unit() const
{
  return mCapacitiveLoadUnit;
}

// @brief 電力単位の取得
string
CiLibrary::leakage_power_unit() const
{
  return mLeakagePowerUnit;
}

// @brief ルックアップテーブルのテンプレートの取得
// @param[in] name テンプレート名
// @note なければ NULL を返す．
const CellLutTemplate*
CiLibrary::lu_table_template(const char* name) const
{
  // 未完
  return NULL;
}

// @brief バスタイプの取得
// @param[in] name バスタイプ名
// @note なければ NULL を返す．
const CellBusType*
CiLibrary::bus_type(const char* name) const
{
  // 未完
  return NULL;
}

// @brief このライブラリの持つセル数の取得
ymuint
CiLibrary::cell_num() const
{
  return mCellNum;
}

// @brief セルの取得
// @param[in] pos 位置番号( 0 <= pos < cell_num() )
const Cell*
CiLibrary::cell(ymuint pos) const
{
  return &mCellArray[pos];
}

// @brief 名前からのセルの取得
const Cell*
CiLibrary::cell(const char* name) const
{
  // 未完
  return NULL;
}

// @brief セル数を設定する．
// @param[in] num 設定する値
void
CiLibrary::set_cell_num(ymuint num)
{
  mCellNum = num;
  void* p = mAlloc.get_memory(sizeof(CiCell) * num);
  mCellArray = new (p) CiCell[num];
}

// @brief セルの内容を設定する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力数
// @param[in] no 出力数
// @param[in] nio 入出力数
// @param[in] nb バス数
// @param[in] nc バンドル数
void
CiLibrary::set_cell(ymuint cell_id,
		    ShString name,
		    CellArea area,
		    ymuint ni,
		    ymuint no,
		    ymuint nio,
		    ymuint nb,
		    ymuint nc)
{
  CiCell& cell = mCellArray[cell_id];
  cell.mName = name;
  cell.mArea = area;

  cell.mInputNum = ni;
  if ( ni > 0 ) {
    void* p = mAlloc.get_memory(sizeof(CiInputPin) * ni);
    cell.mInputArray = new (p) CiInputPin[ni];
  }
  else {
    cell.mInputArray = NULL;
  }

  cell.mOutputNum = no;
  if ( no > 0 ) {
    void* p = mAlloc.get_memory(sizeof(CiOutputPin) * no);
    cell.mOutputArray = new (p) CiOutputPin[no];
  }
  else {
    cell.mOutputArray = NULL;
  }

  cell.mInoutNum = nio;
  if ( nio > 0 ) {
    void* p = mAlloc.get_memory(sizeof(CiInoutPin) * nio);
    cell.mInoutArray = new (p) CiInoutPin[nio];
  }
  else {
    cell.mInoutArray = NULL;
  }

  ymuint pin_num = ni + no + nio;
  assert_cond( pin_num > 0, __FILE__, __LINE__);
  cell.mPinNum = pin_num;
  void* q = mAlloc.get_memory(sizeof(CiPin*) * pin_num);
  cell.mPinArray = new (q) CiPin*[pin_num];

  for (ymuint i = 0; i < no; ++ i) {
    CiOutputPin& pin = cell.mOutputArray[i];
    void* p = mAlloc.get_memory(sizeof(const CellTiming*) * pin_num * 2);
    pin.mTimingArray = new (p) const CellTiming*[pin_num * 2];
    for (ymuint j = 0; j < pin_num; ++ j) {
      pin.mTimingArray[j * 2 + 0] = NULL;
      pin.mTimingArray[j * 2 + 1] = NULL;
    }
  }

  for (ymuint i = 0; i < nio; ++ i) {
    CiOutputPin& pin = cell.mInoutArray[i];
    void* p = mAlloc.get_memory(sizeof(const CellTiming*) * pin_num * 2);
    pin.mTimingArray = new (p) const CellTiming*[pin_num * 2];
    for (ymuint j = 0; j < pin_num; ++ j) {
      pin.mTimingArray[j * 2 + 0] = NULL;
      pin.mTimingArray[j * 2 + 1] = NULL;
    }
  }

  // バス，バンドル関係は未完
}

// @brief セルの入力ピンの内容を設定する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] ipin_id 入力ピン番号 ( 0 <= ipin_id < cell->input_num() )
// @param[in] pin_name 入力ピン名
// @param[in] pin_capacitance 入力ピンの負荷容量
// @param[in] pin_rise_capacitance 入力ピンの立ち上がり負荷容量
// @param[in] pin_fall_capacitance 入力ピンの立ち下がり負荷容量
void
CiLibrary::set_cell_input(ymuint cell_id,
			  ymuint pin_id,
			  ymuint ipin_id,
			  ShString pin_name,
			  CellCapacitance pin_capacitance,
			  CellCapacitance pin_rise_capacitance,
			  CellCapacitance pin_fall_capacitance)
{
  CiCell& cell = mCellArray[cell_id];
  CiInputPin& pin = cell.mInputArray[ipin_id];
  cell.mPinArray[pin_id] = &pin;
  pin.mId = pin_id;
  pin.mName = pin_name;
  pin.mCapacitance = pin_capacitance;
  pin.mRiseCapacitance = pin_rise_capacitance;
  pin.mFallCapacitance = pin_fall_capacitance;
}

// @brief セルの出力ピンの内容を設定する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] opin_id 出力力ピン番号 ( 0 <= opin_id < cell->output_num() )
// @param[in] pin_name 出力ピン名
void
CiLibrary::set_cell_output(ymuint cell_id,
			   ymuint pin_id,
			   ymuint opin_id,
			   ShString pin_name)
{
  CiCell& cell = mCellArray[cell_id];
  CiOutputPin& pin = cell.mOutputArray[opin_id];
  cell.mPinArray[pin_id] = &pin;
  pin.mId = pin_id;
  pin.mName = pin_name;
}

// @brief セルの入出力ピンの内容を設定する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] iopin_id 入出力力ピン番号 ( 0 <= iopin_id < cell->inout_num() )
// @param[in] pin_name 入出力ピン名
// @param[in] pin_capacitance 入力ピンの負荷容量
// @param[in] pin_rise_capacitance 入力ピンの立ち上がり負荷容量
// @param[in] pin_fall_capacitance 入力ピンの立ち下がり負荷容量
void
CiLibrary::set_cell_inout(ymuint cell_id,
			  ymuint pin_id,
			  ymuint iopin_id,
			  ShString pin_name,
			  CellCapacitance pin_capacitance,
			  CellCapacitance pin_rise_capacitance,
			  CellCapacitance pin_fall_capacitance)
{
  CiCell& cell = mCellArray[cell_id];
  CiInoutPin& pin = cell.mInoutArray[iopin_id];
  cell.mPinArray[pin_id] = &pin;
  pin.mId = pin_id;
  pin.mName = pin_name;
  pin.mCapacitance = pin_capacitance;
  pin.mRiseCapacitance = pin_rise_capacitance;
  pin.mFallCapacitance = pin_fall_capacitance;
}

// @brief セルの出力ピン(入出力ピン)の機能を設定する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] pin_id 出力(入出力)ピン番号
// @param[in] function 関数を表す論理式
void
CiLibrary::set_opin_function(ymuint cell_id,
			     ymuint pin_id,
			     const LogExpr& function)
{
  CiCell& cell = mCellArray[cell_id];
  CiPin* pin = cell.mPinArray[pin_id];
  pin->set_function(function);
}

// @brief セルのタイミング情報を設定する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] pin_id 出力(入出力)ピン番号
// @param[in] ipin_list 関連する入力(入出力)ピン番号のリスト
// @param[in] type タイミングの型
// @param[in] sense タイミング条件
// @param[in] intrinsic_rise 立ち上がり固有遅延
// @param[in] intrinsic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
// @param[in] rise_resistance 立ち上がり負荷依存係数
// @param[in] fall_resistance 立ち下がり負荷依存係数
// @note opin は入出力ピンの場合もある．
void
CiLibrary::set_opin_timing(ymuint cell_id,
			   ymuint pin_id,
			   const vector<ymuint>& ipin_list,
			   tCellTimingType type,
			   tCellTimingSense sense,
			   CellTime intrinsic_rise,
			   CellTime intrinsic_fall,
			   CellTime slope_rise,
			   CellTime slope_fall,
			   CellResistance rise_resistance,
			   CellResistance fall_resistance)
{
  CiCell& cell = mCellArray[cell_id];
  CiPin* pin = cell.mPinArray[pin_id];

  void* p = mAlloc.get_memory(sizeof(CiTimingGeneric));
  CellTiming* timing = new (p) CiTimingGeneric(type,
					       intrinsic_rise,
					       intrinsic_fall,
					       slope_rise,
					       slope_fall,
					       rise_resistance,
					       fall_resistance);

  for (vector<ymuint>::const_iterator p = ipin_list.begin();
       p != ipin_list.end(); ++ p) {
    ymuint ipin_id = *p;
    pin->set_timing(ipin_id, sense, timing);
  }
}

END_NAMESPACE_YM_CELL

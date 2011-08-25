
/// @file CiLibrary.cc
/// @brief CiLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
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
CellLibrary::tTechnology
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
CellLibrary::tDelayModel
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
  return mCellArray[pos];
}

// @brief 名前からのセルの取得
const Cell*
CiLibrary::cell(const char* name) const
{
  // 未完
  return NULL;
}

// @brief 属性を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
void
CiLibrary::set_attr(const string& attr_name,
		    const string& value)
{
  if ( attr_name == "bus_naming_style" ) {
    mBusNamingStyle = value;
  }
  else if ( attr_name == "date" ) {
    mDate = value;
  }
  else if ( attr_name == "revision" ) {
    mRevision = value;
  }
  else if ( attr_name == "comment" ) {
    mComment = value;
  }
  else if ( attr_name == "time_unit" ) {
    mTimeUnit = value;
  }
  else if ( attr_name == "voltage_unit" ) {
    mVoltageUnit = value;
  }
  else if ( attr_name == "current_unit" ) {
    mCurrentUnit = value;
  }
  else if ( attr_name == "pulling_resistance_unit" ) {
    mPullingResistanceUnit = value;
  }
  else if ( attr_name == "capacitiveLoad_unit" ) {
    mCapacitiveLoadUnit = value;
  }
  else if ( attr_name == "leakage_power_unit" ) {
    mLeakagePowerUnit = value;
  }
}

// @brief セル数を設定する．
// @param[in] num 設定する値
void
CiLibrary::set_cell_num(ymuint num)
{
  mCellNum = num;
  void* p = mAlloc.get_memory(sizeof(CiCell*) * num);
  mCellArray = new (p) CiCell*[num];
}

// @brief 論理セルを生成する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @return セルへのポインタを返す．
CiCell*
CiLibrary::new_logic_cell(ymuint cell_id,
			  ShString name,
			  CellArea area,
			  ymuint ni,
			  ymuint no,
			  ymuint nio,
			  ymuint nb,
			  ymuint nc)
{
  void* p = mAlloc.get_memory(sizeof(CiLogicCell));
  CiCell* cell = new (p) CiLogicCell(cell_id, name, area);
  mCellArray[cell_id] = cell;

  set_pinnum(cell, ni, no, nio, nb, nc);

  return cell;
}

// @brief FFセルを生成する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] var1, var2 状態変数名
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] clocked_on_also "clocked_on_also" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
// @param[in] clear_preset_var1 "clear_preset_var1" の値
// @param[in] clear_preset_var2 "clear_preset_var2" の値
// @return セルへのポインタを返す．
CiCell*
CiLibrary::new_ff_cell(ymuint cell_id,
		       ShString name,
		       CellArea area,
		       ymuint ni,
		       ymuint no,
		       ymuint nio,
		       ymuint nb,
		       ymuint nc,
		       const ShString& var1,
		       const ShString& var2,
		       const LogExpr& next_state,
		       const LogExpr& clocked_on,
		       const LogExpr& clocked_on_also,
		       const LogExpr& clear,
		       const LogExpr& preset,
		       ymuint clear_preset_var1,
		       ymuint clear_preset_var2)
{
  void* p = mAlloc.get_memory(sizeof(CiFFCell));
  CiCell* cell = new (p) CiFFCell(cell_id, name, area,
				  var1, var2,
				  next_state, clocked_on, clocked_on_also,
				  clear, preset,
				  clear_preset_var1,
				  clear_preset_var2);
  mCellArray[cell_id] = cell;

  set_pinnum(cell, ni, no, nio, nb, nc);

  return cell;
}

// @brief ラッチセルを生成する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] var1, var2 状態変数名
// @param[in] data_in "data_in" 関数の式
// @param[in] enable "enable" 関数の式
// @param[in] enable_also "enable_also" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
// @param[in] clear_preset_var1 "clear_preset_var1" の値
// @param[in] clear_preset_var2 "clear_preset_var2" の値
// @return セルへのポインタを返す．
CiCell*
CiLibrary::new_latch_cell(ymuint cell_id,
			  ShString name,
			  CellArea area,
			  ymuint ni,
			  ymuint no,
			  ymuint nio,
			  ymuint nb,
			  ymuint nc,
			  const ShString& var1,
			  const ShString& var2,
			  const LogExpr& data_in,
			  const LogExpr& enable,
			  const LogExpr& enable_also,
			  const LogExpr& clear,
			  const LogExpr& preset,
			  ymuint clear_preset_var1,
			  ymuint clear_preset_var2)
{
  void* p = mAlloc.get_memory(sizeof(CiLatchCell));
  CiCell* cell = new (p) CiLatchCell(cell_id, name, area,
				     var1, var2,
				     data_in, enable, enable_also,
				     clear, preset,
				     clear_preset_var1,
				     clear_preset_var2);
  mCellArray[cell_id] = cell;

  set_pinnum(cell, ni, no, nio, nb, nc);

  return cell;
}

// @brief セルにピン数，バス数，バンドル数の設定をする．
// @param[in] cell セル
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
void
CiLibrary::set_pinnum(CiCell* cell,
		      ymuint ni,
		      ymuint no,
		      ymuint nio,
		      ymuint nb,
		      ymuint nc)
{
  cell->mInputNum = ni;
  void* p = mAlloc.get_memory(sizeof(CiPin*) * ni);
  cell->mInputArray = new (p) CiPin*[ni];

  cell->mOutputNum = no;
  void* q = mAlloc.get_memory(sizeof(CiPin*) * no);
  cell->mOutputArray = new (q) CiPin*[no];

  cell->mInoutNum = nio;
  void* r = mAlloc.get_memory(sizeof(CiPin*) * nio);
  cell->mInoutArray = new (r) CiPin*[nio];

  ymuint n = (ni + nio) * (no + nio) * 2;
  void* s = mAlloc.get_memory(sizeof(const CiTiming*) * n);
  cell->mTimingArray = new (s) CiTiming*[n];
  for (ymuint i = 0; i < n; ++ i) {
    cell->mTimingArray[i] = NULL;
  }

  // バス，バンドル関係は未完

  cell->mBusNum = nb;

  cell->mBundleNum = nc;
}

// @brief セルの入力ピンの内容を設定する．
// @param[in] cell セル
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] name 入力ピン名
// @param[in] capacitance 入力ピンの負荷容量
// @param[in] rise_capacitance 入力ピンの立ち上がり負荷容量
// @param[in] fall_capacitance 入力ピンの立ち下がり負荷容量
void
CiLibrary::new_cell_input(CiCell* cell,
			  ymuint pin_id,
			  ShString name,
			  CellCapacitance capacitance,
			  CellCapacitance rise_capacitance,
			  CellCapacitance fall_capacitance)
{
  void* p = mAlloc.get_memory(sizeof(CiInputPin));
  CiInputPin* pin = new (p) CiInputPin(name, capacitance,
				       rise_capacitance, fall_capacitance);
  cell->mInputArray[pin_id] = pin;
  pin->mId = pin_id;
}

// @brief セルの出力ピンの内容を設定する．
// @param[in] cell セル
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] name 出力ピン名
// @param[in] max_fanout 最大ファンアウト容量
// @param[in] min_fanout 最小ファンアウト容量
// @param[in] max_capacitance 最大負荷容量
// @param[in] min_capacitance 最小負荷容量
// @param[in] max_transition 最大遷移時間
// @param[in] min_transition 最小遷移時間
void
CiLibrary::new_cell_output(CiCell* cell,
			   ymuint pin_id,
			   ShString name,
			   CellCapacitance max_fanout,
			   CellCapacitance min_fanout,
			   CellCapacitance max_capacitance,
			   CellCapacitance min_capacitance,
			   CellTime max_transition,
			   CellTime min_transition)
{
  void* p = mAlloc.get_memory(sizeof(CiOutputPin));
  CiOutputPin* pin = new (p) CiOutputPin(name, max_fanout, min_fanout,
					 max_capacitance, min_capacitance,
					 max_transition, min_transition);
  cell->mOutputArray[pin_id] = pin;
  pin->mId = pin_id;
}

// @brief セルの入出力ピンの内容を設定する．
// @param[in] cell セル
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] name 入出力ピン名
// @param[in] capacitance 入力ピンの負荷容量
// @param[in] rise_capacitance 入力ピンの立ち上がり負荷容量
// @param[in] fall_capacitance 入力ピンの立ち下がり負荷容量
// @param[in] max_fanout 最大ファンアウト容量
// @param[in] min_fanout 最小ファンアウト容量
// @param[in] max_capacitance 最大負荷容量
// @param[in] min_capacitance 最小負荷容量
// @param[in] max_transition 最大遷移時間
// @param[in] min_transition 最小遷移時間
void
CiLibrary::new_cell_inout(CiCell* cell,
			  ymuint pin_id,
			  ShString name,
			  CellCapacitance capacitance,
			  CellCapacitance rise_capacitance,
			  CellCapacitance fall_capacitance,
			  CellCapacitance max_fanout,
			  CellCapacitance min_fanout,
			  CellCapacitance max_capacitance,
			  CellCapacitance min_capacitance,
			  CellTime max_transition,
			  CellTime min_transition)
{
  void* p = mAlloc.get_memory(sizeof(CiInoutPin));
  CiInoutPin* pin =  new (p) CiInoutPin(name, capacitance,
					rise_capacitance, fall_capacitance,
					max_fanout, min_fanout,
					max_capacitance, min_capacitance,
					max_transition, min_transition);
  cell->mInputArray[pin_id + cell->input_num()] = pin;
  cell->mOutputArray[pin_id + cell->output_num()] = pin;
  cell->mInoutArray[pin_id] = pin;
  pin->mId = pin_id;
}

#if 0
// @brief セルの内部ピンを生成する．
// @param[in] cell セル
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] name 内部ピン名
void
CiLibrary::new_cell_internal(CiCell* cell,
			     ymuint pin_id,
			     ShString name)
{
  void* p = mAlloc.get_memory(sizeof(CiInternalPin));
  CiPin* pin = new (p) CiInternalPin(name);
  cell->mPinArray[pin_id] = pin;
  pin->mId = pin_id;
}
#endif

// @brief タイミング情報を作る．
// @param[in] id ID番号
// @param[in] type タイミングの型
// @param[in] intrinsic_rise 立ち上がり固有遅延
// @param[in] intrinsic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
// @param[in] rise_resistance 立ち上がり負荷依存係数
// @param[in] fall_resistance 立ち下がり負荷依存係数
CellTiming*
CiLibrary::new_timing(ymuint id,
		      CellTiming::tType type,
		      CellTime intrinsic_rise,
		      CellTime intrinsic_fall,
		      CellTime slope_rise,
		      CellTime slope_fall,
		      CellResistance rise_resistance,
		      CellResistance fall_resistance)
{
  void* p = mAlloc.get_memory(sizeof(CiTimingGeneric));
  CellTiming* timing = new (p) CiTimingGeneric(id, type,
					       intrinsic_rise,
					       intrinsic_fall,
					       slope_rise,
					       slope_fall,
					       rise_resistance,
					       fall_resistance);
  return timing;
}

// @brief セルのタイミング情報を設定する．
// @param[in] cell セル
// @param[in] opin_id 出力(入出力)ピン番号
// @param[in] ipin_id 関連する入力(入出力)ピン番号
// @param[in] sense タイミング条件
// @param[in] timing 設定するタイミング情報
void
CiLibrary::set_cell_timing(CiCell* cell,
			   ymuint opin_id,
			   ymuint ipin_id,
			   tCellTimingSense sense,
			   CiTiming* timing)
{
  ymuint base = opin_id * (cell->input_num() + cell->inout_num());
  switch ( sense ) {
  case kCellPosiUnate:
    cell->mTimingArray[base + 0] = timing;
    break;

  case kCellNegaUnate:
    cell->mTimingArray[base + 1] = timing;
    break;

  case kCellNonUnate:
    cell->mTimingArray[base + 0] = timing;
    cell->mTimingArray[base + 1] = timing;
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief 出力ピンの機能を設定する．
// @param[in] cell セル
// @param[in] opin_id 出力(入出力)ピン番号
// @param[in] pin 出力(入出力)ピン
// @param[in] function 機能を表す論理式
void
CiLibrary::set_opin_function(CiCell* cell,
			     ymuint opin_id,
			     const LogExpr& function)
{
  CiPin* pin = cell->mOutputArray[opin_id];
  pin->set_function(function);
}

// @brief 出力ピンの three_state 条件を設定する．
// @param[in] cell セル
// @param[in] opin_id 出力(入出力)ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] expr three_state 条件を表す論理式
void
CiLibrary::set_opin_three_state(CiCell* cell,
				ymuint opin_id,
				const LogExpr& expr)
{
  CiPin* pin = cell->mOutputArray[opin_id];
  pin->set_three_state(expr);
}

END_NAMESPACE_YM_CELL

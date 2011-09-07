
/// @file CiLibrary.cc
/// @brief CiLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiLibrary.h"
#include "CiClass.h"
#include "CiGroup.h"
#include "CiCell.h"
#include "CiLogicCell.h"
#include "CiTriCell.h"
#include "CiFFCell.h"
#include "CiLatchCell.h"
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

// @brief 論理セルクラスの個数を返す．
ymuint
CiLibrary::logic_class_num() const
{
  return mLogicClassNum;
}

// @brief 論理セルクラスを返す．
// @param[in] id クラス番号 ( 0 <= id < logic_class_num() )
const CellClass*
CiLibrary::logic_class(ymuint id) const
{
  return mLogicClassArray[id];
}

// @brief 論理セルグループの個数を返す．
ymuint
CiLibrary::logic_group_num() const
{
  return mLogicGroupNum;
}

// @brief 論理セルグループを返す．
// @param[in] id グループ番号　( 0 <= id < logic_group_num() )
const CellGroup*
CiLibrary::logic_group(ymuint id) const
{
  return mLogicGroupArray[id];
}

// @brief 定数0セルのグループを返す．
const CellGroup*
CiLibrary::const0_func() const
{
  // 決め打ち
  return mLogicGroupArray[0];
}

// @brief 定数1セルのグループを返す．
const CellGroup*
CiLibrary::const1_func() const
{
  return mLogicGroupArray[1];
}

// @brief バッファセルのグループを返す．
const CellGroup*
CiLibrary::buf_func() const
{
  return mLogicGroupArray[2];
}

// @brief インバータセルのグループを返す．
const CellGroup*
CiLibrary::inv_func() const
{
  return mLogicGroupArray[3];
}

// @brief FFセルクラスの個数を返す．
ymuint
CiLibrary::ff_class_num() const
{
  return mFFClassNum;
}

// @brief FFセルクラスを返す．
// @param[in] id クラス番号 ( 0 <= id < ff_class_num() )
const CellClass*
CiLibrary::ff_class(ymuint id) const
{
  return mFFClassArray[id];
}

// @brief FFセルグループの個数を返す．
ymuint
CiLibrary::ff_group_num() const
{
  return mFFGroupNum;
}

// @brief FFセルグループを返す．
// @param[in] id グループ番号 ( 0 <= id < ff_group_num() )
const CellGroup*
CiLibrary::ff_group(ymuint id) const
{
  return mFFGroupArray[id];
}

// @brief 単純な型のFFクラスを返す．
// @param[in] has_clear クリア端子を持つとき true にする．
// @param[in] has_preset プリセット端子を持つとき true にする．
// @note 該当するセルがないときでも空のセルクラスが返される．
const CellClass*
CiLibrary::simple_ff_class(bool has_clear,
			   bool has_preset) const
{
  ymuint pos = 0;
  if ( has_clear ) {
    pos += 1;
  }
  if ( has_preset ) {
    pos += 2;
  }
  return mFFClassArray[pos];
}

// @brief ラッチセルクラスの個数を返す．
ymuint
CiLibrary::latch_class_num() const
{
  return mLatchClassNum;
}

// @brief ラッチセルクラスを返す．
// @param[in] id クラス番号 ( 0 <= id < latch_class_num() )
const CellClass*
CiLibrary::latch_class(ymuint id) const
{
  return mLatchClassArray[id];
}

// @brief ラッチセルグループの個数を返す．
ymuint
CiLibrary::latch_group_num() const
{
  return mLatchGroupNum;
}

// @brief ラッチセルグループを返す．
// @param[in] id グループ番号 ( 0 <= id < latch_group_num() )
const CellGroup*
CiLibrary::latch_group(ymuint id) const
{
  return mLatchGroupArray[id];
}

// @brief 単純な型のラッチクラスを返す．
// @param[in] has_clear クリア端子を持つとき true にする．
// @param[in] has_preset プリセット端子を持つとき true にする．
// @note 該当するセルがないときでも空のセルクラスが返される．
const CellClass*
CiLibrary::simple_latch_class(bool has_clear,
			      bool has_preset) const
{
  ymuint pos = 0;
  if ( has_clear ) {
    pos += 1;
  }
  if ( has_preset ) {
    pos += 2;
  }
  return mLatchClassArray[pos];
}

// @brief 総パタン数を返す．
ymuint
CiLibrary::pat_num() const
{
  return mPatMgr.pat_num();
}

// @brief パタンを返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
const CellPatGraph&
CiLibrary::pat(ymuint id) const
{
  return mPatMgr.pat(id);
}

// @brief パタンの最大の入力数を得る．
ymuint
CiLibrary::max_input() const
{
  return mPatMgr.max_input();
}

// @brief 総ノード数を返す．
ymuint
CiLibrary::node_num() const
{
  return mPatMgr.node_num();
}

// @brief ノードの種類を返す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
tCellPatType
CiLibrary::node_type(ymuint id) const
{
  return mPatMgr.node_type(id);
}

// @brief ノードが入力ノードの時に入力番号を返す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
// @note 入力ノードでない場合の返り値は不定
ymuint
CiLibrary::input_id(ymuint id) const
{
  return mPatMgr.input_id(id);
}

// @brief 入力のノード番号を返す．
// @param[in] input_id 入力番号 ( 0 <= input_id < input_num() )
// @return input_id の入力に対応するノードのノード番号
ymuint
CiLibrary::input_node(ymuint input_id) const
{
  return mPatMgr.input_node(input_id);
}

// @brief 総枝数を返す．
ymuint
CiLibrary::edge_num() const
{
  return mPatMgr.edge_num();
}

// @brief 枝のファンイン元のノード番号を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
ymuint
CiLibrary::edge_from(ymuint id) const
{
  return mPatMgr.edge_from(id);
}

// @brief 枝のファンアウト先のノード番号を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
ymuint
CiLibrary::edge_to(ymint id) const
{
  return mPatMgr.edge_to(id);
}

// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
ymuint
CiLibrary::edge_pos(ymuint id) const
{
  return mPatMgr.edge_pos(id);
}

// @brief 枝の反転属性を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
bool
CiLibrary::edge_inv(ymuint id) const
{
  return mPatMgr.edge_inv(id);
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
// @param[in] logic_array 出力の論理式の配列
// @return セルへのポインタを返す．
CiCell*
CiLibrary::new_logic_cell(ymuint cell_id,
			  ShString name,
			  CellArea area,
			  ymuint ni,
			  ymuint no,
			  ymuint nio,
			  ymuint nb,
			  ymuint nc,
			  const vector<LogExpr>& logic_array)
{
  void* p = mAlloc.get_memory(sizeof(CiLogicCell));
  CiCell* cell = new (p) CiLogicCell(cell_id, name, area,
				     ni, no, nio, nb, nc,
				     mAlloc,
				     logic_array);
  mCellArray[cell_id] = cell;

  return cell;
}

// @brief トライステートセルを生成する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristate_array トライステート条件の論理式の配列
// @return セルへのポインタを返す．
CiCell*
CiLibrary::new_tristate_cell(ymuint cell_id,
			     ShString name,
			     CellArea area,
			     ymuint ni,
			     ymuint no,
			     ymuint nio,
			     ymuint nb,
			     ymuint nc,
			     const vector<LogExpr>& logic_array,
			     const vector<LogExpr>& tristate_array)
{
  void* p = mAlloc.get_memory(sizeof(CiTriCell));
  CiCell* cell = new (p) CiTriCell(cell_id, name, area,
				   ni, no, nio, nb, nc,
				   mAlloc,
				   logic_array, tristate_array);
  mCellArray[cell_id] = cell;

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
// @param[in] logic_array 出力の論理式の配列
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
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
		       const vector<LogExpr>& logic_array,
		       const LogExpr& next_state,
		       const LogExpr& clocked_on,
		       const LogExpr& clear,
		       const LogExpr& preset)
{
  bool has_clear = !clear.is_zero();
  bool has_preset = !preset.is_zero();

  CiCell* cell = NULL;
  if ( has_clear ) {
    if ( has_preset ) {
      void* p = mAlloc.get_memory(sizeof(CiFFSRCell));
      cell = new (p) CiFFSRCell(cell_id, name, area,
				ni, no, nio, nb, nc,
				mAlloc,
				logic_array,
				next_state,
				clocked_on,
				clear,
				preset);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(CiFFRCell));
      cell = new (p) CiFFRCell(cell_id, name, area,
			       ni, no, nio, nb, nc,
			       mAlloc,
			       logic_array,
			       next_state,
			       clocked_on,
			       clear);
    }
  }
  else {
    if ( has_preset ) {
      void* p = mAlloc.get_memory(sizeof(CiFFSCell));
      cell = new (p) CiFFSCell(cell_id, name, area,
			       ni, no, nio, nb, nc,
			       mAlloc,
			       logic_array,
			       next_state,
			       clocked_on,
			       preset);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(CiFFCell));
      cell = new (p) CiFFCell(cell_id, name, area,
			      ni, no, nio, nb, nc,
			      mAlloc,
			      logic_array,
			      next_state,
			      clocked_on);
    }

  }
  mCellArray[cell_id] = cell;

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
/// @param[in] logic_array 出力の論理式の配列
// @param[in] data_in "data_in" 関数の式
// @param[in] enable "enable" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
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
			  const vector<LogExpr>& logic_array,
			  const LogExpr& data_in,
			  const LogExpr& enable,
			  const LogExpr& clear,
			  const LogExpr& preset)
{
  bool has_clear = !clear.is_zero();
  bool has_preset = !preset.is_zero();

  CiCell* cell = NULL;
  if ( has_clear ) {
    if ( has_preset ) {
      void* p = mAlloc.get_memory(sizeof(CiLatchSRCell));
      cell = new (p) CiLatchSRCell(cell_id, name, area,
				   ni, no, nio, nb, nc,
				   mAlloc,
				   logic_array,
				   data_in,
				   enable,
				   clear,
				   preset);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(CiLatchRCell));
      cell = new (p) CiLatchRCell(cell_id, name, area,
				  ni, no, nio, nb, nc,
				  mAlloc,
				  logic_array,
				  data_in,
				  enable,
				  clear);
    }
  }
  else {
    if ( has_preset ) {
      void* p = mAlloc.get_memory(sizeof(CiLatchSCell));
      cell = new (p) CiLatchSCell(cell_id, name, area,
				  ni, no, nio, nb, nc,
				  mAlloc,
				  logic_array,
				  data_in,
				  enable,
				  preset);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(CiLatchCell));
      cell = new (p) CiLatchCell(cell_id, name, area,
				 ni, no, nio, nb, nc,
				 mAlloc,
				 logic_array,
				 data_in,
				 enable);
    }

  }
  mCellArray[cell_id] = cell;

  return cell;
}

#if 0
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

  cell->set_pinnum(ni, no, nio, nb, nc, mAlloc);

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

  cell->set_pinnum(ni, no, nio, nb, nc, mAlloc);

  return cell;
}
#endif

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
CiTiming*
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
  CiTiming* timing = new (p) CiTimingGeneric(id, type,
					     intrinsic_rise,
					     intrinsic_fall,
					     slope_rise,
					     slope_fall,
					     rise_resistance,
					     fall_resistance);
  return timing;
}

END_NAMESPACE_YM_CELL

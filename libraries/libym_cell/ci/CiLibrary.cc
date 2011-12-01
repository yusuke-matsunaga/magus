
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
#include "CiFFCell.h"
#include "CiLatchCell.h"
#include "CiFsmCell.h"
#include "CiPin.h"
#include "CiTiming.h"

#include "libcomp/LibComp.h"
#include "libcomp/LcGroupMgr.h"
#include "libcomp/LcClass.h"
#include "libcomp/LcGroup.h"
#include "libcomp/LcPatMgr.h"


BEGIN_NAMESPACE_YM_CELL

using namespace nsLibcomp;

//////////////////////////////////////////////////////////////////////
// クラス CellLibrary
//////////////////////////////////////////////////////////////////////

// @brief 実際のオブジェクトを作るクラスメソッド
CellLibrary*
CellLibrary::new_obj()
{
  return new CiLibrary();
}


//////////////////////////////////////////////////////////////////////
// クラス CiLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLibrary::CiLibrary() :
  mAlloc(4096)
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
  assert_cond( pos < cell_num(), __FILE__, __LINE__);
  return mCellArray[pos];
}

// @brief 名前からのセルの取得
const Cell*
CiLibrary::cell(const char* name) const
{
  return mCellHash.get(ShString(name));
}

// @brief 名前からのセルの取得
const Cell*
CiLibrary::cell(const string& name) const
{
  return mCellHash.get(ShString(name));
}

// @brief セルグループの個数を返す．
ymuint
CiLibrary::group_num() const
{
  return mGroupNum;
}

// @brief セルグループを返す．
// @param[in] id グループ番号　( 0 <= id < group_num() )
const CellGroup*
CiLibrary::group(ymuint id) const
{
  assert_cond( id < group_num(), __FILE__, __LINE__);
  return &mGroupArray[id];
}

// @brief NPN同値クラスの個数を返す．
ymuint
CiLibrary::npn_class_num() const
{
  return mClassNum;
}

// @brief NPN同値クラスを返す．
// @param[in] id クラス番号 ( 0 <= id < npn_class_num() )
const CellClass*
CiLibrary::npn_class(ymuint id) const
{
  assert_cond( id < npn_class_num(), __FILE__, __LINE__);
  return &mClassArray[id];
}

// @brief 定数0セルのグループを返す．
const CellGroup*
CiLibrary::const0_func() const
{
  // 決め打ち
  return mLogicGroup[0];
}

// @brief 定数1セルのグループを返す．
const CellGroup*
CiLibrary::const1_func() const
{
  // 決め打ち
  return mLogicGroup[1];
}

// @brief バッファセルのグループを返す．
const CellGroup*
CiLibrary::buf_func() const
{
  // 決め打ち
  return mLogicGroup[2];
}

// @brief インバータセルのグループを返す．
const CellGroup*
CiLibrary::inv_func() const
{
  // 決め打ち
  return mLogicGroup[3];
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
  return mFFClass[pos];
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
  return mLatchClass[pos];
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

// @brief 名前を設定する．
// @param[in] name 名前
void
CiLibrary::set_name(const string& name)
{
  mName = name;
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
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristate_array トライステート条件の論理式の配列
// *1: - false 論理式なし
//     - true 論理式あり
void
CiLibrary::new_logic_cell(ymuint cell_id,
			  const string& name,
			  CellArea area,
			  ymuint ni,
			  ymuint no,
			  ymuint nio,
			  ymuint nb,
			  ymuint nc,
			  const vector<bool>& output_array,
			  const vector<LogExpr>& logic_array,
			  const vector<LogExpr>& tristate_array)
{
  void* p = mAlloc.get_memory(sizeof(CiLogicCell));
  CiCell* cell = new (p) CiLogicCell(this, cell_id, ShString(name), area,
				     ni, no, nio, nb, nc,
				     output_array,
				     logic_array,
				     tristate_array,
				     mAlloc);
  add_cell(cell_id, cell);
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
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristate_array トライステート条件の論理式の配列
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] clocked_on_also "clocked_on_also" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
// *1: - false 論理式なし
//     - true 論理式あり
void
CiLibrary::new_ff_cell(ymuint cell_id,
		       const string& name,
		       CellArea area,
		       ymuint ni,
		       ymuint no,
		       ymuint nio,
		       ymuint nb,
		       ymuint nc,
		       const vector<bool>& output_array,
		       const vector<LogExpr>& logic_array,
		       const vector<LogExpr>& tristate_array,
		       const LogExpr& next_state,
		       const LogExpr& clocked_on,
		       const LogExpr& clocked_on_also,
		       const LogExpr& clear,
		       const LogExpr& preset,
		       ymuint clear_preset_var1,
		       ymuint clear_preset_var2)
{
  bool has_clear = !clear.is_zero();
  bool has_preset = !preset.is_zero();

  ShString shname(name);

  CiCell* cell = NULL;
  if ( has_clear ) {
    if ( has_preset ) {
      void* p = mAlloc.get_memory(sizeof(CiFFSRCell));
      cell = new (p) CiFFSRCell(this, cell_id, shname, area,
				ni, no, nio, nb, nc,
				output_array,
				logic_array,
				tristate_array,
				next_state,
				clocked_on,
				clocked_on_also,
				clear,
				preset,
				clear_preset_var1,
				clear_preset_var2,
				mAlloc);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(CiFFRCell));
      cell = new (p) CiFFRCell(this, cell_id, shname, area,
			       ni, no, nio, nb, nc,
			       output_array,
			       logic_array,
			       tristate_array,
			       next_state,
			       clocked_on,
			       clocked_on_also,
			       clear,
			       mAlloc);
    }
  }
  else {
    if ( has_preset ) {
      void* p = mAlloc.get_memory(sizeof(CiFFSCell));
      cell = new (p) CiFFSCell(this, cell_id, shname, area,
			       ni, no, nio, nb, nc,
			       output_array,
			       logic_array,
			       tristate_array,
			       next_state,
			       clocked_on,
			       clocked_on_also,
			       preset,
			       mAlloc);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(CiFFCell));
      cell = new (p) CiFFCell(this, cell_id, shname, area,
			      ni, no, nio, nb, nc,
			      output_array,
			      logic_array,
			      tristate_array,
			      next_state,
			      clocked_on,
			      clocked_on_also,
			      mAlloc);
    }
  }
  add_cell(cell_id, cell);
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
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristate_array トライステート条件の論理式の配列
// @param[in] data_in "data_in" 関数の式
// @param[in] enable "enable" 関数の式
// @param[in] enable_also "enable_also" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
// @param[in] clear_preset_var1 clear と preset が同時にオンになったときの値1
// @param[in] clear_preset_var2 clear と preset が同時にオンになったときの値2
// *1: - false 論理式なし
//     - true 論理式あり
void
CiLibrary::new_latch_cell(ymuint cell_id,
			  const string& name,
			  CellArea area,
			  ymuint ni,
			  ymuint no,
			  ymuint nio,
			  ymuint nb,
			  ymuint nc,
			  const vector<bool>& output_array,
			  const vector<LogExpr>& logic_array,
			  const vector<LogExpr>& tristate_array,
			  const LogExpr& data_in,
			  const LogExpr& enable,
			  const LogExpr& enable_also,
			  const LogExpr& clear,
			  const LogExpr& preset,
			  ymuint clear_preset_var1,
			  ymuint clear_preset_var2)
{
  bool has_clear = !clear.is_zero();
  bool has_preset = !preset.is_zero();

  ShString shname(name);

  CiCell* cell = NULL;
  if ( has_clear ) {
    if ( has_preset ) {
      void* p = mAlloc.get_memory(sizeof(CiLatchSRCell));
      cell = new (p) CiLatchSRCell(this, cell_id, shname, area,
				   ni, no, nio, nb, nc,
				   output_array,
				   logic_array,
				   tristate_array,
				   data_in,
				   enable,
				   enable_also,
				   clear,
				   preset,
				   clear_preset_var1,
				   clear_preset_var2,
				   mAlloc);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(CiLatchRCell));
      cell = new (p) CiLatchRCell(this, cell_id, shname, area,
				  ni, no, nio, nb, nc,
				  output_array,
				  logic_array,
				  tristate_array,
				  data_in,
				  enable,
				  enable_also,
				  clear,
				  mAlloc);
    }
  }
  else {
    if ( has_preset ) {
      void* p = mAlloc.get_memory(sizeof(CiLatchSCell));
      cell = new (p) CiLatchSCell(this, cell_id, shname, area,
				  ni, no, nio, nb, nc,
				  output_array,
				  logic_array,
				  tristate_array,
				  data_in,
				  enable,
				  enable_also,
				  preset,
				  mAlloc);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(CiLatchCell));
      cell = new (p) CiLatchCell(this, cell_id, shname, area,
				 ni, no, nio, nb, nc,
				 output_array,
				 logic_array,
				 tristate_array,
				 data_in,
				 enable,
				 enable_also,
				 mAlloc);
    }
  }
  add_cell(cell_id, cell);
}

// @brief FSMセルを生成する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nit 内部ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristate_array トライステート条件の論理式の配列
// *1: - false 論理式なし
//     - true 論理式あり
void
CiLibrary::new_fsm_cell(ymuint cell_id,
			const string& name,
			CellArea area,
			ymuint ni,
			ymuint no,
			ymuint nio,
			ymuint nit,
			ymuint nb,
			ymuint nc,
			const vector<bool>& output_array,
			const vector<LogExpr>& logic_array,
			const vector<LogExpr>& tristate_array)
{
  void* p = mAlloc.get_memory(sizeof(CiFsmCell));
  CiCell* cell = new (p) CiFsmCell(this, cell_id, ShString(name), area,
				   ni, no, nio, nit, nb, nc,
				   output_array,
				   logic_array,
				   tristate_array,
				   mAlloc);
  add_cell(cell_id, cell);
}

// @brief セルの入力ピンの内容を設定する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] input_id 入力ピン番号 ( 0 <= input_id < cell->input_num2() )
// @param[in] name 入力ピン名
// @param[in] capacitance 入力ピンの負荷容量
// @param[in] rise_capacitance 入力ピンの立ち上がり負荷容量
// @param[in] fall_capacitance 入力ピンの立ち下がり負荷容量
void
CiLibrary::new_cell_input(ymuint cell_id,
			  ymuint pin_id,
			  ymuint input_id,
			  const string& name,
			  CellCapacitance capacitance,
			  CellCapacitance rise_capacitance,
			  CellCapacitance fall_capacitance)
{
  CiCell* cell = mCellArray[cell_id];
  void* p = mAlloc.get_memory(sizeof(CiInputPin));
  CiInputPin* pin = new (p) CiInputPin(cell, ShString(name),
				       capacitance,
				       rise_capacitance, fall_capacitance);
  cell->mPinArray[pin_id] = pin;
  pin->mId = pin_id;
  cell->mInputArray[input_id] = pin;
  pin->mInputId = input_id;
  mPinHash.add(pin);
}

// @brief セルの出力ピンの内容を設定する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] output_id 出力ピン番号 ( 0 <= output_id < cell->output_num2() )
// @param[in] name 出力ピン名
// @param[in] max_fanout 最大ファンアウト容量
// @param[in] min_fanout 最小ファンアウト容量
// @param[in] max_capacitance 最大負荷容量
// @param[in] min_capacitance 最小負荷容量
// @param[in] max_transition 最大遷移時間
// @param[in] min_transition 最小遷移時間
void
CiLibrary::new_cell_output(ymuint cell_id,
			   ymuint pin_id,
			   ymuint output_id,
			   const string& name,
			   CellCapacitance max_fanout,
			   CellCapacitance min_fanout,
			   CellCapacitance max_capacitance,
			   CellCapacitance min_capacitance,
			   CellTime max_transition,
			   CellTime min_transition)
{
  CiCell* cell = mCellArray[cell_id];
  void* p = mAlloc.get_memory(sizeof(CiOutputPin));
  CiOutputPin* pin = new (p) CiOutputPin(cell, ShString(name),
					 max_fanout, min_fanout,
					 max_capacitance, min_capacitance,
					 max_transition, min_transition);
  cell->mPinArray[pin_id] = pin;
  pin->mId = pin_id;
  cell->mOutputArray[output_id] = pin;
  pin->mOutputId = output_id;
  mPinHash.add(pin);
}

// @brief セルの入出力ピンの内容を設定する．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] pin_id 入出力ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] input_id 入力ピン番号 ( 0 <= input_id < cell->input_num2() )
// @param[in] output_id 出力ピン番号 ( 0 <= output_id < cell->output_num2() )
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
CiLibrary::new_cell_inout(ymuint cell_id,
			  ymuint pin_id,
			  ymuint input_id,
			  ymuint output_id,
			  const string& name,
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
  CiCell* cell = mCellArray[cell_id];
  void* p = mAlloc.get_memory(sizeof(CiInoutPin));
  CiInoutPin* pin =  new (p) CiInoutPin(cell, ShString(name),
					capacitance,
					rise_capacitance, fall_capacitance,
					max_fanout, min_fanout,
					max_capacitance, min_capacitance,
					max_transition, min_transition);
  cell->mPinArray[pin_id] = pin;
  pin->mId = pin_id;
  cell->mInputArray[input_id] = pin;
  pin->mInputId = input_id;
  cell->mOutputArray[output_id] = pin;
  pin->mOutputId = output_id;
  mPinHash.add(pin);
}

// @brief セルの内部ピンを生成する．
// @param[in] cell_id セル番号
// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
// @param[in] internal_id 入力ピン番号 ( 0 <= internal_id < cell->internal_num() )
// @param[in] name 内部ピン名
void
CiLibrary::new_cell_internal(ymuint cell_id,
			     ymuint pin_id,
			     ymuint internal_id,
			     const string& name)
{
  CiCell* cell = mCellArray[cell_id];
  void* p = mAlloc.get_memory(sizeof(CiInternalPin));
  CiInternalPin* pin = new (p) CiInternalPin(cell, ShString(name));
  cell->mPinArray[pin_id] = pin;
  pin->mId = pin_id;
  cell->mInternalArray[internal_id] = pin;
  pin->mInternalId = internal_id;
}

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
		      tCellTimingType type,
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

// @brief タイミング情報をセットする．
// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
// @param[in] ipin_id 関連する入力(入出力)ピン番号 ( *2 )
// @param[in] sense タイミング条件
// @param[in] timing_id 設定するタイミング情報の番号
// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->output_num() を使う．
// @note ( *2 ) ipin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->input_num() を使う．
void
CiLibrary::set_timing(ymuint cell_id,
		      ymuint ipin_id,
		      ymuint opin_id,
		      tCellTimingSense sense,
		      CellTiming* timing)
{
  CiCell* cell = mCellArray[cell_id];

  ymuint base = opin_id * cell->input_num2() + ipin_id;
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

// @brief セルのグループ分けを行う．
// @note 論理セルのパタングラフも作成する．
void
CiLibrary::compile()
{
  LibComp libcomp;

  libcomp.compile(*this);

  ymuint nc = libcomp.npn_class_num();
  set_class_num(nc);

  ymuint ng = libcomp.group_num();
  set_group_num(ng);
  for (ymuint g = 0; g < ng; ++ g) {
    const LcGroup* src_group = libcomp.group(g);
    CiGroup& dst_group = mGroupArray[g];
    const CellClass* parent = npn_class(src_group->parent()->id());
    const vector<const Cell*>& cell_list = src_group->cell_list();
    dst_group.init(parent, src_group->map(), cell_list, mAlloc);
  }

  for (ymuint c = 0; c < nc; ++ c) {
    const LcClass* src_class = libcomp.npn_class(c);
    const vector<LcGroup*>& src_group_list = src_class->group_list();
    ymuint n = src_group_list.size();
    vector<const CellGroup*> dst_group_list(n);
    for (ymuint i = 0; i < n; ++ i) {
      const CellGroup* dst_group = group(src_group_list[i]->id());
      dst_group_list[i] = dst_group;
    }
    CiClass& dst_class = mClassArray[c];
    dst_class.init(src_class->idmap_list(), dst_group_list, mAlloc);
  }

  for (ymuint i = 0; i < 4; ++ i) {
    mLogicGroup[i] = &mGroupArray[libcomp.logic_group(i)];
  }

  for (ymuint i = 0; i < 4; ++ i) {
    CiClass* cclass = &mClassArray[libcomp.ff_class(i)];
    mFFClass[i] = cclass;
    ymuint n = cclass->group_num();
    for (ymuint g = 0; g < n; ++ g) {
      // ちょっと面倒な手順を踏む．
      ymuint gid = cclass->cell_group(g)->id();
      CiGroup* cgroup = &mGroupArray[gid];
      NpnMapM map = cgroup->map();
      ymuint pos_array[6] = { 0, 0, 0, 0, 0, 0 };
      ymuint ni = map.ni() - 2;
      assert_cond( ni <= 4, __FILE__, __LINE__);
      for (ymuint i = 0; i < ni; ++ i) {
	NpnVmap imap = map.imap(VarId(i));
	ymuint pos = imap.var().val();
	ymuint pol = (imap.pol() == kPolNega) ? 16U : 8U;
	pos_array[pos] = i | pol;
      }
      pos_array[4] = 0;
      pos_array[5] = 1;
      cgroup->set_ff_info(pos_array);
    }
  }
  for (ymuint i = 0; i < 4; ++ i) {
    CiClass* cclass = &mClassArray[libcomp.latch_class(i)];
    mLatchClass[i] = cclass;
    ymuint n = cclass->group_num();
    for (ymuint g = 0; g < n; ++ g) {
      // ちょっと面倒な手順を踏む．
      ymuint gid = cclass->cell_group(g)->id();
      CiGroup* cgroup = &mGroupArray[gid];
      NpnMapM map = cgroup->map();
      ymuint pos_array[5] = { 0, 0, 0, 0, 0 };
      ymuint ni = map.ni() - 2;
      assert_cond( ni <= 4, __FILE__, __LINE__);
      for (ymuint i = 0; i < ni; ++ i) {
	NpnVmap imap = map.imap(VarId(i));
	ymuint pos = imap.var().val();
	ymuint pol = (imap.pol() == kPolNega) ? 16U : 8U;
	pos_array[pos] = i | pol;
      }
      pos_array[4] = 0;
      cgroup->set_latch_info(pos_array);
    }
  }

  mPatMgr.copy(libcomp.pat_mgr(), mAlloc);
}

// @brief クラス数を設定する．
// @param[in] nc クラス数
// @note 同時にクラスの配列の確保を行う．
void
CiLibrary::set_class_num(ymuint nc)
{
  mClassNum = nc;
  void* p = mAlloc.get_memory(sizeof(CiClass) * mClassNum);
  mClassArray = new (p) CiClass[mClassNum];
  for (ymuint i = 0; i < mClassNum; ++ i) {
    mClassArray[i].mId = i;
  }
}

// @brief グループ数を設定する．
// @param[in] ng グループ数
// @note 同時にグループの配列の確保を行う．
void
CiLibrary::set_group_num(ymuint ng)
{
  mGroupNum = ng;
  void* p = mAlloc.get_memory(sizeof(CiGroup) * mGroupNum);
  mGroupArray = new (p) CiGroup[mGroupNum];
  for (ymuint i = 0; i < mGroupNum; ++ i) {
    mGroupArray[i].mId = i;
  }
}

// @brief セルを追加する．
// @param[in] id セル番号
// @param[in] cell セル
void
CiLibrary::add_cell(ymuint id,
		    CiCell* cell)
{
  mCellArray[id] = cell;
  mCellHash.add(cell);
}

/// @brief 内容をバイナリダンプする．
/// @param[in] s 出力先のストリーム
void
CiLibrary::dump(BinO& s) const
{
  // 名前
  s << name();

  // セル数
  ymuint32 nc = cell_num();
  s << nc;
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = this->cell(i);
    ymuint8 tid = 0;
    if ( cell->is_logic() ) {
      tid = 0;
    }
    else if ( cell->is_ff() ) {
      tid = 1;
    }
    else if ( cell->is_latch() ) {
      tid = 2;
    }
    else if ( cell->is_fsm() ) {
      tid = 3;
    }
    else {
      // 無視？
      assert_not_reached(__FILE__, __LINE__);
    }
    ymuint32 ni = cell->input_num();
    ymuint32 no = cell->output_num();
    ymuint32 nio = cell->inout_num();
    ymuint32 nit = cell->internal_num();
    ymuint32 nbus = cell->bus_num();
    ymuint32 nbundle = cell->bundle_num();

    s << tid
      << cell->name()
      << cell->area()
      << ni
      << no
      << nio
      << nit
      << nbus
      << nbundle;

    ymuint no2 = no + nio;
    for (ymuint opos = 0; opos < no2; ++ opos) {
      s << cell->has_logic()
	<< cell->logic_expr(opos)
	<< cell->tristate_expr(opos);
    }

    if ( cell->is_ff() ) {
      s << cell->next_state_expr()
	<< cell->clock_expr()
	<< cell->clock2_expr()
	<< cell->clear_expr()
	<< cell->preset_expr()
	<< static_cast<ymuint8>(cell->clear_preset_var1())
	<< static_cast<ymuint8>(cell->clear_preset_var2());
    }
    else if ( cell->is_latch() ) {
      s << cell->data_in_expr()
	<< cell->enable_expr()
	<< cell->enable2_expr()
	<< cell->clear_expr()
	<< cell->preset_expr()
	<< static_cast<ymuint8>(cell->clear_preset_var1())
	<< static_cast<ymuint8>(cell->clear_preset_var2());
    }

    // 入力ピンのダンプ
    for (ymuint32 ipin = 0; ipin < ni; ++ ipin) {
      const CellPin* pin = cell->input(ipin);
      s << pin->name()
	<< pin->pin_id()
	<< pin->capacitance()
	<< pin->rise_capacitance()
	<< pin->fall_capacitance();
    }

    // 出力ピンのダンプ
    for (ymuint32 opin = 0; opin < no; ++ opin) {
      const CellPin* pin = cell->output(opin);
      s << pin->name()
	<< pin->pin_id()
	<< pin->max_fanout()
	<< pin->min_fanout()
	<< pin->max_capacitance()
	<< pin->min_capacitance()
	<< pin->max_transition()
	<< pin->min_transition();
    }

    // 入出力ピンのダンプ
    for (ymuint32 iopin = 0; iopin < nio; ++ iopin) {
      const CellPin* pin = cell->output(iopin);
      s << pin->name()
	<< pin->pin_id()
	<< pin->capacitance()
	<< pin->rise_capacitance()
	<< pin->fall_capacitance()
	<< pin->max_fanout()
	<< pin->min_fanout()
	<< pin->max_capacitance()
	<< pin->min_capacitance()
	<< pin->max_transition()
	<< pin->min_transition();
    }

    // 内部ピンのダンプ
    for (ymuint32 itpin = 0; itpin < nit; ++ itpin) {
      const CellPin* pin = cell->internal(itpin);
      s << pin->name()
	<< pin->pin_id();
    }

    // タイミング情報のID -> 通し番号のマップ
    hash_map<ymuint32, ymuint32> timing_map;
    // タイミング情報のリスト
    vector<const CellTiming*> timing_list;
    for (ymuint32 ipos = 0; ipos < ni + nio; ++ ipos) {
      for (ymuint32 opos = 0; opos < no + nio; ++ opos) {
	const CellTiming* timing_p = cell->timing(ipos, opos, kCellPosiUnate);
	if ( timing_p ) {
	  if ( timing_map.count(timing_p->id()) == 0 ) {
	    ymuint pos = timing_list.size();
	    timing_map.insert(make_pair(timing_p->id(), pos));
	    timing_list.push_back(timing_p);
	  }
	}
	const CellTiming* timing_n = cell->timing(ipos, opos, kCellNegaUnate);
	if ( timing_n ) {
	  if ( timing_map.count(timing_n->id()) == 0 ) {
	    ymuint pos = timing_list.size();
	    timing_map.insert(make_pair(timing_n->id(), pos));
	    timing_list.push_back(timing_n);
	  }
	}
      }
    }

    // タイミング情報のダンプ
    ymuint32 nt = timing_list.size();
    s << nt;
    for (ymuint32 j = 0; j < nt; ++ j) {
      const CellTiming* timing = timing_list[j];
      s << timing->intrinsic_rise()
	<< timing->intrinsic_fall()
	<< timing->slope_rise()
	<< timing->slope_fall()
	<< timing->rise_resistance()
	<< timing->fall_resistance();
    }
    for (ymuint32 ipin = 0; ipin < ni + nio; ++ ipin) {
      for (ymuint32 opin = 0; opin < no + nio; ++ opin) {
	const CellTiming* timing_p = cell->timing(ipin, opin, kCellPosiUnate);
	if ( timing_p ) {
	  hash_map<ymuint, ymuint32>::iterator p = timing_map.find(timing_p->id());
	  assert_cond( p != timing_map.end(), __FILE__, __LINE__);
	  s << static_cast<ymuint8>(1)
	    << ipin
	    << opin
	    << p->second;
	}
	const CellTiming* timing_n = cell->timing(ipin, opin, kCellNegaUnate);
	if ( timing_n ) {
	  hash_map<ymuint, ymuint>::iterator p = timing_map.find(timing_n->id());
	  assert_cond( p != timing_map.end(), __FILE__, __LINE__);
	  s << static_cast<ymuint8>(2)
	    << ipin
	    << opin
	    << p->second;
	}
      }
    }
    s << static_cast<ymuint8>(0);
  }

  // セルクラスの個数だけダンプする．
  s << mClassNum
    << mGroupNum;

  // セルグループ情報のダンプ
  for (ymuint g = 0; g < mGroupNum; ++ g) {
    mGroupArray[g].dump(s);
  }

  // セルクラス情報のダンプ
  for (ymuint i = 0; i < mClassNum; ++ i) {
    mClassArray[i].dump(s);
  }

  // 組み込み型の情報のダンプ
  for (ymuint i = 0; i < 4; ++ i) {
    ymuint32 group_id = mLogicGroup[i]->id();
    s << group_id;
  }
  for (ymuint i = 0; i < 4; ++ i) {
    ymuint32 class_id = mFFClass[i]->id();
    s << class_id;
  }
  for (ymuint i = 0; i < 4; ++ i) {
    ymuint32 class_id = mLatchClass[i]->id();
    s << class_id;
  }

  // パタングラフの情報のダンプ
  mPatMgr.dump(s);
}

void
CiLibrary::restore(BinI& s)
{
  string name;
  s >> name;

  set_name(name);

  ymuint32 nc;
  s >> nc;
  set_cell_num(nc);

  for (ymuint cell_id = 0; cell_id < nc; ++ cell_id) {
    ymuint8 type;
    string name;
    CellArea area;
    ymuint32 ni;
    ymuint32 no;
    ymuint32 nio;
    ymuint32 nit;
    ymuint32 nbus;
    ymuint32 nbundle;
    s >> type
      >> name
      >> area
      >> ni
      >> no
      >> nio
      >> nit
      >> nbus
      >> nbundle;

    ymuint no2 = no + nio;
    vector<bool> has_logic(no2);
    vector<LogExpr> logic_array(no2);
    vector<LogExpr> tristate_array(no2);
    for (ymuint opos = 0; opos < no2; ++ opos) {
      bool tmp;
      s >> tmp
	>> logic_array[opos]
	>> tristate_array[opos];
      has_logic[opos] = tmp;
    }

    switch ( type ) {
    case 0: // kLogic
      new_logic_cell(cell_id, name, area,
		     ni, no, nio, nbus, nbundle,
		     has_logic,
		     logic_array,
		     tristate_array);
      break;

    case 1: // kFF
      {
	LogExpr next_state;
	LogExpr clocked_on;
	LogExpr clocked_on_also;
	LogExpr clear;
	LogExpr preset;
	ymuint8 clear_preset_var1;
	ymuint8 clear_preset_var2;
	s >> next_state
	  >> clocked_on
	  >> clocked_on_also
	  >> clear
	  >> preset
	  >> clear_preset_var1
	  >> clear_preset_var2;
	new_ff_cell(cell_id, name, area,
		    ni, no, nio, nbus, nbundle,
		    has_logic,
		    logic_array,
		    tristate_array,
		    next_state,
		    clocked_on, clocked_on_also,
		    clear, preset,
		    clear_preset_var1,
		    clear_preset_var2);
      }
      break;

    case 2: // kLatch
      {
	LogExpr data_in;
	LogExpr enable;
	LogExpr enable_also;
	LogExpr clear;
	LogExpr preset;
	ymuint8 clear_preset_var1;
	ymuint8 clear_preset_var2;
	s >> data_in
	  >> enable
	  >> enable_also
	  >> clear
	  >> preset
	  >> clear_preset_var1
	  >> clear_preset_var2;
	new_latch_cell(cell_id, name, area,
		       ni, no, nio, nbus, nbundle,
		       has_logic,
		       logic_array,
		       tristate_array,
		       data_in,
		       enable, enable_also,
		       clear, preset,
		       clear_preset_var1,
		       clear_preset_var2);
      }
      break;

    case 3: // kFSM
      new_fsm_cell(cell_id, name, area,
		   ni, no, nio, nit, nbus, nbundle,
		   has_logic,
		   logic_array,
		   tristate_array);
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }

    // 入力ピンの設定
    for (ymuint j = 0; j < ni; ++ j) {
      string name;
      ymuint32 pin_id;
      CellCapacitance cap;
      CellCapacitance r_cap;
      CellCapacitance f_cap;
      s >> name
	>> pin_id
	>> cap
	>> r_cap
	>> f_cap;
      new_cell_input(cell_id, pin_id, j, name, cap, r_cap, f_cap);
    }

    // 出力ピンの設定
    for (ymuint j = 0; j < no; ++ j) {
      string name;
      ymuint32 pin_id;
      CellCapacitance max_f;
      CellCapacitance min_f;
      CellCapacitance max_c;
      CellCapacitance min_c;
      CellTime max_t;
      CellTime min_t;
      s >> name
	>> pin_id
	>> max_f
	>> min_f
	>> max_c
	>> min_c
	>> max_t
	>> min_t;
      new_cell_output(cell_id, pin_id, j, name,
		      max_f, min_f,
		      max_c, min_c,
		      max_t, min_t);
    }

    // 入出力ピンの設定
    for (ymuint j = 0; j < nio; ++ j) {
      string name;
      ymuint32 pin_id;
      CellCapacitance cap;
      CellCapacitance r_cap;
      CellCapacitance f_cap;
      CellCapacitance max_f;
      CellCapacitance min_f;
      CellCapacitance max_c;
      CellCapacitance min_c;
      CellTime max_t;
      CellTime min_t;
      s >> name
	>> pin_id
	>> cap
	>> r_cap
	>> f_cap
	>> max_f
	>> min_f
	>> max_c
	>> min_c
	>> max_t
	>> min_t;
      new_cell_inout(cell_id, pin_id, j + ni, j + no, name,
		     cap, r_cap, f_cap,
		     max_f, min_f,
		     max_c, min_c,
		     max_t, min_t);
    }

    // 内部ピンの設定
    for (ymuint j = 0; j < nit; ++ j) {
      string name;
      ymuint32 pin_id;
      s >> name
	>> pin_id;
      new_cell_internal(cell_id, pin_id, j, name);
    }

    // タイミング情報の生成
    ymuint32 nt;
    s >> nt;
    vector<CellTiming*> tmp_list(nt);
    for (ymuint j = 0; j < nt; ++ j) {
      CellTime i_r;
      CellTime i_f;
      CellTime s_r;
      CellTime s_f;
      CellResistance r_r;
      CellResistance f_r;
      s >> i_r
	>> i_f
	>> s_r
	>> s_f
	>> r_r
	>> f_r;
      CellTiming* timing = new_timing(j, kCellTimingCombinational,
				      i_r, i_f, s_r, s_f, r_r, f_r);
      tmp_list[j] = timing;
    }

    // タイミング情報の設定
    for ( ; ; ) {
      ymuint8 unate;
      s >> unate;
      if ( unate == 0 ) {
	// エンドマーカー
	break;
      }
      ymuint32 ipin_id;
      ymuint32 opin_id;
      ymuint32 timing_id;
      s >> ipin_id
	>> opin_id
	>> timing_id;
      tCellTimingSense sense = ( unate == 1 ) ? kCellPosiUnate : kCellNegaUnate;
      CellTiming* timing = tmp_list[timing_id];
      set_timing(cell_id, ipin_id, opin_id, sense, timing);
    }
  }

  // セルクラス数とグループ数の取得
  ymuint32 ncc;
  ymuint32 ng;
  s >> ncc
    >> ng;
  set_class_num(ncc);
  set_group_num(ng);

  // セルグループ情報の設定
  for (ymuint g = 0; g < ng; ++ g) {
    mGroupArray[g].restore(s, *this, mAlloc);
  }

  // セルクラス情報の設定
  for (ymuint c = 0; c < ncc; ++ c) {
    mClassArray[c].restore(s, *this, mAlloc);
  }

  // 組み込み型の設定
  for (ymuint i = 0; i < 4; ++ i) {
    ymuint32 group_id;
    s >> group_id;
    mLogicGroup[i] = &mGroupArray[group_id];
  }
  for (ymuint i = 0; i < 4; ++ i) {
    ymuint32 class_id;
    s >> class_id;
    mFFClass[i] = &mClassArray[class_id];
  }
  for (ymuint i = 0; i < 4; ++ i) {
    ymuint32 class_id;
    s >> class_id;
    mLatchClass[i] = &mClassArray[class_id];
  }

  // パタングラフの情報の設定
  mPatMgr.restore(s, mAlloc);
}

// @brief ピンの登録
// @param[in] pin 登録するピン
void
CiLibrary::add_pin(CiPin* pin)
{
  mPinHash.add(pin);
}

// @brief ピン名からピンを取り出す．
// @param[in] cell セル
// @param[in] name ピン名
CiPin*
CiLibrary::get_pin(const CiCell* cell,
		   ShString name)
{
  return mPinHash.get(cell, name);
}

END_NAMESPACE_YM_CELL

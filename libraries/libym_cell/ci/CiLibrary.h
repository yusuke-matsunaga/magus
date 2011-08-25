#ifndef CILIBRARY_H
#define CILIBRARY_H

/// @file CiLibrary.h
/// @brief CiLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellArea.h"
#include "ym_cell/CellTime.h"
#include "ym_cell/CellCapacitance.h"
#include "ym_cell/CellResistance.h"
#include "ym_cell/CellTiming.h"
#include "ym_cell/CellPin.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/ShString.h"
#include "ym_logic/LogExpr.h"


BEGIN_NAMESPACE_YM_CELL

class CiCell;
class CiPin;
class CiTiming;

//////////////////////////////////////////////////////////////////////
/// @class CiLibrary CiLibrary.h "CiLibrary.h"
/// @brief CellLibrary の実装クラス
//////////////////////////////////////////////////////////////////////
class CiLibrary :
  public CellLibrary
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  CiLibrary(const string& name);

  /// @brief デストラクタ
  virtual
  ~CiLibrary();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const;

  /// @brief テクノロジの取得
  /// このクラスは常に kTechAsic を返す．
  virtual
  tTechnology
  technology() const;

  /// @brief 遅延モデルの取得
  /// 返り値は
  /// - kDelayGeneric
  /// - kDelayPiecewise
  /// - kDelayNonlinear
  /// のいずれか
  virtual
  tDelayModel
  delay_model() const;

  /// @brief バス命名規則の取得
  virtual
  string
  bus_naming_style() const;

  /// @brief 日付情報の取得
  virtual
  string
  date() const;

  /// @brief リビジョン情報の取得
  virtual
  string
  revision() const;

  /// @brief コメント情報の取得
  virtual
  string
  comment() const;

  /// @brief 時間単位の取得
  virtual
  string
  time_unit() const;

  /// @brief 電圧単位の取得
  virtual
  string
  voltage_unit() const;

  /// @brief 電流単位の取得
  virtual
  string
  current_unit() const;

  /// @brief 抵抗単位の取得
  virtual
  string
  pulling_resistance_unit() const;

  /// @brief 容量単位の取得
  virtual
  string
  capacitive_load_unit() const;

  /// @brief 電力単位の取得
  virtual
  string
  leakage_power_unit() const;

  /// @brief ルックアップテーブルのテンプレートの取得
  /// @param[in] name テンプレート名
  /// @note なければ NULL を返す．
  virtual
  const CellLutTemplate*
  lu_table_template(const char* name) const;

  /// @brief バスタイプの取得
  /// @param[in] name バスタイプ名
  /// @note なければ NULL を返す．
  virtual
  const CellBusType*
  bus_type(const char* name) const;


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief このライブラリの持つセル数の取得
  virtual
  ymuint
  cell_num() const;

  /// @brief セルの取得
  /// @param[in] pos 位置番号( 0 <= pos < cell_num() )
  virtual
  const Cell*
  cell(ymuint pos) const;

  /// @brief 名前からのセルの取得
  virtual
  const Cell*
  cell(const char* name) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  void
  set_attr(const string& attr_name,
	   const string& value);

  /// @brief セル数を設定する．
  /// @param[in] num 設定する値
  void
  set_cell_num(ymuint num);

  /// @brief 論理セルを生成する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @return セルへのポインタを返す．
  CiCell*
  new_logic_cell(ymuint cell_id,
		 ShString name,
		 CellArea area,
		 ymuint ni,
		 ymuint no,
		 ymuint nio,
		 ymuint nb,
		 ymuint nc);

  /// @brief FFセルを生成する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] var1, var2 状態変数名
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  /// @param[in] clocked_on_also "clocked_on_also" 関数の式
  /// @param[in] clear "clear" 関数の式
  /// @param[in] preset "preset" 関数の式
  /// @param[in] clear_preset_var1 "clear_preset_var1" の値
  /// @param[in] clear_preset_var2 "clear_preset_var2" の値
  /// @return セルへのポインタを返す．
  CiCell*
  new_ff_cell(ymuint cell_id,
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
	      ymuint clear_preset_var2);

  /// @brief ラッチセルを生成する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] var1, var2 状態変数名
  /// @param[in] data_in "data_in" 関数の式
  /// @param[in] enable "enable" 関数の式
  /// @param[in] enable_also "enable_also" 関数の式
  /// @param[in] clear "clear" 関数の式
  /// @param[in] preset "preset" 関数の式
  /// @param[in] clear_preset_var1 "clear_preset_var1" の値
  /// @param[in] clear_preset_var2 "clear_preset_var2" の値
  /// @return セルへのポインタを返す．
  CiCell*
  new_latch_cell(ymuint cell_id,
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
		 ymuint clear_preset_var2);

  /// @brief セルの入力ピンを生成する．
  /// @param[in] cell セル
  /// @param[in] pin_id 入力ピン番号 ( 0 <= pin_id < cell->input_num() )
  /// @param[in] name 入力ピン名
  /// @param[in] capacitance 入力ピンの負荷容量
  /// @param[in] rise_capacitance 入力ピンの立ち上がり負荷容量
  /// @param[in] fall_capacitance 入力ピンの立ち下がり負荷容量
  void
  new_cell_input(CiCell* cell,
		 ymuint pin_id,
		 ShString name,
		 CellCapacitance capacitance,
		 CellCapacitance rise_capacitance,
		 CellCapacitance fall_capacitance);

  /// @brief セルの出力ピンの内容を設定する．
  /// @param[in] cell セル
  /// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < cell->output_num() )
  /// @param[in] name 出力ピン名
  /// @param[in] max_fanout 最大ファンアウト容量
  /// @param[in] min_fanout 最小ファンアウト容量
  /// @param[in] max_capacitance 最大負荷容量
  /// @param[in] min_capacitance 最小負荷容量
  /// @param[in] max_transition 最大遷移時間
  /// @param[in] min_transition 最小遷移時間
  void
  new_cell_output(CiCell* cell,
		  ymuint pin_id,
		  ShString name,
		  CellCapacitance max_fanout,
		  CellCapacitance min_fanout,
		  CellCapacitance max_capacitance,
		  CellCapacitance min_capacitance,
		  CellTime max_transition,
		  CellTime min_transition);

  /// @brief セルの入出力ピンの内容を設定する．
  /// @param[in] cell セル
  /// @param[in] pin_id 入出力ピン番号 ( 0 <= pin_id < cell->inout_num() )
  /// @param[in] name 入出力ピン名
  /// @param[in] capacitance 入力ピンの負荷容量
  /// @param[in] rise_capacitance 入力ピンの立ち上がり負荷容量
  /// @param[in] fall_capacitance 入力ピンの立ち下がり負荷容量
  /// @param[in] max_fanout 最大ファンアウト容量
  /// @param[in] min_fanout 最小ファンアウト容量
  /// @param[in] max_capacitance 最大負荷容量
  /// @param[in] min_capacitance 最小負荷容量
  /// @param[in] max_transition 最大遷移時間
  /// @param[in] min_transition 最小遷移時間
  void
  new_cell_inout(CiCell* cell,
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
		 CellTime min_transition);

  /// @brief セルの内部ピンを生成する．
  /// @param[in] cell セル
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
  /// @param[in] name 内部ピン名
  void
  new_cell_internal(CiCell* cell,
		    ymuint pin_id,
		    ShString name);

  /// @brief タイミング情報を作る．
  /// @param[in] id ID番号
  /// @param[in] type タイミングの型
  /// @param[in] intrinsic_rise 立ち上がり固有遅延
  /// @param[in] intrinsic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  /// @param[in] rise_resistance 立ち上がり負荷依存係数
  /// @param[in] fall_resistance 立ち下がり負荷依存係数
  CiTiming*
  new_timing(ymuint id,
	     CellTiming::tType type,
	     CellTime intrinsic_rise,
	     CellTime intrinsic_fall,
	     CellTime slope_rise,
	     CellTime slope_fall,
	     CellResistance rise_resistance,
	     CellResistance fall_resistance);

  /// @brief セルのタイミング情報を設定する．
  /// @param[in] cell セル
  /// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
  /// @param[in] ipin_id 関連する入力(入出力)ピン番号 ( *2 )
  /// @param[in] sense タイミング条件
  /// @param[in] timing 設定するタイミング情報
  /// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->output_num() を使う．
  /// @note ( *2 ) ipin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->input_num() を使う．
  void
  set_cell_timing(CiCell* cell,
		  ymuint opin_id,
		  ymuint ipin_id,
		  tCellTimingSense sense,
		  CiTiming* timing);

  /// @brief 出力ピンの機能を設定する．
  /// @param[in] cell セル
  /// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
  /// @param[in] function 機能を表す論理式
  /// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->output_num() を使う．
  void
  set_opin_function(CiCell* cell,
		    ymuint opin_id,
		    const LogExpr& function);

  /// @brief 出力ピンの three_state 条件を設定する．
  /// @param[in] cell セル
  /// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
  /// @param[in] expr three_state 条件を表す論理式
  /// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->output_num() を使う．
  void
  set_opin_three_state(CiCell* cell,
		       ymuint opin_id,
		       const LogExpr& expr);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルにピン数，バス数，バンドル数の設定をする．
  /// @param[in] cell セル
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  void
  set_pinnum(CiCell* cell,
	     ymuint ni,
	     ymuint no,
	     ymuint nio,
	     ymuint nb,
	     ymuint nc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のオブジェクト
  SimpleAlloc mAlloc;

  // 名前
  string mName;

  // バス命名規則
  string mBusNamingStyle;

  // 日付
  string mDate;

  // リビジョン
  string mRevision;

  // コメント
  string mComment;

  // 時間単位
  string mTimeUnit;

  // 電圧単位
  string mVoltageUnit;

  // 電流単位
  string mCurrentUnit;

  // 抵抗単位
  string mPullingResistanceUnit;

  // 容量単位
  string mCapacitiveLoadUnit;

  // 電力単位
  string mLeakagePowerUnit;

  // セル数
  ymuint32 mCellNum;

  // セルの配列
  CiCell** mCellArray;

};

END_NAMESPACE_YM_CELL

#endif // CILIBRARY_H

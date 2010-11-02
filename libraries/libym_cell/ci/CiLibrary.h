#ifndef LIBYM_CELL_CI_CILIBRARY_H
#define LIBYM_CELL_CI_CILIBRARY_H

/// @file libym_cell/ci/CiLibrary.h
/// @brief CiLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/ShString.h"
#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM_CELL

class CiCell;
class CiOutputPin;

//////////////////////////////////////////////////////////////////////
/// @class CiLibrary CiLibrary.h "ci/CiLibrary.h"
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
  tCellTechnology
  technology() const;

  /// @brief 遅延モデルの取得
  /// 返り値は
  /// - kDelayGeneric
  /// - kDelayPiecewise
  /// - kDelayNonlinear
  /// のいずれか
  virtual
  tCellDelayModel
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

  /// @brief セル数を設定する．
  /// @param[in] num 設定する値
  void
  set_cell_num(ymuint num);

  /// @brief セルの内容を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力数
  /// @param[in] no 出力数
  /// @param[in] nio 入出力数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  void
  set_cell(ymuint cell_id,
	   ShString name,
	   CellArea area,
	   ymuint ni,
	   ymuint no,
	   ymuint nio,
	   ymuint nb,
	   ymuint nc);

  /// @brief セルの入力ピンの内容を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
  /// @param[in] ipin_id 入力ピン番号 ( 0 <= ipin_id < cell->input_num() )
  /// @param[in] pin_name 入力ピン名
  /// @param[in] pin_capacitance 入力ピンの負荷容量
  /// @param[in] pin_rise_capacitance 入力ピンの立ち上がり負荷容量
  /// @param[in] pin_fall_capacitance 入力ピンの立ち下がり負荷容量
  void
  set_cell_input(ymuint cell_id,
		 ymuint pin_id,
		 ymuint ipin_id,
		 ShString pin_name,
		 CellCapacitance pin_capacitance,
		 CellCapacitance pin_rise_capacitance,
		 CellCapacitance pin_fall_capacitance);

  /// @brief セルの出力ピンの内容を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
  /// @param[in] opin_id 出力力ピン番号 ( 0 <= opin_id < cell->output_num() )
  /// @param[in] pin_name 出力ピン名
  void
  set_cell_output(ymuint cell_id,
		  ymuint pin_id,
		  ymuint opin_id,
		  ShString pin_name);

  /// @brief セルの入出力ピンの内容を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
  /// @param[in] iopin_id 入出力力ピン番号 ( 0 <= iopin_id < cell->inout_num() )
  /// @param[in] pin_name 入出力ピン名
  /// @param[in] pin_capacitance 入力ピンの負荷容量
  /// @param[in] pin_rise_capacitance 入力ピンの立ち上がり負荷容量
  /// @param[in] pin_fall_capacitance 入力ピンの立ち下がり負荷容量
  void
  set_cell_inout(ymuint cell_id,
		 ymuint pin_id,
		 ymuint iopin_id,
		 ShString pin_name,
		 CellCapacitance pin_capacitance,
		 CellCapacitance pin_rise_capacitance,
		 CellCapacitance pin_fall_capacitance);

  /// @brief セルの出力ピン(入出力ピン)の機能を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id 出力(入出力)ピン番号
  /// @param[in] function 関数を表す論理式
  void
  set_opin_function(ymuint cell_id,
		    ymuint pin_id,
		    const LogExpr& function);

  /// @brief セルのタイミング情報を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id 出力(入出力)ピン番号
  /// @param[in] ipin_list 関連する入力(入出力)ピン番号のリスト
  /// @param[in] type タイミングの型
  /// @param[in] sense タイミング条件
  /// @param[in] intrinsic_rise 立ち上がり固有遅延
  /// @param[in] intrinsic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  /// @param[in] rise_resistance 立ち上がり負荷依存係数
  /// @param[in] fall_resistance 立ち下がり負荷依存係数
  /// @note opin_id, ipin_list のピン番号は全てのピンに対する通し番号
  void
  set_opin_timing(ymuint cell_id,
		  ymuint pin_id,
		  const vector<ymuint>& ipin_list,
		  tCellTimingType type,
		  tCellTimingSense sense,
		  CellTime intrinsic_rise,
		  CellTime intrinsic_fall,
		  CellTime slope_rise,
		  CellTime slope_fall,
		  CellResistance rise_resistance,
		  CellResistance fall_resistance);


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
  CiCell* mCellArray;

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_CI_CILIBRARY_H

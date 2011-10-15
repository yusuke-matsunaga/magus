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
#include "CiCellHash.h"
#include "CiPinHash.h"
#include "CiPatMgr.h"


BEGIN_NAMESPACE_YM_CELL

class CiClass;
class CiGroup;
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
  CiLibrary();

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

  /// @brief 名前からのセルの取得
  virtual
  const Cell*
  cell(const string& name) const;

  /// @brief セルグループの個数を返す．
  virtual
  ymuint
  group_num() const;

  /// @brief セルグループを返す．
  /// @param[in] id グループ番号　( 0 <= id < group_num() )
  virtual
  const CellGroup*
  group(ymuint id) const;

  /// @brief NPN同値クラスの個数を返す．
  virtual
  ymuint
  npn_class_num() const;

  /// @brief NPN同値クラスを返す．
  /// @param[in] id クラス番号 ( 0 <= id < npn_class_num() )
  virtual
  const CellClass*
  npn_class(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理セルの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0セルのグループを返す．
  virtual
  const CellGroup*
  const0_func() const;

  /// @brief 定数1セルのグループを返す．
  virtual
  const CellGroup*
  const1_func() const;

  /// @brief バッファセルのグループを返す．
  virtual
  const CellGroup*
  buf_func() const;

  /// @brief インバータセルのグループを返す．
  virtual
  const CellGroup*
  inv_func() const;


public:
  //////////////////////////////////////////////////////////////////////
  // FFセルの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 単純な型のFFクラスを返す．
  /// @param[in] has_clear クリア端子を持つとき true にする．
  /// @param[in] has_preset プリセット端子を持つとき true にする．
  /// @note 該当するセルがないときでも空のセルクラスが返される．
  virtual
  const CellClass*
  simple_ff_class(bool has_clear,
		  bool has_preset) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ラッチセルの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 単純な型のラッチクラスを返す．
  /// @param[in] has_clear クリア端子を持つとき true にする．
  /// @param[in] has_preset プリセット端子を持つとき true にする．
  /// @note 該当するセルがないときでも空のセルクラスが返される．
  virtual
  const CellClass*
  simple_latch_class(bool has_clear,
		     bool has_preset) const;


public:
  //////////////////////////////////////////////////////////////////////
  // パタングラフ関係の情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 総パタン数を返す．
  virtual
  ymuint
  pat_num() const;

  /// @brief パタンを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  virtual
  const CellPatGraph&
  pat(ymuint id) const;

  /// @brief パタンの最大の入力数を得る．
  virtual
  ymuint
  max_input() const;

  /// @brief 総ノード数を返す．
  virtual
  ymuint
  node_num() const;

  /// @brief ノードの種類を返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  virtual
  tCellPatType
  node_type(ymuint id) const;

  /// @brief ノードが入力ノードの時に入力番号を返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  /// @note 入力ノードでない場合の返り値は不定
  virtual
  ymuint
  input_id(ymuint id) const;

  /// @brief 入力のノード番号を返す．
  /// @param[in] input_id 入力番号 ( 0 <= input_id < input_num() )
  /// @return input_id の入力に対応するノードのノード番号
  virtual
  ymuint
  input_node(ymuint input_id) const;

  /// @brief 総枝数を返す．
  virtual
  ymuint
  edge_num() const;

  /// @brief 枝のファンイン元のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  virtual
  ymuint
  edge_from(ymuint id) const;

  /// @brief 枝のファンアウト先のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  virtual
  ymuint
  edge_to(ymint id) const;

  /// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  virtual
  ymuint
  edge_pos(ymuint id) const;

  /// @brief 枝の反転属性を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  virtual
  bool
  edge_inv(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ダンプ/リストア関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ostream& s) const;

  /// @brief バイナリダンプされた内容を読み込む．
  /// @param[in] s 入力元のストリーム
  virtual
  void
  restore(istream& s);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を設定する．
  /// @param[in] name 名前
  virtual
  void
  set_name(const string& name);

  /// @brief 属性を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  virtual
  void
  set_attr(const string& attr_name,
	   const string& value);

  /// @brief セル数を設定する．
  /// @param[in] num 設定する値
  virtual
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
  /// @param[in] output_array 出力の情報の配列
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] tristated_array トライステート条件の論理式の配列
  /// *1: - false 論理式なし
  ///     - true 論理式あり
  virtual
  void
  new_logic_cell(ymuint cell_id,
		 const string& name,
		 CellArea area,
		 ymuint ni,
		 ymuint no,
		 ymuint nio,
		 ymuint nb,
		 ymuint nc,
		 const vector<bool>& output_array,
		 const vector<LogExpr>& logic_array,
		 const vector<LogExpr>& tristate_array);

  /// @brief FFセルを生成する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] output_array 出力の情報の配列(*1)
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] tristated_array トライステート条件の論理式の配列
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  /// @param[in] clocked_on_also "clocked_on_also" 関数の式
  /// @param[in] clear "clear" 関数の式
  /// @param[in] preset "preset" 関数の式
  /// @param[in] clear_preset_var1 clear と preset が同時にオンになったときの値1
  /// @param[in] clear_preset_var2 clear と preset が同時にオンになったときの値2
  /// *1: - false 論理式なし
  ///     - true 論理式あり
  virtual
  void
  new_ff_cell(ymuint cell_id,
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
  /// @param[in] output_array 出力の情報の配列(*1)
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] tristated_array トライステート条件の論理式の配列
  /// @param[in] data_in "data_in" 関数の式
  /// @param[in] enable "enable" 関数の式
  /// @param[in] clear "clear" 関数の式
  /// @param[in] preset "preset" 関数の式
  /// @param[in] enable_also "enable_also" 関数の式
  /// @param[in] clear_preset_var1 clear と preset が同時にオンになったときの値1
  /// @param[in] clear_preset_var2 clear と preset が同時にオンになったときの値2
  /// *1: - false 論理式なし
  ///     - true 論理式あり
  virtual
  void
  new_latch_cell(ymuint cell_id,
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
		 ymuint clear_preset_var2);

  /// @brief セルの入力ピンを生成する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id 入力ピン番号 ( 0 <= pin_id < cell->input_num() )
  /// @param[in] name 入力ピン名
  /// @param[in] capacitance 入力ピンの負荷容量
  /// @param[in] rise_capacitance 入力ピンの立ち上がり負荷容量
  /// @param[in] fall_capacitance 入力ピンの立ち下がり負荷容量
  virtual
  void
  new_cell_input(ymuint cell_id,
		 ymuint pin_id,
		 const string& name,
		 CellCapacitance capacitance,
		 CellCapacitance rise_capacitance,
		 CellCapacitance fall_capacitance);

  /// @brief セルの出力ピンの内容を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < cell->output_num() )
  /// @param[in] name 出力ピン名
  /// @param[in] max_fanout 最大ファンアウト容量
  /// @param[in] min_fanout 最小ファンアウト容量
  /// @param[in] max_capacitance 最大負荷容量
  /// @param[in] min_capacitance 最小負荷容量
  /// @param[in] max_transition 最大遷移時間
  /// @param[in] min_transition 最小遷移時間
  virtual
  void
  new_cell_output(ymuint cell_id,
		  ymuint pin_id,
		  const string& name,
		  CellCapacitance max_fanout,
		  CellCapacitance min_fanout,
		  CellCapacitance max_capacitance,
		  CellCapacitance min_capacitance,
		  CellTime max_transition,
		  CellTime min_transition);

  /// @brief セルの入出力ピンの内容を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
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
  virtual
  void
  new_cell_inout(ymuint cell_id,
		 ymuint pin_id,
		 const string& name,
		 CellCapacitance capacitance,
		 CellCapacitance rise_capacitance,
		 CellCapacitance fall_capacitance,
		 CellCapacitance max_fanout,
		 CellCapacitance min_fanout,
		 CellCapacitance max_capacitance,
		 CellCapacitance min_capacitance,
		 CellTime max_transition,
		 CellTime min_transition);

#if 0
  /// @brief セルの内部ピンを生成する．
  /// @param[in] cell セル
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
  /// @param[in] name 内部ピン名
  void
  new_cell_internal(CiCell* cell,
		    ymuint pin_id,
		    ShString name);
#endif

  /// @brief タイミング情報を作る．
  /// @param[in] id ID番号
  /// @param[in] type タイミングの型
  /// @param[in] intrinsic_rise 立ち上がり固有遅延
  /// @param[in] intrinsic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  /// @param[in] rise_resistance 立ち上がり負荷依存係数
  /// @param[in] fall_resistance 立ち下がり負荷依存係数
  virtual
  CellTiming*
  new_timing(ymuint id,
	     tCellTimingType type,
	     CellTime intrinsic_rise,
	     CellTime intrinsic_fall,
	     CellTime slope_rise,
	     CellTime slope_fall,
	     CellResistance rise_resistance,
	     CellResistance fall_resistance);

  /// @brief タイミング情報をセットする．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
  /// @param[in] ipin_id 関連する入力(入出力)ピン番号 ( *2 )
  /// @param[in] sense タイミング条件
  /// @param[in] timing 設定するタイミング情報
  /// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->output_num() を使う．
  /// @note ( *2 ) ipin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->input_num() を使う．
  virtual
  void
  set_timing(ymuint cell_id,
	     ymuint ipin_id,
	     ymuint opin_id,
	     tCellTimingSense sense,
	     CellTiming* timing);

  /// @brief セルのグループ分けを行う．
  /// @note 論理セルのパタングラフも作成する．
  virtual
  void
  compile();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラス数を設定する．
  /// @param[in] nc クラス数
  /// @note 同時にクラスの配列の確保を行う．
  void
  set_class_num(ymuint nc);

  /// @brief グループ数を設定する．
  /// @param[in] ng グループ数
  /// @note 同時にグループの配列の確保を行う．
  void
  set_group_num(ymuint ng);

  /// @brief セルを追加する．
  /// @param[in] id セル番号
  /// @param[in] cell セル
  void
  add_cell(ymuint id,
	   CiCell* cell);


public:
  //////////////////////////////////////////////////////////////////////
  // ピンハッシュ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピンの登録
  /// @param[in] pin 登録するピン
  void
  add_pin(CiPin* pin);

  /// @brief ピン名からピンを取り出す．
  /// @param[in] cell セル
  /// @param[in] name ピン名
  CiPin*
  get_pin(const CiCell* cell,
	  ShString name);


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

  // セルのポインタの配列
  CiCell** mCellArray;

  // 名前をキーにしたセルのハッシュ表
  CiCellHash mCellHash;

  // ピン名をキーにしたピンのハッシュ表
  CiPinHash mPinHash;

  // セルグループ数
  ymuint32 mGroupNum;

  // セルグループの配列
  CiGroup* mGroupArray;

  // NPN同値クラスの数
  ymuint32 mClassNum;

  // NPN同値クラスの配列
  CiClass* mClassArray;

  // 論理セルグループの情報
  const CellGroup* mLogicGroup[4];

  // FFクラスの情報
  const CellClass* mFFClass[4];

  // ラッチクラスの情報
  const CellClass* mLatchClass[4];

  // パタングラフを管理するクラス
  CiPatMgr mPatMgr;

};

END_NAMESPACE_YM_CELL

#endif // CILIBRARY_H

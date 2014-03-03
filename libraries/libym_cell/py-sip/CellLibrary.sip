
/// @file CellLibrary.sip
/// @brief CellLibrary の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

%Feature DISABLE_SET

class CellLibrary /Abstract/
{
%TypeHeaderCode
#include "cell/CellLibrary.h"
using nsYm::CellLibrary;
%End

public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  const char*
  name() const = 0;
  %MethodCode
  sipRes = sipCpp->name().c_str();
  %End

  /// @brief テクノロジの取得
  /// 返り値は
  /// - kTechCmos
  /// - kTechFpga
  /// のどちらか
  tCellTechnology
  technology() const;

  /// @brief 遅延モデルの取得
  /// 返り値は
  /// - kCellDelayGenericCmos
  /// - kCellDelayTableLookup
  /// - kCellDelayPiecewiseCmos
  /// - kCellDelayCmos2
  /// - kCellDelayDcm
  /// のいずれか
  tCellDelayModel
  delay_model() const;

  /// @brief バス命名規則の取得
  const char*
  bus_naming_style() const;
  %MethodCode
  sipRes = sipCpp->bus_naming_style().c_str();
  %End

  /// @brief 日付情報の取得
  const char*
  date() const;
  %MethodCode
  sipRes = sipCpp->date().c_str();
  %End

  /// @brief リビジョン情報の取得
  const char*
  revision() const;
  %MethodCode
  sipRes = sipCpp->revision().c_str();
  %End

  /// @brief コメント情報の取得
  const char*
  comment() const;
  %MethodCode
  sipRes = sipCpp->comment().c_str();
  %End

  /// @brief 時間単位の取得
  const char*
  time_unit() const;
  %MethodCode
  sipRes = sipCpp->time_unit().c_str();
  %End

  /// @brief 電圧単位の取得
  const char*
  voltage_unit() const;
  %MethodCode
  sipRes = sipCpp->voltage_unit().c_str();
  %End

  /// @brief 電流単位の取得
  const char*
  current_unit() const;
  %MethodCode
  sipRes = sipCpp->current_unit().c_str();
  %End

  /// @brief 抵抗単位の取得
  const char*
  pulling_resistance_unit() const;
  %MethodCode
  sipRes = sipCpp->pulling_resistance_unit().c_str();
  %End

  /// @brief 容量単位の取得
  /// @note なぜかここだけインターフェイスが異なる．
  double
  capacitive_load_unit() const;

  /// @brief 容量単位文字列の取得
  /// @note なぜかここだけインターフェイスが異なる．
  const char*
  capacitive_load_unit_str() const;
  %MethodCode
  sipRes = sipCpp->capacitive_load_unit_str().c_str();
  %End

  /// @brief 電力単位の取得
  const char*
  leakage_power_unit() const;
  %MethodCode
  sipRes = sipCpp->leakage_power_unit().c_str();
  %End

  /// @brief 遅延テーブルのテンプレート数の取得
  unsigned int
  lu_table_template_num() const;

  /// @brief 遅延テーブルのテンプレートの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < lu_table_template_num() )
  const CellLutTemplate*
  lu_table_template(unsigned int pos) const;

  /// @brief 遅延テーブルのテンプレートの取得
  /// @param[in] name テンプレート名
  /// @note なければ NULL を返す．
  const CellLutTemplate*
  lu_table_template(const char* name) const;

  /// @brief バスタイプの取得
  /// @param[in] name バスタイプ名
  /// @note なければ NULL を返す．
  const CellBusType*
  bus_type(const char* name) const;


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief このライブラリの持つセル数の取得
  unsigned int
  cell_num() const = 0;

  /// @brief セルの取得
  /// @param[in] pos 位置番号( 0 <= pos < cell_num() )
  const Cell*
  cell(unsigned int pos) const;

  /// @brief 名前からのセルの取得
  const Cell*
  cell(const char* name) const;

  /// @brief セルグループの個数を返す．
  unsigned int
  group_num() const;

  /// @brief セルグループを返す．
  /// @param[in] id グループ番号　( 0 <= id < group_num() )
  const CellGroup*
  group(unsigned int id) const;

  /// @brief NPN同値クラスの個数を返す．
  unsigned int
  npn_class_num() const;

  /// @brief NPN同値クラスを返す．
  /// @param[in] id クラス番号 ( 0 <= id < npn_class_num() )
  const CellClass*
  npn_class(unsigned int id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理セルの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0セルのグループを返す．
  const CellGroup*
  const0_func() const;

  /// @brief 定数1セルのグループを返す．
  const CellGroup*
  const1_func() const;

  /// @brief バッファセルのグループを返す．
  const CellGroup*
  buf_func() const;

  /// @brief インバータセルのグループを返す．
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
  const CellClass*
  simple_latch_class(bool has_clear,
		     bool has_preset) const;


public:
  //////////////////////////////////////////////////////////////////////
  // パタングラフ関係の情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 総パタン数を返す．
  unsigned int
  pg_pat_num() const;

  /// @brief パタンを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pg_pat_num() )
  const CellPatGraph&
  pg_pat(unsigned int id) const;

  /// @brief パタンの最大の入力数を得る．
  unsigned int
  pg_max_input() const;

  /// @brief 総ノード数を返す．
  unsigned int
  pg_node_num() const;

  /// @brief ノードの種類を返す．
  /// @param[in] id ノード番号 ( 0 <= id < pg_node_num() )
  tCellPatType
  pg_node_type(unsigned int id) const;

  /// @brief ノードが入力ノードの時に入力番号を返す．
  /// @param[in] id ノード番号 ( 0 <= id < pg_node_num() )
  /// @note 入力ノードでない場合の返り値は不定
  unsigned int
  pg_input_id(unsigned int id) const;

  /// @brief 入力のノード番号を返す．
  /// @param[in] input_id 入力番号 ( 0 <= input_id < pg_input_num() )
  /// @return input_id の入力に対応するノードのノード番号
  unsigned int
  pg_input_node(unsigned int input_id) const;

  /// @brief 総枝数を返す．
  unsigned int
  pg_edge_num() const;

  /// @brief 枝のファンイン元のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < edge_num() )
  unsigned int
  pg_edge_from(unsigned int id) const;

  /// @brief 枝のファンアウト先のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < edge_num() )
  unsigned int
  pg_edge_to(unsigned int id) const;

  /// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
  /// @param[in] id 枝番号 ( 0 <= id < edge_num() )
  unsigned int
  pg_edge_pos(unsigned int id) const;

  /// @brief 枝の反転属性を返す．
  /// @param[in] id 枝番号 ( 0 <= id < edge_num() )
  bool
  pg_edge_inv(unsigned int id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ダンプ/リストア関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  void
  dump(ODO& s) const;

  /// @brief バイナリダンプされた内容を読み込む．
  /// @param[in] s 入力元のストリーム
  void
  restore(IDO& s);

%If ( !DISABLE_SET )
public:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を設定する．
  /// @param[in] name 名前
  void
  set_name(const string& name);

  /// @brief 'technology' を設定する．
  void
  set_technology(tCellTechnology technology);

  /// @brief 遅延モデルを設定する．
  /// @param[in] delay_model 遅延モデル．
  virtual
  void
  set_delay_model(tCellDelayModel delay_model) = 0;

  /// @brief 'capacitive_load_unit' を設定する．
  /// @param[in] unit 単位
  /// @param[in] ustr 単位の後に表示する文字列
  virtual
  void
  set_capacitive_load_unit(double unit,
			   const string& ustr) = 0;

  /// @brief 属性を設定する(浮動小数点型)
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  virtual
  void
  set_attr(const string& attr_name,
	   double value) = 0;

  /// @brief 属性を設定する(文字列型)．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  virtual
  void
  set_attr(const string& attr_name,
	   const string& value) = 0;

  /// @brief 遅延テーブルのテンプレート数を設定する．
  virtual
  void
  set_lu_table_template_num(unsigned int num) = 0;

  /// @brief 1次元の LUT のテンプレートを作る．
  /// @param[in] id ID 番号
  /// @param[in] name 名前
  /// @param[in] var_type1 変数型
  /// @param[in] index_list1 インデックス値のリスト
  virtual
  void
  new_lut_template1(unsigned int id,
		    const string& name,
		    tCellVarType var_type1,
		    const vector<double>& index_list1) = 0;

  /// @brief 2次元の LUT のテンプレートを作る．
  /// @param[in] id ID 番号
  /// @param[in] name 名前
  /// @param[in] var_type1 変数型
  /// @param[in] index_list1 インデックス値のリスト
  /// @param[in] var_type2 変数型
  /// @param[in] index_list2 インデックス値のリスト
  virtual
  void
  new_lut_template2(unsigned int id,
		    const string& name,
		    tCellVarType var_type1,
		    const vector<double>& index_list1,
		    tCellVarType var_type2,
		    const vector<double>& index_list2) = 0;

  /// @brief 3次元の LUT のテンプレートを作る．
  /// @param[in] id ID 番号
  /// @param[in] name 名前
  /// @param[in] var_type1 変数型
  /// @param[in] index_list1 インデックス値のリスト
  /// @param[in] var_type2 変数型
  /// @param[in] index_list2 インデックス値のリスト
  /// @param[in] var_type3 変数型
  /// @param[in] index_list3 インデックス値のリスト
  virtual
  void
  new_lut_template3(unsigned int id,
		    const string& name,
		    tCellVarType var_type1,
		    const vector<double>& index_list1,
		    tCellVarType var_type2,
		    const vector<double>& index_list2,
		    tCellVarType var_type3,
		    const vector<double>& index_list3) = 0;

  /// @brief セル数を設定する．
  /// @param[in] num 設定する値
  virtual
  void
  set_cell_num(unsigned int num) = 0;

  /// @brief セルを取り出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  virtual
  Cell*
  cell(unsigned int pos) = 0;

  /// @brief 論理セルを生成する．
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
  /// @param[in] tristate_array トライステート条件の論理式の配列
  /// *1: - false 論理式なし
  ///     - true 論理式あり
  virtual
  void
  new_logic_cell(unsigned int cell_id,
		 const string& name,
		 CellArea area,
		 unsigned int ni,
		 unsigned int no,
		 unsigned int nio,
		 unsigned int nb,
		 unsigned int nc,
		 const vector<bool>& output_array,
		 const vector<Expr>& logic_array,
		 const vector<Expr>& tristate_array) = 0;

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
  /// @param[in] tristate_array トライステート条件の論理式の配列
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
  new_ff_cell(unsigned int cell_id,
	      const string& name,
	      CellArea area,
	      unsigned int ni,
	      unsigned int no,
	      unsigned int nio,
	      unsigned int nb,
	      unsigned int nc,
	      const vector<bool>& output_array,
	      const vector<Expr>& logic_array,
	      const vector<Expr>& tristate_array,
	      const Expr& next_state,
	      const Expr& clocked_on,
	      const Expr& clocked_on_also,
	      const Expr& clear,
	      const Expr& preset,
	      unsigned int clear_preset_var1,
	      unsigned int clear_preset_var2) = 0;

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
  /// @param[in] tristate_array トライステート条件の論理式の配列
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
  new_latch_cell(unsigned int cell_id,
		 const string& name,
		 CellArea area,
		 unsigned int ni,
		 unsigned int no,
		 unsigned int nio,
		 unsigned int nb,
		 unsigned int nc,
		 const vector<bool>& output_array,
		 const vector<Expr>& logic_array,
		 const vector<Expr>& tristate_array,
		 const Expr& data_in,
		 const Expr& enable,
		 const Expr& enable_also,
		 const Expr& clear,
		 const Expr& preset,
		 unsigned int clear_preset_var1,
		 unsigned int clear_preset_var2) = 0;

  /// @brief FSMセルを生成する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nit 内部ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] output_array 出力の情報の配列(*1)
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] tristate_array トライステート条件の論理式の配列
  /// *1: - false 論理式なし
  ///     - true 論理式あり
  virtual
  void
  new_fsm_cell(unsigned int cell_id,
	       const string& name,
	       CellArea area,
	       unsigned int ni,
	       unsigned int no,
	       unsigned int nio,
	       unsigned int nit,
	       unsigned int nb,
	       unsigned int nc,
	       const vector<bool>& output_array,
	       const vector<Expr>& logic_array,
	       const vector<Expr>& tristate_array) = 0;

  /// @brief セルの入力ピンを生成する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
  /// @param[in] input_id 入力ピン番号 ( 0 <= input_id < cell->input_num2() )
  /// @param[in] name 入力ピン名
  /// @param[in] capacitance 入力ピンの負荷容量
  /// @param[in] rise_capacitance 入力ピンの立ち上がり負荷容量
  /// @param[in] fall_capacitance 入力ピンの立ち下がり負荷容量
  virtual
  void
  new_cell_input(unsigned int cell_id,
		 unsigned int pin_id,
		 unsigned int input_id,
		 const string& name,
		 CellCapacitance capacitance,
		 CellCapacitance rise_capacitance,
		 CellCapacitance fall_capacitance) = 0;

  /// @brief セルの出力ピンの内容を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
  /// @param[in] output_id 出力ピン番号 ( 0 <= output_id < cell->output_num2() )
  /// @param[in] name 出力ピン名
  /// @param[in] has_logic 論理式を持つとき true にするフラグ
  /// @param[in] logic_expr 論理式
  /// @param[in] tristate_expr tristate 条件式
  /// @param[in] max_fanout 最大ファンアウト容量
  /// @param[in] min_fanout 最小ファンアウト容量
  /// @param[in] max_capacitance 最大負荷容量
  /// @param[in] min_capacitance 最小負荷容量
  /// @param[in] max_transition 最大遷移時間
  /// @param[in] min_transition 最小遷移時間
  virtual
  void
  new_cell_output(unsigned int cell_id,
		  unsigned int pin_id,
		  unsigned int output_id,
		  const string& name,
		  bool has_logic,
		  const Expr& logic_expr,
		  const Expr& tristate_expr,
		  CellCapacitance max_fanout,
		  CellCapacitance min_fanout,
		  CellCapacitance max_capacitance,
		  CellCapacitance min_capacitance,
		  CellTime max_transition,
		  CellTime min_transition) = 0;

  /// @brief セルの入出力ピンの内容を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id 入出力ピン番号 ( 0 <= pin_id < cell->pin_num() )
  /// @param[in] input_id 入力ピン番号 ( 0 <= input_id < cell->input_num2() )
  /// @param[in] output_id 出力ピン番号 ( 0 <= output_id < cell->output_num2() )
  /// @param[in] name 入出力ピン名
  /// @param[in] has_logic 論理式を持つとき true にするフラグ
  /// @param[in] logic_expr 論理式
  /// @param[in] tristate_expr tristate 条件式
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
  new_cell_inout(unsigned int cell_id,
		 unsigned int pin_id,
		 unsigned int input_id,
		 unsigned int output_id,
		 const string& name,
		 bool has_logic,
		 const Expr& logic_expr,
		 const Expr& tristate_expr,
		 CellCapacitance capacitance,
		 CellCapacitance rise_capacitance,
		 CellCapacitance fall_capacitance,
		 CellCapacitance max_fanout,
		 CellCapacitance min_fanout,
		 CellCapacitance max_capacitance,
		 CellCapacitance min_capacitance,
		 CellTime max_transition,
		 CellTime min_transition) = 0;

  /// @brief セルの内部ピンを生成する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < cell->pin_num() )
  /// @param[in] int_id 入力ピン番号 ( 0 <= int_id < cell->internal_num() )
  /// @param[in] name 内部ピン名
  virtual
  void
  new_cell_internal(unsigned int cell_id,
		    unsigned int pin_id,
		    unsigned int int_id,
		    const string& name) = 0;

  /// @brief タイミング情報の数を設定する．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] timing_num タイミング情報の数．
  virtual
  void
  set_timing_num(unsigned int cell_id,
		 unsigned int timing_num) = 0;

  /// @brief タイミング情報を作る(ジェネリック遅延モデル)．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] tid タイミングID ( 0 <= tid < cell->timing_num() )
  /// @param[in] type タイミングの型
  /// @param[in] cond タイミング条件を表す式
  /// @param[in] intrinsic_rise 立ち上がり固有遅延
  /// @param[in] intrinsic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  /// @param[in] rise_resistance 立ち上がり負荷依存係数
  /// @param[in] fall_resistance 立ち下がり負荷依存係数
  virtual
  void
  new_timing_generic(unsigned int cell_id,
		     unsigned int tid,
		     tCellTimingType type,
		     const Expr& cond,
		     CellTime intrinsic_rise,
		     CellTime intrinsic_fall,
		     CellTime slope_rise,
		     CellTime slope_fall,
		     CellResistance rise_resistance,
		     CellResistance fall_resistance) = 0;

  /// @brief タイミング情報を作る(折れ線近似)．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] tid タイミングID ( 0 <= tid < cell->timing_num() )
  /// @param[in] timing_type タイミングの型
  /// @param[in] cond タイミング条件を表す式
  /// @param[in] intrinsic_rise 立ち上がり固有遅延
  /// @param[in] intrinsic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  /// @param[in] rise_pin_resistance 立ち上がりピン抵抗
  /// @param[in] fall_pin_resistance 立ち上がりピン抵抗
  virtual
  void
  new_timing_piecewise(unsigned int cell_id,
		       unsigned int tid,
		       tCellTimingType timing_type,
		       const Expr& cond,
		       CellTime intrinsic_rise,
		       CellTime intrinsic_fall,
		       CellTime slope_rise,
		       CellTime slope_fall,
		       CellResistance rise_pin_resistance,
		       CellResistance fall_pin_resistance) = 0;

  /// @brief タイミング情報を作る(非線形タイプ1)．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] tid タイミングID ( 0 <= tid < cell->timing_num() )
  /// @param[in] timing_type タイミングの型
  /// @param[in] cond タイミング条件を表す式
  /// @param[in] cell_rise 立ち上がりセル遅延テーブル
  /// @param[in] cell_fall 立ち下がりセル遅延テーブル
  /// @param[in] rise_transition 立ち上がり遷移遅延テーブル
  /// @param[in] fall_transition 立ち下がり遷移遅延テーブル
  virtual
  void
  new_timing_lut1(unsigned int cell_id,
		  unsigned int tid,
		  tCellTimingType timing_type,
		  const Expr& cond,
		  CellLut* cell_rise,
		  CellLut* cell_fall,
		  CellLut* rise_transition,
		  CellLut* fall_transition) = 0;

  /// @brief タイミング情報を作る(非線形タイプ2)．
  /// @param[in] cell_id セル番号 ( 0 <= cell_id < cell_num() )
  /// @param[in] tid タイミングID ( 0 <= tid < cell->timing_num() )
  /// @param[in] timing_type タイミングの型
  /// @param[in] cond タイミング条件を表す式
  /// @param[in] rise_transition 立ち上がり遷移遅延テーブル
  /// @param[in] fall_transition 立ち下がり遷移遅延テーブル
  /// @param[in] rise_propagation 立ち上がり伝搬遅延テーブル
  /// @param[in] fall_propagation 立ち下がり伝搬遅延テーブル
  virtual
  void
  new_timing_lut2(unsigned int cell_id,
		  unsigned int tid,
		  tCellTimingType timing_type,
		  const Expr& cond,
		  CellLut* rise_transition,
		  CellLut* fall_transition,
		  CellLut* rise_propagation,
		  CellLut* fall_propagation) = 0;

  /// @brief タイミング情報をセットする．
  /// @param[in] cell_id セルID
  /// @param[in] input_id 入力ピンID
  /// @param[in] output_id 出力ピンID
  /// @param[in] timing_sense タイミングセンス
  /// @param[in] tid_list タイミングIDのリスト
  virtual
  void
  set_timing(unsigned int cell_id,
	     unsigned int input_id,
	     unsigned int output_id,
	     tCellTimingSense timing_sense,
	     const vector<unsigned int>& tid_list) = 0;

  /// @brief 1次元の LUT を作る．
  /// @param[in] lut_template テンプレート
  /// @param[in] value_array 値の配列
  /// @param[in] index_array インデックス値のリスト
  virtual
  CellLut*
  new_lut1(const CellLutTemplate* lut_template,
	   const vector<double>& value_array,
	   const vector<double>& index_array = vector<double>()) = 0;

  /// @brief 2次元の LUT を作る．
  /// @param[in] lut_template テンプレート
  /// @param[in] value_array 値の配列
  /// @param[in] index_array1 インデックス値のリスト
  /// @param[in] index_array2 インデックス値のリスト
  virtual
  CellLut*
  new_lut2(const CellLutTemplate* lut_template,
	   const vector<double>& value_array,
	   const vector<double>& index_array1 = vector<double>(),
	   const vector<double>& index_array2 = vector<double>()) = 0;

  /// @brief 3次元の LUT を作る．
  /// @param[in] lut_template テンプレート
  /// @param[in] value_array 値の配列
  /// @param[in] index_array1 インデックス値のリスト
  /// @param[in] index_array2 インデックス値のリスト
  /// @param[in] index_array3 インデックス値のリスト
  virtual
  CellLut*
  new_lut3(const CellLutTemplate* lut_template,
	   const vector<double>& value_array,
	   const vector<double>& index_array1 = vector<double>(),
	   const vector<double>& index_array2 = vector<double>(),
	   const vector<double>& index_array3 = vector<double>()) = 0;

  /// @brief セルのグループ分けを行う．
  /// @note 論理セルのパタングラフも作成する．
  virtual
  void
  compile() = 0;
%End

private:

  ~CellLibrary();

};
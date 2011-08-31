#ifndef YM_CELL_CELLLIBRARY_H
#define YM_CELL_CELLLIBRARY_H

/// @file　ym_cell/CellLibrary.h
/// @brief CellLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellLibrary CellLibrary.h "ym_cell/CellLibrary.h"
/// @brief セルライブラリを表すクラス
//////////////////////////////////////////////////////////////////////
class CellLibrary
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @brief テクノロジを表す列挙型
  //////////////////////////////////////////////////////////////////////
  enum tTechnology {
    kTechAsic,
    kTechFpga
  };

  //////////////////////////////////////////////////////////////////////
  /// @brief 遅延モデルを表す列挙型
  //////////////////////////////////////////////////////////////////////
  enum tDelayModel {
    kDelayGeneric,
    kDelayPiecewise,
    kDelayNonlinear
  };

public:

  /// @brief コンストラクタ
  CellLibrary() { }

  /// @brief デストラクタ
  virtual
  ~CellLibrary() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const = 0;

  /// @brief テクノロジの取得
  /// 返り値は
  /// - kTechAsic
  /// - kTechFpga
  /// のどちらか
  virtual
  tTechnology
  technology() const = 0;

  /// @brief 遅延モデルの取得
  /// 返り値は
  /// - kDelayGeneric
  /// - kDelayPiecewise
  /// - kDelayNonlinear
  /// のいずれか
  virtual
  tDelayModel
  delay_model() const = 0;

  /// @brief バス命名規則の取得
  virtual
  string
  bus_naming_style() const = 0;

  /// @brief 日付情報の取得
  virtual
  string
  date() const = 0;

  /// @brief リビジョン情報の取得
  virtual
  string
  revision() const = 0;

  /// @brief コメント情報の取得
  virtual
  string
  comment() const = 0;

  /// @brief 時間単位の取得
  virtual
  string
  time_unit() const = 0;

  /// @brief 電圧単位の取得
  virtual
  string
  voltage_unit() const = 0;

  /// @brief 電流単位の取得
  virtual
  string
  current_unit() const = 0;

  /// @brief 抵抗単位の取得
  virtual
  string
  pulling_resistance_unit() const = 0;

  /// @brief 容量単位の取得
  virtual
  string
  capacitive_load_unit() const = 0;

  /// @brief 電力単位の取得
  virtual
  string
  leakage_power_unit() const = 0;

  /// @brief ルックアップテーブルのテンプレートの取得
  /// @param[in] name テンプレート名
  /// @note なければ NULL を返す．
  virtual
  const CellLutTemplate*
  lu_table_template(const char* name) const = 0;

  /// @brief バスタイプの取得
  /// @param[in] name バスタイプ名
  /// @note なければ NULL を返す．
  virtual
  const CellBusType*
  bus_type(const char* name) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief このライブラリの持つセル数の取得
  virtual
  ymuint
  cell_num() const = 0;

  /// @brief セルの取得
  /// @param[in] pos 位置番号( 0 <= pos < cell_num() )
  virtual
  const Cell*
  cell(ymuint pos) const = 0;

  /// @brief 名前からのセルの取得
  virtual
  const Cell*
  cell(const char* name) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理セルの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理セルクラスの個数を返す．
  virtual
  ymuint
  logic_class_num() const = 0;

  /// @brief 論理セルクラスを返す．
  /// @param[in] id クラス番号 ( 0 <= id < logic_class_num() )
  virtual
  const CellClass&
  logic_class(ymuint id) const = 0;

  /// @brief 論理セルグループの個数を返す．
  virtual
  ymuint
  logic_group_num() const = 0;

  /// @brief 論理セルグループを返す．
  /// @param[in] id グループ番号　( 0 <= id < logic_group_num() )
  virtual
  const CellGroup&
  logic_group(ymuint id) const = 0;

  /// @brief 定数0セルのグループを返す．
  virtual
  const CellGroup&
  const0_func() const = 0;

  /// @brief 定数1セルのグループを返す．
  virtual
  const CellGroup&
  const1_func() const = 0;

  /// @brief バッファセルのグループを返す．
  virtual
  const CellGroup&
  buf_func() const = 0;

  /// @brief インバータセルのグループを返す．
  virtual
  const CellGroup&
  inv_func() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // FFセルの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief FFセルクラスの個数を返す．
  virtual
  ymuint
  ff_class_num() const = 0;

  /// @brief FFセルクラスを返す．
  /// @param[in] id クラス番号 ( 0 <= id < ff_class_num() )
  virtual
  const CellClass&
  ff_class(ymuint id) const = 0;

  /// @brief FFセルグループの個数を返す．
  virtual
  ymuint
  ff_group_num() const = 0;

  /// @brief FFセルグループを返す．
  /// @param[in] id グループ番号 ( 0 <= id < ff_group_num() )
  virtual
  const CellGroup&
  ff_group(ymuint id) const = 0;

  /// @brief 単純な型のFFクラスを返す．
  /// @param[in] has_clear クリア端子を持つとき true にする．
  /// @param[in] has_preset プリセット端子を持つとき true にする．
  /// @note 該当するセルがないときでも空のセルクラスが返される．
  virtual
  const CellClass&
  simple_ff_class(bool has_clear,
		  bool has_preset) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ラッチセルの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチセルクラスの個数を返す．
  virtual
  ymuint
  latch_class_num() const = 0;

  /// @brief ラッチセルクラスを返す．
  /// @param[in] id クラス番号 ( 0 <= id < latch_class_num() )
  virtual
  const CellClass&
  latch_class(ymuint id) const = 0;

  /// @brief ラッチセルグループの個数を返す．
  virtual
  ymuint
  latch_group_num() const = 0;

  /// @brief ラッチセルグループを返す．
  /// @param[in] id グループ番号 ( 0 <= id < latch_group_num() )
  virtual
  const CellGroup&
  latch_group(ymuint id) const = 0;

  /// @brief 単純な型のラッチクラスを返す．
  /// @param[in] has_clear クリア端子を持つとき true にする．
  /// @param[in] has_preset プリセット端子を持つとき true にする．
  /// @note 該当するセルがないときでも空のセルクラスが返される．
  virtual
  const CellClass&
  simple_latch_class(bool has_clear,
		     bool has_preset) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // パタングラフ関係の情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 総パタン数を返す．
  virtual
  ymuint
  pat_num() const = 0;

  /// @brief パタンを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  virtual
  const CellPatGraph&
  pat(ymuint id) const = 0;

  /// @brief パタンの最大の入力数を得る．
  virtual
  ymuint
  max_input() const = 0;

  /// @brief 総ノード数を返す．
  virtual
  ymuint
  node_num() const = 0;

  /// @brief ノードの種類を返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  virtual
  tCellPatType
  node_type(ymuint id) const = 0;

  /// @brief ノードが入力ノードの時に入力番号を返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  /// @note 入力ノードでない場合の返り値は不定
  virtual
  ymuint
  input_id(ymuint id) const = 0;

  /// @brief 入力のノード番号を返す．
  /// @param[in] input_id 入力番号 ( 0 <= input_id < input_num() )
  /// @return input_id の入力に対応するノードのノード番号
  virtual
  ymuint
  input_node(ymuint input_id) const = 0;

  /// @brief 総枝数を返す．
  virtual
  ymuint
  edge_num() const = 0;

  /// @brief 枝のファンイン元のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  virtual
  ymuint
  edge_from(ymuint id) const = 0;

  /// @brief 枝のファンアウト先のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  virtual
  ymuint
  edge_to(ymint id) const = 0;

  /// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  virtual
  ymuint
  edge_pos(ymuint id) const = 0;

  /// @brief 枝の反転属性を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  virtual
  bool
  edge_inv(ymuint id) const = 0;

};


/// @relates CellLibrary
/// @brief 内容をバイナリダンプする．
void
dump_library(ostream& s,
	     const CellLibrary& library);

/// @relates CellLibrary
/// @brief 内容を出力する．
void
display_library(ostream& s,
		const CellLibrary& library);

/// @relates CellLibrary
/// @brief バイナリダンプされた内容を読み込む．
const CellLibrary*
restore_library(istream& s);

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLLIBRARY_H

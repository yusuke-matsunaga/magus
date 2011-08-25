#ifndef YM_CELL_CELL_H
#define YM_CELL_CELL_H

/// @file　ym_cell/Cell.h
/// @brief Cell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "ym_cell/cell_nsdef.h"
#include "ym_cell/CellArea.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class Cell Cell.h "ym_cell/Cell.h"
/// @brief セル本体のクラス
//////////////////////////////////////////////////////////////////////
class Cell
{
public:

  /// @brief コンストラクタ．
  Cell() { }

  /// @brief デストラクタ
  virtual
  ~Cell() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 基本情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号の取得
  /// @note ここで返される番号は CellLibrary::cell() の引数に対応する．
  virtual
  ymuint
  id() const = 0;

  /// @brief 名前の取得
  virtual
  string
  name() const = 0;

  /// @brief 面積の取得
  virtual
  CellArea
  area() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ピン情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン数の取得
  virtual
  ymuint
  input_num() const = 0;

  /// @brief 入力ピン+入出力ピン数の取得
  /// @note input_num() + inout_num() に等しい．
  virtual
  ymuint
  input_num2() const = 0;

  /// @brief 入力ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
  /// @note pos >= input_num() の場合には入出力ピンが返される．
  virtual
  const CellPin*
  input(ymuint pos) const = 0;

  /// @brief 出力ピン数の取得
  virtual
  ymuint
  output_num() const = 0;

  /// @brief 出力ピン+入出力ピン数の取得
  /// @note output_num() + inout_num() に等しい．
  virtual
  ymuint
  output_num2() const = 0;

  /// @brief 出力ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
  /// @note pos >= output_num() の場合には入出力ピンが返される．
  virtual
  const CellPin*
  output(ymuint pos) const = 0;

  /// @brief 入出力ピン数の取得
  virtual
  ymuint
  inout_num() const = 0;

  /// @brief 入出力ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < inout_num() )
  virtual
  const CellPin*
  inout(ymuint pos) const = 0;

  /// @brief 名前からピンの取得
  /// @param[in] name ピン名
  /// @return name という名前をピンを返す．
  /// @note なければ NULL を返す．
  virtual
  const CellPin*
  pin(const string& name) const = 0;

  /// @brief バス数の取得
  virtual
  ymuint
  bus_num() const = 0;

  /// @brief バスの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < bus_num() )
  virtual
  const CellBus*
  bus(ymuint pos) const = 0;

  /// @brief 名前からバスの取得
  /// @param[in] name バス名
  /// @return name という名前のバスを返す．
  /// @note なければ NULL を返す．
  virtual
  const CellBus*
  bus(const string& name) const = 0;

  /// @brief バンドル数の取得
  virtual
  ymuint
  bundle_num() const = 0;

  /// @brief バンドルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < bundle_num() )
  virtual
  const CellBundle*
  bundle(ymuint pos) const = 0;

  /// @brief 名前からバンドルの取得
  virtual
  const CellBundle*
  bundle(const string& name) const = 0;

  /// @brief 属している CellGroup を返す．
  virtual
  const CellGroup*
  cell_group() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // タイミング情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief タイミング情報の取得
  /// @param[in] ipos 開始ピン番号 ( 0 <= ipos < input_num2() )
  /// @param[in] opos 終了ピン番号 ( 0 <= opos < output_num2() )
  /// @param[in] timing_sense タイミング情報の摘要条件
  /// @return 条件に合致するタイミング情報を返す．
  /// @note なければ NULL を返す．
  virtual
  const CellTiming*
  timing(ymuint ipos,
	 ymuint opos,
	 tCellTimingSense sense) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

#if 0
  /// @brief 組み合わせ論理セルの時に true を返す．
  bool
  is_logic() const;

  /// @brief トライステートセルの場合に true を返す．
  /// @note もちろん論理セルでもある．
  /// @note 複数出力のうち1つでもトライステートなら true を返す．
  bool
  is_tristate() const;

  /// @brief FFセルの時に true を返す．
  bool
  is_ff() const;

  /// @brief ラッチセルの時に true を返す．
  bool
  is_latch() const;

  /// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
  bool
  is_seq() const;

  /// @brief 論理セルの場合に出力の論理式を返す．
  /// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
  /// @note 論理式中の変数番号は入力ピン番号に対応する．
  LogExpr
  logic_function(ymuint pin_id) const;

  /// @brief 出力がトライステートの時に true を返す．
  /// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
  bool
  is_tristate(ymuint pin_id) const;

  /// @brief トライステートセルの場合にトライステート条件式を返す．
  /// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
  /// @note 論理式中の変数番号は入力ピン番号に対応する．
  /// @note is_tristate() が true の時のみ意味を持つ．
  /// @note 通常の論理セルの場合には定数0を返す．
  LogExpr
  tristate_function(ymuint pin_id) const;

  /// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
  bool
  has_clear() const;

  /// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
  bool
  has_preset() const;

  /// @brief FF/ラッチセルの場合にデータ入力ピン番号を返す．
  ymuint
  data_pos() const;

  /// @brief FF/ラッチセルの場合にデータ入力ピンの反転属性を返す．
  bool
  data_inv() const;

  /// @brief FFセルの場合にクロック入力ピン番号を返す．
  ymuint
  clock_pos() const;

  /// @brief FFセルの場合にクロック入力ピンの反転属性を返す．
  /// @note ポジティブエッジが正極性だと思う．
  bool
  clock_inv() const;

  /// @brief ラッチセルの場合にイネーブル入力ピン番号を返す．
  ymuint
  enable_pos() const;

  /// @brief ラッチセルの場合にイネーブル入力ピンの反転属性を返す．
  bool
  enable_inv() const;

  /// @brief FF/ラッチセルの場合にクリア入力ピン番号を返す．
  ymuint
  clear_pos() const;

  /// @brief FF/ラッチセルの場合にクリア入力ピンの反転属性を返す．
  bool
  clear_inv() const;

  /// @brief FF/ラッチセルの場合にプリセット入力ピン番号を返す．
  ymuint
  clear_pos() const;

  /// @brief FF/ラッチセルの場合にプリセット入力ピンの反転属性を返す．
  bool
  clear_inv() const;

  /// @brief FF/ラッチセルの場合に Q出力ピン番号を返す．
  ymuint
  q_pos() const;

  /// @brief FF/ラッチセルの場合に XQ出力ピン番号を返す．
  ymuint
  xq_pos() const;
#endif

#if 0
  /// @brief 状態変数1の名前を返す．
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  string
  var1_name() const = 0;

  /// @brief 状態変数2の名前を返す．
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  string
  var2_name() const = 0;

  /// @brief next_state 関数の取得
  /// @note FFセルの時に意味を持つ．
  virtual
  LogExpr
  next_state() const = 0;

  /// @brief clocked_on 関数の取得
  /// @note FFセルの時に意味を持つ．
  virtual
  LogExpr
  clocked_on() const = 0;

  /// @brief clocked_on_also 関数の取得
  /// @note FFセルの時に意味を持つ．
  virtual
  LogExpr
  clocked_on_also() const = 0;

  /// @brief data_in 関数の取得
  virtual
  LogExpr
  data_in() const = 0;

  /// @brief enable 関数の取得
  virtual
  LogExpr
  enable() const = 0;

  /// @brief enable_also 関数の取得
  virtual
  LogExpr
  enable_also() const = 0;

  /// @brief clear 関数の取得
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  LogExpr
  clear() const = 0;

  /// @brief preset 関数の取得
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  LogExpr
  preset() const = 0;

  /// @brief clear_preset_var1 の取得
  /// @retval 0 "L"
  /// @retval 1 "H"
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  ymuint
  clear_preset_var1() const = 0;

  /// @brief clear_preset_var2 の取得
  /// @retval 0 "L"
  /// @retval 1 "H"
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  ymuint
  clear_preset_var2() const = 0;
#endif

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELL_H

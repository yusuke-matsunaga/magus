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
  // セル情報の取得
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

  /// @brief 論理セルの場合に出力ピン数を返す．
  ymuint
  output_num() const;

  /// @brief 論理セルの場合に関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note 非論理セルの場合の返り値は不定
  const TvFunc&
  logic_function(ymuint pos) const;

  /// @brief トライステートセルの場合にトライステート条件関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note 通常の論理セルの場合には定数0関数を返す．
  const TvFunc&
  tristate_function(ymuint pos) const;

  /// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
  bool
  has_clear() const;

  /// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
  bool
  has_preset() const;

  /// @brief 論理セルの場合に入力ピン番号を返す．
  /// @param[in] pos CellClass の関数における入力番号
  ymuint
  input_pos(ymuint pos) const;

  /// @brief 論理セルの場合に入力ピンの反転属性を返す．
  /// @param[in] pos CellClass の関数における入力番号
  bool
  input_inv(ymuint pos) const;

  /// @brief 論理セルの場合に出力ピン番号を返す．
  /// @param[in] pos CellClass の関数における出力番号
  ymuint
  output_pos(ymuint pos) const;

  /// @brief 論理セルの場合に出力ピンの反転属性を返す．
  /// @param[in] pos CellClass の関数における出力番号
  bool
  output_inv(ymuint pos) const;

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

  /// @brief ピン数の取得
  virtual
  ymuint
  pin_num() const = 0;

  /// @brief ピンの取得
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < pin_num()
  virtual
  const CellPin*
  pin(ymuint pin_id) const = 0;

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

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 組み合わせ論理セルの時に true を返す．
inline
bool
Cell::is_logic() const
{
  return cell_group()->is_logic();
}

// @brief トライステートセルの場合に true を返す．
// @note もちろん論理セルでもある．
// @note 複数出力のうち1つでもトライステートなら true を返す．
inline
bool
Cell::is_tristate() const
{
  return cell_group()->is_tristate();
}

// @brief FFセルの時に true を返す．
inline
bool
Cell::is_ff() const
{
  return cell_group()->is_ff();
}

// @brief ラッチセルの時に true を返す．
inline
bool
Cell::is_latch() const
{
  return cell_group()->is_latch();
}

// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
inline
bool
Cell::is_seq() const
{
  return cell_group()->is_seq();
}

// @brief 論理セルの場合に出力ピン数を返す．
inline
ymuint
Cell::output_num() const
{
  return cell_group()->output_num();
}

// @brief 論理セルの場合に関数を返す．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
// @note 非論理セルの場合の返り値は不定
inline
const TvFunc&
Cell::logic_function(ymuint pos) const
{
  return cell_group()->logic_function(pos);
}

// @brief トライステートセルの場合にトライステート条件関数を返す．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
// @note 通常の論理セルの場合には定数0関数を返す．
inline
const TvFunc&
Cell::tristate_function(ymuint pos) const
{
  return cell_group()->tristate_function(pos);
}

// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
inline
bool
Cell::has_clear() const
{
  return cell_group()->has_clear();
}

// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
inline
bool
Cell::has_preset() const
{
  return cell_group()->has_preset();
}

// @brief 論理セルの場合に入力ピン番号を返す．
// @param[in] pos CellClass の関数における入力番号
inline
ymuint
Cell::input_pos(ymuint pos) const
{
  return cell_group()->input_pos(pos);
}

// @brief 論理セルの場合に出力ピン番号を返す．
// @param[in] pos CellClass の関数における出力番号
inline
ymuint
Cell::output_pos(ymuint pos) const
{
  return cell_group()->output_pos(pos);
}

// @brief FF/ラッチセルの場合にデータ入力ピン番号を返す．
inline
ymuint
Cell::data_pos() const
{
  return cell_group()->data_pos();
}

// @brief FF/ラッチセルの場合にデータ入力ピンの反転属性を返す．
inline
bool
Cell::data_inv() const
{
  return cell_group()->data_inv();
}

// @brief FFセルの場合にクロック入力ピン番号を返す．
inline
ymuint
Cell::clock_pos() const
{
  return cell_group()->clock_pos();
}

// @brief FFセルの場合にクロック入力ピンの反転属性を返す．
// @note ポジティブエッジが正極性だと思う．
inline
bool
Cell::clock_inv() const
{
  return cell_group()->clock_inv();
}

// @brief ラッチセルの場合にイネーブル入力ピン番号を返す．
inline
ymuint
Cell::enable_pos() const
{
  return cell_group()->enable_pos();
}

// @brief ラッチセルの場合にイネーブル入力ピンの反転属性を返す．
inline
bool
Cell::enable_inv() const
{
  return cell_group()->enable_inv();
}

// @brief FF/ラッチセルの場合にクリア入力ピン番号を返す．
inline
ymuint
Cell::clear_pos() const
{
  return cell_group()->clear_pos();
}

// @brief FF/ラッチセルの場合にクリア入力ピンの反転属性を返す．
inline
bool
Cell::clear_inv() const
{
  return cell_group()->clear_inv();
}

// @brief FF/ラッチセルの場合にプリセット入力ピン番号を返す．
inline
ymuint
Cell::clear_pos() const
{
  return cell_group()->clear_pos();
}

// @brief FF/ラッチセルの場合にプリセット入力ピンの反転属性を返す．
inline
bool
Cell::clear_inv() const
{
  return cell_group()->clear_inv();
}

// @brief FF/ラッチセルの場合に Q出力ピン番号を返す．
inline
ymuint
Cell::q_pos() const
{
  return cell_group()->q_pos();
}

// @brief FF/ラッチセルの場合に XQ出力ピン番号を返す．
inline
ymuint
Cell::xq_pos() const
{
  return cell_group()->xq_pos();
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELL_H

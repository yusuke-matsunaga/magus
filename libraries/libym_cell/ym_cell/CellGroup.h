#ifndef YM_CELL_CELLGROUP_H
#define YM_CELL_CELLGROUP_H

/// @file ym_cell/CellGroup.h
/// @brief CellGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellGroup CellGroup.h "ym_cell/CellGroup.h"
/// @brief 論理的に等価なセルのグループを表すクラス
//////////////////////////////////////////////////////////////////////
class CellGroup
{
public:

  /// @brief コンストラクタ
  CellGroup() { }

  /// @brief デストラクタ
  virtual
  ~CellGroup() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属している CellClass を返す．
  virtual
  const CellClass*
  cell_class() const = 0;

  /// @brief 論理セルの場合に true を返す．
  bool
  is_logic() const;

  /// @brief トライステートセルの場合に true を返す．
  /// @note もちろん論理セルでもある．
  /// @note 複数出力のうち1つでもトライステートなら true を返す．
  bool
  is_tristate() const;

  /// @brief FFセルの場合に true を返す．
  bool
  is_ff() const;

  /// @brief ラッチセルの場合に true を返す．
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
  virtual
  const TvFunc&
  logic_function(ymuint pos) const = 0;

  /// @brief トライステートセルの場合にトライステート条件関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note 通常の論理セルの場合には定数0関数を返す．
  virtual
  const TvFunc&
  tristate_function(ymuint pos) const = 0;

  /// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
  bool
  has_clear() const;

  /// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
  bool
  has_preset() const;

  /// @brief 論理セルの場合に入力ピン番号を返す．
  /// @param[in] pos CellClass の関数における入力番号
  virtual
  ymuint
  input_pos(ymuint pos) const = 0;

  /// @brief 論理セルの場合に入力ピンの反転属性を返す．
  /// @param[in] pos CellClass の関数における入力番号
  virtual
  bool
  input_inv(ymuint pos) const = 0;

  /// @brief 論理セルの場合に出力ピン番号を返す．
  /// @param[in] pos CellClass の関数における出力番号
  virtual
  ymuint
  output_pos(ymuint pos) const = 0;

  /// @brief 論理セルの場合に出力ピンの反転属性を返す．
  /// @param[in] pos CellClass の関数における出力番号
  virtual
  bool
  output_inv(ymuint pos) const = 0;

  /// @brief FF/ラッチセルの場合にデータ入力ピン番号を返す．
  virtual
  ymuint
  data_pos() const = 0;

  /// @brief FF/ラッチセルの場合にデータ入力ピンの反転属性を返す．
  virtual
  bool
  data_inv() const = 0;

  /// @brief FFセルの場合にクロック入力ピン番号を返す．
  virtual
  ymuint
  clock_pos() const = 0;

  /// @brief FFセルの場合にクロック入力ピンの反転属性を返す．
  /// @note ポジティブエッジが正極性だと思う．
  virtual
  bool
  clock_inv() const = 0;

  /// @brief ラッチセルの場合にイネーブル入力ピン番号を返す．
  virtual
  ymuint
  enable_pos() const = 0;

  /// @brief ラッチセルの場合にイネーブル入力ピンの反転属性を返す．
  virtual
  bool
  enable_inv() const = 0;

  /// @brief FF/ラッチセルの場合にクリア入力ピン番号を返す．
  virtual
  ymuint
  clear_pos() const = 0;

  /// @brief FF/ラッチセルの場合にクリア入力ピンの反転属性を返す．
  virtual
  bool
  clear_inv() const = 0;

  /// @brief FF/ラッチセルの場合にプリセット入力ピン番号を返す．
  virtual
  ymuint
  clear_pos() const = 0;

  /// @brief FF/ラッチセルの場合にプリセット入力ピンの反転属性を返す．
  virtual
  bool
  clear_inv() const = 0;

  /// @brief FF/ラッチセルの場合に Q出力ピン番号を返す．
  virtual
  ymuint
  q_pos() const = 0;

  /// @brief FF/ラッチセルの場合に XQ出力ピン番号を返す．
  virtual
  ymuint
  xq_pos() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // このグループに属しているセルの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セル数を返す．
  virtual
  ymuint
  cell_num() const = 0;

  /// @brief セルを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  virtual
  const Cell*
  cell(ymuint pos) const = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 論理セルの場合に true を返す．
inline
bool
CellGroup::is_logic() const
{
  return cell_class()->is_logic();
}

// @brief トライステートセルの場合に true を返す．
// @note もちろん論理セルでもある．
// @note 複数出力のうち1つでもトライステートなら true を返す．
inline
bool
CellGroup::is_tristate() const
{
  return cell_class()->is_tristate();
}

// @brief FFセルの場合に true を返す．
inline
bool
CellGroup::is_ff() const
{
  return cell_class()->is_ff();
}

// @brief ラッチセルの場合に true を返す．
inline
bool
CellGroup::is_latch() const
{
  return cell_class()->is_latch();
}

// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
inline
bool
CellGroup::is_seq() const
{
  return cell_class()->is_seq();
}

// @brief 論理セルの場合に出力ピン数を返す．
inline
ymuint
CellGroup::output_num() const
{
  return cell_class()->output_num();
}

// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
inline
bool
CellGroup::has_clear() const
{
  return cell_class()->has_clear();
}

// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
inline
bool
CellGroup::has_preset() const
{
  return cell_class()->has_preset();
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLGROUP_H

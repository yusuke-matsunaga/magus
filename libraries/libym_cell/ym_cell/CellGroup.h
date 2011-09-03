#ifndef YM_CELL_CELLGROUP_H
#define YM_CELL_CELLGROUP_H

/// @file ym_cell/CellGroup.h
/// @brief CellGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_cell/CellClass.h"
#include "ym_logic/TvFunc.h"


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

  /// @brief 入力ピン数の取得
  virtual
  ymuint
  input_num() const = 0;

  /// @brief 出力ピン数の取得
  virtual
  ymuint
  output_num() const = 0;

  /// @brief 入出力ピン数の取得
  virtual
  ymuint
  inout_num() const = 0;

  /// @brief 入力ピンの割り当てを得る．
  /// @param[in] ipos クラスの代表セルの入力ピン番号 ( 0 <= ipos < input_num() )
  /// @return ipos に対応するこのグループのセルのピン番号を返す．
  virtual
  ymuint
  input_map(ymuint ipos) const = 0;

  /// @brief 入力ピンの極性を得る．
  /// @param[in] ipos クラスの代表セルの入力ピン番号 ( 0 <= ipos < input_num() )
  /// @return ipos に対応するこのグループのセルのピンとの極性差を返す．
  /// @note false で同位相，true で逆位相という意味
  virtual
  bool
  input_inv(ymuint ipos) const = 0;

  /// @brief 出力ピンの割り当てを得る．
  /// @param[in] opos クラスの代表セルの入力ピン番号 ( 0 <= opos < output_num() )
  /// @return opos に対応するこのグループのセルのピン番号を返す．
  virtual
  ymuint
  output_map(ymuint opos) const = 0;

  /// @brief 出力ピンの極性を得る．
  /// @param[in] opos クラスの代表セルの入力ピン番号 ( 0 <= opos < output_num() )
  /// @return opos に対応するこのグループのセルのピンとの極性差を返す．
  /// @note false で同位相，true で逆位相という意味
  virtual
  bool
  output_inv(ymuint ipos) const = 0;

  /// @brief 論理セルの場合に true を返す．
  virtual
  bool
  is_logic() const = 0;

  /// @brief トライステートセルの場合に true を返す．
  /// @note もちろん論理セルでもある．
  /// @note 複数出力のうち1つでもトライステートなら true を返す．
  virtual
  bool
  is_tristate() const = 0;

  /// @brief FFセルの場合に true を返す．
  virtual
  bool
  is_ff() const = 0;

  /// @brief ラッチセルの場合に true を返す．
  virtual
  bool
  is_latch() const = 0;

  /// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
  virtual
  bool
  is_seq() const = 0;

  /// @brief 出力の論理関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note FF/ラッチの場合は状態変数の変数(Q, XQ)が2つ入力に加わる．
  virtual
  TvFunc
  logic_function(ymuint pos) const = 0;

  /// @brief 出力のトライステート条件関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note トライステートでない場合には定数0関数を返す．
  virtual
  TvFunc
  tristate_function(ymuint pos) const = 0;

  /// @brief FFセルの場合に次状態関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  next_state_function() const = 0;

  /// @brief FFセルの場合にクロックのアクティブエッジを表す関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  clock_function() const = 0;

  /// @brief ラッチセルの場合にデータ入力関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  data_in_function() const = 0;

  /// @brief ラッチセルの場合にイネーブル条件を表す関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  enable_function() const = 0;

  /// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
  virtual
  bool
  has_clear() const = 0;

  /// @brief FFセル/ラッチセルの場合にクリア条件を表す関数を返す．
  /// @note クリア端子がない場合の返り値は不定
  virtual
  TvFunc
  clear_function() const = 0;

  /// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
  virtual
  bool
  has_preset() const = 0;

  /// @brief FFセル/ラッチセルの場合にプリセット条件を表す関数を返す．
  /// @note プリセット端子がない場合の返り値は不定
  virtual
  TvFunc
  preset_function() const = 0;


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

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLGROUP_H

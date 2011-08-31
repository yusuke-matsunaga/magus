#ifndef CIGROUP_H
#define CIGROUP_H

/// @file CiGroup.h
/// @brief CiGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiGroup CiGroup.h "CiGroup.h"
/// @brief CellGroup の実装クラス
//////////////////////////////////////////////////////////////////////
class CiGroup :
  public CellGroup
{
  friend class CellMgr;

protected:

  /// @brief コンストラクタ
  CiGroup();

  /// @brief デストラクタ
  virtual
  ~CiGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属している CellClass を返す．
  virtual
  const CellClass*
  cell_class() const;

  /// @brief 入力ピン数の取得
  virtual
  ymuint
  input_num() const;

  /// @brief 出力ピン数の取得
  virtual
  ymuint
  output_num() const;

  /// @brief 入出力ピン数の取得
  virtual
  ymuint
  inout_num() const;

  /// @brief 入力ピンの割り当てを得る．
  /// @param[in] ipos クラスの代表セルの入力ピン番号 ( 0 <= ipos < input_num() )
  /// @return ipos に対応するこのグループのセルのピン番号を返す．
  virtual
  ymuint
  input_map(ymuint ipos) const;

  /// @brief 入力ピンの極性を得る．
  /// @param[in] ipos クラスの代表セルの入力ピン番号 ( 0 <= ipos < input_num() )
  /// @return ipos に対応するこのグループのセルのピンとの極性差を返す．
  /// @note false で同位相，true で逆位相という意味
  virtual
  bool
  input_inv(ymuint ipos) const;

  /// @brief 出力ピンの割り当てを得る．
  /// @param[in] opos クラスの代表セルの入力ピン番号 ( 0 <= opos < output_num() )
  /// @return opos に対応するこのグループのセルのピン番号を返す．
  virtual
  ymuint
  output_map(ymuint opos) const;

  /// @brief 出力ピンの極性を得る．
  /// @param[in] opos クラスの代表セルの入力ピン番号 ( 0 <= opos < output_num() )
  /// @return opos に対応するこのグループのセルのピンとの極性差を返す．
  /// @note false で同位相，true で逆位相という意味
  virtual
  bool
  output_inv(ymuint ipos) const;

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

  /// @brief 出力の論理関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note FF/ラッチの場合は状態変数の変数(Q, XQ)が2つ入力に加わる．
  virtual
  TvFunc
  logic_function(ymuint pos) const;

  /// @brief 出力のトライステート条件関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note トライステートでない場合には定数0関数を返す．
  virtual
  TvFunc
  tristate_function(ymuint pos) const;

  /// @brief FFセルの場合に次状態関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  next_state_function() const;

  /// @brief FFセルの場合にクロックのアクティブエッジを表す関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  clock_function() const;

  /// @brief ラッチセルの場合にデータ入力関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  data_in_function() const;

  /// @brief ラッチセルの場合にイネーブル条件を表す関数を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  TvFunc
  enable_function() const;

  /// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
  virtual
  bool
  has_clear() const;

  /// @brief FFセル/ラッチセルの場合にクリア条件を表す関数を返す．
  /// @note クリア端子がない場合の返り値は不定
  virtual
  TvFunc
  clear_function() const;

  /// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
  virtual
  bool
  has_preset() const;

  /// @brief FFセル/ラッチセルの場合にプリセット条件を表す関数を返す．
  /// @note プリセット端子がない場合の返り値は不定
  virtual
  TvFunc
  preset_function() const;


public:
  //////////////////////////////////////////////////////////////////////
  // このグループに属しているセルの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セル数を返す．
  virtual
  ymuint
  cell_num() const;

  /// @brief セルを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  virtual
  const Cell*
  cell(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属している CellClass
  const CellClass* mCellClass;

  // セル数
  ymuint32 mCellNum;

  // セルのポインタの配列
  const Cell** mCellList;

};

END_NAMESPACE_YM_CELL

#endif // CIGROUP_H

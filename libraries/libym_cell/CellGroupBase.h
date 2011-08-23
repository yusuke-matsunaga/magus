#ifndef CELLGROUPBASE_H
#define CELLGROUPBASE_H

/// @file CellGroupImpl.h
/// @brief CellGroupImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellGroupImpl CellGroupImpl.h "ym_cell/CellGroupImpl.h"
/// @brief CellGroup の実装クラス
//////////////////////////////////////////////////////////////////////
class CellGroupImpl :
  public CellGroup
{
  friend class CellMgr;

protected:

  /// @brief コンストラクタ
  CellGroupBase();

  /// @brief デストラクタ
  virtual
  ~CellGroupBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属している CellClass を返す．
  virtual
  const CellClass*
  cell_class() const;

  /// @brief 論理セルの場合に関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note 非論理セルの場合の返り値は不定
  virtual
  const TvFunc&
  logic_function(ymuint pos) const;

  /// @brief トライステートセルの場合にトライステート条件関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note 通常の論理セルの場合には定数0関数を返す．
  virtual
  const TvFunc&
  tristate_function(ymuint pos) const;

  /// @brief 論理セルの場合に入力ピン番号を返す．
  /// @param[in] pos CellClass の関数における入力番号
  virtual
  ymuint
  input_pos(ymuint pos) const;

  /// @brief 論理セルの場合に入力ピンの反転属性を返す．
  /// @param[in] pos CellClass の関数における入力番号
  virtual
  bool
  input_inv(ymuint pos) const;

  /// @brief 論理セルの場合に出力ピン番号を返す．
  /// @param[in] pos CellClass の関数における出力番号
  virtual
  ymuint
  output_pos(ymuint pos) const;

  /// @brief 論理セルの場合に出力ピンの反転属性を返す．
  /// @param[in] pos CellClass の関数における出力番号
  virtual
  bool
  output_inv(ymuint pos) const;

  /// @brief FF/ラッチセルの場合にデータ入力ピン番号を返す．
  virtual
  ymuint
  data_pos() const;

  /// @brief FF/ラッチセルの場合にデータ入力ピンの反転属性を返す．
  virtual
  bool
  data_inv() const;

  /// @brief FFセルの場合にクロック入力ピン番号を返す．
  virtual
  ymuint
  clock_pos() const;

  /// @brief FFセルの場合にクロック入力ピンの反転属性を返す．
  /// @note ポジティブエッジが正極性だと思う．
  virtual
  bool
  clock_inv() const;

  /// @brief ラッチセルの場合にイネーブル入力ピン番号を返す．
  virtual
  ymuint
  enable_pos() const;

  /// @brief ラッチセルの場合にイネーブル入力ピンの反転属性を返す．
  virtual
  bool
  enable_inv() const;

  /// @brief FF/ラッチセルの場合にクリア入力ピン番号を返す．
  virtual
  ymuint
  clear_pos() const;

  /// @brief FF/ラッチセルの場合にクリア入力ピンの反転属性を返す．
  virtual
  bool
  clear_inv() const;

  /// @brief FF/ラッチセルの場合にプリセット入力ピン番号を返す．
  virtual
  ymuint
  clear_pos() const;

  /// @brief FF/ラッチセルの場合にプリセット入力ピンの反転属性を返す．
  virtual
  bool
  clear_inv() const;

  /// @brief FF/ラッチセルの場合に Q出力ピン番号を返す．
  virtual
  ymuint
  q_pos() const;

  /// @brief FF/ラッチセルの場合に XQ出力ピン番号を返す．
  virtual
  ymuint
  xq_pos() const;


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

#endif // CELLGROUPBASE_H

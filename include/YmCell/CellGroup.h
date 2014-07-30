#ifndef CELL_CELLGROUP_H
#define CELL_CELLGROUP_H

/// @file YmCell/CellGroup.h
/// @brief CellGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/cell_nsdef.h"
#include "YmLogic/npn_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellGroup CellGroup.h "YmCell/CellGroup.h"
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
  // 一般的な情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  /// @note CellLibrary::group(id) で返されるオブジェクトの id() は id となる．
  virtual
  ymuint
  id() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 代表クラスを返す．
  virtual
  const CellClass*
  rep_class() const = 0;

  /// @brief 代表クラスに対する変換マップを返す．
  virtual
  const NpnMapM&
  map() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // FF/ラッチセルの場合にピンの情報を返す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief FFセルの場合のピン情報を返す．
  /// @note FFセル以外の場合には返り値は不定
  virtual
  CellFFInfo
  ff_info() const = 0;

  /// @brief ラッチセルの場合のピン情報を返す．
  /// @note ラッチセル以外の場合には返り値は不定
  virtual
  CellLatchInfo
  latch_info() const = 0;

  /// @brief データ入力を持つとき true を返す．
  virtual
  bool
  has_data() const = 0;

  /// @brief データ入力のピン番号を返す．
  virtual
  ymuint
  data_pos() const = 0;

  /// @brief クロック入力のタイプを返す．
  /// @retval 0 該当しない
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  virtual
  ymuint
  clock_sense() const = 0;

  /// @brief クロック入力のピン番号を返す．
  virtual
  ymuint
  clock_pos() const = 0;

  /// @brief イネーブル入力を持つとき true を返す．
  virtual
  bool
  has_enable() const = 0;

  /// @brief イネーブル入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  virtual
  ymuint
  enable_sense() const = 0;

  /// @brief イネーブル入力のピン番号を返す．
  virtual
  ymuint
  enable_pos() const = 0;

  /// @brief クリア入力を持つタイプの時に true を返す．
  virtual
  bool
  has_clear() const = 0;

  /// @brief クリア入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  virtual
  ymuint
  clear_sense() const = 0;

  /// @brief クリア入力のピン番号を返す．
  /// @note クリア入力がない場合の値は不定
  virtual
  ymuint
  clear_pos() const = 0;

  /// @brief プリセット入力を持つタイプの時に true を返す．
  virtual
  bool
  has_preset() const = 0;

  /// @brief プリセット入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  virtual
  ymuint
  preset_sense() const = 0;

  /// @brief プリセット入力のピン番号を返す．
  /// @note プリセット入力がない場合の値は不定
  virtual
  ymuint
  preset_pos() const = 0;

  /// @brief 肯定出力のピン番号を返す．
  virtual
  ymuint
  q_pos() const = 0;

  /// @brief 否定出力のピン番号を返す．
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

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLGROUP_H

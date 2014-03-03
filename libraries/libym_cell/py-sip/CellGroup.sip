
/// @file CellGroup.sip
/// @brief CellGroup の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellGroup /Abstract/
{
%TypeHeaderCode
#include "cell/CellGroup.h"
using nsYm::CellGroup;
%End

public:
  //////////////////////////////////////////////////////////////////////
  // 一般的な情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  /// @note CellLibrary::group(id) で返されるオブジェクトの id() は id となる．
  unsigned int
  id() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 代表クラスを返す．
  const CellClass*
  rep_class() const;

  /// @brief 代表クラスに対する変換マップを返す．
  const NpnMapM&
  map() const;


public:
  //////////////////////////////////////////////////////////////////////
  // FF/ラッチセルの場合にピンの情報を返す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データ入力を持つとき true を返す．
  bool
  has_data() const;

  /// @brief データ入力のピン番号を返す．
  unsigned int
  data_pos() const;

  /// @brief クロック入力のタイプを返す．
  /// @retval 0 該当しない
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  unsigned int
  clock_sense() const;

  /// @brief クロック入力のピン番号を返す．
  unsigned int
  clock_pos() const;

  /// @brief イネーブル入力を持つとき true を返す．
  bool
  has_enable() const;

  /// @brief イネーブル入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  unsigned int
  enable_sense() const;

  /// @brief イネーブル入力のピン番号を返す．
  unsigned int
  enable_pos() const;

  /// @brief クリア入力を持つタイプの時に true を返す．
  bool
  has_clear() const;

  /// @brief クリア入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  unsigned int
  clear_sense() const;

  /// @brief クリア入力のピン番号を返す．
  /// @note クリア入力がない場合の値は不定
  unsigned int
  clear_pos() const;

  /// @brief プリセット入力を持つタイプの時に true を返す．
  bool
  has_preset() const;

  /// @brief プリセット入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  unsigned int
  preset_sense() const;

  /// @brief プリセット入力のピン番号を返す．
  /// @note プリセット入力がない場合の値は不定
  unsigned int
  preset_pos() const;

  /// @brief 肯定出力のピン番号を返す．
  unsigned int
  q_pos() const;

  /// @brief 否定出力のピン番号を返す．
  unsigned int
  xq_pos() const;


public:
  //////////////////////////////////////////////////////////////////////
  // このグループに属しているセルの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セル数を返す．
  unsigned int
  cell_num() const;

  /// @brief セルを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  const Cell*
  cell(unsigned int pos) const;


private:

  ~CellGroup();

};

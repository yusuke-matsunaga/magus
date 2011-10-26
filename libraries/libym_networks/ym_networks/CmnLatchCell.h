#ifndef YM_NETWORKS_CMN_CMNLATCHCELL_H
#define YM_NETWORKS_CMN_CMNLATCHCELL_H

/// @file ym_networks/CmnLatchCell.h
/// @brief CmnLatchCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/cmn.h"
#include "ym_cell/CellLatchInfo.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnLatchCell CmnLatchCell.h "ym_networks/CmnLatchCell.h"
/// @brief CMN で用いる ラッチセルの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class CmnLatchCell
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] cell セル
  /// @param[in] pin_info ピン情報
  CmnLatchCell(const Cell* cell,
	       const CellLatchInfo& pin_info);

  /// @brief デストラクタ
  ~CmnLatchCell();


public:

  /// @brief セルを得る．
  const Cell*
  cell() const;

  /// @brief イネーブル入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  ymuint
  enable_sense() const;

  /// @brief クリア入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  ymuint
  clear_sense() const;

  /// @brief プリセット入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  ymuint
  preset_sense() const;

  /// @brief イネーブル入力を持つタイプの時に true を返す．
  bool
  has_enable() const;

  /// @brief クリア入力を持つタイプの時に true を返す．
  bool
  has_clear() const;

  /// @brief プリセット入力を持つタイプの時に true を返す．
  bool
  has_preset() const;

  /// @brief データ入力のピン番号を返す．
  ymuint
  data_pos() const;

  /// @brief イネーブル入力のピン番号を返す．
  ymuint
  enable_pos() const;

  /// @brief クリア入力のピン番号を返す．
  /// @note クリア入力がない場合の値は不定
  ymuint
  clear_pos() const;

  /// @brief プリセット入力のピン番号を返す．
  /// @note プリセット入力がない場合の値は不定
  ymuint
  preset_pos() const;

  /// @brief 肯定出力のピン番号を返す．
  ymuint
  q_pos() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  const Cell* mCell;

  // ピン情報
  CellLatchInfo mPinInfo;

};

END_NAMESPACE_YM_NETWORKS_CMN

#endif // YM_NETWORKS_CMN_CMNLATCHCELL_H

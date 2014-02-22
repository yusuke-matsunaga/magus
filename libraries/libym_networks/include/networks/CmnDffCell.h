#ifndef NETWORKS_CMN_CMNDFFCELL_H
#define NETWORKS_CMN_CMNDFFCELL_H

/// @file networks/CmnDffCell.h
/// @brief CmnDffCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/cmn.h"
#include "cell/CellFFInfo.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnDffCell CmnDffCell.h "networks/CmnDffCell.h"
/// @brief CMN で用いる D-FF セルの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class CmnDffCell
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] cell セル
  /// @param[in] pin_info ピン情報
  CmnDffCell(const Cell* cell,
	     const CellFFInfo& pin_info);

  /// @brief デストラクタ
  ~CmnDffCell();


public:

  /// @brief セルを得る．
  const Cell*
  cell() const;

  /// @brief クロック入力のタイプを返す．
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  ymuint
  clock_sense() const;

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

  /// @brief クリア入力を持つタイプの時に true を返す．
  bool
  has_clear() const;

  /// @brief プリセット入力を持つタイプの時に true を返す．
  bool
  has_preset() const;

  /// @brief データ入力のピン番号を返す．
  ymuint
  data_pos() const;

  /// @brief クロック入力のピン番号を返す．
  ymuint
  clock_pos() const;

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

  /// @brief 否定出力のピン番号を返す．
  ymuint
  xq_pos() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  const Cell* mCell;

  // ピン情報
  CellFFInfo mPinInfo;

};

END_NAMESPACE_YM_NETWORKS_CMN

#endif // NETWORKS_CMN_CMNDFFCELL_H

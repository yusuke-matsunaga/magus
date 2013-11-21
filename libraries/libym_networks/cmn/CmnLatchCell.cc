
/// @file CmnLatchCell.cc
/// @brief CmnLatchCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnLatchCell.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

//////////////////////////////////////////////////////////////////////
// クラス CmnLatchCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cell セル
// @param[in] pin_info ピン情報
CmnLatchCell::CmnLatchCell(const Cell* cell,
			   const CellLatchInfo& pin_info) :
  mCell(cell),
  mPinInfo(pin_info)
{
}

// @brief デストラクタ
CmnLatchCell::~CmnLatchCell()
{
}

// @brief セルを得る．
const Cell*
CmnLatchCell::cell() const
{
  return mCell;
}

// @brief イネーブル入力のタイプを返す．
// @retval 0 なし
// @retval 1 positive edge
// @retval 2 negative edge
ymuint
CmnLatchCell::enable_sense() const
{
  return mPinInfo.enable_sense();
}

// @brief クリア入力のタイプを返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CmnLatchCell::clear_sense() const
{
  return mPinInfo.clear_sense();
}

// @brief プリセット入力のタイプを返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CmnLatchCell::preset_sense() const
{
  return mPinInfo.preset_sense();
}

// @brief イネーブル入力を持つタイプの時に true を返す．
bool
CmnLatchCell::has_enable() const
{
  return mPinInfo.has_enable();
}

// @brief クリア入力を持つタイプの時に true を返す．
bool
CmnLatchCell::has_clear() const
{
  return mPinInfo.has_clear();
}

// @brief プリセット入力を持つタイプの時に true を返す．
bool
CmnLatchCell::has_preset() const
{
  return mPinInfo.has_preset();
}

// @brief データ入力のピン番号を返す．
ymuint
CmnLatchCell::data_pos() const
{
  return mPinInfo.data_pos();
}

// @brief イネーブル入力のピン番号を返す．
ymuint
CmnLatchCell::enable_pos() const
{
  return mPinInfo.enable_pos();
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
ymuint
CmnLatchCell::clear_pos() const
{
  return mPinInfo.clear_pos();
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
ymuint
CmnLatchCell::preset_pos() const
{
  return mPinInfo.preset_pos();
}

// @brief 肯定出力のピン番号を返す．
ymuint
CmnLatchCell::q_pos() const
{
  return mPinInfo.q_pos();
}

END_NAMESPACE_YM_NETWORKS_CMN

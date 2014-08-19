
/// @file CmnDffCell.cc
/// @brief CmnDffCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/CmnDffCell.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

//////////////////////////////////////////////////////////////////////
// クラス CmnDffCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cell セル
// @param[in] pin_info ピン情報
CmnDffCell::CmnDffCell(const Cell* cell,
		       const CellFFInfo& pin_info) :
  mCell(cell),
  mPinInfo(pin_info)
{
}

// @brief デストラクタ
CmnDffCell::~CmnDffCell()
{
}

// @brief セルを得る．
const Cell*
CmnDffCell::cell() const
{
  return mCell;
}

// @brief クロック入力のタイプを返す．
// @retval 1 positive edge
// @retval 2 negative edge
ymuint
CmnDffCell::clock_sense() const
{
  return mPinInfo.clock_sense();
}

// @brief クリア入力のタイプを返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CmnDffCell::clear_sense() const
{
  return mPinInfo.clear_sense();
}

// @brief プリセット入力のタイプを返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CmnDffCell::preset_sense() const
{
  return mPinInfo.preset_sense();
}

// @brief クリア入力を持つタイプの時に true を返す．
bool
CmnDffCell::has_clear() const
{
  return mPinInfo.has_clear();
}

// @brief プリセット入力を持つタイプの時に true を返す．
bool
CmnDffCell::has_preset() const
{
  return mPinInfo.has_preset();
}

// @brief データ入力のピン番号を返す．
ymuint
CmnDffCell::data_pos() const
{
  return mPinInfo.data_pos();
}

// @brief クロック入力のピン番号を返す．
ymuint
CmnDffCell::clock_pos() const
{
  return mPinInfo.clock_pos();
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
ymuint
CmnDffCell::clear_pos() const
{
  return mPinInfo.clear_pos();
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
ymuint
CmnDffCell::preset_pos() const
{
  return mPinInfo.preset_pos();
}

// @brief 肯定出力のピン番号を返す．
ymuint
CmnDffCell::q_pos() const
{
  return mPinInfo.q_pos();
}

// @brief 否定出力のピン番号を返す．
ymuint
CmnDffCell::xq_pos() const
{
  return mPinInfo.xq_pos();
}

END_NAMESPACE_YM_NETWORKS_CMN

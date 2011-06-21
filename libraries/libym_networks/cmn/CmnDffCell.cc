
/// @file libym_networks/cmn/CmnDffCell.cc
/// @brief CmnDffCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnDffCell.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

const int OUTPUT1 = 0;
const int OUTPUT2 = 1;
const int INPUT   = 2;
const int CLOCK   = 3;
const int CLEAR   = 4;
const int PRESET  = 5;

inline
ymuint32
encode(ymuint val,
       int idx)
{
  return val << (5 * idx);
}

inline
ymuint
get_sense(ymuint32 bits,
	  int idx)
{
  return (bits >> 5 * idx) & 3U;
}

inline
ymuint
get_pos(ymuint32 bits,
	int idx)
{
  return (bits >> (5 * idx + 2)) & 7U;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス CmnDffCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cell セル
// @param[in] pos_array ピン情報の配列
// @note pos_array の意味は以下の通り
//  - pos_array[0] : データ入力のピン番号     (3bit)
//  - pos_array[1] : クロック入力のピン番号   (3bit)
//  - pos_array[2] : クリア入力のピン番号     (3bit) | ピン情報 (2bit)
//  - pos_array[3] : プリセット入力のピン番号 (3bit) | ピン情報 (2bit)
//  - pos_array[4] : 肯定出力のピン番号       (3bit)
//  - pos_array[5] : 否定出力のピン番号       (3bit)
CmnDffCell::CmnDffCell(const Cell* cell,
		       ymuint pos_array[]) :
  mCell(cell),
  mBits(0U)
{
  mBits |= encode(pos_array[0], INPUT);
  mBits |= encode(pos_array[1], CLOCK);
  mBits |= encode(pos_array[2], CLEAR);
  mBits |= encode(pos_array[3], PRESET);
  mBits |= encode(pos_array[4], OUTPUT1);
  mBits |= encode(pos_array[5], OUTPUT2);
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
  return get_sense(mBits, CLOCK);
}

// @brief クリア入力のタイプを返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CmnDffCell::clear_sense() const
{
  return get_sense(mBits, CLEAR);
}

// @brief プリセット入力のタイプを返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CmnDffCell::preset_sense() const
{
  return get_sense(mBits, PRESET);
}

// @brief クリア入力を持つタイプの時に true を返す．
bool
CmnDffCell::has_clear() const
{
  return clear_sense() != 0;
}

// @brief プリセット入力を持つタイプの時に true を返す．
bool
CmnDffCell::has_preset() const
{
  return preset_sense() != 0;
}

// @brief データ入力のピン番号を返す．
ymuint
CmnDffCell::data_pos() const
{
  return get_pos(mBits, INPUT);
}

// @brief クロック入力のピン番号を返す．
ymuint
CmnDffCell::clock_pos() const
{
  return get_pos(mBits, CLOCK);
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
ymuint
CmnDffCell::clear_pos() const
{
  return get_pos(mBits, CLEAR);
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
ymuint
CmnDffCell::preset_pos() const
{
  return get_pos(mBits, PRESET);
}

// @brief 肯定出力のピン番号を返す．
ymuint
CmnDffCell::q_pos() const
{
  return get_pos(mBits, OUTPUT1);
}

// @brief 否定出力のピン番号を返す．
ymuint
CmnDffCell::iq_pos() const
{
  return get_pos(mBits, OUTPUT2);
}

END_NAMESPACE_YM_NETWORKS

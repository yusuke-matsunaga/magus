
/// @file libym_networks/cmn/CmnLatchCell.cc
/// @brief CmnLatchCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnLatchCell.h"


BEGIN_NAMESPACE_YM_CMN

BEGIN_NONAMESPACE

const int OUTPUT1 = 0;
const int OUTPUT2 = 1;
const int INPUT   = 2;
const int ENABLE  = 3;
const int CLEAR   = 4;
const int PRESET  = 5;

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
// クラス CmnLatchCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cell セル
CmnLatchCell::CmnLatchCell(const Cell* cell) :
  mCell(cell)
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
  return get_sense(mBits, ENABLE);
}

// @brief クリア入力のタイプを返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CmnLatchCell::clear_sense() const
{
  return get_sense(mBits, CLEAR);
}

// @brief プリセット入力のタイプを返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CmnLatchCell::preset_sense() const
{
  return get_sense(mBits, PRESET);
}

// @brief イネーブル入力を持つタイプの時に true を返す．
bool
CmnLatchCell::has_enable() const
{
  return enable_sense() != 0;
}

// @brief クリア入力を持つタイプの時に true を返す．
bool
CmnLatchCell::has_clear() const
{
  return clear_sense() != 0;
}

// @brief プリセット入力を持つタイプの時に true を返す．
bool
CmnLatchCell::has_preset() const
{
  return preset_sense() != 0;
}

// @brief データ入力のピン番号を返す．
ymuint
CmnLatchCell::data_pos() const
{
  return get_pos(mBits, INPUT);
}

// @brief イネーブル入力のピン番号を返す．
ymuint
CmnLatchCell::enable_pos() const
{
  return get_pos(mBits, ENABLE);
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
ymuint
CmnLatchCell::clear_pos() const
{
  return get_pos(mBits, CLEAR);
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
ymuint
CmnLatchCell::preset_pos() const
{
  return get_pos(mBits, PRESET);
}

// @brief 肯定出力のピン番号を返す．
ymuint
CmnLatchCell::q_pos() const
{
  return get_pos(mBits, OUTPUT1);
}

// @brief 否定出力のピン番号を返す．
ymuint
CmnLatchCell::iq_pos() const
{
  return get_pos(mBits, OUTPUT2);
}

END_NAMESPACE_YM_CMN

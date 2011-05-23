
/// @file libym_techmap/cellmap/FFGroup.cc
/// @brief FFGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "FFGroup.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
// クラス FFGroup
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// 各ピンのインデックス
const ymuint DATA   = 0;
const ymuint CLOCK  = 1;
const ymuint CLEAR  = 2;
const ymuint PRESET = 3;
const ymuint Q      = 4;
const ymuint IQ     = 5;

// インデックスからピン番号を取り出す．
inline
ymuint
get_pos(ymuint32 data,
	ymuint index)
{
  return (data >> (index * 3)) & 7U;
}

END_NONAMESPACE


// @brief コンストラクタ
FFGroup::FFGroup() :
  mPosArray(0U)
{
}

// @brief デストラクタ
FFGroup::~FFGroup()
{
}

// @brief データ入力のピン番号を返す．
ymuint
FFGroup::data_pos() const
{
  return get_pos(mPosArray, DATA);
}

// @brief クロック入力のピン番号を返す．
ymuint
FFGroup::clock_pos() const
{
  return get_pos(mPosArray, CLOCK);
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
ymuint
FFGroup::clear_pos() const
{
  return get_pos(mPosArray, CLEAR);
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
ymuint
FFGroup::preset_pos() const
{
  return get_pos(mPosArray, PRESET);
}

// @brief 肯定出力のピン番号を返す．
ymuint
FFGroup::q_pos() const
{
  return get_pos(mPosArray, Q);
}

// @brief 否定出力のピン番号を返す．
ymuint
FFGroup::iq_pos() const
{
  return get_pos(mPosArray, IQ);
}

END_NAMESPACE_YM_CELLMAP

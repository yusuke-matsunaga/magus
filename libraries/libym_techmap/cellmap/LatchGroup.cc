
/// @file libym_techmap/cellmap/LatchGroup.cc
/// @brief LatchGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LatchGroup.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
// クラス LatchGroup
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// 各ピンのインデックス
const ymuint DATA   = 0;
const ymuint ENABLE  = 1;
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

// インデックスから極性を取り出す．
// 簡単化のため間が空いたマッピングになっている．
inline
ymuint
get_pol(ymuint32 data,
	ymuint index)
{
  return (data >> (index + 18)) & 1U;
}

END_NONAMESPACE


// @brief コンストラクタ
LatchGroup::LatchGroup() :
  mData(0U)
{
}

// @brief デストラクタ
LatchGroup::~LatchGroup()
{
}

// @brief データ入力のピン番号を返す．
ymuint
LatchGroup::data_pos() const
{
  return get_pos(mData, DATA);
}

// @brief イネーブル入力のピン番号を返す．
ymuint
LatchGroup::enable_pos() const
{
  return get_pos(mData, ENABLE);
}

// @brief イネーブルのタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
LatchGroup::enable_sense() const
{
  return get_pol(mData, ENABLE);
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
ymuint
LatchGroup::clear_pos() const
{
  return get_pos(mData, CLEAR);
}

// @brief クリア入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
LatchGroup::clear_sense() const
{
  return get_pol(mData, CLEAR);
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
ymuint
LatchGroup::preset_pos() const
{
  return get_pos(mData, PRESET);
}

// @brief プリセット入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
LatchGroup::preset_sense() const
{
  return get_pol(mData, PRESET);
}

// @brief 肯定出力のピン番号を返す．
ymuint
LatchGroup::q_pos() const
{
  return get_pos(mData, Q);
}

// @brief 否定出力のピン番号を返す．
ymuint
LatchGroup::iq_pos() const
{
  return get_pos(mData, IQ);
}

END_NAMESPACE_YM_CELLMAP

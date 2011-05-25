
/// @file libym_techmap/cellmap/FFPosArray.cc
/// @brief FFPosArray の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "FFPosArray.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
// クラス FFPosArray
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

// ピン番号をエンコードする．
inline
ymuint
encode(ymuint pos,
       ymuint index)
{
  return ((pos & 7U) << (index * 3));
}

END_NONAMESPACE


// @brief コンストラクタ
FFPosArray::FFPosArray() :
  mData(0U)
{
}

// @brief デストラクタ
FFPosArray::~FFPosArray()
{
}

// @brief シグネチャを返す．
ymuint
FFPosArray::signature() const
{
  return mData;
}

// @brief データ入力のピン番号を返す．
ymuint
FFPosArray::data_pos() const
{
  return get_pos(mData, DATA);
}

// @brief クロック入力のピン番号を返す．
ymuint
FFPosArray::clock_pos() const
{
  return get_pos(mData, CLOCK);
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
ymuint
FFPosArray::clear_pos() const
{
  return get_pos(mData, CLEAR);
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
ymuint
FFPosArray::preset_pos() const
{
  return get_pos(mData, PRESET);
}

// @brief 肯定出力のピン番号を返す．
ymuint
FFPosArray::q_pos() const
{
  return get_pos(mData, Q);
}

// @brief 否定出力のピン番号を返す．
ymuint
FFPosArray::iq_pos() const
{
  return get_pos(mData, IQ);
}

// @brief シグネチャを設定する．
// @param[in] sig 設定するシグネチャ
void
FFPosArray::set_signature(ymuint sig)
{
  mData = sig;
}

// @brief 個々の情報を設定する．
// @param[in] data_pos データ入力のピン番号
// @param[in] clock_pos クロック入力のピン番号
// @param[in] clear_pos クリア入力のピン番号
// @param[in] preset_pos プリセット入力のピン番号
// @param[in] q_pos 肯定出力のピン番号
// @param[in] iq_pos 否定出力のピン番号
void
FFPosArray::set(ymuint data_pos,
		ymuint clock_pos,
		ymuint clear_pos,
		ymuint preset_pos,
		ymuint q_pos,
		ymuint iq_pos)
{
  ymuint sig = 0U;
  sig |= encode(data_pos, DATA);
  sig |= encode(clock_pos, CLOCK);
  sig |= encode(clear_pos, CLEAR);
  sig |= encode(preset_pos, PRESET);
  sig |= encode(q_pos, Q);
  sig |= encode(iq_pos, IQ);
  mData = sig;
}

END_NAMESPACE_YM_CELLMAP

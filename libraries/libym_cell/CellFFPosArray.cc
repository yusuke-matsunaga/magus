
/// @file CellFFPosArray.cc
/// @brief CellFFPosArray の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellFFPosArray.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellFFPosArray
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
  return (data >> (index * 5)) & 7U;
}

// インデックスから極性情報を取り出す．
inline
ymuint
get_sense(ymuint32 data,
	  ymuint index)
{
  return (data >> ((index * 5) + 3)) & 3U;
}

// ピン番号をエンコードする．
inline
ymuint
encode(ymuint pos,
       ymuint sense,
       ymuint index)
{
  return ((pos & 7U) | ((sense & 3U) << 3)) << (index * 5);
}

END_NONAMESPACE


// @brief コンストラクタ
CellFFPosArray::CellFFPosArray() :
  mData(0U)
{
}

// @brief シグネチャを指定するコンストラクタ
CellFFPosArray::CellFFPosArray(ymuint sig) :
  mData(sig)
{
}

// @brief デストラクタ
CellFFPosArray::~CellFFPosArray()
{
}

// @brief シグネチャを返す．
ymuint
CellFFPosArray::signature() const
{
  return mData;
}

// @brief データ入力のピン番号を返す．
ymuint
CellFFPosArray::data_pos() const
{
  return get_pos(mData, DATA);
}

// @brief クロック入力のピン番号を返す．
ymuint
CellFFPosArray::clock_pos() const
{
  return get_pos(mData, CLOCK);
}

// @brief クロック入力の極性情報を返す．
// @retval 1 positive edge
// @retval 2 negative edge
ymuint
CellFFPosArray::clock_sense() const
{
  return get_sense(mData, CLOCK);
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
ymuint
CellFFPosArray::clear_pos() const
{
  return get_pos(mData, CLEAR);
}

// @brief クリア入力の極性情報を返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CellFFPosArray::clear_sense() const
{
  return get_sense(mData, CLEAR);
}

// @brief クリア入力を持つとき true を返す．
bool
CellFFPosArray::has_clear() const
{
  return clear_sense() != 0U;
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
ymuint
CellFFPosArray::preset_pos() const
{
  return get_pos(mData, PRESET);
}

// @brief プリセット入力の極性情報を返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CellFFPosArray::preset_sense() const
{
  return get_sense(mData, PRESET);
}

// @brief プリセット入力を持つとき true を返す．
bool
CellFFPosArray::has_preset() const
{
  return preset_sense() != 0U;
}

// @brief 肯定出力のピン番号を返す．
ymuint
CellFFPosArray::q_pos() const
{
  return get_pos(mData, Q);
}

// @brief 否定出力のピン番号を返す．
ymuint
CellFFPosArray::iq_pos() const
{
  return get_pos(mData, IQ);
}

// @brief シグネチャを設定する．
// @param[in] sig 設定するシグネチャ
void
CellFFPosArray::set_signature(ymuint sig)
{
  mData = sig;
}

// @brief 個々の情報を設定する．
// @param[in] data_pos データ入力のピン番号
// @param[in] clock_pos クロック入力のピン番号
// @param[in] clock_sense クロック入力の極性情報
// @param[in] clear_pos クリア入力のピン番号
// @param[in] clear_sense クリア入力の極性情報
// @param[in] preset_pos プリセット入力のピン番号
// @param[in] preset_sense プリセット入力の極性情報
// @param[in] q_pos 肯定出力のピン番号
// @param[in] iq_pos 否定出力のピン番号
void
CellFFPosArray::set(ymuint data_pos,
		    ymuint clock_pos,
		    ymuint clock_sense,
		    ymuint clear_pos,
		    ymuint clear_sense,
		    ymuint preset_pos,
		    ymuint preset_sense,
		    ymuint q_pos,
		    ymuint iq_pos)
{
  ymuint sig = 0U;
  sig |= encode(data_pos, 0, DATA);
  sig |= encode(clock_pos, clock_sense, CLOCK);
  sig |= encode(clear_pos, clear_sense, CLEAR);
  sig |= encode(preset_pos, preset_sense, PRESET);
  sig |= encode(q_pos, 0, Q);
  sig |= encode(iq_pos, 0, IQ);
  mData = sig;
}

END_NAMESPACE_YM_CELL

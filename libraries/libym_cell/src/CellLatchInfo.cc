
/// @file CellLatchInfo.cc
/// @brief CellLatchInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "cell/CellLatchInfo.h"


BEGIN_NAMESPACE_YM_CELL

BEGIN_NONAMESPACE

const int OUTPUT1 = 0;
const int INPUT   = 2;
const int ENABLE  = 3;
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
  return (bits >> (5 * idx + 3)) & 3U;
}

inline
ymuint
get_pos(ymuint32 bits,
	int idx)
{
  return (bits >> (5 * idx)) & 7U;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス CellLatchInfo
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 内容は不定
CellLatchInfo::CellLatchInfo() :
  mBits(0U)
{
}

// @brief コンストラクタ
// @param[in] pos_array ピン位置と極性情報の配列
// @note pos_array の意味は以下の通り
//  - pos_array[0] : データ入力のピン番号     (3bit)
//  - pos_array[1] : イネーブル入力のピン番号 (3bit) | 極性情報 (2bit)
//  - pos_array[2] : クリア入力のピン番号     (3bit) | 極性情報 (2bit)
//  - pos_array[3] : プリセット入力のピン番号 (3bit) | 極性情報 (2bit)
//  - pos_array[4] : 肯定出力のピン番号       (3bit)
CellLatchInfo::CellLatchInfo(ymuint pos_array[]) :
  mBits(0U)
{
  mBits |= encode(pos_array[0], INPUT);
  mBits |= encode(pos_array[1], ENABLE);
  mBits |= encode(pos_array[2], CLEAR);
  mBits |= encode(pos_array[3], PRESET);
  mBits |= encode(pos_array[4], OUTPUT1);
}

// @brief デストラクタ
CellLatchInfo::~CellLatchInfo()
{
}

// @brief イネーブル入力のタイプを返す．
// @retval 0 なし
// @retval 1 positive edge
// @retval 2 negative edge
ymuint
CellLatchInfo::enable_sense() const
{
  return get_sense(mBits, ENABLE);
}

// @brief クリア入力のタイプを返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CellLatchInfo::clear_sense() const
{
  return get_sense(mBits, CLEAR);
}

// @brief プリセット入力のタイプを返す．
// @retval 0 なし
// @retval 1 High sensitive
// @retval 2 Low sensitive
ymuint
CellLatchInfo::preset_sense() const
{
  return get_sense(mBits, PRESET);
}

// @brief データ入力を持つとき true を返す．
bool
CellLatchInfo::has_data() const
{
  return get_sense(mBits, INPUT) != 0;
}

// @brief イネーブル入力を持つとき true を返す．
bool
CellLatchInfo::has_enable() const
{
  return enable_sense() != 0;
}

// @brief クリア入力を持つタイプの時に true を返す．
bool
CellLatchInfo::has_clear() const
{
  return clear_sense() != 0;
}

// @brief プリセット入力を持つタイプの時に true を返す．
bool
CellLatchInfo::has_preset() const
{
  return preset_sense() != 0;
}

// @brief データ入力のピン番号を返す．
ymuint
CellLatchInfo::data_pos() const
{
  return get_pos(mBits, INPUT);
}

// @brief クロック入力のピン番号を返す．
ymuint
CellLatchInfo::enable_pos() const
{
  return get_pos(mBits, ENABLE);
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
ymuint
CellLatchInfo::clear_pos() const
{
  return get_pos(mBits, CLEAR);
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
ymuint
CellLatchInfo::preset_pos() const
{
  return get_pos(mBits, PRESET);
}

// @brief 肯定出力のピン番号を返す．
ymuint
CellLatchInfo::q_pos() const
{
  return get_pos(mBits, OUTPUT1);
}

// @brief バイナリダンプを行う．
// @param[in] bos 出力先のストリーム
// @param[in] latchinfo 対象の CellLatchInfo
// @return ストリームを返す．
ODO&
operator<<(ODO& bos,
	   const CellLatchInfo& latchinfo)
{
  return bos << latchinfo.mBits;
}

// @brief バイナリファイルを読み込む．
// @param[in] bis 入力元のストリーム
// @param[out] latchinfo 結果を格納する変数
// @return ストリームを返す．
IDO&
operator>>(IDO& bis,
	   CellLatchInfo& latchinfo)
{
  return bis >> latchinfo.mBits;
}

END_NAMESPACE_YM_CELL

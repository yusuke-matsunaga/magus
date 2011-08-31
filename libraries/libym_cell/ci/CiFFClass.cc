
/// @file CiFFClass.cc
/// @brief CiFFClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiFFClass.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiFFClass
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// 各ピンのインデックス
const ymuint CLOCK  = 0;
const ymuint CLEAR  = 1;
const ymuint PRESET = 2;

// インデックスからタイプを取り出す．
inline
ymuint
get_pol(ymuint32 data,
	ymuint index)
{
  return (data >> (index * 2)) & 3U;
}

END_NONAMESPACE


// @brief クロック入力のタイプを返す．
// @retval 1 positive edge
// @retval 2 negative edge
ymuint
CiFFClass::clock_sense() const
{
  return get_pol(mBits, CLOCK);
}

// @brief クリア入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
CiFFClass::clear_sense() const
{
  return get_pol(mBits, CLEAR);
}

// @brief プリセット入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
CiFFClass::preset_sense() const
{
  return get_pol(mBits, PRESET);
}

END_NAMESPACE_YM_CELL

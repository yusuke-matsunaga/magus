
/// @file CiLatchClass.cc
/// @brief CiLatchClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiLatchClass.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiLatchClass
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// 各ピンのインデックス
const ymuint ENABLE  = 0;
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


// @brief コンストラクタ
CiLatchClass::CiLatchClass() :
  mBits(0U)
{
}

// @brief デストラクタ
CiLatchClass::~CiLatchClass()
{
}

// @brief クロック入力のタイプを返す．
// @retval 0 なし
// @retval 1 positive edge
// @retval 2 negative edge
ymuint
CiLatchClass::enable_sense() const
{
  return get_pol(mBits, ENABLE);
}

// @brief クリア入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
CiLatchClass::clear_sense() const
{
  return get_pol(mBits, CLEAR);
}

// @brief プリセット入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
CiLatchClass::preset_sense() const
{
  return get_pol(mBits, PRESET);
}

// @brief クリア入力を持つタイプの時に true を返す．
bool
CiLatchClass::has_clear() const
{
  return clear_sense() != 0U;
}

// @brief プリセット入力を持つタイプの時に true を返す．
bool
CiLatchClass::has_preset() const
{
  return preset_sense() != 0U;
}

END_NAMESPACE_YM_CELL

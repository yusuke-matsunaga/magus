
/// @file libym_techmap/cellmap/LatchClass.cc
/// @brief LatchClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LatchClass.h"
#include "LatchGroup.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
// クラス LatchClass
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
LatchClass::LatchClass() :
  mBits(0U)
{
}

// @brief デストラクタ
LatchClass::~LatchClass()
{
}

// @brief クロック入力のタイプを返す．
// @retval 0 なし
// @retval 1 positive edge
// @retval 2 negative edge
ymuint
LatchClass::enable_sense() const
{
  return get_pol(mBits, ENABLE);
}

// @brief クリア入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
LatchClass::clear_sense() const
{
  return get_pol(mBits, CLEAR);
}

// @brief プリセット入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
LatchClass::preset_sense() const
{
  return get_pol(mBits, PRESET);
}

// @brief クリア入力を持つタイプの時に true を返す．
bool
LatchClass::has_clear() const
{
  return clear_sense() != 0U;
}

// @brief プリセット入力を持つタイプの時に true を返す．
bool
LatchClass::has_preset() const
{
  return preset_sense() != 0U;
}

// @brief このクラスに属する LatchGroup の数を返す．
ymuint
LatchClass::group_num() const
{
  return mGroupNum;
}

// @brief LatchGroup を得る．
// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
const LatchGroup&
LatchClass::group(ymuint pos) const
{
  return mGroupList[pos];
}

END_NAMESPACE_YM_CELLMAP

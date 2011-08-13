
/// @file CellLatchClass.cc
/// @brief CellLatchClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLatchClass.h"
#include "ym_cell/CellLatchGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellLatchClass
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
CellLatchClass::CellLatchClass() :
  mBits(0U)
{
}

// @brief デストラクタ
CellLatchClass::~CellLatchClass()
{
}

// @brief クロック入力のタイプを返す．
// @retval 0 なし
// @retval 1 positive edge
// @retval 2 negative edge
ymuint
CellLatchClass::enable_sense() const
{
  return get_pol(mBits, ENABLE);
}

// @brief クリア入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
CellLatchClass::clear_sense() const
{
  return get_pol(mBits, CLEAR);
}

// @brief プリセット入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
CellLatchClass::preset_sense() const
{
  return get_pol(mBits, PRESET);
}

// @brief クリア入力を持つタイプの時に true を返す．
bool
CellLatchClass::has_clear() const
{
  return clear_sense() != 0U;
}

// @brief プリセット入力を持つタイプの時に true を返す．
bool
CellLatchClass::has_preset() const
{
  return preset_sense() != 0U;
}

// @brief このクラスに属する CellLatchGroup の数を返す．
ymuint
CellLatchClass::group_num() const
{
  return mGroupNum;
}

// @brief CellLatchGroup を得る．
// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
const CellLatchGroup&
CellLatchClass::group(ymuint pos) const
{
  return mGroupList[pos];
}

END_NAMESPACE_YM_CELL

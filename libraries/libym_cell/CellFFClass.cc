
/// @file CellFFClass.cc
/// @brief CellFFClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellFFClass.h"
#include "ym_cell/CellFFGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellFFClass
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


// @brief コンストラクタ
CellFFClass::CellFFClass() :
  mBits(0U)
{
}

// @brief デストラクタ
CellFFClass::~CellFFClass()
{
}

// @brief シグネチャを返す．
ymuint
CellFFClass::signature() const
{
  return mBits;
}

// @brief クロック入力のタイプを返す．
// @retval 1 positive edge
// @retval 2 negative edge
ymuint
CellFFClass::clock_sense() const
{
  return get_pol(mBits, CLOCK);
}

// @brief クリア入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
CellFFClass::clear_sense() const
{
  return get_pol(mBits, CLEAR);
}

// @brief プリセット入力のタイプを返す．
// @retval 0 Low sensitive
// @retval 1 High sensitive
ymuint
CellFFClass::preset_sense() const
{
  return get_pol(mBits, PRESET);
}

// @brief クリア入力を持つタイプの時に true を返す．
bool
CellFFClass::has_clear() const
{
  return clear_sense() != 0U;
}

// @brief プリセット入力を持つタイプの時に true を返す．
bool
CellFFClass::has_preset() const
{
  return preset_sense() != 0U;
}

// @brief このクラスに属する CellFFGroup の数を返す．
ymuint
CellFFClass::group_num() const
{
  return mGroupNum;
}

// @brief CellFFGroup を得る．
// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
const CellFFGroup&
CellFFClass::group(ymuint pos) const
{
  return *mGroupList[pos];
}

END_NAMESPACE_YM_CELL


/// @file libym_techmap/cellmap/libdump/LdFFGroup.cc
/// @brief LdFFGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdFFGroup.h"


BEGIN_NAMESPACE_YM_TECHMAP_CELLMAP_LIDBUMP

//////////////////////////////////////////////////////////////////////
// クラス LdFFGroup
//////////////////////////////////////////////////////////////////////

// @brief シグネチャを返す．
ymuint
LdFFGroup::signature() const
{
  return mPosArray;
}

// @brief データ入力のピン番号を返す．
ymuint
LdFFGroup::data_pos() const
{
  return (mPosArray >> 0) & 7U;
}

// @brief クロック入力のピン番号を返す．
ymuint
LdFFGroup::clock_pos() const
{
  return (mPosArray >> 3) & 7U;
}

// @brief クリア入力のピン番号を返す．
ymuint
LdFFGroup::clear_pos() const
{
  return (mPosArray >> 12) & 7U;
}

// @brief プリセット入力のピン番号を返す．
ymuint
LdFFGroup::preset_pos() const
{
  return (mPosArray >> 15) & 7U;
}

// @brief 肯定出力のピン番号を返す．
ymuint
LdFFGroup::q_pos() const
{
  return (mPosArray >> 6) & 7U;
}

// @brief 否定出力のピン番号を返す．
ymuint
LdFFGroup::iq_pos() const
{
  return (mPosArray >> 9) & 7U;
}

END_NAMESPACE_YM_TECHMAP_CELLMAP_LIDBUMP


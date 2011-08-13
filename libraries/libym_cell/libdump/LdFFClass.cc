
/// @file LdFFClass.cc
/// @brief LdFFClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdFFClass.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
// クラス LdFFClass
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LdFFClass::LdFFClass() :
  mId(0),
  mBits(0)
{
}

// @brief デストラクタ
LdFFClass::~LdFFClass()
{
}

// @brief シグネチャを返す．
ymuint
LdFFClass::signature() const
{
  return mBits;
}

// @brief クロック入力のタイプを返す．
ymuint
LdFFClass::clock_sense() const
{
  return (mBits >> 0) & 3U;
}

// @brief クリア入力のタイプを返す．
ymuint
LdFFClass::clear_sense() const
{
  return (mBits >> 2) & 3U;
}

// @brief プリセット入力のタイプを返す．
ymuint
LdFFClass::preset_sense() const
{
  return (mBits >> 4) & 3U;
}

END_NAMESPACE_YM_CELL_LIBDUMP


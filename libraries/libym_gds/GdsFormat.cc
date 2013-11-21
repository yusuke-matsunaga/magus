
/// @file GdsFormat.cc
/// @brief GdsFormat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsFormat.h"
#include "ym_gds/GdsString.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsFormat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type フォーマットタイプ ( 0 - 3 )
GdsFormat::GdsFormat(ymint16 type) :
  mType(type),
  mMaskNum(0)
{
}

// @brief デストラクタ
GdsFormat::~GdsFormat()
{
}

// @brief フォーマットタイプを返す．
int
GdsFormat::type() const
{
  return mType;
}

// @brief マスク数を返す．
ymuint
GdsFormat::mask_num() const
{
  return mMaskNum;
}

// @brief マスクを返す．
// @param[in] pos 位置番号 ( 0 <= pos < mask_num() )
const char*
GdsFormat::mask(ymuint pos) const
{
  return mMasks[pos]->str();
}

END_NAMESPACE_YM_GDS

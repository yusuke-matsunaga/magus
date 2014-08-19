
/// @file GdsAref.cc
/// @brief GdsAref の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GdsAref.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsAref
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] strname 構造名
// @param[in] strans STRANS の値
// @param[in] colrow 列と行の数
// @param[in] xy 座標
GdsAref::GdsAref(ymuint16 elflags,
		 ymint32 plex,
		 GdsString* strname,
		 GdsStrans* strans,
		 ymuint32 colrow,
		 GdsXY* xy) :
  GdsRefBase(elflags, plex, strname, strans),
  mXY(xy)
{
  mColumn = static_cast<ymint16>(colrow >> 16);
  mRow = static_cast<ymint16>(colrow & 0xFFFF);
}

// @brief デストラクタ
GdsAref::~GdsAref()
{
}

// @brief column 数を返す．
int
GdsAref::column() const
{
  return mColumn;
}

// @brief row 数を返す．
int
GdsAref::row() const
{
  return mRow;
}

// @brief XY 座標を返す．
GdsXY*
GdsAref::xy() const
{
  return mXY;
}

END_NAMESPACE_YM_GDS

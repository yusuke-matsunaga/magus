
/// @file GdsSref.cc
/// @brief GdsSref の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GdsSref.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsSref
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] strname 構造名
// @param[in] strans STRANS の値
// @param[in] xy 座標
GdsSref::GdsSref(ymuint16 elflags,
		 ymint32 plex,
		 GdsString* strname,
		 GdsStrans* strans,
		 GdsXY* xy) :
  GdsRefBase(elflags, plex, strname, strans),
  mXY(xy)
{
}

// デストラクタ
GdsSref::~GdsSref()
{
}

// XY 座標を返す．
GdsXY*
GdsSref::xy() const
{
  return mXY;
}

END_NAMESPACE_YM_GDS


/// @file GdsBox.cc
/// @brief GdsBox の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GdsBox.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsBox
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] boxtype BOXTYPE の値
// @param[in] xy XY の値
GdsBox::GdsBox(ymuint16 elflags,
	       ymuint32 plex,
	       ymint16 layer,
	       ymint16 boxtype,
	       GdsXY* xy) :
  GdsElement(elflags, plex),
  mLayer(layer),
  mBoxType(boxtype),
  mXY(xy)
{
}

// デストラクタ
GdsBox::~GdsBox()
{
}

// @brief 層番号を返す．
int
GdsBox::layer() const
{
  return mLayer;
}

// @brief ボックス型を返す．
int
GdsBox::boxtype() const
{
  return mBoxType;
}

// @brief 座標を返す．
GdsXY*
GdsBox::xy() const
{
  return mXY;
}

END_NAMESPACE_YM_GDS

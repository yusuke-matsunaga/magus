
/// @file GdsBoundary.cc
/// @brief GdsBoundary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GdsBoundary.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsBoundary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] datatype DATATYPE の値
// @param[in] xy XY の値
GdsBoundary::GdsBoundary(ymuint16 elflags,
			 ymint32 plex,
			 ymint16 layer,
			 ymint16 datatype,
			 GdsXY* xy) :
  GdsElement(elflags, plex),
  mLayer(layer),
  mDataType(datatype),
  mXY(xy)
{
}

// @brief デストラクタ
GdsBoundary::~GdsBoundary()
{
}

// @brief 層番号を返す．
int
GdsBoundary::layer() const
{
  return mLayer;
}

// @brief データ型を返す．
int
GdsBoundary::datatype() const
{
  return mDataType;
}

// @brief 座標のリストを返す．
GdsXY*
GdsBoundary::xy() const
{
  return mXY;
}

END_NAMESPACE_YM_GDS

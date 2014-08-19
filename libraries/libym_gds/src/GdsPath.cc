
/// @file GdsPath.cc
/// @brief GdsPath の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GdsPath.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsPath
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] datatype DATATYPE の値
// @param[in] pathtype PATYTYPE の値
// @param[in] width WIDTH の値
// @param[in] bgn_extn BGNEXTN の値
// @param[in] end_extn ENDEXTN の値
// @param[in] xy XY の値
GdsPath::GdsPath(ymuint16 elflags,
		 ymint32 plex,
		 ymint16 layer,
		 ymint16 datatype,
		 ymint16 pathtype,
		 ymint32 width,
		 ymint32 bgn_extn,
		 ymint32 end_extn,
		 GdsXY* xy) :
  GdsElement(elflags, plex),
  mLayer(layer),
  mDataType(datatype),
  mPathType(pathtype),
  mWidth(width),
  mBgnExtn(bgn_extn),
  mEndExtn(end_extn),
  mXY(xy)
{
}

// @brief デストラクタ
GdsPath::~GdsPath()
{
}

// 層番号を返す．
int
GdsPath::layer() const
{
  return mLayer;
}

// データ型を返す．
int
GdsPath::datatype() const
{
  return mDataType;
}

// パスタイプを返す．
int
GdsPath::pathtype() const
{
  return mPathType;
}

// 幅を返す．
int
GdsPath::width() const
{
  return mWidth;
}

// BGNEXTN を返す．
int
GdsPath::bgn_extn() const
{
  return mBgnExtn;
}

// ENDEXTN を返す．
int
GdsPath::end_extn() const
{
  return mEndExtn;
}

// 座標のリストを返す．
GdsXY*
GdsPath::xy() const
{
  return mXY;
}

END_NAMESPACE_YM_GDS

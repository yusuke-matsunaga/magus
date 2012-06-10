#ifndef YM_GDS_GDSBOUNDARY_H
#define YM_GDS_GDSBOUNDARY_H

/// @file ym_gds/GdsBoundery.h
/// @brief GdsBoundary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsElement.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsBoundary GdsBoundary.h "ym_gds/GdsBoundary.h"
/// @brief BOUNDARY を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsBoundary :
  public GdsElement
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] datatype DATATYPE の値
  /// @param[in] xy XY の値
  GdsBoundary(ymuint16 elflags,
	      ymint32 plex,
	      ymint16 layer,
	      ymint16 datatype,
	      GdsXY* xy);

  /// @brief デストラクタ
  ~GdsBoundary();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 層番号を返す．
  int
  layer() const;

  /// @brief データ型を返す．
  int
  datatype() const;

  /// @brief 座標のリストを返す．
  GdsXY*
  xy() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号 ( 0 - 255 )
  ymuint8 mLayer;

  // データ型 ( 0 - 255 )
  ymuint8 mDataType;

  // 座標のリスト
  GdsXY* mXY;

};

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] datatype DATATYPE の値
// @param[in] xy XY の値
inline
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
inline
GdsBoundary::~GdsBoundary()
{
}

// @brief 層番号を返す．
inline
int
GdsBoundary::layer() const
{
  return mLayer;
}

// @brief データ型を返す．
inline
int
GdsBoundary::datatype() const
{
  return mDataType;
}

// @brief 座標のリストを返す．
inline
GdsXY*
GdsBoundary::xy() const
{
  return mXY;
}

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSBOUNDARY_H

#ifndef YM_GDS_GDSPATH_H
#define YM_GDS_GDSPATH_H

/// @file ym_gds/GdsPath.h
/// @brief GdsPath のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsElement.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsPath GdsPath.h "ym_gds/GdsPath.h"
/// @brief PATH を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsPath :
  public GdsElement
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] datatype DATATYPE の値
  /// @param[in] pathtype PATYTYPE の値
  /// @param[in] width WIDTH の値
  /// @param[in] bgn_extn BGNEXTN の値
  /// @param[in] end_extn ENDEXTN の値
  /// @param[in] xy XY の値
  GdsPath(ymuint16 elflags,
	  ymint32 plex,
	  ymint16 layer,
	  ymint16 datatype,
	  ymint16 pathtype,
	  ymint32 width,
	  ymint32 bgn_extn,
	  ymint32 end_extn,
	  GdsXY* xy);

  /// @brief デストラクタ
  ~GdsPath();


public:

  /// @brief 層番号を返す．
  int
  layer() const;

  /// @brief データ型を返す．
  int
  datatype() const;

  /// @brief パスタイプを返す．
  int
  pathtype() const;

  /// @brief 幅を返す．
  int
  width() const;

  /// @brief BGNEXTN を返す．
  int
  bgn_extn() const;

  /// @brief ENDEXTN を返す．
  int
  end_extn() const;

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

  // パスタイプ ( 0 - 4 )
  ymuint8 mPathType;

  // 幅
  ymint32 mWidth;

  // BGNEXTN
  ymint32 mBgnExtn;

  // ENDEXTN
  ymint32 mEndExtn;

  // 座標のリスト
  GdsXY* mXY;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
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
inline
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
  mXY(x)
{
}

// @brief デストラクタ
inline
GdsPath::~GdsPath()
{
}

// 層番号を返す．
inline
int
GdsPath::layer() const
{
  return mLayer;
}

// データ型を返す．
inline
int
GdsPath::datatype() const
{
  return mDataType;
}

// パスタイプを返す．
inline
int
GdsPath::pathtype() const
{
  return mPathType;
}

// 幅を返す．
inline
int
GdsPath::width() const
{
  return mWidth;
}

// BGNEXTN を返す．
inline
int
GdsPath::bgn_extn() const
{
  return mBgnExtn;
}

// ENDEXTN を返す．
inline
int
GdsPath::end_extn() const
{
  return mEndExtn;
}

// 座標のリストを返す．
inline
GdsXY*
GdsPath::xy() const
{
  return mXY;
}

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSPATH_H

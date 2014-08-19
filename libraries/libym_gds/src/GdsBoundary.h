#ifndef GDSBOUNDARY_H
#define GDSBOUNDARY_H

/// @file GdsBoundery.h
/// @brief GdsBoundary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsElement.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsBoundary GdsBoundary.h "GdsBoundary.h"
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
  virtual
  ~GdsBoundary();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 層番号を返す．
  virtual
  int
  layer() const;

  /// @brief データ型を返す．
  virtual
  int
  datatype() const;

  /// @brief 座標のリストを返す．
  virtual
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

END_NAMESPACE_YM_GDS

#endif // GDSBOUNDARY_H

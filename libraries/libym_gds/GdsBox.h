#ifndef GDSBOX_H
#define GDSBOX_H

/// @file GdsBox.h
/// @brief GdsBox のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsElement.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsBox GdsBox.h "GdsBox.h"
/// @brief BOX を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsBox :
  public GdsElement
{
  friend class GdsParser;
private:

  /// @brief コンストラクタ
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] boxtype BOXTYPE の値
  /// @param[in] xy XY の値
  GdsBox(ymuint16 elflags,
	 ymuint32 plex,
	 ymint16 layer,
	 ymint16 boxtype,
	 GdsXY* xy);

  /// デストラクタ
  virtual
  ~GdsBox();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 層番号を返す．
  virtual
  int
  layer() const;

  /// @brief ボックス型を返す．
  virtual
  int
  boxtype() const;

  /// @brief 座標を返す．
  virtual
  GdsXY*
  xy() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号
  ymuint8 mLayer;

  // ボックス型
  ymuint8 mBoxType;

  // 座標のリスト
  GdsXY* mXY;

};

END_NAMESPACE_YM_GDS

#endif // GDSBOX_H

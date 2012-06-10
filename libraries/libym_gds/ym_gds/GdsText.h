#ifndef YM_GDS_GDSTEXT_H
#define YM_GDS_GDSTEXT_H

/// @file ym_gds/GdsText.h
/// @brief GdsText のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsElement.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsText GdsText.h "ym_gds/GdsText.h"
/// @brief TEXT を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsText :
  public GdsElement
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] texttype TEXTTYPE の値
  /// @param[in] presentation PRESENTATION の値
  /// @param[in] pathtype PATHTYPE の値
  /// @param[in] width WIDTH の値
  /// @param[in] strans STRANS の値
  /// @param[in] xy XY座標
  /// @param[in] body 本体の文字列
  GdsText(ymuint16 elflags,
	  ymint32 plex,
	  ymint16 layer,
	  ymint16 texttype,
	  ymuint16 presentation,
	  ymint16 pathtype,
	  ymint32 width,
	  GdsStrans* strans,
	  GdsXY* xy,
	  GdsString* body);

  /// @brief デストラクタ
  ~GdsText();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 層番号を返す．
  int
  layer() const;

  /// @brief テキスト型を返す．
  int
  texttype() const;

  /// @brief パスタイプを返す．
  int
  pathtype() const;

  /// @brief 幅を返す．
  int
  width() const;

  /// @brief reflection ビットが立っていたら true を返す．
  bool
  reflection() const;

  /// @brief absolute magnification ビットが立っていたら true を返す．
  bool
  absolute_magnification() const;

  /// @brief absolute angle ビットが立っていたら true を返す．
  bool
  absolute_angle() const;

  /// @brief magnification factor を返す．
  double
  mag() const;

  /// @brief angular rotation factor を返す．
  double
  angle() const;

  /// @brief 座標を返す．
  XY
  xy() const;

  /// @brief 本体の文字列を返す．
  const char*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号 ( 0 - 255 )
  ymuint8 mLayer;

  // TEXTTYPE
  int mTextType;

  // PRESENTATION
  ymuint16 mPresentation;

  // パスタイプ ( 0 - 4 )
  ymuint8 mPathType;

  /// 幅
  ymint32 mWidth;

  // STRANS
  GdsStrans* mStrans;

  // XY座標
  GdsXY* mXY;

  // 本体の文字列
  GdsString* mBody;

};

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSTEXT_H

#ifndef GDSTEXT_H
#define GDSTEXT_H

/// @file GdsText.h
/// @brief GdsText のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsElement.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsText GdsText.h "GdsText.h"
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
  virtual
  ~GdsText();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 層番号を返す．
  virtual
  int
  layer() const;

  /// @brief テキスト型を返す．
  virtual
  int
  texttype() const;

  /// @brief パスタイプを返す．
  virtual
  int
  pathtype() const;

  /// @brief 幅を返す．
  virtual
  int
  width() const;

  /// @brief reflection ビットが立っていたら true を返す．
  virtual
  bool
  reflection() const;

  /// @brief absolute magnification ビットが立っていたら true を返す．
  virtual
  bool
  absolute_magnification() const;

  /// @brief absolute angle ビットが立っていたら true を返す．
  virtual
  bool
  absolute_angle() const;

  /// @brief magnification factor を返す．
  virtual
  double
  mag() const;

  /// @brief angular rotation factor を返す．
  virtual
  double
  angle() const;

  /// @brief 座標を返す．
  virtual
  GdsXY*
  xy() const;

  /// @brief 本体の文字列を返す．
  virtual
  const char*
  text() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号 ( 0 - 255 )
  ymuint8 mLayer;

  // TEXTTYPE ( 0 - 255 )
  ymuint8 mTextType;

  // PRESENTATION
  ymuint16 mPresentation;

  // パスタイプ ( 0 - 4 )
  ymuint8 mPathType;

  // 幅
  ymint32 mWidth;

  // STRANS
  GdsStrans* mStrans;

  // XY座標
  GdsXY* mXY;

  // 本体の文字列
  GdsString* mBody;

};

END_NAMESPACE_YM_GDS

#endif // GDSTEXT_H

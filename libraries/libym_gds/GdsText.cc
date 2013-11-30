
/// @file GdsText.cc
/// @brief GdsText の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GdsText.h"
#include "ym_gds/GdsString.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsText
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] texttype TEXTTYPE の値
// @param[in] presentation PRESENTATION の値
// @param[in] pathtype PATHTYPE の値
// @param[in] width WIDTH の値
// @param[in] strans STRANS の値
// @param[in] xy XY座標
// @param[in] body 本体の文字列
GdsText::GdsText(ymuint16 elflags,
		 ymint32 plex,
		 ymint16 layer,
		 ymint16 texttype,
		 ymuint16 presentation,
		 ymint16 pathtype,
		 ymint32 width,
		 GdsStrans* strans,
		 GdsXY* xy,
		 GdsString* body) :
  GdsElement(elflags, plex),
  mLayer(layer),
  mTextType(texttype),
  mPresentation(presentation),
  mPathType(pathtype),
  mWidth(width),
  mStrans(strans),
  mXY(xy),
  mBody(body)
{
}

// @brief デストラクタ
GdsText::~GdsText()
{
}

// @brief 層番号を返す．
int
GdsText::layer() const
{
  return mLayer;
}

// @brief テキスト型を返す．
int
GdsText::texttype() const
{
  return mTextType;
}

// @brief パスタイプを返す．
int
GdsText::pathtype() const
{
  return mPathType;
}

// @brief 幅を返す．
int
GdsText::width() const
{
  return mWidth;
}

// @brief reflection ビットが立っていたら true を返す．
bool
GdsText::reflection() const
{
  // mPresentation ビットを解析する．
  return false;
}

// @brief absolute magnification ビットが立っていたら true を返す．
bool
GdsText::absolute_magnification() const
{
  // mPresentation ビットを解析する．
  return false;
}

// @brief absolute angle ビットが立っていたら true を返す．
bool
GdsText::absolute_angle() const
{
  // mPresentation ビットを解析する．
  return false;
}

// @brief magnification factor を返す．
double
GdsText::mag() const
{
  return 0.0;
}

// @brief angular rotation factor を返す．
double
GdsText::angle() const
{
  return 0.0;
}

// @brief 座標を返す．
GdsXY*
GdsText::xy() const
{
  return mXY;
}

// @brief 本体の文字列を返す．
const char*
GdsText::text() const
{
  return mBody->str();
}

END_NAMESPACE_YM_GDS


/// @file GdsElement.cc
/// @brief GdsElement の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsElement.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsElement
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
GdsElement::GdsElement(ymuint16 elflags,
		       ymint32 plex) :
  mElFlags(elflags),
  mPlex(plex),
  mProperty(NULL)
{
}

// @brief デストラクタ
GdsElement::~GdsElement()
{
}

// @brief external data ビットが立っているとき true を返す．
bool
GdsElement::external_data() const
{
  // conv_bitarray(mElFlags, 15, 1);
  return false;
}

// @brief template data ビットが立っているとき true を返す．
bool
GdsElement::template_data() const
{
  // conv_bitarray(mElFlags, 14, 1);
  return false;
}

// @brief plex 番号を返す．
int
GdsElement::plex() const
{
  return mPlex;
}

// @brief 層番号を返す．
int
GdsElement::layer() const
{
  return 0;
}

// @brief データ型を返す．
int
GdsElement::datatype() const
{
  return 0;
}

// @brief ボックス型を返す．
int
GdsElement::boxtype() const
{
  return 0;
}

// @brief パスタイプを返す．
int
GdsElement::pathtype() const
{
  return 0;
}

// @brief テキスト型を返す．
int
GdsElement::texttype() const
{
  return 0;
}

// @brief 幅を返す．
int
GdsElement::width() const
{
  return 0;
}

// @brief reflection ビットが立っていたら true を返す．
bool
GdsElement::reflection() const
{
  return false;
}

// @brief absolute magnification ビットが立っていたら true を返す．
bool
GdsElement::absolute_magnification() const
{
  return false;
}

// @brief absolute angle ビットが立っていたら true を返す．
bool
GdsElement::absolute_angle() const
{
  return false;
}

// @brief magnification factor を返す．
double
GdsElement::mag() const
{
  return 0.0;
}

// @brief angular rotation factor を返す．
double
GdsElement::angle() const
{
  return 0.0;
}

// @brief BGNEXTN を返す．
int
GdsElement::bgn_extn() const
{
  return 0;
}

// @brief ENDEXTN を返す．
int
GdsElement::end_extn() const
{
  return 0;
}

// @brief 座標のリストを返す．
GdsXY*
GdsElement::xy() const
{
  return NULL;
}

// @brief 本体の文字列を返す．
const char*
GdsElement::text() const
{
  return NULL;
}

// @brief property の先頭要素を返す．
const GdsProperty*
GdsElement::property() const
{
  return mProperty;
}

// @brief 次の要素を返す．
const GdsElement*
GdsElement::next()
{
  return mLink;
}

END_NAMESPACE_YM_GDS

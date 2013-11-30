
/// @file GdsRefBase.cc
/// @brief GdsRefBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GdsRefBase.h"
#include "ym_gds/GdsString.h"
#include "ym_gds/GdsStrans.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsRefBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
GdsRefBase::GdsRefBase(ymuint16 elflags,
		       ymint32 plex,
		       GdsString* strname,
		       GdsStrans* strans) :
  GdsElement(elflags, plex),
  mStrName(strname),
  mStrans(strans)
{
}

// @brief デストラクタ
GdsRefBase::~GdsRefBase()
{
}

// @brief 参照している構造名を返す．
const char*
GdsRefBase::strname()
{
  return mStrName->str();
}

// @brief reflection ビットが立っていたら true を返す．
bool
GdsRefBase::reflection() const
{
  // mStrans->flags() を解析する．
  return false;
}

// @brief absolute magnification ビットが立っていたら true を返す．
bool
GdsRefBase::absolute_magnification() const
{
  // mStrans->flags() を解析する．
  return false;
}

// @brief absolute angle ビットが立っていたら true を返す．
bool
GdsRefBase::absolute_angle() const
{
  // mStrans->flags() を解析する．
  return false;
}

// @brief magnification factor を返す．
double
GdsRefBase::mag() const
{
  return mStrans->mag();
}

// @brief angular rotation factor を返す．
double
GdsRefBase::angle() const
{
  return mStrans->angle();
}

END_NAMESPACE_YM_GDS

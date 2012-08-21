#ifndef YM_GDS_GDSPROPERTY_H
#define YM_GDS_GDSPROPERTY_H

/// @file ym_gds/GdsProperty.h
/// @brief GdsProperty のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"
#include "ym_gds/GdsString.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsProperty GdsProperty.h "GdsProperty.h"
/// @brief (PROPATTR, PROPVALUE) を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsProperty
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] attr PROPATTR の値
  /// @param[in] value PROPVALUE の値
  GdsProperty(ymuint attr,
	      GdsString* value);

  /// @brief デストラクタ
  ~GdsProperty();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PROPATTR の値を返す．
  ymuint
  attr() const;

  /// @brief PROPVALUE の値を返す．
  const char*
  value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // PROPATTR の値
  ymuint8 mAttr;

  // PROPVALUE の値
  GdsString* mValue;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] attr PROPATTR の値
// @param[in] value PROPVALUE の値
inline
GdsProperty::GdsProperty(ymuint attr,
			 GdsString* value) :
  mAttr(attr),
  mValue(value)
{
}

// @brief デストラクタ
inline
GdsProperty::~GdsProperty()
{
}

// @brief PROPATTR の値を返す．
inline
ymuint
GdsProperty::attr() const
{
  return mAttr;
}

// @brief PROPVALUE の値を返す．
inline
const char*
GdsProperty::value() const
{
  return mValue->str();
}

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSPROPERTY_H

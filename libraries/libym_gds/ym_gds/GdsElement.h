#ifndef YM_GDS_GDSELEMENT_H
#define YM_GDS_GDSELEMENT_H

/// @file GdsElement.h
/// @brief GdsElement のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsElement GdsElement.h "ym_gds/GdsElement.h"
/// @brief 要素の基底クラス
//////////////////////////////////////////////////////////////////////
class GdsElement
{
  friend class GdsParser;

protected:

  /// @brief コンストラクタ
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  GdsElement(ymuint16 elflags,
	     ymint32 plex);

  /// @brief デストラクタ
  ~GdsElement();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief external data ビットが立っているとき true を返す．
  bool
  external_data() const;

  /// @brief template data ビットが立っているとき true を返す．
  bool
  template_data() const;

  /// @brief plex 番号を返す．
  int
  plex() const;

  /// @brief property のリストを返す．
  const vector<GdsProperty*>&
  property_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ELFLAGS
  ymuint16 mElFlags;

  // PLEX
  ymint32 mPlex;

  // property のリスト
  vector<GdsProperty*> mPropertyList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
inline
GdsElement::GdsElement(ymuint16 elflags,
		       ymint32 plex) :
  mElFlags(elflags),
  mPlex(plex)
{
}

// @brief デストラクタ
inline
GdsElement::~GdsElement()
{
}

// @brief external data ビットが立っているとき true を返す．
inline
bool
GdsElement::external_data() const
{
  // conv_bitarray(mElFlags, 15, 1);
  return false;
}

// @brief template data ビットが立っているとき true を返す．
inline
bool
GdsElement::template_data() const
{
  // conv_bitarray(mElFlags, 14, 1);
  return false;
}

// @brief plex 番号を返す．
inline
int
GdsElement::plex() const
{
  return mPlex;
}

// @brief property のリストを返す．
inline
const vector<GdsProperty*>&
GdsElement::property_list() const
{
  return mPropertyList;
}

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSELEMENT_H

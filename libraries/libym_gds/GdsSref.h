#ifndef GDSSREF_H
#define GDSSREF_H

/// @file GdsSref.h
/// @brief GdsSref のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GdsRefBase.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsSref GdsSref.h "GdsSref.h"
/// @brief SREF を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsSref :
  public GdsRefBase
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] strname 構造名
  /// @param[in] strans STRANS の値
  /// @param[in] xy 座標
  GdsSref(ymuint16 elflags,
	  ymint32 plex,
	  GdsString* strname,
	  GdsStrans* strans,
	  GdsXY* xy);

  /// @brief デストラクタ
  virtual
  ~GdsSref();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// XY 座標を返す．
  virtual
  GdsXY*
  xy() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // XY座標
  GdsXY* mXY;

};

END_NAMESPACE_YM_GDS

#endif // GDSSREF_H

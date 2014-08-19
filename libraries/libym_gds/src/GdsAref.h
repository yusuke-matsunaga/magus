#ifndef GDSAREF_H
#define GDSAREF_H

/// @file GdsAref.h
/// @brief GdsAref のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GdsRefBase.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsAref GdsAref.h "GdsAref.h"
/// @brief AREF
//////////////////////////////////////////////////////////////////////
class GdsAref :
  public GdsRefBase
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] strname 構造名
  /// @param[in] strans STRANS の値
  /// @param[in] colrow 列と行の数
  /// @param[in] xy 座標
  GdsAref(ymuint16 elflags,
	  ymint32 plex,
	  GdsString* strname,
	  GdsStrans* strans,
	  ymuint32 colrow,
	  GdsXY* xy);

  /// @brief デストラクタ
  virtual
  ~GdsAref();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief column 数を返す．
  int
  column() const;

  /// @brief row 数を返す．
  int
  row() const;

  /// @brief XY 座標を返す．
  GdsXY*
  xy() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // column 数
  ymint16 mColumn;

  // row 数
  ymint16 mRow;

  // XY座標
  GdsXY* mXY;

};

END_NAMESPACE_YM_GDS

#endif // GDSAREF_H

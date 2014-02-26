#ifndef GDSREFBASE_H
#define GDSREFBASE_H

/// @file GdsRefBase.h
/// @brief GdsRefBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsElement.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsRefBase GdsRefBase.h "GdsRefBase.h"
/// @brief 参照型要素の基底クラス
//////////////////////////////////////////////////////////////////////
class GdsRefBase :
  public GdsElement
{
  friend class GdsParser;

protected:

  /// @brief コンストラクタ
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] strname 構造名
  /// @param[in] strans STRANS の値
  GdsRefBase(ymuint16 elflags,
	     ymint32 plex,
	     GdsString* strname,
	     GdsStrans* strans);

  /// @brief デストラクタ
  virtual
  ~GdsRefBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照している構造名を返す．
  virtual
  const char*
  strname();

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照している構造名
  GdsString* mStrName;

  // STRANS
  GdsStrans* mStrans;

};

END_NAMESPACE_YM_GDS

#endif // GDSREFBASE_H

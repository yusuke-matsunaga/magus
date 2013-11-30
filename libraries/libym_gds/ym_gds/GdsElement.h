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
  virtual
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

  /// @brief 層番号を返す．
  virtual
  int
  layer() const;

  /// @brief データ型を返す．
  virtual
  int
  datatype() const;

  /// @brief ボックス型を返す．
  virtual
  int
  boxtype() const;

  /// @brief パスタイプを返す．
  virtual
  int
  pathtype() const;

  /// @brief テキスト型を返す．
  virtual
  int
  texttype() const;

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

  /// @brief BGNEXTN を返す．
  virtual
  int
  bgn_extn() const;

  /// @brief ENDEXTN を返す．
  virtual
  int
  end_extn() const;

  /// @brief 座標のリストを返す．
  virtual
  GdsXY*
  xy() const;

  /// @brief 本体の文字列を返す．
  virtual
  const char*
  text() const;

  /// @brief property の先頭要素を返す．
  const GdsProperty*
  property() const;

  /// @brief 次の要素を返す．
  const GdsElement*
  next();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ELFLAGS
  ymuint16 mElFlags;

  // PLEX
  ymint32 mPlex;

  // property の先頭要素
  GdsProperty* mProperty;

  // 次の要素
  GdsElement* mLink;

};

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSELEMENT_H

#ifndef GDSPATH_H
#define GDSPATH_H

/// @file GdsPath.h
/// @brief GdsPath のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsElement.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsPath GdsPath.h "GdsPath.h"
/// @brief PATH を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsPath :
  public GdsElement
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] elflags ELFLAGS の値
  /// @param[in] plex PLEX の値
  /// @param[in] layer LAYER の値
  /// @param[in] datatype DATATYPE の値
  /// @param[in] pathtype PATYTYPE の値
  /// @param[in] width WIDTH の値
  /// @param[in] bgn_extn BGNEXTN の値
  /// @param[in] end_extn ENDEXTN の値
  /// @param[in] xy XY の値
  GdsPath(ymuint16 elflags,
	  ymint32 plex,
	  ymint16 layer,
	  ymint16 datatype,
	  ymint16 pathtype,
	  ymint32 width,
	  ymint32 bgn_extn,
	  ymint32 end_extn,
	  GdsXY* xy);

  /// @brief デストラクタ
  ~GdsPath();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 層番号を返す．
  virtual
  int
  layer() const;

  /// @brief データ型を返す．
  virtual
  int
  datatype() const;

  /// @brief パスタイプを返す．
  virtual
  int
  pathtype() const;

  /// @brief 幅を返す．
  virtual
  int
  width() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号 ( 0 - 255 )
  ymuint8 mLayer;

  // データ型 ( 0 - 255 )
  ymuint8 mDataType;

  // パスタイプ ( 0 - 4 )
  ymuint8 mPathType;

  // 幅
  ymint32 mWidth;

  // BGNEXTN
  ymint32 mBgnExtn;

  // ENDEXTN
  ymint32 mEndExtn;

  // 座標のリスト
  GdsXY* mXY;

};

END_NAMESPACE_YM_GDS

#endif // GDSPATH_H

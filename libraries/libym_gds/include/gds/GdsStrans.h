#ifndef GDS_GDSSTRANS_H
#define GDS_GDSSTRANS_H

/// @file gds/GdsStrans.h
/// @brief GdsStrans のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsStrans GdsStrans.h "gds/GdsStrans.h"
/// @brief STRANS を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsStrans
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] flags フラグ
  /// @param[in] mag 拡大倍率
  /// @param[in] angle 回転角度
  GdsStrans(ymuint16 flags,
	    double mag,
	    double angle);

  /// @brief デストラクタ
  ~GdsStrans();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief フラグを返す．
  ymuint
  flags() const;

  /// @brief 拡大倍率を返す．
  double
  mag() const;

  /// @brief 回転角度を返す．
  double
  angle() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // フラグ
  ymuint16 mFlags;

  // 拡大倍率
  double mMag;

  // 回転角度
  double mAngle;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] flags フラグ
// @param[in] mag 拡大倍率
// @param[in] angle 回転角度
inline
GdsStrans::GdsStrans(ymuint16 flags,
		     double mag,
		     double angle) :
  mFlags(flags),
  mMag(mag),
  mAngle(angle)
{
}

// @brief デストラクタ
inline
GdsStrans::~GdsStrans()
{
}

// @brief フラグを返す．
inline
ymuint
GdsStrans::flags() const
{
  return mFlags;
}

// @brief 拡大倍率を返す．
inline
double
GdsStrans::mag() const
{
  return mMag;
}

// @brief 回転角度を返す．
inline
double
GdsStrans::angle() const
{
  return mAngle;
}

END_NAMESPACE_YM_GDS

#endif // GDS_GDSSTRANS_H

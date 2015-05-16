#ifndef NLPOINT_H
#define NLPOINT_H

/// @file NlPoint.h
/// @brief NlPoint のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlPoint NlPoint.h "NlPoint.h"
/// @brief 点(座標)を表すクラス
//////////////////////////////////////////////////////////////////////
class NlPoint
{
public:

  /// @brief コンストラクタ
  /// @param[in] x X座標
  /// @param[in] y Y座標
  NlPoint(ymuint x = 0,
	  ymuint y = 0);

  /// @brief デストラクタ
  ~NlPoint();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief X 座標を得る．
  ymuint
  x() const;

  /// @brief Y 座標を得る．
  ymuint
  y() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // X座標
  ymuint mX;

  // Y座標
  ymuint mY;

};


/// @relates NlPoint
/// @brief NlPoint の内容を出力する．
ostream&
operator<<(ostream& s,
	   const NlPoint& point);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] x X座標
// @param[in] y Y座標
inline
NlPoint::NlPoint(ymuint x,
		 ymuint y) :
  mX(x),
  mY(y)
{
}

// @brief デストラクタ
inline
NlPoint::~NlPoint()
{
}

// @brief X 座標を得る．
inline
ymuint
NlPoint::x() const
{
  return mX;
}

// @brief Y 座標を得る．
inline
ymuint
NlPoint::y() const
{
  return mY;
}

// @relates NlPoint
// @brief NlPoint の内容を出力する．
inline
ostream&
operator<<(ostream& s,
	   const NlPoint& point)
{
  s << "(" << point.x() << ", " << point.y() << ")";
  return s;
}

END_NAMESPACE_YM_NLINK

#endif // NLPOINT_H

#ifndef NLCONNECTION_H
#define NLCONNECTION_H

/// @file NlConnection.h
/// @brief NlConnection のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "NlPoint.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlConnection NlConnection.h "NlConnection.h"
/// @brief 線分(結線要求)を表すクラス
//////////////////////////////////////////////////////////////////////
class NlConnection
{
public:

  /// @brief コンストラクタ
  /// @param[in] start_point 始点
  /// @param[in] end_point 終点
  NlConnection(const NlPoint& start_point,
	       const NlPoint& end_point);

  /// @brief デストラクタ
  ~NlConnection();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 始点を得る．
  NlPoint
  start_point() const;

  /// @brief 終点を得る．
  NlPoint
  end_point() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 始点
  NlPoint mStartPoint;

  // 終点
  NlPoint mEndPoint;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] start_point 始点
// @param[in] end_point 終点
inline
NlConnection::NlConnection(const NlPoint& start_point,
			   const NlPoint& end_point) :
  mStartPoint(start_point),
  mEndPoint(end_point)
{
}

// @brief デストラクタ
inline
NlConnection::~NlConnection()
{
}

// @brief 始点を得る．
inline
NlPoint
NlConnection::start_point() const
{
  return mStartPoint;
}

// @brief 終点を得る．
inline
NlPoint
NlConnection::end_point() const
{
  return mEndPoint;
}

END_NAMESPACE_YM_NLINK

#endif // NLCONNECTION_H

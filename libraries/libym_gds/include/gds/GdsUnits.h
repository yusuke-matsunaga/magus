#ifndef GDS_GDSUNITS_H
#define GDS_GDSUNITS_H

/// @file gds/GdsUnits.h
/// @brief GdsUnits のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsUnits GdsUnits.h "gds/GdsUnits.h"
/// @brief 単位を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsUnits
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] user ユーザー単位
  /// @param[in] meter メーター単位
  GdsUnits(double user,
	   double meter);

  /// @brief デストラクタ
  ~GdsUnits();


public:

  /// @brief ユーザー単位を返す．
  double
  user_unit() const;

  /// @brief メーター単位を返す．
  double
  meter_unit() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ユーザー単位
  double mUser;

  // メーター単位
  double mMeter;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] user ユーザー単位
// @param[in] meter メーター単位
inline
GdsUnits::GdsUnits(double user,
		   double meter) :
  mUser(user),
  mMeter(meter)
{
}

// @brief デストラクタ
inline
GdsUnits::~GdsUnits()
{
}

// @brief ユーザー単位を返す．
inline
double
GdsUnits::user_unit() const
{
  return mUser;
}

// @brief メーター単位を返す．
inline
double
GdsUnits::meter_unit() const
{
  return mMeter;
}

END_NAMESPACE_YM_GDS

#endif // GDS_GDSUNIT_H

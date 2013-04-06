#ifndef MINPATSTATS_H
#define MINPATSTATS_H

/// @file MinPatStats.h
/// @brief MinPatStats のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPatStats MinPatStats.h "MinPatStats.h"
/// @brief MinPat の statistics data を表すクラス
//////////////////////////////////////////////////////////////////////
class MinPatStats
{
public:

  /// @brief コンストラクタ
  MinPatStats();

  /// @brief デストラクタ
  ~MinPatStats();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 情報を設定する．
  void
  set(ymuint pat1,
      ymuint pat2,
      UStime time);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 元のパタン数を得る．
  ymuint
  original_patterns() const;

  /// @brief 最小化後のパタン数を得る．
  ymuint
  minimized_patterns() const;

  /// @brief 計算時間を得る．
  USTime
  time() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 元のパタン数
  ymuint32 mPatNum1;

  // 最小化後のパタン数
  ymuint32 mPatNum2;

  // 計算時間
  USTime mTime;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
MinPatStats::MinPatStats()
{
}

// @brief デストラクタ
inline
MinPatStats::~MinPatStats()
{
}

// @brief 情報を設定する．
inline
void
MinPatStats::set(ymuint pat1,
		 ymuint pat2,
		 UStime time)
{
  mPatNum1 = pat1;
  mPatNum2 = pat2;
  mTime = time;
}

// @brief 元のパタン数を得る．
inline
ymuint
MinPatStats::original_patterns() const
{
  return mPatNum1;
}

// @brief 最小化後のパタン数を得る．
inline
ymuint
MinPatStats::minimized_patterns() const
{
  return mPatNum2;
}

// @brief 計算時間を得る．
inline
USTime
MinPatStats::time() const
{
  return mTime;
}

END_NAMESPACE_YM_ATPG

#endif // MINPATSTATS_H

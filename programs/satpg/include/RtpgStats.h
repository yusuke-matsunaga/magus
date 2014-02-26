#ifndef RTPGSTATS_H
#define RTPGSTATS_H

/// @file RtpgStats.h
/// @brief RtpgStats のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "utils/USTime.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class RtpgStats Rtpg.h "Rtpg.h"
/// @brief RTPG の statistics data を表すクラス
//////////////////////////////////////////////////////////////////////
class RtpgStats
{
public:

  /// @brief コンストラクタ
  RtpgStats();

  /// @brief デストラクタ
  ~RtpgStats();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 情報を設定する関数
  void
  set(ymuint det_num,
      ymuint pat_num,
      ymuint epat_num,
      USTime time);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 検出した故障数を得る．
  ymuint
  detected_faults() const;

  /// @brief シミュレーションしたパタン数を得る．
  ymuint
  simulated_patterns() const;

  /// @brief 有効なパタン数を得る．
  ymuint
  effective_patterns() const;

  /// @brief 計算時間を得る．
  USTime
  time() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 検出した故障数
  ymuint32 mDetectNum;

  // 総パタン数
  ymuint32 mPatNum;

  // 有効パタン数
  ymuint32 mEfPatNum;

  // 計算時間
  USTime mTime;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 情報を設定する関数
inline
void
RtpgStats::set(ymuint det_num,
	       ymuint pat_num,
	       ymuint epat_num,
	       USTime time)
{
  mDetectNum = det_num;
  mPatNum = pat_num;
  mEfPatNum = epat_num;
  mTime = time;
}

// @brief コンストラクタ
inline
RtpgStats::RtpgStats()
{
  mDetectNum = 0;
  mPatNum = 0;
  mEfPatNum = 0;
}

// @brief デストラクタ
inline
RtpgStats::~RtpgStats()
{
}

// @brief 検出した故障数を得る．
inline
ymuint
RtpgStats::detected_faults() const
{
  return mDetectNum;
}

// @brief シミュレーションしたパタン数を得る．
inline
ymuint
RtpgStats::simulated_patterns() const
{
  return mPatNum;
}

// @brief 有効なパタン数を得る．
inline
ymuint
RtpgStats::effective_patterns() const
{
  return mEfPatNum;
}

// @brief 計算時間を得る．
inline
USTime
RtpgStats::time() const
{
  return mTime;
}

END_NAMESPACE_YM_SATPG

#endif // RTPGSTATS_H

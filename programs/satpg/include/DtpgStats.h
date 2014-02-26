#ifndef DTPGSTATS_H
#define DTPGTTATS_H

/// @file DtpgStats.h
/// @brief DtpgStats のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "utils/USTime.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgStats DtpgStats.h "DtpgStats.h"
/// @brief DTPG の統計情報を表すクラス
//////////////////////////////////////////////////////////////////////
struct DtpgStats
{
  /// @brief CNF 式を生成した回数
  ymuint32 mCnfGenCount;

  /// @brief CNF 式の生成に費やした時間
  USTime mCnfGenTime;

  /// @brief テスト生成に成功した回数．
  ymuint32 mDetCount;

  /// @brief テスト生成に成功した時の SAT に要した時間
  USTime mDetTime;

  /// @brief 冗長故障と判定した回数
  ymuint32 mRedCount;

  /// @brief 冗長故障と判定した時の SAT に要した時間
  USTime mRedTime;

  /// @brief アボートした回数
  ymuint32 mAbortCount;

  /// @brief アボートした時の SAT に要した時間
  USTime mAbortTime;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGSTATS_H

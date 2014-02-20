#ifndef LOGIC_SATSTATS_H
#define LOGIC_SATSTATS_H

/// @file logic/SatStats.h
/// @brief SatStats のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/sat_nsdef.h"
#include "utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatStats SatStats.h "logic/SatStats.h"
/// @ingroup SatGroup
/// @brief SAT ソルバの状態を表すクラス
/// @sa SatSolver
//////////////////////////////////////////////////////////////////////
struct SatStats
{
  /// @brief restartの回数
  ymuint mRestart;

  /// @brief 変数の数．
  ymuint mVarNum;

  /// @brief 制約節の数
  ymuint64 mConstrClauseNum;

  /// @brief 制約節のリテラル数
  ymuint64 mConstrLitNum;

  /// @brief 学習節の数
  ymuint64 mLearntClauseNum;

  /// @brief 学習節のリテラル数
  ymuint64 mLearntLitNum;

  /// @brief コンフリクト数
  ymuint64 mConflictNum;

  /// @brief decision 数
  ymuint64 mDecisionNum;

  /// @brief implication数
  ymuint64 mPropagationNum;

  /// @brief コンフリクト数の制限
  ymuint64 mConflictLimit;

  /// @brief 学習節の制限
  ymuint64 mLearntLimit;

  /// @brief 計算時間
  USTime mTime;

};

END_NAMESPACE_YM_SAT

#endif // LOGIC_SATSTATS_H

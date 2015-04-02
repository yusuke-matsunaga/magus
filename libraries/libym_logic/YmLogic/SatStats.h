#ifndef YMYMLOGIC_SATSTATS_H
#define YMYMLOGIC_SATSTATS_H

/// @file YmLogic/SatStats.h
/// @brief SatStats のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/sat_nsdef.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatStats SatStats.h "YmLogic/SatStats.h"
/// @ingroup SatGroup
/// @brief SAT ソルバの状態を表すクラス
/// @sa SatSolver
//////////////////////////////////////////////////////////////////////
struct SatStats
{
  /// @brief 内容をクリアする ．
  void
  clear();

  /// @brief 加算付き代入
  const SatStats&
  operator+=(const SatStats& right);

  /// @brief 減算付き代入
  const SatStats&
  operator-=(const SatStats& right);

  /// @brief MAX演算つき代入
  const SatStats&
  max_assign(const SatStats& right);

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

/// @brief 加算
inline
SatStats
operator+(const SatStats& left,
	  const SatStats& right)
{
  return SatStats(left).operator+=(right);
}

/// @brief 減算
inline
SatStats
operator-(const SatStats& left,
	  const SatStats& right)
{
  return SatStats(left).operator-=(right);
}

/// @brief MAX演算
inline
SatStats
max(const SatStats& left,
    const SatStats& right)
{
  return SatStats(left).max_assign(right);
}

END_NAMESPACE_YM_SAT

#endif // YMYMLOGIC_SATSTATS_H

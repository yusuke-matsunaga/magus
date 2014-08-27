
/// @file SatStats.sip
/// @brief SatStats の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

struct SatStats
{
%TypeHeaderCode
#include "YmLogic/SatStats.h"
using nsYm::SatStats;
%End

public:

  /// @brief restartの回数
  unsigned int mRestart;

  /// @brief 変数の数．
  unsigned int mVarNum;

  /// @brief 制約節の数
  unsigned long mConstrClauseNum;

  /// @brief 制約節のリテラル数
  unsigned long mConstrLitNum;

  /// @brief 学習節の数
  unsigned long mLearntClauseNum;

  /// @brief 学習節のリテラル数
  unsigned long mLearntLitNum;

  /// @brief コンフリクト数
  unsigned long mConflictNum;

  /// @brief decision 数
  unsigned long mDecisionNum;

  /// @brief implication数
  unsigned long mPropagationNum;

  /// @brief コンフリクト数の制限
  unsigned long mConflictLimit;

  /// @brief 学習節の制限
  unsigned long mLearntLimit;

  /// @brief 計算時間
  USTime mTime;

};


/// @file SatStats.cc
/// @brief SatStats の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/SatStats.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// クラス SatStats
//////////////////////////////////////////////////////////////////////

// @brief 内容をクリアする ．
void
SatStats::clear()
{
  mRestart = 0;
  mVarNum = 0;
  mConstrClauseNum = 0;
  mConstrLitNum = 0;
  mLearntClauseNum = 0;
  mLearntLitNum = 0;
  mConflictNum = 0;
  mDecisionNum = 0;
  mPropagationNum = 0;
}

// @brief 加算付き代入
const SatStats&
SatStats::operator+=(const SatStats& right)
{
  mRestart += right.mRestart;
  mVarNum += right.mVarNum;
  mConstrClauseNum += right.mConstrClauseNum;
  mConstrLitNum += right.mConstrLitNum;
  mLearntClauseNum += right.mLearntClauseNum;
  mLearntLitNum += right.mLearntLitNum;
  mConflictNum += right.mConflictNum;
  mDecisionNum += right.mDecisionNum;
  mPropagationNum += right.mPropagationNum;

  return *this;
}

// @brief 減算付き代入
const SatStats&
SatStats::operator-=(const SatStats& right)
{
  mRestart -= right.mRestart;
  mVarNum -= right.mVarNum;
  mConstrClauseNum -= right.mConstrClauseNum;
  mConstrLitNum -= right.mConstrLitNum;
  mLearntClauseNum -= right.mLearntClauseNum;
  mLearntLitNum -= right.mLearntLitNum;
  mConflictNum -= right.mConflictNum;
  mDecisionNum -= right.mDecisionNum;
  mPropagationNum -= right.mPropagationNum;

  return *this;
}

// @brief MAX演算つき代入
const SatStats&
SatStats::max_assign(const SatStats& right)
{
  if ( mRestart < right.mRestart ) {
    mRestart = right.mRestart;
  }
  if ( mVarNum < right.mVarNum ) {
    mVarNum = right.mVarNum;
  }
  if ( mConstrClauseNum < right.mConstrClauseNum ) {
    mConstrClauseNum = right.mConstrClauseNum;
  }
  if ( mConstrLitNum < right.mConstrLitNum ) {
    mConstrLitNum = right.mConstrLitNum;
  }
  if ( mLearntClauseNum < right.mLearntClauseNum ) {
    mLearntClauseNum = right.mLearntClauseNum;
  }
  if ( mLearntLitNum < right.mLearntLitNum ) {
    mLearntLitNum = right.mLearntLitNum;
  }
  if ( mConflictNum < right.mConflictNum ) {
    mConflictNum = right.mConflictNum;
  }
  if ( mDecisionNum < right.mDecisionNum ) {
    mDecisionNum = right.mDecisionNum;
  }
  if ( mPropagationNum < right.mPropagationNum ) {
    mPropagationNum = right.mPropagationNum;
  }

  return *this;
}


END_NAMESPACE_YM_SAT

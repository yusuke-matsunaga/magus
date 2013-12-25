
/// @file YmsatMsgHandler.cc
/// @brief YmsatMsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmsatMsgHandler.h"
#include "ym_logic/SatStats.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
YmsatMsgHandler::YmsatMsgHandler(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
YmsatMsgHandler::~YmsatMsgHandler()
{
}

// @brief ヘッダの出力
void
YmsatMsgHandler::print_header()
{
  mS << "===================================================================" << endl
     << "| conflicts |       ORIGINAL      |             LEARNT            |" << endl
     << "|           |   Clauses      Lits |     limit   Clauses      Lits |" << endl
     << "===================================================================" << endl;
}

// @brief メッセージ出力関数
void
YmsatMsgHandler::operator()(const SatStats& stats)
{
  mS << "| "
     << setw(9) << stats.mConflictNum
     << " | "
     << setw(9) << stats.mConstrClauseNum
     << " "
     << setw(9) << stats.mConstrLitNum
     << " | "
     << setw(9) << stats.mLearntLimit
     << " "
     << setw(9) << stats.mLearntClauseNum
     << " "
     << setw(9) << stats.mLearntLitNum
     << " |" << endl;
}

// @brief 終了メッセージの出力
// @param[in] stats SAT ソルバの内部状態
void
YmsatMsgHandler::print_tailer(const SatStats& stats)
{
  mS << "===================================================================" << endl
     << "restarts          : " << stats.mRestart << endl
     << "conflicts         : " << stats.mConflictNum << endl
     << "decisions         : " << stats.mDecisionNum << endl
     << "propagations      : " << stats.mPropagationNum << endl
     << "conflict literals : " << stats.mLearntLitNum << endl
     << "CPU time          : " << stats.mTime << endl;
}

END_NAMESPACE_YM

﻿
/// @file FraigSat.cc
/// @brief FraigSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigSat.h"
#include "FraigNode.h"
#include "ym/Range.h"
#include "ym/Timer.h"
#include "ym/SatStats.h"


#if defined(YM_DEBUG)
#define DEBUG_FLAG 1
#endif

#if !defined(DEBUG_FLAG)
#define DEBUG_FLAG 0
#endif


BEGIN_NAMESPACE_FRAIG

BEGIN_NONAMESPACE

const int debug = DEBUG_FLAG;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// FraigSat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FraigSat::FraigSat(
  const SatSolverType& solver_type
) : mSolver{solver_type},
    mLogLevel{0},
    mLogStream{new ofstream("/dev/null")}
{
}

// @brief デストラクタ
FraigSat::~FraigSat()
{
  if ( mLogStream != &cout ) {
    delete mLogStream;
  }
}

// @brief ノードを登録する．
SatLiteral
FraigSat::new_var()
{
  auto var = mSolver.new_variable();
  mSolver.freeze_literal(var);
  return var;
}

// @brief ノードの入出力の関係を表す CNF 式を作る．
void
FraigSat::make_cnf(
  FraigNode* node
)
{
  auto handle1 = node->fanin0_handle();
  auto handle2 = node->fanin1_handle();
  auto lito = node->varid();
  auto lit1 = handle1.literal();
  auto lit2 = handle2.literal();
  mSolver.add_clause(~lit1, ~lit2, lito);
  mSolver.add_clause( lit1, ~lito);
  mSolver.add_clause( lit2, ~lito);
}

// @brief ログレベルを設定する．
void
FraigSat::set_loglevel(
  SizeType level
)
{
  mLogLevel = level;
}

// @brief ログ出力用ストリームを設定する．
void
FraigSat::set_logstream(
  ostream* out
)
{
  if ( mLogStream != &cout ) {
    delete mLogStream;
  }
  mLogStream = out;
}

// node が定数かどうか調べる．
SatBool3
FraigSat::check_const(
  FraigNode* node,
  bool inv
)
{
  if ( debug ) {
    cout << "CHECK CONST";
    if ( inv ) {
      cout << "1";
    }
    else {
      cout << "0";
    }
    cout << " " << setw(6) << node->varid()
	 << "       ";
    cout.flush();
  }

  Timer timer;
  timer.start();

  SatLiteral lit{node->varid(), inv};

  // この関数の戻り値
  SatBool3 code = SatBool3::X;

  // lit = 1 が成り立つか調べる
  SatBool3 stat = check_condition(lit);
  if ( stat == SatBool3::False ) {
    // 成り立たないということは lit = 0
    mSolver.add_clause(~lit);
    if ( debug ) {
      cout << "\tSUCCEED" << endl;
    }
    code = SatBool3::True;
  }
  else if ( stat == SatBool3::True ) {
    // 成り立ったということは ~lit が常に成り立つわけではない．
    if ( debug ) {
      cout << "\tFAILED" << endl;
    }
    code = SatBool3::False;
  }
  else {
    // 分からない．
    if ( debug ) {
      cout << "\tABORTED" << endl;
    }
  }
  mCheckConstInfo.set_result(code, timer.get_time());
  return code;
}

// node1 と node2 機能的に等価かどうか調べる．
SatBool3
FraigSat::check_equiv(
  FraigNode* node1,
  FraigNode* node2,
  bool inv
)
{
  SatLiteral id1 = node1->varid();
  SatLiteral id2 = node2->varid();

  if ( debug ) {
    cout << "CHECK EQUIV  "
	 << setw(6) << id1 << " "
	 << setw(6) << id2;
    if ( inv ) {
      cout << " N";
    }
    cout.flush();
  }

  Timer timer;
  timer.start();

  SatLiteral lit1(id1);
  SatLiteral lit2(id2, inv);

  // この関数の戻り値
  SatBool3 code = SatBool3::X;

  // 等価でない条件
  // - lit1 = 0 かつ lit2 = 1 が成り立つ
  // - lit0 = 1 かつ lit2 = 0 が成り立つ
  SatBool3 stat = check_condition(~lit1,  lit2);
  if ( stat == SatBool3::False ) {
    stat = check_condition( lit1, ~lit2);
    if ( stat == SatBool3::False ) {
      // どの条件も成り立たなかったので等しい
      mSolver.add_clause(~lit1,  lit2);
      mSolver.add_clause( lit1, ~lit2);

      if ( debug ) {
	cout << "\tSUCCEED" << endl;
      }
      code = SatBool3::True;
      goto end;
    }
  }
  if ( stat == SatBool3::True ) {
    if ( debug ) {
      cout << "\tFAILED" << endl;
    }
    code = SatBool3::False;
  }
  else {
    if ( debug ) {
      cout << "\tABORTED" << endl;
    }
  }

 end:
  mCheckEquivInfo.set_result(code, timer.get_time());
  return code;
}

// lit1 が成り立つか調べる．
SatBool3
FraigSat::check_condition(
  SatLiteral lit1
)
{
  vector<SatLiteral> assumptions{lit1};
  SatBool3 ans1 = mSolver.solve(assumptions);

#if defined(VERIFY_SATSOLVER)
  SatSolver solver(nullptr, "minisat");
  for ( auto node: mAllNodes ) {
    SatVarId id = solver.new_variable();
    ASSERT_COND(id == node->varid() );
    if ( node->is_and() ) {
      SatLiteral lito(id, false);
      SatLiteral lit1(node->fanin0()->varid(), node->fanin0_inv());
      SatLiteral lit2(node->fanin1()->varid(), node->fanin1_inv());
      solver.add_clause(~lit1, ~lit2, lito);
      solver.add_clause( lit1, ~lito);
      solver.add_clause( lit2, ~lito);
    }
  }
  SatBool3 ans2 = solver.solve(assumptions);
  if ( ans1 != ans2 ) {
    cout << endl << "ERROR!" << endl;
    cout << "check_condition(" << lit1 << ")" << endl;
    cout << " ans1 = " << ans1 << endl;
    cout << " ans2 = " << ans2 << endl;
    cout << " clauses" << endl;
    for ( auto node: mAllNodes ) {
      if ( node->is_and() ) {
	SatVarId id = node->varid();
	SatLiteral lito(id, false);
	SatLiteral lit1(node->fanin0()->varid(), node->fanin0_inv());
	SatLiteral lit2(node->fanin1()->varid(), node->fanin1_inv());
	cout << "   " << ~lit1 << " + " << ~lit2 << " + " << lito << endl;
	cout << "   " << lit1 << " + " << ~lito << endl;
	cout << "   " << lit2 << " + " << ~lito << endl;
      }
    }
  }
#endif
  return ans1;
}

// lit1 & lit2 が成り立つか調べる．
SatBool3
FraigSat::check_condition(
  SatLiteral lit1,
  SatLiteral lit2
)
{
  vector<SatLiteral> assumptions{lit1, lit2};
  SatBool3 ans1 = mSolver.solve(assumptions);

#if defined(VERIFY_SATSOLVER)
  SatSolver solver(nullptr, "minisat");
  for ( auto node: mAllNodes ) {
    SatVarId id = solver.new_variable();
    ASSERT_COND(id == node->varid() );
    if ( node->is_and() ) {
      SatLiteral lito(id, false);
      SatLiteral lit1(node->fanin0()->varid(), node->fanin0_inv());
      SatLiteral lit2(node->fanin1()->varid(), node->fanin1_inv());
      solver.add_clause(~lit1, ~lit2, lito);
      solver.add_clause(lit1, ~lito);
      solver.add_clause(lit2, ~lito);
    }
  }
  SatBool3 ans2 = solver.solve(assumptions);
  if ( ans1 != ans2 ) {
    cout << endl << "ERROR!" << endl;
    cout << "check_condition("
	 << lit1 << " & " << lit2 << ")" << endl;
    cout << " ans1 = " << ans1 << endl;
    cout << " ans2 = " << ans2 << endl;
    cout << " clauses" << endl;
    for ( auto node: mAllNodes ) {
      if ( node->is_and() ) {
	SatVarId id = node->varid();
	SatLiteral lito(id, false);
	SatLiteral lit1(node->fanin0()->varid(), node->fanin0_inv());
	SatLiteral lit2(node->fanin1()->varid(), node->fanin1_inv());
	cout << "   " << ~lit1 << " + " << ~lit2 << " + " << lito << endl;
	cout << "   " << lit1 << " + " << ~lito << endl;
	cout << "   " << lit2 << " + " << ~lito << endl;
      }
    }
  }
#endif
  return ans1;
}

// @brief 直前の sat_sweep に関する統計情報を出力する．
void
FraigSat::dump_stats(
  ostream& s
)
{
  s << "=====<< AigMgr Statistics >> =====" << endl;
  s << "check_const:" << endl;
  mCheckConstInfo.dump(s);
  s << "----------------------------------" << endl;
  s << "check_equiv:" << endl;
  mCheckEquivInfo.dump(s);
  s << endl;

  SatStats stats;
  mSolver.get_stats(stats);
  s << "----------------------------------" << endl;
  s << "sat stat:" << endl
    << "  restarts          : " << stats.mRestart << endl
    << "  conflicts         : " << stats.mConflictNum << endl
    << "  decisions         : " << stats.mDecisionNum << endl
    << "  propagations      : " << stats.mPropagationNum << endl
    << "  conflict literals : " << stats.mLearntLitNum << endl;
}

FraigSat::SatStat::SatStat()
{
  mTotalCount = 0;
  for ( auto i: { 0, 1, 2 } ) {
    mTimeStat[i].mCount = 0;
    mTimeStat[i].mTotalTime = 0.0;
    mTimeStat[i].mMaxTime = 0.0;
  }
}

void
FraigSat::SatStat::set_result(
  SatBool3 code,
  double t
)
{
  ++ mTotalCount;

  int idx = 0;
  if ( code == SatBool3::True ) {
    idx = 1;
  }
  else if ( code == SatBool3::False ) {
    idx = 2;
  }

  ++ mTimeStat[idx].mCount;
  mTimeStat[idx].mTotalTime += t;
  if ( mTimeStat[idx].mMaxTime < t ) {
    mTimeStat[idx].mMaxTime = t;
  }
}

void
FraigSat::SatStat::dump(
  ostream& s
) const
{
  s << mTimeStat[1].mCount << " / " << mTotalCount << endl;
  if ( mTimeStat[1].mCount > 0 ) {
    s << " In success(total/ave./max): "
      << mTimeStat[1].mTotalTime << " / "
      << mTimeStat[1].mTotalTime / mTimeStat[1].mCount << " / "
      << mTimeStat[1].mMaxTime << endl;
  }
  if ( mTimeStat[2].mCount > 0 ) {
    s << " In failure(total/ave./max): "
      << mTimeStat[2].mTotalTime << " / "
      << mTimeStat[2].mTotalTime / mTimeStat[2].mCount << " / "
      << mTimeStat[2].mMaxTime << endl;
  }
  if ( mTimeStat[0].mCount > 0 ) {
    s << " In abort(total/ave./max): "
      << mTimeStat[0].mTotalTime << " / "
      << mTimeStat[0].mTotalTime / mTimeStat[0].mCount << " / "
      << mTimeStat[0].mMaxTime << endl;
  }
}

END_NAMESPACE_FRAIG
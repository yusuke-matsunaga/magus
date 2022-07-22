﻿
/// @file FraigMgrImpl.cc
/// @brief FraigMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "FraigMgrImpl.h"
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
// FraigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @brief sig_size シグネチャのサイズ
// @param[in] solver_type SAT-solver の種類を表すオブジェクト
FraigMgrImpl::FraigMgrImpl(int sig_size,
			   const SatSolverType& solver_type) :
  mPatSize(sig_size * 2),
  mPatInit(sig_size),
  mPatUsed(sig_size),
  mSolver(solver_type),
  mSimCount(0),
  mSimTime(0.0),
  mLogLevel(0),
  mLogStream(new ofstream("/dev/null")),
  mLoopLimit(1000)
{
}

// @brief デストラクタ
FraigMgrImpl::~FraigMgrImpl()
{
  for ( auto node: mAllNodes ) {
    delete node;
  }
  if ( mLogStream != &cout ) {
    delete mLogStream;
  }
}

// @brief 外部入力を作る．
FraigHandle
FraigMgrImpl::make_input()
{
  if ( debug ) {
    cout << "make_input ...";
  }

  FraigNode* node = new_node();
  int iid = mInputNodes.size();
  node->set_input(iid);
  mInputNodes.push_back(node);
  vector<ymuint64> tmp(mPatUsed);
  std::uniform_int_distribution<ymuint64> rd;
  for ( int i: Range(mPatUsed) ) {
    tmp[i] = rd(mRandGen);
  }
  node->set_pat(0, mPatUsed, tmp);
  FraigHandle ans = FraigHandle(node, false);

  if ( debug ) {
    cout << " -> " << ans << endl;
  }

  return ans;
}

// @brief 2つのノードの AND を取る．
// @param[in] handle1, handle2 入力の AIG ハンドル
FraigHandle
FraigMgrImpl::make_and(FraigHandle handle1,
		       FraigHandle handle2)
{
  if ( debug ) {
    cout << "make_and(" << handle1 << ", " << handle2 << ") ..." << endl;
  }

  FraigHandle ans;

  // trivial な場合の処理
  if ( handle1.is_zero() || handle2.is_zero() ) {
    ans = FraigHandle::zero();
  }
  else if ( handle1.is_one() ) {
    ans = handle2;
  }
  else if ( handle2.is_one() ) {
    ans = handle1;
  }
  else if ( handle1 == handle2 ) {
    ans = handle1;
  }
  else if ( handle1.node() == handle2.node() ) {
    // handle1.inv != handle2.inv() のはず
    ans = FraigHandle::zero();
  }
  else {
    // 順番の正規化
    if ( handle1.literal().varid() < handle2.literal().varid() ) {
      FraigHandle tmp = handle1;
      handle1 = handle2;
      handle2 = tmp;
    }

    if ( debug ) {
      cout << "  after normalize: " << handle1 << ", " << handle2 << endl;
    }

    // 同じ構造を持つノードが既にないか調べる．
    if ( !mHashTable1.find(handle1, handle2, ans) ) {
      // ノードを作る．
      FraigNode* node = new_node();
      node->set_fanin(handle1, handle2);
      node->calc_pat(0, mPatUsed);

      // 構造ハッシュに追加する．
      mHashTable1.add(node);

      // 入出力の関係を表す CNF を作る．
      make_cnf(node);

      if ( debug ) {
	cout << "  new node: " << FraigHandle(node, false) << endl;
      }

      // 縮退検査を行う．
      if ( verify_const(node, ans) != SatBool3::True ) {
	// パタンハッシュで等しいノードがないか調べる．
	if ( !mHashTable2.find(node, *this, ans) ) {
	  mHashTable2.add(node);
	  ans = FraigHandle(node, false);
	}
      }
    }
  }

  if ( debug ) {
    cout << "  -> " << ans << endl;
  }

  return ans;
}

// @brief ノードの入出力の関係を表す CNF 式を作る．
void
FraigMgrImpl::make_cnf(FraigNode* node)
{
  FraigHandle handle1 = node->fanin0_handle();
  FraigHandle handle2 = node->fanin1_handle();
  SatLiteral lito(node->varid(), false);
  SatLiteral lit1 = handle1.literal();
  SatLiteral lit2 = handle2.literal();
  mSolver.add_clause(~lit1, ~lit2, lito);
  mSolver.add_clause( lit1, ~lito);
  mSolver.add_clause( lit2, ~lito);
}

// @brief 0縮退検査を行う．
// @param[in] node 対象のノード
// @param[out] ans 答
// @retval SatBool3::True 定数に縮退していた．
// @retval SatBool3::False 定数ではなかった．
// @retval SatBool3::X 不明(アボート)
SatBool3
FraigMgrImpl::verify_const(FraigNode* node,
			   FraigHandle& ans)
{
  SatBool3 stat = SatBool3::False;
  if ( !node->check_1mark() ) {
    // 定数0の可能性があるか調べる．
    stat = check_const(node, false);
    if ( stat == SatBool3::True ) {
      // 定数0と等価だった．
      node->set_rep(nullptr, false);
      ans = FraigHandle::zero();
    }
    else if ( stat == SatBool3::False ) {
      // 反例をパタンに加えておく．
      add_pat(node);

      ASSERT_COND(node->check_1mark() );
    }
  }
  if ( !node->check_0mark() ) {
    // 定数1の可能性があるか調べる．
    stat = check_const(node, true);
    if ( stat == SatBool3::True ) {
      // 定数1と等価だった．
      node->set_rep(nullptr, true);
      ans = FraigHandle::one();
    }
    else if ( stat == SatBool3::False ) {
      // 反例をパタンに加えておく．
      add_pat(node);

      ASSERT_COND(node->check_0mark() );
    }
  }

  return stat;
}

// @brief パタンハッシュで同値と判定されたノードを比較する．
bool
FraigMgrImpl::compare_node(FraigNode* node1,
			   FraigNode* node2,
			   bool inv,
			   bool& retry)
{
  retry = false;
  if ( compare_pat(node1, node2, inv) ) {
    // node1 と node2 が等価かどうか調べる．
    SatBool3 stat = check_equiv(node1, node2, inv);
    if ( stat == SatBool3::True ) {
      // 等価なノードが見つかった．
      node1->set_rep(node2, inv);
      return true;
    }
    else if ( stat == SatBool3::False ) {
      // 反例をパタンに加えて再ハッシュする．
      add_pat(node2);

      ASSERT_COND( !compare_pat(node1, node2, inv) );
      retry = true;
      return false;
    }
  }
  return false;
}

// @breif 直前の SAT の反例を加えて再ハッシュする．
void
FraigMgrImpl::add_pat(FraigNode* node)
{
  if ( mPatSize <= mPatUsed ) {
    resize_pat(mPatSize * 2);
  }
  mHashTable2.clear();

  // 反例をパタンに加える．
  const SatModel& model = mSolver.model();
  vector<ymuint64> tmp(1);
  int nn = node_num();
  std::uniform_int_distribution<int> rd100(0, 99);
  for ( int i = 0; i < nn; ++ i ) {
    FraigNode* node1 = mAllNodes[i];
    if ( node1->is_input() ) {
      ymuint64 pat = 0U;
      if ( model[node1->varid()] == SatBool3::True ) {
	pat = ~0U;
      }
      else {
	pat = 0U;
      }
      for ( int b = 1; b < 32; ++ b ) {
	if ( rd100(mRandGen) <= 3 ) {
	  pat ^= (1U << b);
	}
      }
      tmp[0] = pat;
      node1->set_pat(mPatUsed, mPatUsed + 1, tmp);
    }
    else {
      node1->calc_pat(mPatUsed, mPatUsed + 1);
    }

    if ( node1 != node ) {
      mHashTable2.add(node1);
    }
  }
  ++ mPatUsed;
}

// @brief 2つのハンドルが等価かどうか調べる．
SatBool3
FraigMgrImpl::check_equiv(FraigHandle aig1,
			  FraigHandle aig2)
{
  if ( aig1 == aig2 ) {
    // もっとも簡単なパタン
    return SatBool3::True;
  }

  FraigNode* node1 = aig1.node();
  FraigNode* node2 = aig2.node();

  if ( node1 == node2 ) {
    // ということは逆極性なので絶対に等価ではない．
    return SatBool3::False;
  }

  bool inv1 = aig1.inv();
  bool inv2 = aig2.inv();

  if ( aig1.is_zero () ) {
    // 上のチェックで aig2 は定数でないことは明らか
    SatBool3 stat = check_const(node2, inv2);
    return stat;
  }

  if ( aig1.is_one() ) {
    // 上のチェックで aig2 は定数でないことは明らか
    SatBool3 stat = check_const(node2, !inv2);
    return stat;
  }

  if ( aig2.is_zero() ) {
    // 上のチェックで aig1 は定数でないことは明らか
    SatBool3 stat = check_const(node1, inv1);
    return stat;
  }

  if ( aig2.is_one() ) {
    // 上のチェックで aig1 は定数でないことは明らか
    SatBool3 stat = check_const(node1, !inv1);
    return stat;
  }

  bool inv = inv1 ^ inv2;
  SatBool3 stat = check_equiv(node1, node2, inv);
  return stat;
}

// @brief ログレベルを設定する．
void
FraigMgrImpl::set_loglevel(int level)
{
  mLogLevel = level;
}

// @brief ログ出力用ストリームを設定する．
void
FraigMgrImpl::set_logstream(ostream* out)
{
  if ( mLogStream != &cout ) {
    delete mLogStream;
  }
  mLogStream = out;
}

// @brief ランダムシミュレーション制御用のパラメータを設定する．
void
FraigMgrImpl::set_loop_limit(int val)
{
  mLoopLimit = val;
}

// @brief ノードのシミュレーションパタン用配列を確保する．
void
FraigMgrImpl::init_pat(FraigNode* node)
{
  ASSERT_COND(node->mPat == nullptr );
  node->mPat = new ymuint64[mPatSize];
}

// @brief 全ノードのシミュレーションパタン用配列を拡大する．
void
FraigMgrImpl::resize_pat(int size)
{
  int n = mAllNodes.size();
  for ( int i = 0; i < n; ++ i ) {
    FraigNode* node = mAllNodes[i];
    ymuint64* old_array = node->mPat;
    node->mPat = new ymuint64[size];
    for ( int j = 0; j < mPatUsed; ++ j ) {
      node->mPat[j] = old_array[j];
    }
    delete [] old_array;
  }
  mPatSize = size;
}

// @brief シミュレーションパタンが等しいか調べる．
bool
FraigMgrImpl::compare_pat(FraigNode* node1,
			  FraigNode* node2,
			  bool inv)
{
  ymuint64* src1 = node1->mPat;
  ymuint64* src2 = node2->mPat;
  ymuint64* end = src1 + mPatUsed;
  if ( inv ) {
    for ( ; src1 != end; ++ src1, ++ src2) {
      if ( *src1 != ~*src2 ) {
	return false;
      }
    }
  }
  else {
    for ( ; src1 != end; ++ src1, ++ src2) {
      if ( *src1 != *src2 ) {
	return false;
      }
    }
  }
  return true;
}

// @brief 新しいノードを生成する．
FraigNode*
FraigMgrImpl::new_node()
{
  FraigNode* node = new FraigNode();
  node->mVarId = mSolver.new_variable();
  ASSERT_COND(node->mVarId.varid() == mAllNodes.size() );
  mSolver.freeze_literal(SatLiteral(node->mVarId));
  init_pat(node);
  mAllNodes.push_back(node);
  return node;
}

// node が定数かどうか調べる．
SatBool3
FraigMgrImpl::check_const(FraigNode* node,
			  bool inv)
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
FraigMgrImpl::check_equiv(FraigNode* node1,
			  FraigNode* node2,
			  bool inv)
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
FraigMgrImpl::check_condition(SatLiteral lit1)
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
FraigMgrImpl::check_condition(SatLiteral lit1,
			      SatLiteral lit2)
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

#if 0
// @brief FraigHandle に対応するリテラルを返す．
// @note 定数の場合の返り値は未定
SatLiteral
FraigMgrImpl::fraig2literal(FraigHandle aig)
{
  if ( aig.is_const() ) {
    return SatLiteral(SatVarId(0), false);
  }
  SatVarId id = aig.node()->varid();
  bool inv = aig.inv();
  return SatLiteral(id, inv);
}
#endif

#if 0
// @brief 等価候補グループの情報を出力する．
void
FraigMgrImpl::dump_eqgroup(ostream& s) const
{
  for ( int i = 0; i < mNodeNum; ++ i ) {
    SweepNode* snode = &mNodeArray[i];
    if ( snode->mAigNode == nullptr ) continue;
    if ( snode->rep_node() != snode ) continue;
    if ( snode->next_eqnode() == nullptr &&
	 snode->check_1mark() &&
	 snode->check_0mark() ) continue;
    if ( !snode->check_1mark() ) {
      cout << "C0?";
    }
    else if ( !snode->check_0mark() ) {
      cout << "C1?";
    }
    else {
      cout << "   ";
    }
    cout << " {";
    for ( SweepNode* snode1 = snode; snode1; snode1 = snode1->next_eqnode() ) {
      cout << " ";
      if ( snode1->eq_inv() ) {
	cout << "~";
      }
      cout << snode1->aignode()->node_id();
    }
    cout << "}";
    cout << endl;
  }
}
#endif

// @brief 直前の sat_sweep に関する統計情報を出力する．
void
FraigMgrImpl::dump_stats(ostream& s)
{
  s << "=====<< AigMgr Statistics >> =====" << endl;
  s << "simulation:" << endl
    << " total " << mSimCount << " loops" << endl
    << " total " << mSimTime << " sec." << endl;
  s << "----------------------------------" << endl;
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

FraigMgrImpl::SatStat::SatStat()
{
  mTotalCount = 0;
  for ( auto i: { 0, 1, 2 } ) {
    mTimeStat[i].mCount = 0;
    mTimeStat[i].mTotalTime = 0.0;
    mTimeStat[i].mMaxTime = 0.0;
  }
}

void
FraigMgrImpl::SatStat::set_result(SatBool3 code,
				  double t)
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
FraigMgrImpl::SatStat::dump(ostream& s) const
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

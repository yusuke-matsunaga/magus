
/// @file FraigMgrImpl.cc
/// @brief FraigMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigMgrImpl.h"
#include "FraigNode.h"
#include "PatTable.h"
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
FraigMgrImpl::FraigMgrImpl(
  SizeType sig_size,
  const SatSolverType& solver_type
) : mSolver(solver_type),
    mLogLevel(0),
    mLogStream(new ofstream("/dev/null")),
    mLoopLimit(1000)
{
  FraigNode::mPatSize = sig_size * 2;
  FraigNode::mPatUsed = sig_size;
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

  auto node = new FraigNode{};
  reg_node(node);
  int iid = mInputNodes.size();
  node->set_input(iid);
  mInputNodes.push_back(node);
  vector<ymuint64> tmp(FraigNode::mPatUsed);
  std::uniform_int_distribution<ymuint64> rd;
  for ( int i: Range(FraigNode::mPatUsed) ) {
    tmp[i] = rd(mRandGen);
  }
  node->set_pat(0, FraigNode::mPatUsed, tmp);
  FraigHandle ans = FraigHandle(node, false);

  if ( debug ) {
    cout << " -> " << ans << endl;
  }

  return ans;
}

// @brief 2つのノードの AND を取る．
FraigHandle
FraigMgrImpl::make_and(
  FraigHandle handle1,
  FraigHandle handle2
)
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
    FraigNode key{handle1, handle2};
    auto p = mStructTable.find(&key);
    if ( p != mStructTable.end() ) {
      // 等価なノードが存在した．
      auto node = *p;
      ans = FraigHandle{node, false};
    }
    else {
      // ノードを作る．
      auto node = new FraigNode{handle1, handle2};
      reg_node(node);
      node->calc_pat(0, FraigNode::mPatUsed);

      // 構造ハッシュに追加する．
      mStructTable.insert(node);

      // 入出力の関係を表す CNF を作る．
      make_cnf(node);

      if ( debug ) {
	cout << "  new node: " << FraigHandle{node, false} << endl;
      }

      // 縮退検査を行う．
      if ( verify_const(node, ans) != SatBool3::True ) {
	bool go_on;
	do { // do-while() は今風の C++ のスコープに合っていない．
	  go_on = false;
	  auto range = mPatTable.equal_range(node);
	  auto start = range.first;
	  auto end = range.second;
	  bool inv0 = node->pat_hash_inv();
	  for ( auto p = start; p != end; ++ p ) {
	    auto node1 = *p;
	    bool inv = inv0 ^ node1->pat_hash_inv();
	    bool retry = false;
	    bool stat = compare_node(node1, node, inv, retry);
	    if ( stat ) {
	      ans = FraigHandle(node1, inv);
	      break;
	    }
	    if ( retry ) {
	      go_on = true;
	      break;
	    }
	  }
	} while ( go_on );
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
FraigMgrImpl::make_cnf(
  FraigNode* node
)
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
SatBool3
FraigMgrImpl::verify_const(
  FraigNode* node,
  FraigHandle& ans
)
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

      ASSERT_COND( node->check_1mark() );
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

      ASSERT_COND( node->check_0mark() );
    }
  }

  return stat;
}

// @brief パタンハッシュで同値と判定されたノードを比較する．
bool
FraigMgrImpl::compare_node(
  FraigNode* node1,
  FraigNode* node2,
  bool inv,
  bool& retry
)
{
  retry = false;
  PatEq eq;
  if ( eq(node1, node2) ) {
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

      ASSERT_COND( !eq(node1, node2) );
      retry = true;
      return false;
    }
  }

  return false;
}

// @breif 直前の SAT の反例を加えて再ハッシュする．
void
FraigMgrImpl::add_pat(
  FraigNode* node
)
{
  if ( FraigNode::mPatSize <= FraigNode::mPatUsed ) {
    resize_pat(FraigNode::mPatSize * 2);
  }
  mPatTable.clear();

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
      node1->set_pat(FraigNode::mPatUsed, FraigNode::mPatUsed + 1, tmp);
    }
    else {
      node1->calc_pat(FraigNode::mPatUsed, FraigNode::mPatUsed + 1);
    }

    if ( node1 != node ) {
      mPatTable.insert(node1);
    }
  }
  ++ FraigNode::mPatUsed;
}

// @brief 2つのハンドルが等価かどうか調べる．
SatBool3
FraigMgrImpl::check_equiv(
  FraigHandle aig1,
  FraigHandle aig2
)
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
FraigMgrImpl::set_loglevel(
  SizeType level
)
{
  mLogLevel = level;
}

// @brief ログ出力用ストリームを設定する．
void
FraigMgrImpl::set_logstream(
  ostream* out
)
{
  if ( mLogStream != &cout ) {
    delete mLogStream;
  }
  mLogStream = out;
}

// @brief ランダムシミュレーション制御用のパラメータを設定する．
void
FraigMgrImpl::set_loop_limit(
  SizeType val
)
{
  mLoopLimit = val;
}

// @brief 全ノードのシミュレーションパタン用配列を拡大する．
void
FraigMgrImpl::resize_pat(
  SizeType size
)
{
  for ( auto node: mAllNodes ) {
    node->resize_pat(size);
  }
  FraigNode::mPatSize = size;
}

// @brief ノードを登録する．
void
FraigMgrImpl::reg_node(
  FraigNode* node
)
{
  node->mVarId = mSolver.new_variable();
  ASSERT_COND(node->mVarId.varid() == mAllNodes.size() );
  mSolver.freeze_literal(SatLiteral{node->mVarId});
  mAllNodes.push_back(node);
}

// node が定数かどうか調べる．
SatBool3
FraigMgrImpl::check_const(
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
FraigMgrImpl::check_equiv(
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
FraigMgrImpl::check_condition(
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
FraigMgrImpl::check_condition(
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
FraigMgrImpl::dump_stats(
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
FraigMgrImpl::SatStat::set_result(
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
FraigMgrImpl::SatStat::dump(
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

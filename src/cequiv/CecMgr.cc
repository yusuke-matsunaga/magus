
/// @file libym_cec/CecMgr.cc
/// @brief CecMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CecMgr.h"
#include "CecNode.h"
#include "ym/StopWatch.h"
#include "ym/SatStats.h"


#if defined(YM_DEBUG)
#define DEBUG_FLAG 1
#endif

#if !defined(DEBUG_FLAG)
#define DEBUG_FLAG 0
#endif

// 毎回，個別の SatSolver を用いて検証結果をチェックする
// #define VERIFY_SATSOLVER


BEGIN_NAMESPACE_YM_CEC

BEGIN_NONAMESPACE

const ymuint debug = DEBUG_FLAG;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// CecHandle
//////////////////////////////////////////////////////////////////////

// @brief ノードの通し番号を得る．
SatVarId
CecHandle::varid() const
{
  CecNode* n = node();
  if ( n ) {
    return n->varid();
  }
  else {
    return kSatVarIdIllegal;
  }
}

// @brief 代表ハンドルを得る．
CecHandle
CecHandle::rep_handle() const
{
  CecNode* n = node();
  if ( n ) {
    CecHandle ans = n->rep_handle();
    if ( inv() ) {
      return ~ans;
    }
    else {
      return ans;
    }
  }
  else {
    return *this;
  }
}

// @brief 外部入力ノードへのハンドルのとき true を返す．
bool
CecHandle::is_input() const
{
  CecNode* n = node();
  if ( n && n->is_input() ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief 外部入力ノードへのハンドルのとき，入力番号を返す．
// @note is_input() の時のみ意味を持つ．
ymuint
CecHandle::input_id() const
{
  CecNode* n = node();
  if ( n && n->is_input() ) {
    return n->input_id();
  }
  return 0;
}

// @relates CecHandle
// @brief 内容を出力する関数
ostream&
operator<<(ostream& s,
	   CecHandle src)
{
  if ( src.is_zero() ) {
    s << "ZERO";
  }
  else if ( src.is_one() ) {
    s << "ONE";
  }
  else {
    if ( src.inv() ) {
      s << "~";
    }
    if ( src.is_input() ) {
      s << "I" << src.input_id();
    }
    else {
      s << "A" << src.varid();
    }
  }
  return s;
}



//////////////////////////////////////////////////////////////////////
// CecHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CecHash::CecHash() :
  mNum(0),
  mHashSize(0),
  mHashTable(nullptr)
{
  alloc_table(1024);
}

// @brief デストラクタ
CecHash::~CecHash()
{
  delete [] mHashTable;
}

// @brief ハッシュ表を確保する．
void
CecHash::alloc_table(ymuint req_size)
{
  delete [] mHashTable;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }

  mHashTable = new CecNode*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = nullptr;
  }

  mNum = 0;

  mNextLimit = static_cast<ymuint32>(mHashSize * 1.8);
}


//////////////////////////////////////////////////////////////////////
// CecMgr の実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SAT-solver の種類を表す文字列
// @param[in] sat_opt SAT-solver に渡すオプション文字列
// @param[in] sat_log ログの出力用ストリーム
CecMgr::CecMgr(const string& sat_type,
	       const string& sat_opt,
	       ostream* sat_log) :
  mAlloc(4096),
  mSolver(sat_type, sat_opt, sat_log),
  mSimCount(0),
  mSimTime(0.0),
  mOutP(sat_log),
  mLogLevel(0),
  mLogStream(new ofstream("/dev/null"))
{
}

// @brief デストラクタ
CecMgr::~CecMgr()
{
  if ( mLogStream != &cout ) {
    delete mLogStream;
  }
}

// @brief 定数0関数をつくる．
CecHandle
CecMgr::make_zero()
{
  return CecHandle(nullptr, false);
}

// @brief 定数1関数をつくる．
CecHandle
CecMgr::make_one()
{
  return CecHandle(nullptr, true);
}

// @brief 外部入力を作る．
CecHandle
CecMgr::make_input()
{
  CecNode* node = new_node();
  ymuint iid = mInputNodes.size();
  node->set_input(iid);
  mInputNodes.push_back(node);
  return CecHandle(node, false);
}

// @brief 2つのノードの AND を取る．
// @param[in] handle1, handle2 入力の FRAIG ハンドル
CecHandle
CecMgr::make_and(CecHandle handle1,
		 CecHandle handle2)
{
  // trivial な場合の処理
  if ( handle1.is_zero() || handle2.is_zero() ) {
    return make_zero();
  }
  if ( handle1.is_one() ) {
    return handle2;
  }
  if ( handle2.is_one() ) {
    return handle1;
  }
  if ( handle1 == handle2 ) {
    return handle1;
  }
  if ( handle1.node() == handle2.node() ) {
    // handle1.inv != handle2.inv() のはず
    return make_zero();
  }

  // 順番の正規化
  if ( handle1.varid().val() < handle2.varid().val() ) {
    CecHandle tmp = handle1;
    handle1 = handle2;
    handle2 = tmp;
  }

  // 同じ構造を持つノードが既にないか調べる．
  ymuint pos1 = hash_func(handle1, handle2);
  for (CecNode* node1 = mHashTable1.elem(pos1);
       node1; node1 = node1->mLink1) {
    if ( node1->fanin0_handle() == handle1 &&
	 node1->fanin1_handle() == handle2 ) {
      // 同じノードがあった．
      return node1->rep_handle();
    }
  }

  if ( mHashTable1.need_expand() ) {
    // 再ハッシュする．
    ymuint n = node_num();
    for (ymuint i = 0; i < n; ++ i) {
      CecNode* node1 = mAllNodes[i];
      ymuint pos1 = hash_func(node1->fanin0_handle(), node1->fanin1_handle());
      mHashTable1.add_elem(pos1, node1, node1->mLink1);
    }
  }

  // ノードを作る．
  CecNode* node = new_node();
  node->set_fanin(handle1, handle2);

  // 構造ハッシュ表に登録する．
  mHashTable1.add_elem(pos1, node, node->mLink1);

  // 入出力の関係を表す CNF を作る．
  {
    SatVarId id = node->varid();
    SatVarId id0 = handle1.varid();
    SatVarId id1 = handle2.varid();
    bool inv0 = handle1.inv();
    bool inv1 = handle2.inv();
    SatLiteral lito(id, false);
    SatLiteral lit1(id0, inv0);
    SatLiteral lit2(id1, inv1);
    mSolver.add_clause(~lit1, ~lit2, lito);
    mSolver.add_clause( lit1, ~lito);
    mSolver.add_clause( lit2, ~lito);
  }

  return CecHandle(node, false);
}

// @brief 2つのハンドルが等価かどうか調べる．
SatBool3
CecMgr::check_equiv(CecHandle aig1,
		    CecHandle aig2)
{
  if ( aig1 == aig2 ) {
    // もっとも簡単なパタン
    return kB3True;
  }

  CecNode* node1 = aig1.node();
  CecNode* node2 = aig2.node();

  if ( node1 == node2 ) {
    // ということは逆極性なので絶対に等価ではない．
    return kB3False;
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
CecMgr::set_loglevel(int level)
{
  mLogLevel = level;
}

// @brief ログ出力用ストリームを設定する．
void
CecMgr::set_logstream(ostream* out)
{
  mLogStream = out;
}


// @brief 新しいノードを生成する．
CecNode*
CecMgr::new_node()
{
  void* p = mAlloc.get_memory(sizeof(CecNode));
  CecNode* node = new (p) CecNode();
  node->mVarId = mSolver.new_var();
  ASSERT_COND(node->mVarId.val() == mAllNodes.size() );
  mAllNodes.push_back(node);
  return node;
}

// node が定数かどうか調べる．
SatBool3
CecMgr::check_const(CecNode* node,
		    bool inv)
{
  SatVarId id = node->varid();

  if ( debug ) {
    cout << "CHECK CONST";
    if ( inv ) {
      cout << "1";
    }
    else {
      cout << "0";
    }
    cout << " " << setw(6) << id
	 << "       ";
    cout.flush();
  }

  StopWatch sw;
  sw.start();

  SatLiteral lit(id, inv);

  // この関数の戻り値
  SatBool3 code = kB3X;

  // lit = 1 が成り立つか調べる
  SatBool3 stat = check_condition(lit);
  if ( stat == kB3False ) {
    // 成り立たないということは lit = 0
    mSolver.add_clause(~lit);
    if ( debug ) {
      cout << "\tSUCCEED" << endl;
    }
    code = kB3True;
  }
  else if ( stat == kB3True ) {
    // 成り立ったということは ~lit が常に成り立つわけではない．
    if ( debug ) {
      cout << "\tFAILED" << endl;
    }
    code = kB3False;
  }
  else {
    // 分からない．
    if ( debug ) {
      cout << "\tABORTED" << endl;
    }
  }
  mCheckConstInfo.set_result(code, sw.time().usr_time());
  return code;
}

// node1 と node2 機能的に等価かどうか調べる．
SatBool3
CecMgr::check_equiv(CecNode* node1,
		    CecNode* node2,
		    bool inv)
{
  SatVarId id1 = node1->varid();
  SatVarId id2 = node2->varid();

  if ( debug ) {
    cout << "CHECK EQUIV  "
	 << setw(6) << id1 << " "
	 << setw(6) << id2;
    if ( inv ) {
      cout << " N";
    }
    cout.flush();
  }

  StopWatch sw;
  sw.start();

  SatLiteral lit1(id1, false);
  SatLiteral lit2(id2, inv);

  // この関数の戻り値
  SatBool3 code = kB3X;

  // 等価でない条件
  // - lit1 = 0 かつ lit2 = 1 が成り立つ
  // - lit0 = 1 かつ lit2 = 0 が成り立つ
  SatBool3 stat = check_condition(~lit1,  lit2);
  if ( stat == kB3False ) {
    stat = check_condition( lit1, ~lit2);
    if ( stat == kB3False ) {
      // どの条件も成り立たなかったので等しい
      mSolver.add_clause(~lit1,  lit2);
      mSolver.add_clause( lit1, ~lit2);

      if ( debug ) {
	cout << "\tSUCCEED" << endl;
      }
      code = kB3True;
      goto end;
    }
  }
  if ( stat == kB3True ) {
    if ( debug ) {
      cout << "\tFAILED" << endl;
    }
    code = kB3False;
  }
  else {
    if ( debug ) {
      cout << "\tABORTED" << endl;
    }
  }

 end:
  mCheckEquivInfo.set_result(code, sw.time().usr_time());
  return code;
}

// lit1 が成り立つか調べる．
SatBool3
CecMgr::check_condition(SatLiteral lit1)
{
  vector<SatLiteral> assumptions(1);
  assumptions[0] = lit1;
  SatBool3 ans1 = mSolver.solve(assumptions, mModel);

#if defined(VERIFY_SATSOLVER)
  SatSolver solver(nullptr, "minisat");
  for (vector<CecNode*>::iterator p = mAllNodes.begin();
       p != mAllNodes.end(); ++ p) {
    CecNode* node = *p;
    SatVarId id = solver.new_var();
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
  SatBool3 ans2 = solver.solve(assumptions, mModel);
  if ( ans1 != ans2 ) {
    cout << endl << "ERROR!" << endl;
    cout << "check_condition(" << lit1 << ")" << endl;
    cout << " ans1 = " << ans1 << endl;
    cout << " ans2 = " << ans2 << endl;
    cout << " clauses" << endl;
    for (vector<CecNode*>::iterator p = mAllNodes.begin();
	 p != mAllNodes.end(); ++ p) {
      CecNode* node = *p;
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
CecMgr::check_condition(SatLiteral lit1,
			SatLiteral lit2)
{
  vector<SatLiteral> assumptions(2);
  assumptions[0] = lit1;
  assumptions[1] = lit2;
  SatBool3 ans1 = mSolver.solve(assumptions, mModel);

#if defined(VERIFY_SATSOLVER)
  SatSolver solver(nullptr, "minisat");
  for (vector<CecNode*>::iterator p = mAllNodes.begin();
       p != mAllNodes.end(); ++ p) {
    CecNode* node = *p;
    SatVarId id = solver.new_var();
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
  SatBool3 ans2 = solver.solve(assumptions, mModel);
  if ( ans1 != ans2 ) {
    cout << endl << "ERROR!" << endl;
    cout << "check_condition("
	 << lit1 << " & " << lit2 << ")" << endl;
    cout << " ans1 = " << ans1 << endl;
    cout << " ans2 = " << ans2 << endl;
    cout << " clauses" << endl;
    for (vector<CecNode*>::iterator p = mAllNodes.begin();
	 p != mAllNodes.end(); ++ p) {
      CecNode* node = *p;
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

// @brief CecHandle に対応するリテラルを返す．
// @note 定数の場合の返り値は未定
SatLiteral
CecMgr::fraig2literal(CecHandle aig)
{
  if ( aig.is_const() ) {
    return SatLiteral(SatVarId(0), false);
  }
  SatVarId id = aig.node()->varid();
  bool inv = aig.inv();
  return SatLiteral(id, inv);
}

#if 0
// @brief 等価候補グループの情報を出力する．
void
CecMgr::dump_eqgroup(ostream& s) const
{
  for (ymuint i = 0; i < mNodeNum; ++ i) {
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
    for (SweepNode* snode1 = snode; snode1; snode1 = snode1->next_eqnode()) {
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
CecMgr::dump_stats(ostream& s) const
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

CecMgr::SatStat::SatStat()
{
  mTotalCount = 0;
  for (ymuint i = 0; i < 3; ++ i) {
    mTimeStat[i].mCount = 0;
    mTimeStat[i].mTotalTime = 0.0;
    mTimeStat[i].mMaxTime = 0.0;
  }
}

void
CecMgr::SatStat::set_result(SatBool3 code,
			    double t)
{
  ++ mTotalCount;

  ymuint idx = 0;
  if ( code == kB3True ) {
    idx = 1;
  }
  else if ( code == kB3False ) {
    idx = 2;
  }

  ++ mTimeStat[idx].mCount;
  mTimeStat[idx].mTotalTime += t;
  if ( mTimeStat[idx].mMaxTime < t ) {
    mTimeStat[idx].mMaxTime = t;
  }
}

void
CecMgr::SatStat::dump(ostream& s) const
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

END_NAMESPACE_YM_CEC

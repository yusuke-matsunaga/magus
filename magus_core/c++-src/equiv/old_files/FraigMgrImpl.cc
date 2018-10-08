﻿
/// @file libym_cec/FraigMgrImpl.cc
/// @brief FraigMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FraigMgrImpl.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "FraigMgrImpl.h"
#include "FraigNode.h"
#include "FraigMgr.h"
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

const int debug = DEBUG_FLAG;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// FraigHandle
//////////////////////////////////////////////////////////////////////

// @brief ノードの通し番号を得る．
SatVarId
FraigHandle::varid() const
{
  FraigNode* n = node();
  if ( n ) {
    return n->varid();
  }
  else {
    return kSatVarIdIllegal;
  }
}

// @brief 代表ハンドルを得る．
FraigHandle
FraigHandle::rep_handle() const
{
  FraigNode* n = node();
  if ( n ) {
    FraigHandle ans = n->rep_handle();
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
FraigHandle::is_input() const
{
  FraigNode* n = node();
  if ( n && n->is_input() ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief 外部入力ノードへのハンドルのとき，入力番号を返す．
// @note is_input() の時のみ意味を持つ．
int
FraigHandle::input_id() const
{
  FraigNode* n = node();
  if ( n && n->is_input() ) {
    return n->input_id();
  }
  return 0;
}

// @relates FraigHandle
// @brief 内容を出力する関数
ostream&
operator<<(ostream& s,
	   FraigHandle src)
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
// FraigHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FraigHash::FraigHash() :
  mNum(0),
  mHashSize(0),
  mTable(nullptr)
{
  alloc_table(1024);
}

// @brief デストラクタ
FraigHash::~FraigHash()
{
  delete [] mTable;
}

// @brief ハッシュ表を確保する．
void
FraigHash::alloc_table(int req_size)
{
  delete [] mTable;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }

  mTable = new FraigNode*[mHashSize];
  for ( int i = 0; i < mHashSize; ++ i ) {
    mTable[i] = nullptr;
  }

  mNum = 0;

  mNextLimit = static_cast<ymuint32>(mHashSize * 1.8);
}


//////////////////////////////////////////////////////////////////////
// FraigMgr の実装クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pat_size 初期パタンのサイズ
// @param[in] sat_type SAT-solver の種類を表す文字列
// @param[in] sat_opt SAT-solver に渡すオプション文字列
// @param[in] sat_log ログの出力用ストリーム
FraigMgrImpl::FraigMgrImpl(int pat_size,
			   const SatSolverType& solver_type) :
  mAlloc(4096),
  mPatSize(pat_size * 2),
  mPatInit(pat_size),
  mPatUsed(pat_size),
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
  if ( mLogStream != &cout ) {
    delete mLogStream;
  }
}

// @brief 定数0関数をつくる．
FraigHandle
FraigMgrImpl::make_zero()
{
  return FraigHandle(nullptr, false);
}

// @brief 定数1関数をつくる．
FraigHandle
FraigMgrImpl::make_one()
{
  return FraigHandle(nullptr, true);
}

// @brief 外部入力を作る．
FraigHandle
FraigMgrImpl::make_input()
{
  FraigNode* node = new_node();
  int iid = mInputNodes.size();
  node->set_input(iid);
  mInputNodes.push_back(node);
  vector<ymuint32> tmp(mPatUsed);
  for ( int i = 0; i < mPatUsed; ++ i ) {
    tmp[i] = mRandGen.int32();
  }
  node->set_pat(0, mPatUsed, tmp);
  return FraigHandle(node, false);
}

// @brief 2つのノードの AND を取る．
// @param[in] handle1, handle2 入力の FRAIG ハンドル
FraigHandle
FraigMgrImpl::make_and(FraigHandle handle1,
		       FraigHandle handle2)
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
    FraigHandle tmp = handle1;
    handle1 = handle2;
    handle2 = tmp;
  }

  // 同じ構造を持つノードが既にないか調べる．
  SizeType pos1 = hash_func(handle1, handle2);
  for (FraigNode* node1 = mHashTable1.elem(pos1);
       node1; node1 = node1->mLink1) {
    if ( node1->fanin0_handle() == handle1 &&
	 node1->fanin1_handle() == handle2 ) {
      // 同じノードがあった．
      return node1->rep_handle();
    }
  }

  if ( mHashTable1.need_expand() ) {
    // 再ハッシュする．
    int n = node_num();
    for ( int i = 0; i < n; ++ i ) {
      FraigNode* node1 = mAllNodes[i];
      SizeType pos1 = hash_func(node1->fanin0_handle(), node1->fanin1_handle());
      mHashTable1.add_elem(pos1, node1, node1->mLink1);
    }
  }

  // ノードを作る．
  FraigNode* node = new_node();
  node->set_fanin(handle1, handle2);
  node->calc_pat(0, mPatUsed);

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

  // パタンハッシュで等しいノードがないか調べる．
  bool inv0 = node->pat_hash_inv();
  for ( ; ; ) {
    bool rehash = false;
    FraigNode* node2;
    bool inv2;
    if ( !node->check_1mark() ) {
      // 定数0の可能性があるか調べる．
      SatBool3 stat = check_const(node, false);
      if ( stat == SatBool3::True ) {
	node->set_rep(nullptr, false);
	return make_zero();
      }
      if ( stat == SatBool3::False ) {
	add_pat(node);
	ASSERT_COND(node->check_1mark() );
	continue;
      }
      break;
    }
    else if ( !node->check_0mark() ) {
      // 定数1の可能性があるか調べる．
      SatBool3 stat = check_const(node, true);
      if ( stat == SatBool3::True ) {
	node->set_rep(nullptr, true);
	return make_one();
      }
      if ( stat == SatBool3::False ) {
	add_pat(node);
	ASSERT_COND(node->check_0mark() );
	continue;
      }
      break;
    }
    SizeType pos2 = node->pat_hash();
    for (FraigNode* node1 = mHashTable2.elem(pos2);
	 node1; node1 = node1->mLink2) {
      bool inv = node1->pat_hash_inv() ^ inv0;
      if ( compare_pat(node1, node, inv) ) {
	// node1 と node が等価かどうか調べる．
	SatBool3 stat = check_equiv(node1, node, inv);
	if ( stat == SatBool3::True ) {
	  node->set_rep(node1, inv);
	  return FraigHandle(node1, inv);
	}
	if ( stat == SatBool3::False ) {
	  // 反例をパタンに加えて再ハッシュする．
	  rehash = true;
	  node2 = node1;
	  inv2 = inv;
	  break;
	}
      }
    }

    if ( rehash ) {
      add_pat(node);
      ASSERT_COND(!compare_pat(node2, node, inv2) );
    }
    else {
      break;
    }
  }

  // パタンハッシュ表に登録する．
  if ( mHashTable2.need_expand() ) {
    // 再ハッシュする．
    int n = node_num() - 1; // ！-1 するのは node を除外するため！
    for ( int i = 0; i < n; ++ i ) {
      FraigNode* node1 = mAllNodes[i];
      SizeType pos2 = node1->pat_hash();
      mHashTable2.add_elem(pos2, node1, node1->mLink2);
    }
  }
  SizeType pos2 = node->pat_hash();
  mHashTable2.add_elem(pos2, node, node->mLink2);

  return FraigHandle(node, false);
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
  vector<ymuint32> tmp(1);
  int nn = node_num();
  for ( int i = 0; i < nn; ++ i ) {
    FraigNode* node1 = mAllNodes[i];
    if ( node1->is_input() ) {
      ymuint32 pat = 0U;
      if ( mModel[node1->varid().val()] == SatBool3::True ) {
	pat = ~0U;
      }
      else {
	pat = 0U;
      }
      for ( int b = 1; b < 32; ++ b ) {
	if ( (mRandGen.int32() % 100) <= 3 ) {
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
      SizeType pos2 = node1->pat_hash();
      mHashTable2.add_elem(pos2, node1, node1->mLink2);
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
  node->mPat = new ymuint32[mPatSize];
}

// @brief 全ノードのシミュレーションパタン用配列を拡大する．
void
FraigMgrImpl::resize_pat(int size)
{
  int n = mAllNodes.size();
  for ( int i = 0; i < n; ++ i ) {
    FraigNode* node = mAllNodes[i];
    ymuint32* old_array = node->mPat;
    node->mPat = new ymuint32[size];
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
  ymuint32* src1 = node1->mPat;
  ymuint32* src2 = node2->mPat;
  ymuint32* end = src1 + mPatUsed;
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
  void* p = mAlloc.get_memory(sizeof(FraigNode));
  FraigNode* node = new (p) FraigNode();
  node->mVarId = mSolver.new_variable();
  ASSERT_COND(node->mVarId.val() == mAllNodes.size() );
  init_pat(node);
  mAllNodes.push_back(node);
  return node;
}

// node が定数かどうか調べる．
SatBool3
FraigMgrImpl::check_const(FraigNode* node,
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
  mCheckConstInfo.set_result(code, sw.time().usr_time());
  return code;
}

// node1 と node2 機能的に等価かどうか調べる．
SatBool3
FraigMgrImpl::check_equiv(FraigNode* node1,
			  FraigNode* node2,
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
  mCheckEquivInfo.set_result(code, sw.time().usr_time());
  return code;
}

// lit1 が成り立つか調べる．
SatBool3
FraigMgrImpl::check_condition(SatLiteral lit1)
{
  vector<SatLiteral> assumptions(1);
  assumptions[0] = lit1;
  SatBool3 ans1 = mSolver.solve(assumptions, mModel);

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
  SatBool3 ans2 = solver.solve(assumptions, mModel);
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
  vector<SatLiteral> assumptions(2);
  assumptions[0] = lit1;
  assumptions[1] = lit2;
  SatBool3 ans1 = mSolver.solve(assumptions, mModel);

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
  SatBool3 ans2 = solver.solve(assumptions, mModel);
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
FraigMgrImpl::dump_stats(ostream& s) const
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

END_NAMESPACE_YM_CEC
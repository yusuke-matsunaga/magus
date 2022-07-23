
/// @file FraigMgrImpl.cc
/// @brief FraigMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigMgrImpl.h"
#include "FraigNode.h"
#include "ym/Range.h"
#include "ym/Timer.h"


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
) : mSolver{solver_type},
    mLogLevel{0},
    mLogStream{new ofstream("/dev/null")}
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
  FraigHandle ans{node, false};

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
      std::swap(handle1, handle2);
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
      mSolver.make_cnf(node);

      if ( debug ) {
	cout << "  new node: " << FraigHandle{node, false} << endl;
      }

      PatEq eq;

      // 縮退検査を行う．
      if ( verify_const(node, ans) != SatBool3::True ) {
	while ( true ) {
	  auto range = mPatTable.equal_range(node);
	  bool inv0 = node->pat_hash_inv();
	  bool change{false};
	  auto end = range.second;
	  for ( auto p = range.first; p != end; ++ p ) {
	    auto node1 = *p;
	    bool inv = inv0 ^ node1->pat_hash_inv();
	    bool retry = false;
	    if ( eq(node, node1) ) {
	      // node と node1 が等価かどうか調べる．
	      auto stat = mSolver.check_equiv(node, node1, inv);
	      if ( stat == SatBool3::True ) {
		// 等価なノードが見つかった．
		node->set_rep(node1, inv);
		ans = FraigHandle{node1, inv};
		goto exit;
	      }
	      else if ( stat == SatBool3::False ) {
		// 反例をパタンに加えて再ハッシュする．
		add_pat(node);

		ASSERT_COND( !eq(node, node1) );
		retry = true;
		change = true;
		break;
	      }
	    }
	  }
	  if ( !change ) {
	    ans = FraigHandle{node, false};
	    break;
	  }
	}
      }
    }
  }

 exit:
  if ( debug ) {
    cout << "  -> " << ans << endl;
  }

  return ans;
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
    SatBool3 stat = mSolver.check_const(node2, inv2);
    return stat;
  }

  if ( aig1.is_one() ) {
    // 上のチェックで aig2 は定数でないことは明らか
    SatBool3 stat = mSolver.check_const(node2, !inv2);
    return stat;
  }

  if ( aig2.is_zero() ) {
    // 上のチェックで aig1 は定数でないことは明らか
    SatBool3 stat = mSolver.check_const(node1, inv1);
    return stat;
  }

  if ( aig2.is_one() ) {
    // 上のチェックで aig1 は定数でないことは明らか
    SatBool3 stat = mSolver.check_const(node1, !inv1);
    return stat;
  }

  bool inv = inv1 ^ inv2;
  SatBool3 stat = mSolver.check_equiv(node1, node2, inv);
  return stat;
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
    stat = mSolver.check_const(node, false);
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
    stat = mSolver.check_const(node, true);
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
  std::uniform_int_distribution<int> rd100(0, 99);
  for ( auto node1: mAllNodes ) {
    if ( node1->is_input() ) {
      ymuint64 pat = 0U;
      if ( model[node1->literal()] == SatBool3::True ) {
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
  node->mLiteral = mSolver.new_var();
  ASSERT_COND(node->literal().varid() == mAllNodes.size() );
  mAllNodes.push_back(node);
}

// @brief 内部の統計情報を出力する．
void
FraigMgrImpl::dump_stats(
  ostream& s
)
{
  mSolver.dump_stats(s);
}

END_NAMESPACE_FRAIG

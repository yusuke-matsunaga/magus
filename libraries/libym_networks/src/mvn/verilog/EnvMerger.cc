
/// @file EnvMerger.cc
/// @brief EnvMerger の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EnvMerger.h"
#include "YmNetworks/MvnMgr.h"
#include "YmNetworks/MvnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EnvMerger
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr MvnMgr
EnvMerger::EnvMerger(MvnMgr* mgr) :
  mMvnMgr(mgr)
{
}

// @brief デストラクタ
EnvMerger::~EnvMerger()
{
}

// @brief 代入条件をマージする．
// @param[in] parent_module 親のモジュール
// @param[in] cond 切り替え条件
// @param[in] then_cond cond が成り立ったときの代入条件
// @param[in] else_cond cond が成り立たなかったときの代入条件
// @note 基本的には ITE(cond, then_cond, else_cond) だが，NULL の場合がある．
MvnNode*
EnvMerger::merge_cond(MvnModule* parent_module,
		      MvnNode* cond,
		      MvnNode* then_cond,
		      MvnNode* else_cond)
{
  MvnNode* new_cond = NULL;
  if ( then_cond == else_cond ) {
    // 両方の条件が等しければ ITE を挿入しない
    new_cond = then_cond;
  }
  else if ( then_cond == NULL ) {
    // 代入条件は cond | else_cond
    new_cond = mMvnMgr->new_or(parent_module, 2);
    mMvnMgr->connect(cond, 0, new_cond, 0);
    mMvnMgr->connect(else_cond, 0, new_cond, 1);
  }
  else if ( else_cond == NULL ) {
    // 代入条件は ~cond | then_cond
    MvnNode* cond_bar = mMvnMgr->new_not(parent_module, 1);
    mMvnMgr->connect(cond, 0, cond_bar, 0);
    new_cond = mMvnMgr->new_or(parent_module, 2);
    mMvnMgr->connect(cond_bar, 0, new_cond, 0);
    mMvnMgr->connect(then_cond, 0, new_cond, 1);
  }
  else {
    new_cond = mMvnMgr->new_ite(parent_module, 1);
    mMvnMgr->connect(cond, 0, new_cond, 0);
    mMvnMgr->connect(then_cond, 0, new_cond, 1);
    mMvnMgr->connect(else_cond, 0, new_cond, 2);
  }
  return new_cond;
}


//////////////////////////////////////////////////////////////////////
// クラス EnvMerger1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr MvnMgr
EnvMerger1::EnvMerger1(MvnMgr* mgr) :
  EnvMerger(mgr)
{
}

// @brief デストラクタ
EnvMerger1::~EnvMerger1()
{
}

// @brief Env をマージする．
// @param[in] parent_module 親のモジュール
// @param[in] env 対象の環境
// @param[in] cond 条件を表すノード
// @param[in] then_env 条件が成り立ったときに通るパスの環境
// @param[in] else_env 条件が成り立たなかったときに通るパスの環境
void
EnvMerger1::operator()(MvnModule* parent_module,
		       ProcEnv& env,
		       MvnNode* cond,
		       const ProcEnv& then_env,
		       const ProcEnv& else_env)
{
  ymuint n = env.max_id();
  for (ymuint i = 0; i < n; ++ i) {
    AssignInfo info1 = then_env.get_from_id(i);
    MvnNode* node1 = info1.mRhs;
    MvnNode* cond1 = info1.mCond;

    AssignInfo info2 = else_env.get_from_id(i);
    MvnNode* node2 = info2.mRhs;
    MvnNode* cond2 = info2.mCond;

    MvnNode* new_node = NULL;
    MvnNode* new_cond = NULL;
    bool new_block = false;
    if ( node1 == node2 ) {
      if ( node1 == NULL ) {
	continue;
      }
      // 両方の結果が等しければ ITE を挿入しない．
      new_node = node1;
      if ( info1.mBlock != info2.mBlock ) {
	// blocking 代入と non-blocking 代入の混在はエラー
#warning "TODO: エラー処理"
	continue;
      }
      new_block = info1.mBlock;
      // 条件は ITE になる．
      new_cond = merge_cond(parent_module, cond, cond1, cond2);
    }
    else if ( node1 == NULL ) {
      // node1 が NULL
      // cond1 も NULL
      assert_cond( node2 != NULL, __FILE__, __LINE__);
      new_node = node2;
      new_block = info2.mBlock;

      MvnNode* cond_bar = mgr()->new_not(parent_module, 1);
      mgr()->connect(cond, 0, cond_bar, 0);
      if ( cond2 == NULL ) {
	// 代入条件は ~cond
	new_cond = cond_bar;
      }
      else {
	// 代入条件は ~cond & cond2
	new_cond = mgr()->new_and(parent_module, 1);
	mgr()->connect(cond_bar, 0, new_cond, 0);
	mgr()->connect(cond2, 0, new_cond, 1);
      }
    }
    else if ( node2 == NULL ) {
      // node2 が NULL
      // cond2 も NULL
      assert_cond( node1 != NULL, __FILE__, __LINE__);
      new_node = node1;
      new_block = info1.mBlock;

      if ( cond1 == NULL ) {
	// 代入条件は cond
	new_cond = cond;
      }
      else {
	// 代入条件は cond & cond1
	new_cond = mgr()->new_and(parent_module, 1);
	mgr()->connect(cond, 0, new_cond, 0);
	mgr()->connect(cond1, 0, new_cond, 1);
      }
    }
    else {
      // node1 も node2 も NULL ではない．
      ymuint bw = node1->bit_width();
      new_node = mgr()->new_ite(parent_module, bw);
      mgr()->connect(cond, 0, new_node, 0);
      mgr()->connect(node1, 0, new_node, 1);
      mgr()->connect(node2, 0, new_node, 2);
      if ( info1.mBlock != info2.mBlock ) {
	// blocking 代入と non-blocking 代入の混在はエラー
#warning "TODO: エラー処理"
	continue;
      }
      new_block = info1.mBlock;
      new_cond = merge_cond(parent_module, cond, cond1, cond2);
    }
    env.add_by_id(i, new_node, new_cond, new_block);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EnvMerger2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr MvnMgr
// @param[in] global_env グローバル環境
EnvMerger2::EnvMerger2(MvnMgr* mgr,
		       Env& global_env) :
  EnvMerger(mgr),
  mGlobalEnv(global_env)
{
}

// @brief デストラクタ
EnvMerger2::~EnvMerger2()
{
}

// @brief Env をマージする．
// @param[in] parent_module 親のモジュール
// @param[in] env 対象の環境
// @param[in] cond 条件を表すノード
// @param[in] then_env 条件が成り立ったときに通るパスの環境
// @param[in] else_env 条件が成り立たなかったときに通るパスの環境
void
EnvMerger2::operator()(MvnModule* parent_module,
		       ProcEnv& env,
		       MvnNode* cond,
		       const ProcEnv& then_env,
		       const ProcEnv& else_env)
{
  ymuint n = env.max_id();
  for (ymuint i = 0; i < n; ++ i) {
    AssignInfo info0 = env.get_from_id(i);
    MvnNode* node0 = info0.mRhs;
    assert_cond( info0.mCond == NULL, __FILE__, __LINE__);

    AssignInfo info1 = then_env.get_from_id(i);
    MvnNode* node1 = info1.mRhs;
    assert_cond( info1.mCond == NULL, __FILE__, __LINE__);

    AssignInfo info2 = else_env.get_from_id(i);
    MvnNode* node2 = info2.mRhs;
    assert_cond( info2.mCond == NULL, __FILE__, __LINE__);

    MvnNode* new_node = NULL;
    bool new_block = false;
    if ( node1 == node2 ) {
      if ( node1 == NULL ) {
	continue;
      }
      // 両方の結果が等しければ ITE を挿入しない．
      new_node = node1;
      if ( info1.mBlock != info2.mBlock ) {
	// blocking 代入と non-blocking 代入の混在はエラー
#warning "TODO: エラー処理"
	continue;
      }
      new_block = info1.mBlock;
    }
    else if ( node1 == NULL ) {
      // node1 が NULL
      assert_cond( node2 != NULL, __FILE__, __LINE__);
      if ( node0 == NULL ) {
	node0 = mGlobalEnv.get_from_id(i);
      }
      ymuint bw = node0->bit_width();
      if ( node2->bit_width() != bw ) {
	// ビット幅が異なる．
#warning "TODO: エラー処理"
      }
      new_node = mgr()->new_ite(parent_module, bw);
      mgr()->connect(cond, 0, new_node, 0);
      mgr()->connect(node0, 0, new_node, 1);
      mgr()->connect(node2, 0, new_node, 2);
      new_block = info2.mBlock;
    }
    else if ( node2 == NULL ) {
      // node2 が NULL
      assert_cond( node1 != NULL, __FILE__, __LINE__);
      if ( node0 == NULL ) {
	node0 = mGlobalEnv.get_from_id(i);
      }
      ymuint bw = node0->bit_width();
      if ( node1->bit_width() != bw ) {
	// ビット幅が異なる．
#warning "TODO: エラー処理"
      }
      new_node = mgr()->new_ite(parent_module, bw);
      mgr()->connect(cond, 0, new_node, 0);
      mgr()->connect(node1, 0, new_node, 1);
      mgr()->connect(node0, 0, new_node, 2);
      new_block = info1.mBlock;
    }
    else {
      // node1 も node2 も NULL ではない．
      //assert_cond( node0 != NULL, __FILE__, __LINE__);
      ymuint bw = node1->bit_width();
      if ( node1->bit_width() != bw ) {
	// ビット幅が異なる．
#warning "TODO: エラー処理"
      }
      if ( node2->bit_width() != bw ) {
	// ビット幅が異なる．
#warning "TODO: エラー処理"
      }
      new_node = mgr()->new_ite(parent_module, bw);
      mgr()->connect(cond, 0, new_node, 0);
      mgr()->connect(node1, 0, new_node, 1);
      mgr()->connect(node2, 0, new_node, 2);
      if ( info1.mBlock != info2.mBlock ) {
	// blocking 代入と non-blocking 代入の混在はエラー
#warning "TODO: エラー処理"
	continue;
      }
      new_block = info1.mBlock;
    }
    env.add_by_id(i, new_node, NULL, new_block);
  }
}

END_NAMESPACE_YM_NETWORKS_MVN_VERILOG

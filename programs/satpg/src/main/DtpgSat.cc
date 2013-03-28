
/// @file atpg/src/dtpg/DtpgSat.cc
/// @brief DtpgSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"


#define VERIFY_MAIMP 0


BEGIN_NAMESPACE_YM_SATPG

Dtpg*
new_DtpgSat(AtpgMgr& mgr)
{
  return new DtpgSat(mgr);
}

// @brief コンストラクタ
// @param[in] mgr AtpgMgr
DtpgSat::DtpgSat(AtpgMgr& mgr)
{
  mSatEngine = new_SatEngine(mgr);

  mNetwork = NULL;
}

// @brief デストラクタ
DtpgSat::~DtpgSat()
{
  delete mSatEngine;
}

// @brief 使用する SAT エンジンを指定する．
void
DtpgSat::set_mode(const string& type,
		  const string& option,
		  ostream* outp)
{
  mSatEngine->set_mode(type, option, outp);
}

// @brief 回路と故障リストを設定する．
// @param[in] tgnetwork 対象のネットワーク
void
DtpgSat::set_network(TpgNetwork& tgnetwork)
{
  mNetwork = &tgnetwork;
  mMaxId = mNetwork->node_num();
}

// @brief テスト生成を行なう．
// @param[in] mode メインモード
// @param[in] po_mode PO分割モード
// @param[in] bt バックトラッカー
void
DtpgSat::run(tDtpgMode mode,
	     tDtpgPoMode po_mode,
	     BackTracer& bt)
{
  switch ( po_mode ) {
  case kDtpgPoNone:
    mNetwork->activate_all();

    switch ( mode ) {
    case kDtpgSingle:
      single_mode(bt);
      break;

    case kDtpgDual:
      dual_mode(bt);
      break;

    case kDtpgNode:
      node_mode(bt);
      break;

    case kDtpgFFR:
      ffr_mode(bt);
      break;

    case kDtpgMFFC:
      mffc_mode(bt);
      break;

    case kDtpgAll:
      all_mode(bt);
      break;
    }
    break;

  case kDtpgPoInc:
    {
      ymuint no = mNetwork->output_num2();
      for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
	mNetwork->activate_po(po_pos);

	switch ( mode ) {
	case kDtpgSingle:
	  single_mode(bt);
	  break;

	case kDtpgDual:
	  dual_mode(bt);
	  break;

	case kDtpgNode:
	  node_mode(bt);
	  break;

	case kDtpgFFR:
	  ffr_mode(bt);
	  break;

	case kDtpgMFFC:
	case kDtpgAll:
	  all_mode(bt);
	  break;
	}
      }
    }
    break;

  case kDtpgPoDec:
    {
      ymuint no = mNetwork->output_num2();
      for (ymuint i = 0; i < no; ++ i) {
	ymuint po_pos = no - i - 1;
	mNetwork->activate_po(po_pos);

	switch ( mode ) {
	case kDtpgSingle:
	  single_mode(bt);
	  break;

	case kDtpgDual:
	  dual_mode(bt);
	  break;

	case kDtpgNode:
	  node_mode(bt);
	  break;

	case kDtpgFFR:
	  ffr_mode(bt);
	  break;

	case kDtpgMFFC:
	case kDtpgAll:
	  all_mode(bt);
	  break;
	}
      }
    }
    break;
  }
}

// @brief single モードでテスト生成を行なう．
void
DtpgSat::single_mode(BackTracer& bt)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = mNetwork->active_node(i);

    // 出力の故障
    TpgFault* f0 = node->output_fault(0);
    dtpg_single(f0, bt);

    TpgFault* f1 = node->output_fault(1);
    dtpg_single(f1, bt);

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* f0 = node->input_fault(0, j);
      dtpg_single(f0, bt);

      TpgFault* f1 = node->input_fault(1, j);
      dtpg_single(f1, bt);
    }
  }
}

// @brief dual モードでテスト生成を行なう．
void
DtpgSat::dual_mode(BackTracer& bt)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    // 出力の故障
    TpgFault* f0 = node->output_fault(0);
    TpgFault* f1 = node->output_fault(1);
    dtpg_dual(f0, f1, bt);

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* f0 = node->input_fault(0, j);
      TpgFault* f1 = node->input_fault(1, j);
      dtpg_dual(f0, f1, bt);
    }
  }
}

// @brief node モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::node_mode(BackTracer& bt)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    mFaultList.clear();

    TpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* f0 = node->input_fault(0, j);
      add_fault(f0);

      TpgFault* f1 = node->input_fault(1, j);
      add_fault(f1);
    }

    // 出力の故障
    TpgFault* f0 = node->output_fault(0);
    add_fault(f0);

    TpgFault* f1 = node->output_fault(1);
    add_fault(f1);

    do_dtpg(bt);
  }
}

// @brief ffr モードでテスト生成を行なう．
void
DtpgSat::ffr_mode(BackTracer& bt)
{
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() || node->active_fanout_num() > 1 ) {
      mFaultList.clear();

      dfs_ffr(node);

      do_dtpg(bt);
    }
  }
}

// @brief mffc モードでテスト生成を行なう．
void
DtpgSat::mffc_mode(BackTracer& bt)
{
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->imm_dom() == NULL ) {
      mFaultList.clear();

      vector<bool> mark(mNetwork->node_num(), false);
      dfs_mffc(node, mark);

      do_dtpg(bt);
    }
  }
}

// @brief all モードでテスト生成を行なう．
void
DtpgSat::all_mode(BackTracer& bt)
{
  mFaultList.clear();

  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( !node->is_output() ) {
      add_node_faults(node);
    }
  }

  do_dtpg(bt);
}

// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] f 故障
void
DtpgSat::dtpg_single(TpgFault* f,
		     BackTracer& bt)
{
  mFaultList.clear();

  add_fault(f);

  do_dtpg(bt);
}

// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
// @param[in] f0 0縮退故障
// @param[in] f1 1縮退故障
void
DtpgSat::dtpg_dual(TpgFault* f0,
		   TpgFault* f1,
		   BackTracer& bt)
{
  mFaultList.clear();

  add_fault(f0);
  add_fault(f1);

  do_dtpg(bt);
}

void
DtpgSat::dfs_ffr(TpgNode* node)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( inode->active_fanout_num() == 1 ) {
      dfs_ffr(inode);
    }
  }

  if ( !node->is_output() ) {
    add_node_faults(node);
  }
}

void
DtpgSat::dfs_mffc(TpgNode* node,
		  vector<bool>& mark)
{
  mark[node->id()] = true;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( mark[inode->id()] == false && inode->imm_dom() != NULL ) {
      dfs_mffc(inode, mark);
    }
  }

  if ( !node->is_output() ) {
    add_node_faults(node);
  }
}

// @brief ノードの故障を追加する．
void
DtpgSat::add_node_faults(TpgNode* node)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgFault* f0 = node->input_fault(0, i);
    add_fault(f0);

    TpgFault* f1 = node->input_fault(1, i);
    add_fault(f1);
  }
  TpgFault* f0 = node->output_fault(0);
  add_fault(f0);

  TpgFault* f1 = node->output_fault(1);
  add_fault(f1);
}

END_NAMESPACE_YM_SATPG

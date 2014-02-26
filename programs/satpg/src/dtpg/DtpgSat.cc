
/// @file DtpgSat.cc
/// @brief DtpgSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "DetectOp.h"
#include "UntestOp.h"
#include "DtpgNgMgr.h"
#include "DtpgNodeGroup.h"
#include "DtpgNgEdge.h"
#include "utils/HeapTree.h"


#define VERIFY_MAIMP 0


BEGIN_NAMESPACE_YM_SATPG

Dtpg*
new_DtpgSat()
{
  return new DtpgSat();
}

// @brief コンストラクタ
DtpgSat::DtpgSat()
{
  mSatEngine = new_SatEngine(*this);

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
// @param[in] bt バックトレーサー
// @param[in] dop_list DetectOp のリスト
// @param[in] uop_list UntestOp のリスト
// @param[in] stats 結果を格納する構造体
void
DtpgSat::run(DtpgMode mode,
	     tDtpgPoMode po_mode,
	     BackTracer& bt,
	     const vector<DetectOp*>& dop_list,
	     const vector<UntestOp*>& uop_list,
	     DtpgStats& stats)
{
  mDetectOpList = dop_list;
  mUntestOpList = uop_list;

  mSatEngine->clear_stats();

  switch ( po_mode ) {
  case kDtpgPoNone:
    // PO 分割を行わないモード

    mNetwork->activate_all();

    dtpg1(mode, bt);

    break;

  case kDtpgPoInc:
    { // 正順で PO を選び分割するモード
      ymuint no = mNetwork->output_num2();
      for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
	mNetwork->activate_po(po_pos);

	dtpg1(mode, bt);
      }
    }
    break;

  case kDtpgPoDec:
    { // 逆順で PO を選び分割するモード
      ymuint no = mNetwork->output_num2();
      for (ymuint i = 0; i < no; ++ i) {
	ymuint po_pos = no - i - 1;
	mNetwork->activate_po(po_pos);

	dtpg1(mode, bt);
      }
    }
    break;
  }

  mSatEngine->get_stats(stats);
}

// @brief テストパタンが見つかった場合に呼ばれる関数
// @param[in] f 故障
// @param[in] tv テストパタン
void
DtpgSat::set_detected(TpgFault* f,
		      TestVector* tv)
{
  for (vector<DetectOp*>::iterator p = mDetectOpList.begin();
       p != mDetectOpList.end(); ++ p) {
    DetectOp& op = **p;
    op(f, tv);
  }
}

// @brief 検出不能のときに呼ばれる関数
// @param[in] f 故障
void
DtpgSat::set_untestable(TpgFault* f)
{
  for (vector<UntestOp*>::iterator p = mUntestOpList.begin();
       p != mUntestOpList.end(); ++ p) {
    UntestOp& op = **p;
    op(f);
  }
}

// @brief activate された部分回路に大してテスト生成を行う．
// @param[in] mode メインモード
// @param[in] bt バックトレーサー
void
DtpgSat::dtpg1(DtpgMode mode,
	       BackTracer& bt)
{
  switch ( mode.mode() ) {
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

  case kDtpgFFR2:
    ffr2_mode(mode.ffr2_limit(), bt);
    break;

  case kDtpgMFFC:
    mffc_mode(bt);
    break;

  case kDtpgAll:
    all_mode(bt);
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
    if ( node->is_output() ) {
      continue;
    }

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
    TpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    add_node_faults(node);

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
    ymuint nfo = node->active_fanout_num();
    if ( nfo > 1 || (nfo == 1 && node->active_fanout(0)->is_output()) ) {
      clear_faults();

      dfs_ffr(node);

      do_dtpg(bt);
    }
  }
}

BEGIN_NONAMESPACE

void
get_ffr(TpgNode* node,
	vector<TpgNode*>& node_list)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( inode->active_fanout_num() == 1 ) {
      get_ffr(inode, node_list);
    }
  }

  node_list.push_back(node);
}

struct NgLt
{
  bool
  operator()(DtpgNgEdge* left,
	     DtpgNgEdge* right)
  {
    return left->size() < right->size();
  }

};

END_NONAMESPACE

// @brief ffr2 モードでテスト生成を行なう．
// @param[in] size_limit サイズの上限
// @param[in] bt バックトレーサー
void
DtpgSat::ffr2_mode(ymuint size_limit,
		   BackTracer& bt)
{
  ymuint n = mNetwork->active_node_num();

  // FFR ごとに DtpgNodeGroup を作る．
  DtpgNgMgr mgr;
  vector<DtpgNodeGroup*> rep_map(n, NULL);
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() ) {
      continue;
    }
    ymuint nfo = node->active_fanout_num();
    if ( nfo > 1 || (nfo == 1 && node->active_fanout(0)->is_output()) ) {
      vector<TpgNode*> node_list;
      get_ffr(node, node_list);

      DtpgNodeGroup* ng = mgr.add_node_group(node_list);

      for (vector<TpgNode*>::iterator p = node_list.begin();
	   p != node_list.end(); ++ p) {
	TpgNode* node1 = *p;
	rep_map[node1->id()] = ng;
      }
    }
  }

  // DtpgNodeGroup 間の枝を作る．
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() ) {
      continue;
    }
    DtpgNodeGroup* dst_ng = rep_map[node->id()];
    ymuint dst_size = dst_ng->size();
    ymuint nfi = node->fanin_num();
    for (ymuint i = 0; i < nfi; ++ i) {
      TpgNode* inode = node->fanin(i);
      DtpgNodeGroup* src_ng = rep_map[inode->id()];
      if ( dst_ng == src_ng ) {
	continue;
      }
      ymuint size = dst_size + src_ng->size();
      if ( size <= size_limit ) {
	mgr.add_edge(src_ng, dst_ng);
      }
    }
  }

  mgr.merge(size_limit);

#if 0
  const vector<DtpgNodeGroup*>& ng_list = mgr.node_list();
  for (vector<DtpgNodeGroup*>::const_iterator p = ng_list.begin();
       p != ng_list.end(); ++ p) {
    DtpgNodeGroup* ng = *p;
    const vector<TpgNode*>& node_list = ng->node_list();
    clear_faults();
    for (vector<TpgNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      TpgNode* node = *p;
      add_node_faults(node);
    }
    do_dtpg(bt);
  }
#endif
}


// @brief mffc モードでテスト生成を行なう．
void
DtpgSat::mffc_mode(BackTracer& bt)
{
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->imm_dom() == NULL ) {
      clear_faults();

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
  clear_faults();

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
  clear_faults();

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
  clear_faults();

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

  add_node_faults(node);
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

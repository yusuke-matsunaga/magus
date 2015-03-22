
/// @file DtpgDriverImpl.cc
/// @brief DtpgDriverImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgDriverImpl.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "DtpgEngine.h"
#include "YmUtils/HeapTree.h"


BEGIN_NAMESPACE_YM_SATPG

DtpgDriver*
new_DtpgDriver()
{
  return new DtpgDriverImpl();
}

// @brief コンストラクタ
DtpgDriverImpl::DtpgDriverImpl()
{
  mNetwork = NULL;
}

// @brief デストラクタ
DtpgDriverImpl::~DtpgDriverImpl()
{
}

// @brief テスト生成を行なう．
// @param[in] tpgnetwork 対象のネットワーク
// @param[in] mode メインモード
// @param[in] po_mode PO分割モード
// @param[in] engine DTPG エンジン
// @param[in] stats 結果を格納する構造体
void
DtpgDriverImpl::run(TpgNetwork& tgnetwork,
		    tDtpgMode mode,
		    tDtpgPoMode po_mode,
		    DtpgEngine& engine,
		    DtpgStats& stats)
{
  engine.clear_stats();

  mNetwork = &tgnetwork;

  switch ( po_mode ) {
  case kDtpgPoNone:
    // PO 分割を行わないモード

    mNetwork->activate_all();

    dtpg1(mode, engine);

    break;

  case kDtpgPoInc:
    { // 正順で PO を選び分割するモード
      ymuint no = mNetwork->output_num2();
      for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
	mNetwork->activate_po(po_pos);

	dtpg1(mode, engine);
      }
    }
    break;

  case kDtpgPoDec:
    { // 逆順で PO を選び分割するモード
      ymuint no = mNetwork->output_num2();
      for (ymuint i = 0; i < no; ++ i) {
	ymuint po_pos = no - i - 1;
	mNetwork->activate_po(po_pos);

	dtpg1(mode, engine);
      }
    }
    break;
  }

  engine.get_stats(stats);
}

// @brief activate された部分回路に大してテスト生成を行う．
// @param[in] mode メインモード
// @param[in] engine DTPG エンジン
void
DtpgDriverImpl::dtpg1(tDtpgMode mode,
		      DtpgEngine& engine)
{
  switch ( mode ) {
  case kDtpgSingle:
    single_mode(engine);
    break;

  case kDtpgFFR:
    ffr_mode(engine);
    break;

  case kDtpgMFFC:
    mffc_mode(engine);
    break;
  }
}

// @brief single モードでテスト生成を行なう．
// @param[in] engine DTPG エンジン
void
DtpgDriverImpl::single_mode(DtpgEngine& engine)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      TpgFault* f = node->fault(i);
      if ( f->status() != kFsDetected &&
	   !f->is_skip() ) {
	engine.run_single(f);
      }
    }
  }
}

// @brief ffr モードでテスト生成を行なう．
// @param[in] engine DTPG エンジン
void
DtpgDriverImpl::ffr_mode(DtpgEngine& engine)
{
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    ymuint nfo = node->active_fanout_num();
    if ( nfo > 1 || (nfo == 1 && node->active_fanout(0)->is_output()) ) {
      clear_faults();

      dfs_ffr(node);

      do_dtpg(engine);
    }
  }
}

// @brief DFS で FFR を求める．
// @param[in] node 対象のノード
void
DtpgDriverImpl::dfs_ffr(TpgNode* node)
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

// @brief mffc モードでテスト生成を行なう．
// @param[in] engine DTPG エンジン
void
DtpgDriverImpl::mffc_mode(DtpgEngine& engine)
{
  ymuint n = mNetwork->active_node_num();
  vector<bool> mark(mNetwork->max_node_id(), false);
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->imm_dom() == NULL ) {
      clear_faults();

      dfs_mffc(node, mark);

      do_dtpg(engine);
    }
  }
}

// @brief DFS で MFFC を求める．
// @param[in] node 対象のノード
// @param[inout] mark 処理済みのマーク
void
DtpgDriverImpl::dfs_mffc(TpgNode* node,
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

// @brief 故障リストをクリアする．
void
DtpgDriverImpl::clear_faults()
{
  mFaultList.clear();
}

// @brief ノードの故障を追加する．
// @param[in] node 対象のノード
void
DtpgDriverImpl::add_node_faults(TpgNode* node)
{
  ymuint nf = node->fault_num();
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* f = node->fault(i);
    if ( f->status() != kFsDetected &&
	 !f->is_skip() ) {
      mFaultList.push_back(f);
    }
  }
}

// @brief テストパタン生成を行なう．
// @param[in] engine DTPG エンジン
void
DtpgDriverImpl::do_dtpg(DtpgEngine& engine)
{
  if ( mFaultList.empty() ) {
    return;
  }

  engine.run_multi(mFaultList);
}

END_NAMESPACE_YM_SATPG

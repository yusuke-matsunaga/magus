
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
#include "utils/HeapTree.h"

#include "logic/Bdd.h"
#include "logic/BddMgr.h"


#define VERIFY_MAIMP 0


BEGIN_NAMESPACE_YM_SATPG

bool debug_fault_analysis = false;

Dtpg*
new_DtpgSat()
{
  return new DtpgSat();
}

// @brief コンストラクタ
DtpgSat::DtpgSat()
{
  mNetwork = NULL;
}

// @brief デストラクタ
DtpgSat::~DtpgSat()
{
}

// @brief 使用する SAT エンジンを指定する．
void
DtpgSat::set_mode(const string& type,
		  const string& option,
		  ostream* outp)
{
  mSatEngine.set_mode(type, option, outp);
  mSatEngineSingle.set_mode(type, option, outp);
}

// @brief テスト生成を行なう．
// @param[in] tpgnetwork 対象のネットワーク
// @param[in] mode メインモード
// @param[in] po_mode PO分割モード
// @param[in] option_str オプション文字列
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
// @param[in] stats 結果を格納する構造体
void
DtpgSat::run(TpgNetwork& tgnetwork,
	     DtpgMode mode,
	     tDtpgPoMode po_mode,
	     const string& option_str,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop,
	     DtpgStats& stats)
{
  mSatEngine.set_option(option_str);
  mSatEngineSingle.set_option(option_str);

  mSatEngine.clear_stats();
  mSatEngineSingle.clear_stats();

  mNetwork = &tgnetwork;

  switch ( po_mode ) {
  case kDtpgPoNone:
    // PO 分割を行わないモード

    mNetwork->activate_all();

    dtpg1(mode, bt, dop, uop);

    break;

  case kDtpgPoInc:
    { // 正順で PO を選び分割するモード
      ymuint no = mNetwork->output_num2();
      for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
	mNetwork->activate_po(po_pos);

	dtpg1(mode, bt, dop, uop);
      }
    }
    break;

  case kDtpgPoDec:
    { // 逆順で PO を選び分割するモード
      ymuint no = mNetwork->output_num2();
      for (ymuint i = 0; i < no; ++ i) {
	ymuint po_pos = no - i - 1;
	mNetwork->activate_po(po_pos);

	dtpg1(mode, bt, dop, uop);
      }
    }
    break;
  }

  if ( mode.mode() == kDtpgSingle ) {
    mSatEngineSingle.get_stats(stats);
  }
  else {
    mSatEngine.get_stats(stats);
  }
}

// @brief activate された部分回路に大してテスト生成を行う．
// @param[in] mode メインモード
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
void
DtpgSat::dtpg1(DtpgMode mode,
	       BackTracer& bt,
	       DetectOp& dop,
	       UntestOp& uop)
{
  switch ( mode.mode() ) {
  case kDtpgSingle:
    single_mode(bt, dop, uop);
    break;

  case kDtpgFFR:
    ffr_mode(bt, dop, uop);
    break;

  case kDtpgMFFC:
    mffc_mode(bt, dop, uop);
    break;
  }
}

// @brief single モードでテスト生成を行なう．
void
DtpgSat::single_mode(BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop)
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
	mSatEngineSingle.run(f, mNetwork->max_node_id(), bt, dop, uop);
      }
    }
  }
}

// @brief ffr モードでテスト生成を行なう．
void
DtpgSat::ffr_mode(BackTracer& bt,
		  DetectOp& dop,
		  UntestOp& uop)
{
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    ymuint nfo = node->active_fanout_num();
    if ( nfo > 1 || (nfo == 1 && node->active_fanout(0)->is_output()) ) {
      clear_faults();

      dfs_ffr(node);

      do_dtpg(bt, dop, uop);
    }
  }
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

// @brief mffc モードでテスト生成を行なう．
void
DtpgSat::mffc_mode(BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop)
{
  ymuint n = mNetwork->active_node_num();
  vector<bool> mark(mNetwork->max_node_id(), false);
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->imm_dom() == NULL ) {
      clear_faults();

      dfs_mffc(node, mark);

      do_dtpg(bt, dop, uop);
    }
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
void
DtpgSat::do_dtpg(BackTracer& bt,
		 DetectOp& dop,
		 UntestOp& uop)
{
  if ( mFaultList.empty() ) {
    return;
  }

  mSatEngine.run(mFaultList, mNetwork->max_node_id(), bt, dop, uop);
}

END_NAMESPACE_YM_SATPG

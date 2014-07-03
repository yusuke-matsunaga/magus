
/// @file DtpgSmt.cc
/// @brief DtpgSmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSmt.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

Dtpg*
new_DtpgSmt()
{
  //return new DtpgSmt();
  return NULL;
}

// @brief コンストラクタ
DtpgSmt::DtpgSmt()
{
  mNetwork = NULL;
}

// @brief デストラクタ
DtpgSmt::~DtpgSmt()
{
}

// @brief 使用する SAT エンジンを指定する．
void
DtpgSmt::set_mode(const string& type,
		  const string& option,
		  ostream* outp)
{
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
DtpgSmt::run(TpgNetwork& tgnetwork,
	     DtpgMode mode,
	     tDtpgPoMode po_mode,
	     const string& option_str,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop,
	     DtpgStats& stats)
{
  mSatEngineSingle.set_option(option_str);

  mSatEngineSingle.clear_stats();

  mNetwork = &tgnetwork;
  mMaxId = mNetwork->node_num();

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

  mSatEngineSingle.get_stats(stats);
}

// @brief activate された部分回路に大してテスト生成を行う．
// @param[in] mode メインモード
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
void
DtpgSmt::dtpg1(DtpgMode mode,
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
DtpgSmt::single_mode(BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    // 出力の故障
    TpgFault* f0 = node->output_fault(0);
    dtpg_single(f0, bt, dop, uop);

    TpgFault* f1 = node->output_fault(1);
    dtpg_single(f1, bt, dop, uop);

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* f0 = node->input_fault(0, j);
      dtpg_single(f0, bt, dop, uop);

      TpgFault* f1 = node->input_fault(1, j);
      dtpg_single(f1, bt, dop, uop);
    }
  }
}

// @brief ffr モードでテスト生成を行なう．
void
DtpgSmt::ffr_mode(BackTracer& bt,
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



// @brief mffc モードでテスト生成を行なう．
void
DtpgSmt::mffc_mode(BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop)
{
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->imm_dom() == NULL ) {
      clear_faults();

      vector<bool> mark(mNetwork->node_num(), false);
      dfs_mffc(node, mark);

      do_dtpg(bt, dop, uop);
    }
  }
}

// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] f 故障
void
DtpgSmt::dtpg_single(TpgFault* f,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop)
{
  if ( f != NULL &&
       f->is_rep() &&
       f->status() != kFsDetected &&
       !f->is_skip() ) {
    mSatEngineSingle.run(f, mNetwork->max_node_id(), bt, dop, uop);
  }
}

void
DtpgSmt::dfs_ffr(TpgNode* node)
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
DtpgSmt::dfs_mffc(TpgNode* node,
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
DtpgSmt::add_node_faults(TpgNode* node)
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

// @brief テストパタン生成を行なう．
void
DtpgSmt::do_dtpg(BackTracer& bt,
		 DetectOp& dop,
		 UntestOp& uop)
{
  if ( mFaultList.empty() ) {
    return;
  }

  //mSatEngine.run(mFaultList, mNetwork->max_node_id(), bt, dop, uop);
}

END_NAMESPACE_YM_SATPG

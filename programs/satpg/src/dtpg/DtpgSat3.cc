
/// @file DtpgSat3.cc
/// @brief DtpgSat3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat3.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

Dtpg*
new_DtpgSat3()
{
  return new DtpgSat3();
}

// @brief コンストラクタ
DtpgSat3::DtpgSat3()
{
}

// @brief デストラクタ
DtpgSat3::~DtpgSat3()
{
}

// @brief 使用する SAT エンジンを指定する．
void
DtpgSat3::set_mode(const string& type,
		   const string& option,
		   ostream* outp)
{
  mSatEngineS.set_mode(type, option, outp);
  mSatEngineM.set_mode(type, option, outp);
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
DtpgSat3::run(TpgNetwork& network,
	      DtpgMode mode,
	      tDtpgPoMode po_mode,
	      const string& option_str,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop,
	      DtpgStats& stats)
{
  // 正順で PO を選び分割するモード
  ymuint no = network.output_num2();

  switch ( mode.mode() ) {
  case kDtpgSingle:
    mSatEngineS.set_option(option_str);
    mSatEngineS.clear_stats();

    for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
      network.activate_po(po_pos);

      single_mode(network, bt, dop, uop);
    }

    mSatEngineS.get_stats(stats);
    break;

  case kDtpgFFR:
    mSatEngineM.set_option(option_str);
    mSatEngineM.clear_stats();

    for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
      network.activate_po(po_pos);

      ffr_mode(network, bt, dop, uop);
    }

    mSatEngineM.get_stats(stats);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

// @brief single モードでテスト生成を行なう．
void
DtpgSat3::single_mode(TpgNetwork& network,
		      BackTracer& bt,
		      DetectOp& dop,
		      UntestOp& uop)
{
  ymuint nn = network.active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    // 出力の故障
    TpgFault* f0 = node->output_fault(0);
    dtpg_single(network, f0, bt, dop, uop);

    TpgFault* f1 = node->output_fault(1);
    dtpg_single(network, f1, bt, dop, uop);

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* f0 = node->input_fault(0, j);
      dtpg_single(network, f0, bt, dop, uop);

      TpgFault* f1 = node->input_fault(1, j);
      dtpg_single(network, f1, bt, dop, uop);
    }
  }
}

// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] f 故障
void
DtpgSat3::dtpg_single(TpgNetwork& network,
		      TpgFault* f,
		      BackTracer& bt,
		      DetectOp& dop,
		      UntestOp& uop)
{
  if ( f != NULL &&
       f->is_rep() &&
       f->status() != kFsDetected &&
       !f->is_skip() ) {
    mSatEngineS.run(f, network.max_node_id(), bt, dop, uop);
  }
}

// @brief ffr モードでテスト生成を行なう．
void
DtpgSat3::ffr_mode(TpgNetwork& network,
		   BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop)
{
  ymuint nn = network.active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = network.active_node(i);
    ymuint nfo = node->active_fanout_num();
    if ( nfo > 1 || (nfo == 1 && node->active_fanout(0)->is_output()) ) {
      clear_faults();

      dfs_ffr(node);

      do_dtpg(network, bt, dop, uop);
    }
  }
}

void
DtpgSat3::dfs_ffr(TpgNode* node)
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

// @brief ノードの故障を追加する．
void
DtpgSat3::add_node_faults(TpgNode* node)
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
DtpgSat3::do_dtpg(TpgNetwork& network,
		  BackTracer& bt,
		  DetectOp& dop,
		  UntestOp& uop)
{
  if ( mFaultList.empty() ) {
    return;
  }

  vector<TpgNode*> fnode_list;
  fnode_list.reserve(mFaultList.size());
  network.begin_fault_injection();
  for (vector<TpgFault*>::iterator p = mFaultList.begin();
       p != mFaultList.end(); ++ p) {
    TpgFault* f = *p;
    TpgNode* node = f->node();
    if ( f->is_input_fault() ) {
      TpgNode* fnode = network.inject_fnode(node, f->pos());
      fnode_list.push_back(fnode);
    }
    else {
      fnode_list.push_back(node);
    }
  }

  mSatEngineM.run(mFaultList, fnode_list, network.max_node_id(), bt, dop, uop);

  network.end_fault_injection();
}

END_NAMESPACE_YM_SATPG


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
new_DtpgSat()
{
  return new DtpgSat();
}

// @brief コンストラクタ
DtpgSat::DtpgSat()
{
  mSatEngine = new_SatEngine();

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

// @brief get_pat フラグを設定する．
void
DtpgSat::set_get_pat(ymuint val)
{
  mSatEngine->set_get_pat(val);
}

// @brief 回路と故障リストを設定する．
// @param[in] tgnetwork 対象のネットワーク
void
DtpgSat::set_network(TpgNetwork& tgnetwork)
{
  mNetwork = &tgnetwork;
  mMaxId = mNetwork->node_num();
}

// @brief スキップモードを指定する．
// @param[in] threshold しきい値
void
DtpgSat::set_skip(ymuint threshold)
{
  mSatEngine->set_skip(threshold);
}

// @brief スキップモードをクリアする．
void
DtpgSat::clear_skip()
{
  mSatEngine->clear_skip();
}

// @brief モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
// @param[in] option オプション文字列
void
DtpgSat::run(TpgOperator& op,
	     const string& option)
{
  bool single = false;
  bool dual = false;
  bool ffr = false;
  bool mffc = false;
  bool all = false;
  bool po = false;
  bool rpo = false;
  bool skip = false;

  if ( option == string() || option == "single" ) {
    single = true;
  }
  else if ( option == "dual" ) {
    dual = true;
  }
  else if ( option == "ffr" ) {
    ffr = true;
  }
  else if ( option == "mffc" ) {
    mffc = true;
  }
  else if ( option == "single_po" ) {
    single = true;
    po = true;
  }
  else if ( option == "single_po_skip" ) {
    single = true;
    po = true;
    skip = true;
  }
  else if ( option == "dual_po" ) {
    dual = true;
    po = true;
  }
  else if ( option == "dual_po_skip" ) {
    dual = true;
    po = true;
    skip = true;
  }
  else if ( option == "ffr_po" ) {
    ffr = true;
    po = true;
  }
  else if ( option == "ffr_po_skip" ) {
    ffr = true;
    po = true;
    skip = true;
  }
  else if ( option == "mffc_po" ) {
    all = true;
    po = true;
  }
  else if ( option == "mffc_po_skip" ) {
    all = true;
    po = true;
    skip = true;
  }
  else if ( option == "all_po" ) {
    all = true;
    po = true;
  }
  else if ( option == "all_po_skip" ) {
    all = true;
    po = true;
    skip = true;
  }
  else if ( option == "single_rpo" ) {
    single = true;
    rpo = true;
  }
  else if ( option == "single_rpo_skip" ) {
    single = true;
    rpo = true;
    skip = true;
  }
  else if ( option == "dual_rpo" ) {
    dual = true;
    rpo = true;
  }
  else if ( option == "dual_rpo_skip" ) {
    dual = true;
    rpo = true;
    skip = true;
  }
  else if ( option == "ffr_rpo" ) {
    ffr = true;
    po = true;
  }
  else if ( option == "ffr_rpo_skip" ) {
    ffr = true;
    rpo = true;
    skip = true;
  }
  else if ( option == "mffc_rpo" ) {
    all = true;
    rpo = true;
  }
  else if ( option == "mffc_rpo_skip" ) {
    all = true;
    rpo = true;
    skip = true;
  }
  else if ( option == "all_rpo" ) {
    all = true;
    rpo = true;
  }
  else if ( option == "all_rpo_skip" ) {
    all = true;
    rpo = true;
    skip = true;
  }
  else {
    cout << "illegal option " << option << ". ignored" << endl;
    return;
  }

  if ( po ) {
    if ( skip ) {
      set_skip(3);
    }
    ymuint no = mNetwork->output_num2();
    for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
      mNetwork->activate_po(po_pos);

      if ( single ) {
	single_mode(op);
      }
      else if ( dual ) {
	dual_mode(op);
      }
      else if ( ffr ) {
	ffr_mode(op);
      }
      else if ( all ) {
	all_mode(op);
      }
    }

    if ( skip ) {
      clear_skip();
    }
  }
  else if ( rpo ) {
    if ( skip ) {
      set_skip(3);
    }
    ymuint no = mNetwork->output_num2();
    for (ymuint i = 0; i < no; ++ i) {
      ymuint po_pos = no - i - 1;
      mNetwork->activate_po(po_pos);

      if ( single ) {
	single_mode(op);
      }
      else if ( dual ) {
	dual_mode(op);
      }
      else if ( ffr ) {
	ffr_mode(op);
      }
      else if ( all ) {
	all_mode(op);
      }
    }

    if ( skip ) {
      clear_skip();
    }
  }
  else { // !po && !rpo
    mNetwork->activate_all();
    if ( single ) {
      single_mode(op);
    }
    else if ( dual ) {
      dual_mode(op);
    }
    else if ( ffr ) {
      ffr_mode(op);
    }
    else if ( mffc ) {
      mffc_mode(op);
    }
  }
}

// @brief single モードでテスト生成を行なう．
void
DtpgSat::single_mode(TpgOperator& op)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = mNetwork->active_node(i);

    // 出力の故障
    TpgFault* f0 = node->output_fault(0);
    dtpg_single(f0, op);

    TpgFault* f1 = node->output_fault(1);
    dtpg_single(f1, op);

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* f0 = node->input_fault(0, j);
      dtpg_single(f0, op);

      TpgFault* f1 = node->input_fault(1, j);
      dtpg_single(f1, op);
    }
  }
}

// @brief dual モードでテスト生成を行なう．
void
DtpgSat::dual_mode(TpgOperator& op)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = mNetwork->active_node(i);

    // 出力の故障
    TpgFault* f0 = node->output_fault(0);
    TpgFault* f1 = node->output_fault(1);
    dtpg_dual(f0, f1, op);

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* f0 = node->input_fault(0, j);
      TpgFault* f1 = node->input_fault(1, j);
      dtpg_dual(f0, f1, op);
    }
  }
}

// @brief ffr モードでテスト生成を行なう．
void
DtpgSat::ffr_mode(TpgOperator& op)
{
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() || node->active_fanout_num() > 1 ) {
      mFaultList.clear();

      dfs_ffr(node);

      do_dtpg(op);
    }
  }
}

// @brief mffc モードでテスト生成を行なう．
void
DtpgSat::mffc_mode(TpgOperator& op)
{
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->imm_dom() == NULL ) {
      mFaultList.clear();

      vector<bool> mark(mNetwork->node_num(), false);
      dfs_mffc(node, mark);

      do_dtpg(op);
    }
  }
}

// @brief all モードでテスト生成を行なう．
void
DtpgSat::all_mode(TpgOperator& op)
{
  mFaultList.clear();

  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    add_node_faults(node);
  }

  do_dtpg(op);
}

// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] f 故障
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_single(TpgFault* f,
		     TpgOperator& op)
{
  mFaultList.clear();

  add_fault(f);

  do_dtpg(op);
}

// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
// @param[in] f0 0縮退故障
// @param[in] f1 1縮退故障
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_dual(TpgFault* f0,
		   TpgFault* f1,
		   TpgOperator& op)
{
  mFaultList.clear();

  add_fault(f0);
  add_fault(f1);

  do_dtpg(op);
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

  add_node_faults(node);
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

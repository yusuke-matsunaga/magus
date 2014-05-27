
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
#include "DtpgNgMgr.h"
#include "DtpgNodeGroup.h"
#include "DtpgNgEdge.h"
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
  mSatEngine.clear_stats();
  mSatEngine.set_option(option_str);

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

  mSatEngine.get_stats(stats);
}

BEGIN_NONAMESPACE

void
mark_mffc(TpgNode* node,
	  vector<bool>& mark,
	  vector<TpgNode*>& node_list,
	  vector<TpgNode*>& input_list)
{
  mark[node->id()] = true;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( mark[inode->id()] ) {
      continue;
    }
    if ( inode->imm_dom() != NULL ) {
      mark_mffc(inode, mark, node_list, input_list);
    }
    else {
      input_list.push_back(inode);
    }
  }

  if ( !node->is_output() ) {
    node_list.push_back(node);
    if ( node->is_input() ) {
      input_list.push_back(node);
    }
  }
}

Bdd
calc_bdd(tTgGateType gate_type,
	 const vector<Bdd>& input_bdd)
{
  switch ( gate_type ) {
  case kTgGateBuff:
    return input_bdd[0];

  case kTgGateNot:
    return ~input_bdd[0];

  case kTgGateAnd:
    {
      ymuint ni = input_bdd.size();
      Bdd ans = input_bdd[0];
      for (ymuint i = 1; i < ni; ++ i) {
	ans &= input_bdd[i];
      }
      return ans;
    }

  case kTgGateNand:
    {
      ymuint ni = input_bdd.size();
      Bdd ans = input_bdd[0];
      for (ymuint i = 1; i < ni; ++ i) {
	ans &= input_bdd[i];
      }
      return ~ans;
    }

  case kTgGateOr:
    {
      ymuint ni = input_bdd.size();
      Bdd ans = input_bdd[0];
      for (ymuint i = 1; i < ni; ++ i) {
	ans |= input_bdd[i];
      }
      return ans;
    }

  case kTgGateNor:
    {
      ymuint ni = input_bdd.size();
      Bdd ans = input_bdd[0];
      for (ymuint i = 1; i < ni; ++ i) {
	ans |= input_bdd[i];
      }
      return ~ans;
    }

  case kTgGateXor:
    {
      ymuint ni = input_bdd.size();
      Bdd ans = input_bdd[0];
      for (ymuint i = 1; i < ni; ++ i) {
	ans ^= input_bdd[i];
      }
      return ans;
    }

  case kTgGateXnor:
    {
      ymuint ni = input_bdd.size();
      Bdd ans = input_bdd[0];
      for (ymuint i = 1; i < ni; ++ i) {
	ans ^= input_bdd[i];
      }
      return ~ans;
    }

  default:
    assert_not_reached(__FILE__, __LINE__);
  }

  return Bdd();
}

Bdd
calc_node_bdd(TpgNode* node,
	      const vector<Bdd>& f_array)
{
  assert_cond( node->is_logic(), __FILE__, __LINE__);

  ymuint ni = node->fanin_num();
  vector<Bdd> input_bdd(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    input_bdd[i] = f_array[node->fanin(i)->id()];
  }
  return calc_bdd(node->gate_type(), input_bdd);
}

Bdd
calc_faulty_bdd(TpgNode* node,
		const vector<Bdd>& f_array,
		TpgFault* f,
		BddMgr& mgr)
{
  ymuint ni = node->fanin_num();
  vector<Bdd> input_bdd(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    input_bdd[i] = f_array[node->fanin(i)->id()];
  }

  if ( f->node() == node ) {
    Bdd fval;
    if ( f->val() == 0 ) {
      fval = mgr.make_zero();
    }
    else {
      fval = mgr.make_one();
    }
    if ( f->is_input_fault() ) {
      ymuint pos = f->pos();
      input_bdd[pos] = fval;
    }
    else {
      return fval;
    }
  }

  if ( node->is_input() ) {
    return f_array[node->id()];
  }

  assert_cond( node->is_logic(), __FILE__, __LINE__);

  return calc_bdd(node->gate_type(), input_bdd);
}

inline
bool
check_fault(TpgFault* f)
{
  if ( f != NULL &&
       f->is_rep() &&
       f->status() != kFsDetected &&
       !f->is_skip() ) {
    return true;
  }
  return false;
}

END_NONAMESPACE

// @brief MFFC 単位で故障差関数を作り，支配関係をもとにした順序付けを行う．
void
DtpgSat::fault_analyze()
{
  ymuint nrep = 0;
  ymuint ndom = 0;
  BddMgr bdd_mgr("bmc");
  vector<Bdd> f_array(mNetwork->node_num());
  vector<Bdd> g_array(mNetwork->node_num());
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->imm_dom() != NULL ) {
      continue;
    }

    vector<bool> mark(mNetwork->node_num(), false);
    vector<TpgNode*> node_list;
    vector<TpgNode*> input_list;
    mark_mffc(node, mark, node_list, input_list);
    ymuint ni = input_list.size();
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = input_list[i];
      f_array[inode->id()] = bdd_mgr.make_posiliteral(VarId(i));
      g_array[inode->id()] = f_array[inode->id()];
    }

    // 正常回路の関数を計算する．
    ymuint nn = node_list.size();
    for (ymuint i = 0; i < nn; ++ i) {
      TpgNode* node = node_list[i];
      if ( !node->is_input() ) {
	f_array[node->id()] = calc_node_bdd(node, f_array);
      }
    }

    vector<TpgFault*> fault_list;
    for (ymuint i = 0; i < nn; ++ i) {
      TpgNode* node = node_list[i];
      ymuint ni = node->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	TpgFault* f0 = node->input_fault(0, i);
	if ( check_fault(f0) ) {
	  fault_list.push_back(f0);
	}
	TpgFault* f1 = node->input_fault(1, i);
	if ( check_fault(f1) ) {
	  fault_list.push_back(f1);
	}
      }
      TpgFault* f0 = node->output_fault(0);
      if ( check_fault(f0) ) {
	fault_list.push_back(f0);
      }
      TpgFault* f1 = node->output_fault(1);
      if ( check_fault(f1) ) {
	fault_list.push_back(f1);
      }
    }
    ymuint nf = fault_list.size();
    nrep += nf;
    vector<Bdd> df(nf);
    for (ymuint i = 0; i < nf; ++ i) {
      TpgFault* f = fault_list[i];
      for (ymuint j = 0; j < nn; ++ j) {
	TpgNode* node = node_list[j];
	g_array[node->id()] = calc_faulty_bdd(node, g_array, f, bdd_mgr);
      }
      ymuint oid = node_list[nn - 1]->id();
      df[i] = f_array[oid] ^ g_array[oid];
    }
    if ( debug_fault_analysis ) {
      cout << "# of faults: " << nf << endl;
      for (ymuint i = 0; i < nf; ++ i) {
	cout << " " << fault_list[i];
      }
      cout << endl;
    }
    vector<ymuint> dom_array(nf, nf);
    for (ymuint i = 0; i < nf; ++ i) {
      Bdd df1 = df[i];
      if ( dom_array[i] != nf ) {
	continue;
      }
      for (ymuint j = i + 1; j < nf; ++ j) {
	Bdd df2 = df[j];
	if ( dom_array[j] != nf ) {
	  continue;
	}
	if ( df1 <= df2 ) {
	  fault_list[j]->set_dominance(fault_list[i]);
	  dom_array[j] = i;
	}
	else if ( df1 >= df2 ) {
	  fault_list[i]->set_dominance(fault_list[j]);
	  dom_array[i] = j;
	}
	if ( dom_array[i] != nf ) {
	  break;
	}
      }
    }
    ymuint nd = 0;
    for (ymuint i = 0; i < nf; ++ i) {
      if ( dom_array[i] == nf ) {
	++ nd;
      }
    }
    ndom += nd;
    if ( debug_fault_analysis ) {
      cout << "# of dominant faults: " << nd << endl;
      for (ymuint i = 0; i < nf; ++ i) {
	if ( dom_array[i] == nf ) {
	  cout << " " << fault_list[i];
	}
      }
      cout << endl
	   << endl;
    }
  }
  cout << "Fault Analysis end: " << ndom << " / " << nrep << endl;
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

  case kDtpgDual:
    dual_mode(bt, dop, uop);
    break;

  case kDtpgNode:
    node_mode(bt, dop, uop);
    break;

  case kDtpgFFR:
    ffr_mode(bt, dop, uop);
    break;

  case kDtpgFFR2:
    ffr2_mode(mode.ffr2_limit(), bt, dop, uop);
    break;

  case kDtpgMFFC:
    mffc_mode(bt, dop, uop);
    break;

  case kDtpgAll:
    all_mode(bt, dop, uop);
    break;
  }
}

BEGIN_NONAMESPACE

bool
check_dom(TpgFault* f)
{
  if ( f->status() == kFsDetected ||
       f->status() == kFsUntestable ) {
    return true;
  }
  if ( !f->is_rep() ) {
    return true;
  }
  const vector<TpgFault*>& dom_list = f->dom_list();
  for (vector<TpgFault*>::const_iterator p = dom_list.begin();
       p != dom_list.end(); ++ p) {
    TpgFault* f1 = *p;
    switch ( f1->status() ) {
    case kFsDetected: return true;
    case kFsUndetected: return true;
    default: break;
    }
  }
  return false;
}

END_NONAMESPACE


// @brief single モードでテスト生成を行なう．
void
DtpgSat::single_mode(BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop)
{
  ymuint nn = mNetwork->active_node_num();
#if 0
  for ( ; ; ) {
    bool skipped = false;
    bool update = false;
    for (ymuint i = 0; i < nn; ++ i) {
      TpgNode* node = mNetwork->active_node(i);
      if ( node->is_output() ) {
	continue;
      }

      // 出力の故障
      TpgFault* f0 = node->output_fault(0);
      if ( check_dom(f0) ) {
	skipped = true;
      }
      else {
	dtpg_single(f0, bt, dop, uop);
	update = true;
      }

      TpgFault* f1 = node->output_fault(1);
      if ( check_dom(f1) ) {
	skipped = true;
      }
      else {
	dtpg_single(f1, bt, dop, uop);
	update = true;
      }

      // 入力の故障
      ymuint ni = node->fanin_num();
      for (ymuint j = 0; j < ni; ++ j) {
	TpgFault* f0 = node->input_fault(0, j);
	if ( check_dom(f0) ) {
	  skipped = true;
	}
	else {
	  dtpg_single(f0, bt, dop, uop);
	  update = true;
	}

	TpgFault* f1 = node->input_fault(1, j);
	if ( check_dom(f1) ) {
	  skipped = true;
	}
	else {
	  dtpg_single(f1, bt, dop, uop);
	  update = true;
	}
      }
    }
    if ( !update ) {
      break;
    }
  }
#endif
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

// @brief dual モードでテスト生成を行なう．
void
DtpgSat::dual_mode(BackTracer& bt,
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
    TpgFault* f1 = node->output_fault(1);
    dtpg_dual(f0, f1, bt, dop, uop);

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* f0 = node->input_fault(0, j);
      TpgFault* f1 = node->input_fault(1, j);
      dtpg_dual(f0, f1, bt, dop, uop);
    }
  }
}

// @brief node モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::node_mode(BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    add_node_faults(node);

    do_dtpg(bt, dop, uop);
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
		   BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop)
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
DtpgSat::mffc_mode(BackTracer& bt,
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

// @brief all モードでテスト生成を行なう．
void
DtpgSat::all_mode(BackTracer& bt,
		  DetectOp& dop,
		  UntestOp& uop)
{
  clear_faults();

  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
    if ( !node->is_output() ) {
      add_node_faults(node);
    }
  }

  do_dtpg(bt, dop, uop);
}

// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] f 故障
void
DtpgSat::dtpg_single(TpgFault* f,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop)
{
  clear_faults();

  add_fault(f);

  do_dtpg(bt, dop, uop);
}

// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
// @param[in] f0 0縮退故障
// @param[in] f1 1縮退故障
void
DtpgSat::dtpg_dual(TpgFault* f0,
		   TpgFault* f1,
		   BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop)
{
  clear_faults();

  add_fault(f0);
  add_fault(f1);

  do_dtpg(bt, dop, uop);
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

// @brief テストパタン生成を行なう．
void
DtpgSat::do_dtpg(BackTracer& bt,
		 DetectOp& dop,
		 UntestOp& uop)
{
  if ( mFaultList.empty() ) {
    return;
  }

  vector<TpgNode*> fnode_list;
  fnode_list.reserve(mFaultList.size());
  mNetwork->begin_fault_injection();
  for (vector<TpgFault*>::iterator p = mFaultList.begin();
       p != mFaultList.end(); ++ p) {
    TpgFault* f = *p;
    TpgNode* node = f->node();
    if ( f->is_input_fault() ) {
      TpgNode* fnode = mNetwork->inject_fnode(node, f->pos());
      fnode_list.push_back(fnode);
    }
    else {
      fnode_list.push_back(node);
    }
  }

  mSatEngine.run(mFaultList, fnode_list, mNetwork->max_node_id(), bt, dop, uop);

  mNetwork->end_fault_injection();
}

END_NAMESPACE_YM_SATPG

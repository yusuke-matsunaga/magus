
/// @file atpg/src/dtpg/DtpgSat.cc
/// @brief DtpgSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"
#include "DtpgNetwork.h"
#include "DtpgNode.h"
#include "DtpgFault.h"
#include "DtpgOperator.h"


#define VERIFY_MAIMP 0


BEGIN_NAMESPACE_YM_SATPG

Dtpg*
new_DtpgSat()
{
  return new nsDtpg::DtpgSat();
}

END_NAMESPACE_YM_SATPG


BEGIN_NAMESPACE_YM_SATPG_DTPG

// @brief コンストラクタ
DtpgSat::DtpgSat()
{
  mNetwork = NULL;
}

// @brief デストラクタ
DtpgSat::~DtpgSat()
{
  delete mNetwork;
}

// @brief 使用する SAT エンジンを指定する．
void
DtpgSat::set_mode(const string& type,
		  const string& option,
		  ostream* outp)
{
  mSatEngine.set_mode(type, option, outp);
}

// @brief get_pat フラグを設定する．
void
DtpgSat::set_get_pat(ymuint val)
{
  mSatEngine.set_get_pat(val);
}

// @brief dry-run フラグを設定する．
void
DtpgSat::set_dry_run(bool flag)
{
  mSatEngine.set_dry_run(flag);
}

// @brief 回路と故障リストを設定する．
// @param[in] tgnetwork 対象のネットワーク
// @param[in] fault_list 故障リスト
void
DtpgSat::set_network(const TgNetwork& tgnetwork,
		     const vector<SaFault*>& fault_list)
{
  delete mNetwork;
  mNetwork = new DtpgNetwork(tgnetwork, fault_list);
}

// @brief モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
// @param[in] option オプション文字列
void
DtpgSat::run(DtpgOperator& op,
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
      mSatEngine.set_skip(true);
    }
    ymuint no = mNetwork->output_num2();
    for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
      mNetwork->activate_po(po_pos);

      if ( single ) {
	single_sub(op);
      }
      else if ( dual ) {
	dual_sub(op);
      }
      else if ( ffr ) {
	ffr_sub(op);
      }
      else if ( all ) {
	all_sub(op);
      }
    }

    if ( skip ) {
      mSatEngine.set_skip(false);
    }
  }
  else if ( rpo ) {
    if ( skip ) {
      mSatEngine.set_skip(true);
    }
    ymuint no = mNetwork->output_num2();
    for (ymuint i = 0; i < no; ++ i) {
      ymuint po_pos = no - i - 1;
      mNetwork->activate_po(po_pos);

      if ( single ) {
	single_sub(op);
      }
      else if ( dual ) {
	dual_sub(op);
      }
      else if ( ffr ) {
	ffr_sub(op);
      }
      else if ( all ) {
	all_sub(op);
      }
    }

    if ( skip ) {
      mSatEngine.set_skip(false);
    }
  }
  else { // !po && !rpo
    mNetwork->activate_all();
    if ( single ) {
      single_sub(op);
    }
    else if ( dual ) {
      dual_sub(op);
    }
    else if ( ffr ) {
      ffr_sub(op);
    }
    else if ( mffc ) {
      mffc_sub(op);
    }
  }
}

// @brief single モードの共通処理
void
DtpgSat::single_sub(DtpgOperator& op)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);

    // 出力の故障
    DtpgFault* f0 = node->output_fault(0);
    dtpg_single(f0, op);

    DtpgFault* f1 = node->output_fault(1);
    dtpg_single(f1, op);

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      DtpgFault* f0 = node->input_fault(0, j);
      dtpg_single(f0, op);

      DtpgFault* f1 = node->input_fault(1, j);
      dtpg_single(f1, op);
    }
  }
}

// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] f 故障
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_single(DtpgFault* f,
		     DtpgOperator& op)
{
  mFaultList.clear();

  add_fault(f);

  do_dtpg(op);
}

// @brief dual モードの共通処理
void
DtpgSat::dual_sub(DtpgOperator& op)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);

    // 出力の故障
    DtpgFault* f0 = node->output_fault(0);
    DtpgFault* f1 = node->output_fault(1);
    dtpg_dual(f0, f1, op);

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      DtpgFault* f0 = node->input_fault(0, j);
      DtpgFault* f1 = node->input_fault(1, j);
      dtpg_dual(f0, f1, op);
    }
  }
}

// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
// @param[in] f0 0縮退故障
// @param[in] f1 1縮退故障
// @param[in] op テスト生成の結果を処理するファンクター
void
DtpgSat::dtpg_dual(DtpgFault* f0,
		   DtpgFault* f1,
		   DtpgOperator& op)
{
  mFaultList.clear();

  add_fault(f0);
  add_fault(f1);

  do_dtpg(op);
}

// @brief ffr モードの共通処理
void
DtpgSat::ffr_sub(DtpgOperator& op)
{
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);
    if ( node->is_output() || node->active_fanout_num() > 1 ) {
      mFaultList.clear();

      dfs_ffr(node);

      do_dtpg(op);
    }
  }
}

void
DtpgSat::dfs_ffr(DtpgNode* node)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgNode* inode = node->fanin(i);
    if ( inode->active_fanout_num() == 1 ) {
      dfs_ffr(inode);
    }
  }

  add_node_faults(node);
}

// @brief mffc モードの共通処理
void
DtpgSat::mffc_sub(DtpgOperator& op)
{
  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);
    if ( node->imm_dom() == NULL ) {
      mFaultList.clear();

      vector<bool> mark(mNetwork->node_num(), false);
      dfs_mffc(node, mark);

      do_dtpg(op);
    }
  }
}

void
DtpgSat::dfs_mffc(DtpgNode* node,
		  vector<bool>& mark)
{
  mark[node->id()] = true;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgNode* inode = node->fanin(i);
    if ( mark[inode->id()] == false && inode->imm_dom() != NULL ) {
      dfs_mffc(inode, mark);
    }
  }

  add_node_faults(node);
}

// @brief all モードの共通処理
void
DtpgSat::all_sub(DtpgOperator& op)
{
  mFaultList.clear();

  ymuint n = mNetwork->active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    DtpgNode* node = mNetwork->active_node(i);
    add_node_faults(node);
  }

  do_dtpg(op);
}

// @brief ノードの故障を追加する．
void
DtpgSat::add_node_faults(DtpgNode* node)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    DtpgFault* f0 = node->input_fault(0, i);
    add_fault(f0);

    DtpgFault* f1 = node->input_fault(1, i);
    add_fault(f1);
  }
  DtpgFault* f0 = node->output_fault(0);
  add_fault(f0);

  DtpgFault* f1 = node->output_fault(1);
  add_fault(f1);
}

// @brief 統計情報をクリアする．
void
DtpgSat::clear_stats()
{
  mSatEngine.clear_stats();
}

#if 0
BEGIN_NONAMESPACE

string
f_str(DtpgFault* f)
{
  ostringstream buf;
  buf << "Node#" << f->node()->id() << ": ";
  if ( f->is_output_fault() ) {
    buf << "O";
  }
  else {
    buf << "I" << f->pos();
  }
  buf << ": SA" << f->val();
  return buf.str();
}

struct FaultGroup
{
  vector<DtpgFault*> mFaultList;

  vector<ymuint32> mMaList;

};

bool
check_conflict(const vector<ymuint32>& list_a,
	       const vector<ymuint32>& list_b)
{
  if ( list_a.empty() || list_b.empty() ) {
    return false;
  }

  vector<ymuint32>::const_iterator p_a = list_a.begin();
  vector<ymuint32>::const_iterator p_b = list_b.begin();
  ymuint32 tmp_a = *p_a;
  ymuint32 tmp_b = *p_b;
  ymuint idx_a = tmp_a / 2;
  ymuint idx_b = tmp_b / 2;
  for ( ; ; ) {
    if ( idx_a < idx_b ) {
      ++ p_a;
      if ( p_a == list_a.end() ) {
	return false;
      }
      tmp_a = *p_a;
      idx_a = tmp_a / 2;
    }
    else if ( idx_a > idx_b ) {
      ++ p_b;
      if ( p_b == list_b.end() ) {
	return false;
      }
      tmp_b = *p_b;
      idx_b = tmp_b / 2;
    }
    else {
      if ( tmp_a != tmp_b ) {
	// インデックスが同じで極性が異なっていた．
	return true;
      }
      ++ p_a;
      if ( p_a == list_a.end() ) {
	return false;
      }
      tmp_a = *p_a;
      idx_a = tmp_a / 2;

	++ p_b;
      if ( p_b == list_b.end() ) {
	return false;
      }
      tmp_b = *p_b;
      idx_b = tmp_b / 2;
    }
  }
}

void
merge(vector<ymuint32>& list_a,
      const vector<ymuint32>& list_b)
{
  if ( list_a.empty() ) {
    list_a = list_b;
    return;
  }
  if ( list_b.empty() ) {
    return;
  }

  vector<ymuint32> dst_list;

  vector<ymuint32>::const_iterator p_a = list_a.begin();
  vector<ymuint32>::const_iterator p_b = list_b.begin();
  ymuint32 tmp_a = *p_a;
  ymuint32 tmp_b = *p_b;
  ymuint idx_a = tmp_a / 2;
  ymuint idx_b = tmp_b / 2;
  for ( ; ; ) {
    if ( idx_a < idx_b ) {
      dst_list.push_back(tmp_a);
      ++ p_a;
      if ( p_a == list_a.end() ) {
	break;
      }
      tmp_a = *p_a;
      idx_a = tmp_a / 2;
    }
    else if ( idx_a > idx_b ) {
      dst_list.push_back(tmp_b);
      ++ p_b;
      if ( p_b == list_b.end() ) {
	break;
      }
      tmp_b = *p_b;
      idx_b = tmp_b / 2;
    }
    else {
      assert_cond( tmp_a == tmp_b, __FILE__, __LINE__);
      dst_list.push_back(tmp_a);

      ++ p_a;
      if ( p_a == list_a.end() ) {
	break;
      }
      tmp_a = *p_a;
      idx_a = tmp_a / 2;

      ++ p_b;
      if ( p_b == list_b.end() ) {
	break;
      }
      tmp_b = *p_b;
      idx_b = tmp_b / 2;
    }
  }
  for ( ; p_a != list_a.end(); ++ p_a) {
    dst_list.push_back(*p_a);
  }
  for ( ; p_b != list_b.end(); ++ p_b) {
    dst_list.push_back(*p_b);
  }

  list_a = dst_list;
}

END_NONAMESPACE
#endif

#if 0
// @brief 必要割り当ての情報に基づいて故障をグループ分けする．
// @note 内部で dtpg_ffr() を呼ぶ．
void
DtpgSat::dtpg_ffr2(const vector<DtpgFault*>& flist,
		   DtpgNode* root,
		   const vector<DtpgNode*>& node_list,
		   DtpgOperator& op)
{
#if VERIFY_MAIMP
  // 必要割り当てで冗長と判定された故障のリストを作る．
  vector<DtpgFault*> r_list;
  for (vector<DtpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    DtpgFault* f = *p;
    bool stat = get_mandatory_assignment(f, f->ma_list());
    if ( !stat ) {
      r_list.push_back(f);
    }
  }

  dtpg_ffr(flist, root, node_list, op);

  // SAT でも冗長と判定されたかチェックする．
  for (vector<DtpgFault*>::iterator p = r_list.begin();
       p != r_list.end(); ++ p) {
    DtpgFault* f = *p;
    if ( !f->is_untestable() ) {
      cout << "Error! " << f_str(f)
	   << " is not redundant" << endl;
    }
    assert_cond( f->is_untestable(), __FILE__, __LINE__);
  }

#else

  // 必要割り当てに基づいて故障をグループ分けする．
  vector<FaultGroup> fault_group;
  for (vector<DtpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    DtpgFault* f = *p;
    bool stat = get_mandatory_assignment(f, f->ma_list());
    if ( stat ) {
      ymuint n = fault_group.size();
      bool found = false;
      for (ymuint i = 0; i < n; ++ i) {
	FaultGroup& fg = fault_group[i];
	if ( !check_conflict(fg.mMaList, f->ma_list()) ) {
	  fg.mFaultList.push_back(f);
	  merge(fg.mMaList, f->ma_list());
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	fault_group.push_back(FaultGroup());
	FaultGroup& fg = fault_group.back();
	fg.mFaultList.push_back(f);
	fg.mMaList = f->ma_list();
      }
    }
    else {
      // ここで冗長とわかったら以下の処理は必要ない．
      f->set_untestable();
      if ( mSkip ) {
	f->set_skip();
      }
      else {
	op.set_untestable(f->safault());
      }
    }
  }

  if ( fault_group.empty() ) {
    return;
  }

#if 0
  for (vector<FaultGroup>::iterator p = fault_group.begin();
       p != fault_group.end(); ++ p) {
    FaultGroup& fg = *p;
    dtpg_ffr(fg.mFaultList, root, node_list, op);
  }
#else
  dtpg_ffr(flist, root, node_list, op);
#endif
#endif
}
#endif


// @brief 統計情報を得る．
// @param[in] stats 結果を格納する構造体
void
DtpgSat::get_stats(DtpgStats& stats) const
{
  mSatEngine.get_stats(stats);
}

// @breif 時間計測を制御する．
void
DtpgSat::timer_enable(bool enable)
{
  mSatEngine.timer_enable(enable);
}

END_NAMESPACE_YM_SATPG_DTPG

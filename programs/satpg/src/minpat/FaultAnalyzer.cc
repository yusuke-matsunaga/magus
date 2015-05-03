
/// @file FaultAnalyzer.cc
/// @brief FaultAnalyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FaultAnalyzer.h"

#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "NodeSet.h"
#include "NodeValList.h"

#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"

#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

void
mark_tfi(const TpgNode* node,
	 HashSet<ymuint>& tfi_mark,
	 vector<ymuint>& input_list)
{
  if ( tfi_mark.check(node->id()) ) {
    return;
  }
  tfi_mark.add(node->id());

  if ( node->is_input() ) {
    input_list.push_back(node->input_id());
  }
  else {
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = node->fanin(i);
      mark_tfi(inode, tfi_mark, input_list);
    }
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス FaultAnalyzer
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FaultAnalyzer::FaultAnalyzer()
{
  mVerbose = 0;
}

// @brief デストラクタ
FaultAnalyzer::~FaultAnalyzer()
{
}

// @brief verbose フラグを設定する．
// @param[in] verbose 表示を制御するフラグ
void
FaultAnalyzer::set_verbose(int verbose)
{
  mVerbose = verbose;
}

// @brief verbose フラグを得る．
int
FaultAnalyzer::verbose() const
{
  return mVerbose;
}

// @brief 初期化する．
// @param[in] network ネットワーク
// @param[in] tvmgr テストベクタのマネージャ
// @param[out] fault_list 検出された故障のリスト
void
FaultAnalyzer::init(const TpgNetwork& network,
		    TvMgr& tvmgr,
		    vector<const TpgFault*>& fault_list)
{
  StopWatch local_timer;
  local_timer.start();

  ymuint nn = network.active_node_num();
  mMaxNodeId = network.max_node_id();
  mMaxFaultId = 0;
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }
    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      const TpgFault* fault = node->fault(i);
      ymuint f_id = fault->id();
      if ( mMaxFaultId < f_id ) {
	mMaxFaultId = f_id;
      }
    }
  }
  ++ mMaxFaultId;

  mInputListArray.clear();
  mInputListArray.resize(mMaxNodeId);

  mInputList2Array.clear();
  mInputList2Array.resize(mMaxNodeId);

  mNodeSetArray.clear();
  mNodeSetArray.resize(mMaxNodeId);

  mFaultInfoArray.clear();
  mFaultInfoArray.resize(mMaxFaultId);

  mDomCheckCount = 0;

  ymuint f_all = 0;
  ymuint f_det = 0;
  ymuint f_red = 0;
  ymuint f_abt = 0;

  mTestVectorList.clear();
  vector<bool> det_flag(mMaxFaultId, false);
  for (ymuint i = 0; i < nn; ++ i) {
    if ( verbose() > 1 ) {
      cout << "\r" << setw(6) << i << " / " << setw(6) << nn;
      cout.flush();
    }

    const TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    NodeSet& node_set = mNodeSetArray[node->id()];
    node_set.mark_region(mMaxNodeId, node);

    vector<ymuint>& input_list = mInputListArray[node->id()];
    for (ymuint j = 0; j < node_set.tfo_tfi_size(); ++ j) {
      const TpgNode* node1 = node_set.tfo_tfi_node(j);
      if ( node1->is_input() ) {
	input_list.push_back(node1->input_id());
      }
    }
    // ソートしておく．
    sort(input_list.begin(), input_list.end());

    // 故障箇所の TFI に含まれる入力番号を mInputList2Array に入れる．
    HashSet<ymuint> tfi_mark;
    vector<ymuint>& input_list2 = mInputList2Array[node->id()];
    mark_tfi(node, tfi_mark, input_list2);
    // ソートしておく．
    sort(input_list2.begin(), input_list2.end());

    ymuint nf = node->fault_num();
    for (ymuint j = 0; j < nf; ++ j) {
      const TpgFault* fault = node->fault(j);
      Bool3 stat = analyze_fault(fault, tvmgr);
      ++ f_all;
      switch ( stat ) {
      case kB3True:
	++ f_det;
	det_flag[fault->id()] = true;
	break;

      case kB3False:
	++ f_red;
	break;

      case kB3X:
	++ f_abt;
      }
    }
  }

  mOrigFaultList.clear();
  mOrigFaultList.reserve(f_det);
  for (ymuint i = 0; i < network.active_node_num(); ++ i) {
    const TpgNode* node = network.active_node(i);
    ymuint ni = node->fanin_num();
    bool has_ncfault = false;
    for (ymuint j = 0; j < ni; ++ j) {
      const TpgFault* f0 = node->input_fault(0, j);
      if ( f0 != NULL ) {
	if ( f0->is_rep() && det_flag[f0->id()] ) {
	  mOrigFaultList.push_back(f0);
	}
	if ( node->nval() == kVal0 && det_flag[f0->rep_fault()->id()] ) {
	  has_ncfault = true;
	}
      }
      const TpgFault* f1 = node->input_fault(1, j);
      if ( f1 != NULL ) {
	if ( f1->is_rep() && det_flag[f1->id()] ) {
	  mOrigFaultList.push_back(f1);
	}
	if ( node->nval() == kVal1 && det_flag[f1->rep_fault()->id()] ) {
	  has_ncfault = true;
	}
      }
    }
    const TpgFault* f0 = node->output_fault(0);
    if ( f0 != NULL && f0->is_rep() && det_flag[f0->id()] ) {
      if ( node->noval() != kVal0 || !has_ncfault ) {
	mOrigFaultList.push_back(f0);
      }
    }
    const TpgFault* f1 = node->output_fault(1);
    if ( f1 != NULL && f1->is_rep() && det_flag[f1->id()] ) {
      if ( node->noval() != kVal1 || !has_ncfault ) {
	mOrigFaultList.push_back(f1);
      }
    }
  }

  fault_list = mOrigFaultList;

  local_timer.stop();

  if ( verbose() > 0 ) {
    if ( verbose() > 1 ) {
      cout << endl;
    }
    cout << "Total " << setw(6) << f_all << " faults" << endl
	 << "Total " << setw(6) << f_det << " detected faults" << endl
	 << "Total " << setw(6) << f_red << " redundant faults" << endl
	 << "Total " << setw(6) << f_abt << " aborted faults" << endl
	 << "CPU time " << local_timer.time() << endl;
  }
}

// @brief 故障の解析を行う．
// @param[in] fault 故障
// @param[in] tvmgr テストベクタのマネージャ
Bool3
FaultAnalyzer::analyze_fault(const TpgFault* fault,
			     TvMgr& tvmgr)
{
  ymuint f_id = fault->id();
  FaultInfo& fi = mFaultInfoArray[f_id];

  fi.mFault = fault;

  GvalCnf gval_cnf(mMaxNodeId);
  FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
  SatEngine engine(string(), string(), NULL);

  engine.make_fval_cnf(fval_cnf, fault, node_set(f_id), kVal1);

  vector<Bool3> sat_model;
  Bool3 sat_stat = engine.check_sat(sat_model);
  if ( sat_stat == kB3True ) {
    NodeValList& suf_list = fi.mSufficientAssignment;
    NodeValList& pi_suf_list = fi.mPiSufficientAssignment;

    fval_cnf.get_pi_suf_list(sat_model, fault, node_set(f_id), suf_list, pi_suf_list);

    // テストベクタを作る．
    TestVector* tv = tvmgr.new_vector();
    ymuint npi = pi_suf_list.size();
    for (ymuint i = 0; i < npi; ++ i) {
      NodeVal nv = pi_suf_list[i];
      const TpgNode* node = nv.node();
      ASSERT_COND ( node->is_input() );
      ymuint id = node->input_id();
      if ( nv.val() ) {
	tv->set_val(id, kVal1);
      }
      else {
	tv->set_val(id, kVal0);
      }
    }
    // X の部分をランダムに設定しておく
    tv->fix_x_from_random(mRandGen);

    fi.mTestVector = tv;

    // 必要割当を求める．
    NodeValList& ma_list = fi.mMandatoryAssignment;
    ymuint n = suf_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      NodeVal nv = suf_list[i];

      NodeValList list1;
      const TpgNode* node = nv.node();
      bool val = nv.val();
      list1.add(node, !val);
      if ( engine.check_sat(gval_cnf, list1) == kB3False ) {
	// node の値を反転したら検出できなかった．
	// -> この割当は必須割当
	ma_list.add(node, val);
      }
    }

    if ( suf_list.size() == ma_list.size() ) {
      fi.mSingleCube = true;
    }
#if 0
    else {
      NodeValList diff_list = suf_list;
      diff_list.diff(ma_list);
      ASSERT_COND( diff_list.size() > 0 );
      engine.add_negation(gval_cnf, diff_list);
      vector<Bool3> sat_model;
      Bool3 sat_ans = engine.check_sat(sat_model);
      ASSERT_COND( sat_ans == kB3True );
      for (ymuint i = 0; i < 2; ++ i) {
	NodeValList suf_list;
	fval_cnf.get_suf_list(sat_model, fault, node_set(f_id), suf_list);
	suf_list.diff(ma_list);

	fi.mOtherSufListArray.push_back(suf_list);
	engine.add_negation(gval_cnf, suf_list);
	sat_ans = engine.check_sat(sat_model);
	if ( sat_ans == kB3False ) {
	  break;
	}
      }
    }
#endif
  }
  return sat_stat;
}

// @brief 故障の情報をクリアする．
//
// 非支配故障の情報をクリアすることでメモリを減らす．
void
FaultAnalyzer::clear_fault_info(ymuint fid)
{
  ASSERT_COND( fid < mMaxFaultId );
  FaultInfo& fi = mFaultInfoArray[fid];
  fi.mMandatoryAssignment.clear();
  fi.mSufficientAssignment.clear();
  fi.mPiSufficientAssignment.clear();
  fi.mOtherSufListArray.clear();
}

// @brief ノード番号の最大値を得る．
ymuint
FaultAnalyzer::max_node_id() const
{
  return mMaxNodeId;
}

// @brief 故障番号の最大値を得る．
ymuint
FaultAnalyzer::max_fault_id() const
{
  return mMaxFaultId;
}

// @brief 検出可能な故障のリストを得る．
const vector<const TpgFault*>&
FaultAnalyzer::fault_list() const
{
  return mOrigFaultList;
}

// @brief 故障を得る．
// @param[in] fid 故障番号
const TpgFault*
FaultAnalyzer::fault(ymuint fid) const
{
  ASSERT_COND( fid < mMaxFaultId );
  return mFaultInfoArray[fid].fault();
}

// @brief 個別の故障の情報を得る．
// @param[in] fid 故障番号
const FaultInfo&
FaultAnalyzer::fault_info(ymuint fid) const
{
  ASSERT_COND( fid < mMaxFaultId );
  return mFaultInfoArray[fid];
}

// @brief 故障のTFOのTFIに含まれる入力番号のリスト返す．
// @param[in] fid 故障番号
const vector<ymuint>&
FaultAnalyzer::input_list(ymuint fid) const
{
  ASSERT_COND( fid < mMaxFaultId );
  const TpgFault* fault = mFaultInfoArray[fid].fault();
  return mInputListArray[fault->node()->id()];
}

// @brief 故障のTFIに含まれる入力番号のリスト返す．
// @param[in] fid 故障番号
const vector<ymuint>&
FaultAnalyzer::input_list2(ymuint fid) const
{
  ASSERT_COND( fid < mMaxFaultId );
  const TpgFault* fault = mFaultInfoArray[fid].fault();
  return mInputList2Array[fault->node()->id()];
}

// @brief 故障に関連するノード集合を返す．
const NodeSet&
FaultAnalyzer::node_set(ymuint fid) const
{
  ASSERT_COND( fid < mMaxFaultId );
  const TpgFault* fault = mFaultInfoArray[fid].fault();
  return mNodeSetArray[fault->node()->id()];
}

// @brief 故障の等価性をチェックする．
// @param[in] f1, f2 対象の故障
// @retval true f1 と f2 が等価だった．
// @retval false f1 と f2 は等価ではなかった．
//
// f1 を検出するパタン集合と f2 を検出するパタン集合
// が完全に一致するとき f1 と f2 が等価であると言う．
// f1 が f2 を支配し，f2 が f1 を支配することと同値
bool
FaultAnalyzer::check_equivalence(const TpgFault* f1,
				 const TpgFault* f2) const
{
  return check_dominance(f1, f2) && check_dominance(f2, f1);
}

BEGIN_NONAMESPACE

const TpgNode*
common_node(const TpgNode* node1,
	    const TpgNode* node2)
{
  ymuint id1 = node1->id();
  ymuint id2 = node2->id();
  for ( ; ; ) {
    if ( node1 == node2 ) {
      return node1;
    }
    if ( id1 < id2 ) {
      node1 = node1->imm_dom();
      if ( node1 == NULL ) {
	return NULL;
      }
      id1 = node1->id();
    }
    else if ( id1 > id2 ) {
      node2 = node2->imm_dom();
      if ( node2 == NULL ) {
	return NULL;
      }
      id2 = node2->id();
    }
  }
}

END_NONAMESPACE

// @brief 故障の支配関係をチェックする．
// @param[in] f1, f2 対象の故障
// @retval true f1 が f2 を支配している．
// @retval false f1 が f2 を支配していない．
//
// f1 を検出するいかなるパタンも f2 を検出する時
// f1 が f2 を支配すると言う．
bool
FaultAnalyzer::check_dominance(const TpgFault* f1,
			       const TpgFault* f2) const
{
  StopWatch timer;
  timer.start();

  ymuint f1_id = f1->id();
  ymuint f2_id = f2->id();

  const FaultInfo& fi1 = fault_info(f1_id);
  const FaultInfo& fi2 = fault_info(f2_id);

  const TpgNode* fnode1 = f1->node();
  const TpgNode* fnode2 = f2->node();
  const TpgNode* dom_node = common_node(fnode1, fnode2);

  SatEngine engine(string(), string(), NULL);
  GvalCnf gval_cnf(mMaxNodeId);

  // f1 の必要条件を追加する．
  const NodeValList& ma_list1 = fi1.mandatory_assignment();
  engine.add_assignments(gval_cnf, ma_list1);

  // f2 の十分条件の否定を追加する．
  const NodeValList& suf_list2 = fi2.sufficient_assignment();
  engine.add_negation(gval_cnf, suf_list2);

  // これだけで充足不能なら答は YES
  Bool3 sat_stat = engine.check_sat();
  if ( sat_stat == kB3False ) {
    goto end;
  }

  if ( dom_node != NULL ) {
    // 伝搬経路に共通な dominator がある時
    ++ mDomCheckCount;

    // 共通部分のノード集合
    NodeSet node_set0;
    node_set0.mark_region(mMaxNodeId, dom_node);

    // 故障1に固有のノード集合
    NodeSet node_set1;
    node_set1.mark_region2(mMaxNodeId, fnode1, dom_node);

    // 故障2に固有のノード集合
    NodeSet node_set2;
    node_set2.mark_region2(mMaxNodeId, fnode2, dom_node);

    // dom_node から出力までの故障伝搬条件を作る．
    FvalCnf fval_cnf0(mMaxNodeId, gval_cnf);
    engine.make_fval_cnf(fval_cnf0, dom_node, node_set0, kVal1);
    engine.add_diff_clause(fval_cnf0.gvar(dom_node), fval_cnf0.fvar(dom_node));

    // f1 を検出する条件を追加する．
    FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
    engine.make_fval_cnf(fval_cnf1, f1, node_set1, kVal1);

    // f2 を検出しない条件を追加する．
    FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);
    engine.make_fval_cnf(fval_cnf2, f2, node_set2, kVal0);

    sat_stat = engine.check_sat();
  }
  else {
    if ( !fi1.single_cube() ) {
      // f1 を検出する CNF を生成
      FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
      const NodeSet& node_set1 = node_set(f1_id);
      engine.make_fval_cnf(fval_cnf1, f1, node_set1, kVal1);

      sat_stat = engine.check_sat();
      if ( sat_stat == kB3False ) {
	goto end;
      }
    }

    if ( !fi2.single_cube() ) {
      // f2 を検出しない CNF を生成
      FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);
      const NodeSet& node_set2 = node_set(f2_id);
      engine.make_fval_cnf(fval_cnf2, f2, node_set2, kVal0);

      sat_stat = engine.check_sat();
      if ( sat_stat == kB3False ) {
	goto end;
      }
    }
  }

 end:

  timer.stop();
  USTime time = timer.time();
  if ( sat_stat == kB3False ) {
    mSuccessTime += time;
    if ( mSuccessMax.usr_time_usec() < time.usr_time_usec() ) {
      if ( time.usr_time() > 1.0 ) {
	cout << "UNSAT: " << f1 << ": " << f2 << "  " << time << endl;
      }
      mSuccessMax = time;
    }
    return true;
  }
  else if ( sat_stat == kB3True ) {
    mFailureTime += time;
    if ( mFailureMax.usr_time_usec() < time.usr_time_usec() ) {
      if ( time.usr_time() > 1.0 ) {
	cout << "SAT: " << f1 << ": " << f2 << "  " << time << endl;
      }
      mFailureMax = time;
    }
    return false;
  }
  else {
    mAbortTime += timer.time();
    if ( mAbortMax.usr_time_usec() < time.usr_time_usec() ) {
      if ( time.usr_time() > 1.0 ) {
	cout << "ABORT: " << f1 << ": " << f2 << "  " << time << endl;
      }
      mAbortMax = time;
    }
    return false;
  }
}

// @brief 故障の両立性をチェックする．
// @param[in] f1, f2 対象の故障
// @retval true f1 と f2 が両立する．
// @retval false f1 と f2 が衝突している．
//
// f1 を検出するパタン集合と f2 を検出するパタン集合
// の共通部分がからでない時 f1 と f2 は両立すると言う．
bool
FaultAnalyzer::check_compatibility(const TpgFault* f1,
				   const TpgFault* f2) const
{
  return false;
}

// @brief 処理時間の情報を出力する．
// @param[in] s 出力先のストリーム
void
FaultAnalyzer::print_stats(ostream& s) const
{
  s << "  CPU time (success)     " << mSuccessTime << "(MAX " << mSuccessMax << ")" << endl
    << "  CPU time (failure)     " << mFailureTime << "(MAX " << mFailureMax << ")" << endl
    << "  CPU time (abort)       " << mAbortTime   << "(MAX " << mAbortMax << ")" << endl
    << "  # of common dominator checkes " << mDomCheckCount << endl;
}

END_NAMESPACE_YM_SATPG

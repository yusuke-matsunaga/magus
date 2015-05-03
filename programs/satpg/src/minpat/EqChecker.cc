
/// @file EqChecker.cc
/// @brief EqChecker の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "EqChecker.h"

#include "FaultAnalyzer.h"

#include "TpgFault.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "Fsim.h"
#include "DetOp.h"
#include "NodeSet.h"

#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス EqChecker
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] analyzer 故障の情報を持つクラス
// @param[in] fsim 故障シミュレータ
// @param[in] tvmgr テストベクタのマネージャ
EqChecker::EqChecker(FaultAnalyzer& analyzer,
		     TvMgr& tvmgr,
		     Fsim& fsim) :
  mAnalyzer(analyzer),
  mTvMgr(tvmgr),
  mFsim(fsim)
{
  mVerbose = mAnalyzer.verbose();
  mMaxNodeId = mAnalyzer.max_node_id();
  mMaxFaultId = mAnalyzer.max_fault_id();
}

// @brief デストラクタ
EqChecker::~EqChecker()
{
}

// @brief verbose フラグを設定する．
// @param[in] verbose 表示を制御するフラグ
void
EqChecker::set_verbose(int verbose)
{
  mVerbose = verbose;
}

// @brief 等価故障の代表故障を求める．
void
EqChecker::get_rep_faults(const vector<const TpgFault*>& src_list,
			  vector<const TpgFault*>& rep_fault_list)
{
  StopWatch local_timer;
  local_timer.start();
  {
    vector<ymuint> elem_list;
    elem_list.reserve(src_list.size());
    for (ymuint i = 0; i < src_list.size(); ++ i) {
      elem_list.push_back(src_list[i]->id());
    }
    mEqSet.init(elem_list);
  }

  do_fsim(src_list);

  USTime fsim_time = local_timer.time();

  ymuint n_check = 0;
  ymuint n_success = 0;

  vector<bool> mark(mMaxFaultId, false);
  ymuint nc = mEqSet.class_num();
  for (ymuint i = 0; i < nc; ++ i) {
    vector<ymuint> elem_list;
    mEqSet.class_list(i, elem_list);
    ymuint n = elem_list.size();
    for (ymuint i1 = 0; i1 < n; ++ i1) {
      ymuint f1_id = elem_list[i1];
      if ( mark[f1_id] ) {
	continue;
      }

      if ( mVerbose > 1 ) {
	cout << "\rEQ:   "
	     << setw(6) << i << " / " << setw(6) << nc
	     << "  " << setw(6) << i1;
      }

      const FaultInfo& fi1 = mAnalyzer.fault_info(f1_id);
      const TpgFault* f1 = fi1.fault();
      rep_fault_list.push_back(f1);

      for (ymuint i2 = i1 + 1; i2 < n; ++ i2) {
	ymuint f2_id = elem_list[i2];
	if ( mark[f2_id] ) {
	  continue;
	}

	const FaultInfo& fi2 = mAnalyzer.fault_info(f2_id);
	const TpgFault* f2 = fi2.fault();
	++ n_check;
	if ( mAnalyzer.check_equivalence(f1, f2) ) {
	  mark[f2_id] = true;
	  ++ n_success;
	}
      }
    }
  }

  local_timer.stop();

  if ( mVerbose > 0 ) {
    if ( mVerbose > 1 ) {
      cout << endl;
    }
    cout << "# original faults:       " << setw(8) << src_list.size() << endl
	 << "# representative faults: " << setw(8) << rep_fault_list.size() << endl
	 << "  # equivalence checks:  " << setw(8) << n_check << endl
	 << "  # sucess:              " << setw(8) << n_success << endl
	 << "  # patterns simulated:  " << setw(8) << mPat << endl
	 << "CPU time:                " << local_timer.time() << endl
	 << "  CPU time (fsim)        " << fsim_time << endl;
    mAnalyzer.print_stats(cout);
  }
}

// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
// @param[in] fault_list 故障リスト
void
EqChecker::do_fsim(const vector<const TpgFault*>& fault_list)
{
  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);

  mFsim.set_faults(fault_list);

  DetOp op;

  ymuint nf = fault_list.size();
  ymuint npat = nf;
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* fault = fault_list[i];
    const FaultInfo& fi = mAnalyzer.fault_info(fault->id());
    TestVector* tv = fi.testvector();
    cur_array.push_back(tv);
    if ( cur_array.size() == kPvBitLen ) {
      if ( mVerbose > 1 ) {
	cout << "\rFSIM: " << setw(6) << i;
	cout.flush();
      }
      mFsim.ppsfp(cur_array, op);
      const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
      mEqSet.multi_refinement(det_list);
      cur_array.clear();
      op.clear_det_list();
    }
  }
  if ( !cur_array.empty() ) {
    mFsim.ppsfp(cur_array, op);
    const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
    mEqSet.multi_refinement(det_list);
    op.clear_det_list();
    cur_array.clear();
  }

  vector<TestVector*> cur_array2(kPvBitLen);
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    TestVector* tv = mTvMgr.new_vector();
    cur_array2[i] = tv;
  }

  for ( ; ; ) {
    npat += kPvBitLen;
    for (ymuint i = 0; i < kPvBitLen; ++ i) {
      cur_array2[i]->set_from_random(mRandGen);
    }
    mFsim.ppsfp(cur_array2, op);
    ymuint nchg = 0;
    const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
    if ( mEqSet.multi_refinement(det_list) ) {
      ++ nchg;
    }

    op.clear_det_list();
    if ( nchg == 0 ) {
      break;
    }
    if ( mVerbose > 1 ) {
      cout << "\rFSIM: " << setw(6) << npat;
      cout.flush();
    }
  }
  if ( mVerbose > 1 ) {
    cout << endl;
  }

  // 乱数パタンは削除しておく．
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    mTvMgr.delete_vector(cur_array2[i]);
  }

  mPat = npat;
}

#if 0
// @brief f1 と f2 が等価かどうか調べる．
bool
EqChecker::check_fault_equivalence(const TpgFault* f1,
				   const TpgFault* f2)
{
  const TpgNode* fnode1 = f1->node();
  const TpgNode* fnode2 = f2->node();
  const TpgNode* dom_node = common_node(fnode1, fnode2);

  if ( dom_node != NULL ) {
    ++ mDomCheckCount;

    StopWatch local_timer;
    local_timer.start();

    SatEngine engine(string(), string(), NULL);
    GvalCnf gval_cnf(mMaxNodeId);

    NodeSet node_set0;
    node_set0.mark_region(mMaxNodeId, dom_node);
    FvalCnf fval_cnf0(mMaxNodeId, gval_cnf);
    engine.make_fval_cnf(fval_cnf0, dom_node, node_set0, kVal1);

    NodeSet node_set1;
    node_set1.mark_region2(mMaxNodeId, fnode1, dom_node);
    FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
    engine.make_fval_cnf(fval_cnf1, f1, node_set1, kValX);

    NodeSet node_set2;
    node_set2.mark_region2(mMaxNodeId, fnode2, dom_node);
    FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);
    engine.make_fval_cnf(fval_cnf2, f2, node_set2, kValX);

    VarId dvar1 = fval_cnf1.dvar(dom_node);
    VarId dvar2 = fval_cnf2.dvar(dom_node);
    engine.add_diff_clause(dvar1, dvar2);

    Bool3 sat_stat = engine.check_sat();

    local_timer.stop();
    USTime time = local_timer.time();

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
    else {
      mFailureTime += time;
      if ( mFailureMax.usr_time_usec() < time.usr_time_usec() ) {
	if ( time.usr_time() > 1.0 ) {
	  cout << "SAT: " << f1 << ": " << f2 << "  " << time << endl;
	}
	mFailureMax = time;
      }

      return false;
    }
  }
  else {
    return check_fault_dominance(f1, f2) && check_fault_dominance(f2, f1);
  }
}

// @brief f1 が f2 を支配しているか調べる．
bool
EqChecker::check_fault_dominance(const TpgFault* f1,
				 const TpgFault* f2)
{
  StopWatch timer;
  timer.start();

  const FaultInfo& fi1 = mAnalyzer.fault_info(f1->id());
  const FaultInfo& fi2 = mAnalyzer.fault_info(f2->id());

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

  Bool3 sat_stat = engine.check_sat();
  if ( sat_stat == kB3False ) {
    goto end;
  }

  if ( dom_node != NULL ) {
    // 伝搬経路に共通な dominator がある時
    //++ mDomCheckCount;

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
      const NodeSet& node_set1 = mAnalyzer.node_set(f1->id());
      engine.make_fval_cnf(fval_cnf1, f1, node_set1, kVal1);

      sat_stat = engine.check_sat();
      if ( sat_stat == kB3False ) {
	goto end;
      }
    }

    if ( !fi2.single_cube() ) {
      // f2 を検出しない CNF を生成
      FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);
      const NodeSet& node_set2 = mAnalyzer.node_set(f2->id());
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
#endif

END_NAMESPACE_YM_SATPG

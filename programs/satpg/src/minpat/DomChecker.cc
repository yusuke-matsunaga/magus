
/// @file DomChecker.cc
/// @brief DomChecker の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DomChecker.h"

#include "FaultAnalyzer.h"

#include "TpgFault.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "Fsim.h"
#include "KDet2Op.h"
#include "DetOp.h"
#include "NodeSet.h"

#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"

#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

const bool verify_dom_check = false;

struct FaultLt
{
  FaultLt(DomChecker& checker) :
    mChecker(checker)
  {
  }

  bool
  operator()(ymuint left,
	     ymuint right)
  {
    return mChecker.dom_cand_size(left) < mChecker.dom_cand_size(right);
  }

  DomChecker& mChecker;

};

struct FaultLt2
{
  FaultLt2(DomChecker& checker) :
    mChecker(checker)
  {
  }

  bool
  operator()(ymuint left,
	     ymuint right)
  {
    return mChecker.dom_cand2_size(left) < mChecker.dom_cand2_size(right);
  }

  DomChecker& mChecker;

};

struct FaultGt
{
  FaultGt(DomChecker& checker) :
    mChecker(checker)
  {
  }

  bool
  operator()(ymuint left,
	     ymuint right)
  {
    return mChecker.dom_cand_size(left) > mChecker.dom_cand_size(right);
  }

  DomChecker& mChecker;

};

struct FaultGt2
{
  FaultGt2(DomChecker& checker) :
    mChecker(checker)
  {
  }

  bool
  operator()(ymuint left,
	     ymuint right)
  {
    return mChecker.dom_cand2_size(left) > mChecker.dom_cand2_size(right);
  }

  DomChecker& mChecker;

};

// 2つのリストが共通要素を持つとき true を返す．
// リストはソートされていると仮定する．
inline
bool
check_intersect(const vector<ymuint>& list1,
		const vector<ymuint>& list2)
{
  ymuint n1 = list1.size();
  ymuint n2 = list2.size();
  ymuint i1 = 0;
  ymuint i2 = 0;
  ymuint v1 = list1[i1];
  ymuint v2 = list2[i2];
  for ( ; ; ) {
    if ( v1 < v2 ) {
      ++ i1;
      if ( i1 >= n1 ) {
	return false;
      }
      v1 = list1[i1];
    }
    else if ( v1 > v2 ) {
      ++ i2;
      if ( i2 >= n2 ) {
	return false;
      }
      v2 = list2[i2];
    }
    else {
      return true;
    }
  }
  return false;
}

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


//////////////////////////////////////////////////////////////////////
// クラス DomChecker
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] analyzer 故障の情報を持つクラス
// @param[in] fsim 故障シミュレータ
// @param[in] tvmgr テストベクタのマネージャ
DomChecker::DomChecker(FaultAnalyzer& analyzer,
		       TvMgr& tvmgr,
		       Fsim& fsim) :
  mAnalyzer(analyzer),
  mTvMgr(tvmgr),
  mFsim(fsim)
{
  mVerbose = mAnalyzer.verbose();
  mMaxNodeId = mAnalyzer.max_node_id();
  mMaxFaultId = mAnalyzer.max_fault_id();
  mFaultDataArray.resize(mMaxFaultId);
  mDetFlag.resize(mMaxFaultId, 0UL);

  for (ymuint i = 0; i < mMaxFaultId; ++ i) {
    mFaultDataArray[i].mDetCount = 0;
  }
}

// @brief デストラクタ
DomChecker::~DomChecker()
{
}

// @brief verbose フラグを設定する．
// @param[in] verbose 表示を制御するフラグ
void
DomChecker::set_verbose(int verbose)
{
  mVerbose = verbose;
}

// @brief 支配故障を求める．
//
// 結果は mDomFaultList に格納される．
void
DomChecker::get_dom_faults(ymuint method,
			   const vector<const TpgFault*>& src_list,
			   vector<const TpgFault*>& dom_fault_list)
{
  {
    vector<ymuint> elem_list;
    elem_list.reserve(src_list.size());
    for (ymuint i = 0; i < src_list.size(); ++ i) {
      elem_list.push_back(src_list[i]->id());
    }
    mEqSet.init(elem_list);
  }

  mDomCheckCount = 0;

  do_fsim1(src_list);

  vector<const TpgFault*> rep_fault_list;
  get_rep_faults(src_list, rep_fault_list);

  mDomCheckCount = 0;

  do_fsim2(rep_fault_list);

  switch ( method ) {
  case 1: get_dom_faults1(rep_fault_list, dom_fault_list); break;
  case 2: get_dom_faults2(0, rep_fault_list, dom_fault_list); break;
  case 3: get_dom_faults2(1, rep_fault_list, dom_fault_list); break;
  case 4: get_dom_faults2(2, rep_fault_list, dom_fault_list); break;
  case 5: get_dom_faults2(3, rep_fault_list, dom_fault_list); break;
  default: ASSERT_NOT_REACHED;
  }
}

// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
// @param[in] fault_list 故障リスト
void
DomChecker::do_fsim1(const vector<const TpgFault*>& fault_list)
{
  StopWatch local_timer;
  local_timer.start();

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
	cout << "\r " << i;
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
      cout << "\r " << npat;
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

  local_timer.stop();

  if ( mVerbose ) {
    cout << "CPU time (fault simulation1)  " << local_timer.time() << endl
	 << "Total " << npat << " patterns simulated" << endl;
  }
}

// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
// @param[in] fault_list 故障リスト
void
DomChecker::do_fsim2(const vector<const TpgFault*>& fault_list)
{
  StopWatch local_timer;
  local_timer.start();

  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);

  DetOp op;

  ymuint nf = fault_list.size();
  ymuint npat = nf;
  ymuint base = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* fault = fault_list[i];
    const FaultInfo& fi = mAnalyzer.fault_info(fault->id());
    TestVector* tv = fi.testvector();
    cur_array.push_back(tv);
    if ( cur_array.size() == kPvBitLen ) {
      if ( mVerbose > 1 ) {
	cout << "\r " << base;
	cout.flush();
      }
      mFsim.ppsfp(cur_array, op);
      const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
      record_pat(det_list);
      cur_array.clear();
      op.clear_det_list();
      base += kPvBitLen;
    }
  }
  if ( !cur_array.empty() ) {
    mFsim.ppsfp(cur_array, op);
    const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
    record_pat(det_list);
    op.clear_det_list();
    base += cur_array.size();
    cur_array.clear();
  }

  vector<TestVector*> cur_array2(kPvBitLen);
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    TestVector* tv = mTvMgr.new_vector();
    cur_array2[i] = tv;
  }

  for ( ; ; ) {
    for (ymuint i = 0; i < kPvBitLen; ++ i) {
      cur_array2[i]->set_from_random(mRandGen);
    }
    mFsim.ppsfp(cur_array2, op);
    ymuint nchg = 0;
    const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
    nchg += record_pat(det_list);
    op.clear_det_list();
    base += kPvBitLen;
    if ( nchg == 0 ) {
      break;
    }
    if ( mVerbose > 1 ) {
      cout << "\r " << base;
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

  // 構造的に独立な故障対を対象外にする．
  for (ymuint i1 = 0; i1 < nf; ++ i1) {
    const TpgFault* f1 = fault_list[i1];
    FaultData& fd1 = mFaultDataArray[f1->id()];
    const vector<ymuint>& input_list1 = mAnalyzer.input_list(f1->id());

    vector<ymuint>& dst_list = fd1.mDomCandList;
    ymuint wpos = 0;
    for (ymuint rpos = 0; rpos < dst_list.size(); ++ rpos) {
      ymuint f2_id = dst_list[rpos];
      const vector<ymuint>& input_list2 = mAnalyzer.input_list(f2_id);
      bool intersect = check_intersect(input_list1, input_list2);
      if ( !intersect ) {
	// 共通部分を持たない故障は独立
	continue;
      }
      if ( wpos < rpos ) {
	dst_list[wpos] = f2_id;
      }
      ++ wpos;
    }
    if ( wpos < dst_list.size() ) {
      dst_list.erase(dst_list.begin() + wpos, dst_list.end());
    }
  }

  local_timer.stop();

  if ( mVerbose ) {
    cout << "CPU time (fault simulation2)  " << local_timer.time() << endl
	 << "Total " << base << " patterns simulated" << endl;
  }
}

ymuint
DomChecker::record_pat(const vector<pair<ymuint, PackedVal> >& det_list)
{
  ymuint n = det_list.size();
  ymuint nchg = 0;

  vector<pair<ymuint, PackedVal> > det_list_array[kPvBitLen];
  for (ymuint i = 0; i < n; ++ i) {
    ymuint f_id = det_list[i].first;
    PackedVal bv = det_list[i].second;
    ymuint b = count_ones(bv);
    ASSERT_COND( b > 0 );
    det_list_array[b - 1].push_back(det_list[i]);
    mDetFlag[f_id] = bv;
  }

  // 検出結果を用いて支配される故障の候補リストを作る．
  for (ymuint i1 = 0; i1 < n; ++ i1) {
    ymuint f1_id = det_list[i1].first;
    PackedVal bv1 = det_list[i1].second;
    FaultData& fd1 = mFaultDataArray[f1_id];
    vector<ymuint>& dst_list = fd1.mDomCandList;
    if ( fd1.mDetCount == 0 ) {
      // 初めて検出された場合
      ymuint b1 = count_ones(bv1);
      for (ymuint b = b1; b < kPvBitLen; ++ b) {
	const vector<pair<ymuint, PackedVal> >& tmp_list = det_list_array[b - 1];
	ymuint n2 = tmp_list.size();
	for (ymuint i2 = 0; i2 < n2; ++ i2) {
	  ymuint f2_id = tmp_list[i2].first;
	  if ( f2_id == f1_id ) {
	    continue;
	  }
	  PackedVal bv2 = tmp_list[i2].second;
	  if ( (bv1 & bv2) == bv1 ) {
	    // bv1 が 1 の部分は bv2 も 1
	    dst_list.push_back(f2_id);
	  }
	}
      }
    }
    else {
      // 二回目以降
      // dst_list と src_list の intersection を取る．
      ymuint n = dst_list.size();
      ymuint wpos = 0;
      for (ymuint rpos = 0; rpos < n; ++ rpos) {
	ymuint f2_id = dst_list[rpos];
	PackedVal bv2 = mDetFlag[f2_id];
	if ( (bv1 & bv2) == bv1 ) {
	  if ( wpos < rpos ) {
	    dst_list[wpos] = f2_id;
	  }
	  ++ wpos;
	}
      }
      if ( wpos < dst_list.size() ) {
	nchg += dst_list.size() - wpos;
	dst_list.erase(dst_list.begin() + wpos, dst_list.end());
      }
    }
    fd1.mDetCount += count_ones(bv1);
  }

  for (ymuint i1 = 0; i1 < n; ++ i1) {
    ymuint f1_id = det_list[i1].first;
    mDetFlag[f1_id] = 0UL;
  }

  return nchg;
}

// @brief 等価故障の代表故障を求める．
void
DomChecker::get_rep_faults(const vector<const TpgFault*>& src_list,
			   vector<const TpgFault*>& rep_fault_list)
{
  StopWatch local_timer;
  local_timer.start();

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
	cout << "\r"
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
	if ( check_fault_equivalence(f1, f2) ) {
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
    cout << src_list.size() << " ==> " << rep_fault_list.size()
	 << ": " << local_timer.time() << endl;
    cout << "  " << n_check << " checkes" << endl
	 << "  " << n_success << " success" << endl;
    cout << "  CPU time (success) " << mSuccessTime << "(MAX " << mSuccessMax << ")" << endl
	 << "  CPU time (failure) " << mFailureTime << "(MAX " << mFailureMax << ")" << endl
	 << "  CPU time (abort)   " << mAbortTime   << "(MAX " << mAbortMax << ")" << endl;
  }
}

// @brief 支配故障を求める．
//
// 結果は mDomFaultList に格納される．
void
DomChecker::get_dom_faults1(const vector<const TpgFault*>& src_list,
			    vector<const TpgFault*>& dom_fault_list)
{
  StopWatch local_timer;

  local_timer.start();

  vector<bool> dom_flag(mMaxFaultId, false);

  ymuint fault_num = src_list.size();

  ymuint n_sat2 = 0;
  ymuint n_dom2 = 0;

  ymuint n_sat1 = 0;
  ymuint n_nodom = 0;

  vector<ymuint> pending_f1_list;
  vector<vector<ymuint> > pending_list_array(mMaxFaultId);

  // 故障を支配故障候補数の少ない順に並べる．
  vector<ymuint> fault_list(fault_num);
  for (ymuint i = 0; i < fault_num; ++ i) {
    fault_list[i] = src_list[i]->id();
  }

  FaultLt fault_lt(*this);
  sort(fault_list.begin(), fault_list.end(), fault_lt);

  ymuint cur_num = fault_num;

  // シミュレーション結果から故障の支配関係のヒントを作り，
  // SAT で正確に判定する．
  for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
    ymuint f1_id = fault_list[i1];
    if ( dom_flag[f1_id] ) {
      continue;
    }

    FaultData& fd1 = mFaultDataArray[f1_id];
    const vector<ymuint>& cand_list = fd1.mDomCandList;
    if ( cand_list.empty() ) {
      continue;
    }

    if ( mVerbose > 1 ) {
      cout << "\r" << setw(6) << i1 << " / " << setw(6) << fault_num
	   << " / " << setw(6) << cur_num;
      cout.flush();
    }

    const FaultInfo& fi1 = mAnalyzer.fault_info(f1_id);
    const TpgFault* f1 = fi1.fault();
    const vector<ymuint>& input_list1_2 = mAnalyzer.input_list2(f1_id);

    bool pending = false;
    for (ymuint i2 = 0; i2 < cand_list.size(); ++ i2) {
      ymuint f2_id = cand_list[i2];
      if ( dom_flag[f2_id] ) {
	continue;
      }

      const vector<ymuint>& input_list2_2 = mAnalyzer.input_list2(f2_id);

      bool intersect2 = check_intersect(input_list1_2, input_list2_2);
      if ( !intersect2 ) {
	// TFI が共通部分を持たない場合は望みが薄いので後回し．
	pending_list_array[f1_id].push_back(f2_id);
	if ( !pending ) {
	  pending = true;
	  pending_f1_list.push_back(f1_id);
	}
	continue;
      }

      const TpgFault* f2 = mAnalyzer.fault(f2_id);

      ++ n_sat1;
      GvalCnf gval_cnf(mMaxNodeId);
      FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
      SatEngine engine(string(), string(), NULL);
      // f2 を検出しない CNF を作成
      engine.make_fval_cnf(fval_cnf, f2, mAnalyzer.node_set(f2_id), kVal0);

      // これが f1 の十分割当のもとで成り立ったら支配しない
      if ( engine.check_sat(gval_cnf, fi1.sufficient_assignment()) == kB3True ) {
	++ n_nodom;
	continue;
      }

      // 実際にチェックを行う．
      ++ n_sat2;
      if ( check_fault_dominance(f1, f2) ) {
	//cout << f2->str() << " is dominated by " << f1->str() << endl;
	dom_flag[f2_id] = true;
	++ n_dom2;
	-- cur_num;
	continue;
      }
    }
  }
  vector<ymuint> fault_list2;
  fault_list2.reserve(fault_num);
  for (ymuint i = 0; i < fault_num; ++ i) {
    ymuint f_id = fault_list[i];
    if ( !dom_flag[f_id] ) {
      fault_list2.push_back(f_id);
    }
  }
  ymuint fault_num2 = fault_list2.size();
  if ( mVerbose ) {
    cout << " --> " << setw(6) << fault_num2 << ": " << setw(6) << n_dom2 << " / " << setw(6) << n_sat2
	 << ": Lap CPU time " << local_timer.time() << endl;
  }

  ymuint n_sat3 = 0;
  ymuint n_dom3 = 0;

  // シミュレーション結果から故障の支配関係のヒントを作り，
  // SAT で正確に判定する．
  for (ymuint i1 = 0; i1 < pending_f1_list.size(); ++ i1) {
    ymuint f1_id = pending_f1_list[i1];
    if ( dom_flag[f1_id] ) {
      continue;
    }
    if ( mVerbose > 1 ) {
      cout << "\r                  ";
      cout << "\r" << setw(6) << i1 << " / " << setw(6) << pending_f1_list.size()
	   << " / " << setw(6) << cur_num;
      cout.flush();
    }

    const FaultInfo& fi1 = mAnalyzer.fault_info(f1_id);
    const TpgFault* f1 = fi1.fault();

    const vector<ymuint>& f_list = pending_list_array[f1_id];
    for (ymuint i2 = 0; i2 < f_list.size(); ++ i2) {
      ymuint f2_id = f_list[i2];
      if ( dom_flag[f2_id] ) {
	continue;
      }

      const TpgFault* f2 = mAnalyzer.fault(f2_id);

      ++ n_sat1;
      GvalCnf gval_cnf(mMaxNodeId);
      FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
      SatEngine engine(string(), string(), NULL);
      // f2 を検出しない CNF を作成
      engine.make_fval_cnf(fval_cnf, f2, mAnalyzer.node_set(f2_id), kVal0);

      // これが f1 の十分割当のもとで成り立ったら支配しない
      if ( engine.check_sat(gval_cnf, fi1.sufficient_assignment()) == kB3True ) {
	++ n_nodom;
	continue;
      }

      ++ n_sat3;
      if ( check_fault_dominance(f1, f2) ) {
	//cout << f2->str() << " is dominated by " << f1->str() << endl;
	dom_flag[f2_id] = true;
	++ n_dom3;
	-- cur_num;
	continue;
      }
    }
  }

  ymuint n_single = 0;
  ymuint n_double = 0;
  ymuint n_triple = 0;
  // 支配されていない故障を dom_fault_list に入れる．
  dom_fault_list.clear();
  dom_fault_list.reserve(fault_num2);
  for (ymuint i = 0; i < fault_num2; ++ i) {
    ymuint f_id = fault_list2[i];
    if ( !dom_flag[f_id] ) {
      const TpgFault* fault = mAnalyzer.fault(f_id);
      dom_fault_list.push_back(fault);
      const FaultInfo& fi = mAnalyzer.fault_info(f_id);
      if ( fi.single_cube() ) {
	++ n_single;
      }
      else {
	ymuint n = fi.other_sufficient_assignment_num();
	if ( n == 1 ) {
	  ++ n_double;
	}
	else if ( n == 2 ) {
	  ++ n_triple;
	}
      }
    }
  }

  ymuint dom_fault_num = dom_fault_list.size();
  local_timer.stop();
  if ( mVerbose ) {
    cout << " --> " << setw(6) << dom_fault_num << ": " << setw(6) << n_dom3 << " / " << setw(6) << n_sat3 << endl;
    cout << "Total    " << fault_num << " original faults" << endl;
    cout << "Total    " << dom_fault_num << " dominator faults" << endl;
    cout << "Total    " << n_single << " single cube faults" << endl;
    cout << "Total    " << n_double << " double cube faults" << endl;
    cout << "Total    " << n_triple << " triple cube faults" << endl;
    cout << "Total    " << n_sat1 << " simple non-dominance test" << endl;
    cout << "Total    " << n_nodom  << " non-dominance" << endl;
    cout << "Total    " << n_sat2 + n_sat3 << " dominance test" << endl;
    cout << "CPU time for dominance test" << local_timer.time() << endl;
  }
}

// @brief 支配故障を求める．
//
// 結果は dom_fault_list に格納される．
void
DomChecker::get_dom_faults2(ymuint option,
			    const vector<const TpgFault*>& src_list,
			    vector<const TpgFault*>& dom_fault_list)
{
  StopWatch local_timer;

  local_timer.start();

  vector<bool> dom_flag(mMaxFaultId, false);

  ymuint fault_num = src_list.size();

  ymuint n_sat2 = 0;
  ymuint n_dom2 = 0;

  ymuint n_sat1 = 0;
  ymuint n_nodom = 0;

  vector<ymuint> pending_f1_list;
  vector<vector<ymuint> > pending_list_array(mMaxFaultId);

  vector<ymuint> fault_list(fault_num);
  for (ymuint i = 0; i < fault_num; ++ i) {
    fault_list[i] = src_list[i]->id();
  }

  // 故障を被支配故障候補数の多い順に並べる．
  {
    FaultGt comp(*this);
    sort(fault_list.begin(), fault_list.end(), comp);
  }

#if 1
  // 非支配故障の候補から支配故障の候補を作る．
  for (ymuint i = 0; i < fault_num; ++ i) {
    ymuint f1_id = fault_list[i];
    FaultData& fd1 = mFaultDataArray[f1_id];
    const vector<ymuint>& cand_list = fd1.mDomCandList;
    for (ymuint j = 0; j < cand_list.size(); ++ j) {
      ymuint f2_id = cand_list[j];
      FaultData& fd2 = mFaultDataArray[f2_id];
      fd2.mDomCandList2.push_back(f1_id);
    }
  }
#endif

  if ( option == 0 ) {
    // 故障を被支配故障数の少ない順に並べる．
    FaultLt comp(*this);
    sort(fault_list.begin(), fault_list.end(), comp);
  }
  else if ( option == 1 ) {
    // 故障を被支配故障数の多い順に並べる．
    FaultGt comp(*this);
    sort(fault_list.begin(), fault_list.end(), comp);
  }
  else if ( option == 2 ) {
    // 故障を支配故障数の少ない順に並べる．
    FaultLt2 comp(*this);
    sort(fault_list.begin(), fault_list.end(), comp);
  }
  else if ( option == 3 ) {
    // 故障を支配故障数の多い順に並べる．
    FaultGt2 comp(*this);
    sort(fault_list.begin(), fault_list.end(), comp);
  }
  else if ( option == 4 ) {
    // オリジナル順
  }

  ymuint cur_num = fault_num;

  // シミュレーション結果から故障の支配関係のヒントを作り，
  // SAT で正確に判定する．
  for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
    ymuint f1_id = fault_list[i1];
    if ( dom_flag[f1_id] ) {
      continue;
    }

    FaultData& fd1 = mFaultDataArray[f1_id];
    const vector<ymuint>& cand_list = fd1.mDomCandList2;
    if ( cand_list.empty() ) {
      continue;
    }

    if ( mVerbose > 1 ) {
      cout << "\r                  ";
      cout << "\r" << setw(6) << i1 << " / " << setw(6) << fault_num
	   << " / " << setw(6) << cur_num;
      cout.flush();
    }

    const FaultInfo& fi1 = mAnalyzer.fault_info(f1_id);
    const TpgFault* f1 = fi1.fault();
    const vector<ymuint>& input_list1_2 = mAnalyzer.input_list2(f1_id);

    SatEngine engine(string(), string(), NULL);
    GvalCnf gval_cnf(mMaxNodeId);

    if ( fi1.single_cube() ) {
      // f1 を検出しない CNF を作成
      engine.add_negation(gval_cnf, fi1.sufficient_assignment());
    }
    else {
      FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
      // f1 を検出しない CNF を作成
      engine.make_fval_cnf(fval_cnf, f1, mAnalyzer.node_set(f1_id), kVal0);
    }

    bool pending = false;
    for (ymuint i2 = 0; i2 < cand_list.size(); ++ i2) {
      ymuint f2_id = cand_list[i2];
      if ( dom_flag[f2_id] ) {
	continue;
      }

      const FaultInfo& fi2 = mAnalyzer.fault_info(f2_id);
      const TpgFault* f2 = fi2.fault();
      const vector<ymuint>& input_list2_2 = mAnalyzer.input_list2(f2_id);

      bool intersect2 = check_intersect(input_list1_2, input_list2_2);
      if ( !intersect2 ) {
	// TFI が共通部分を持たない場合は望みが薄いので後回し．
	pending_list_array[f1_id].push_back(f2_id);
	if ( !pending ) {
	  pending = true;
	  pending_f1_list.push_back(f1_id);
	}
	continue;
      }

      ++ n_sat1;
      // これが f2 の十分割当のもとで成り立ったら支配しない
      if ( engine.check_sat(gval_cnf, fi2.sufficient_assignment()) == kB3True ) {
	++ n_nodom;
	continue;
      }
      if ( fi2.single_cube() ) {
	// これ以上のチェックは必要ない．
	dom_flag[f1_id] = true;
	++ n_dom2;
	-- cur_num;
	break;
      }

      // 実際にチェックを行う．
      ++ n_sat2;
      if ( check_fault_dominance(f2, f1) ) {
	dom_flag[f1_id] = true;
	++ n_dom2;
	-- cur_num;
	break;
      }
    }
  }

  vector<ymuint> fault_list2;
  fault_list2.reserve(fault_num);
  for (ymuint i = 0; i < fault_num; ++ i) {
    ymuint f_id = fault_list[i];
    if ( !dom_flag[f_id] ) {
      fault_list2.push_back(f_id);
    }
  }
  ymuint fault_num2 = fault_list2.size();
  if ( mVerbose ) {
    cout << " --> " << setw(6) << fault_num2 << ": " << setw(6) << n_dom2 << " / " << setw(6) << n_sat2
	 << ": Lap CPU time " << local_timer.time() << endl;
  }

  ymuint n_sat3 = 0;
  ymuint n_dom3 = 0;

  // シミュレーション結果から故障の支配関係のヒントを作り，
  // SAT で正確に判定する．
  for (ymuint i1 = 0; i1 < pending_f1_list.size(); ++ i1) {
    ymuint f1_id = pending_f1_list[i1];
    if ( dom_flag[f1_id] ) {
      continue;
    }
    if ( mVerbose > 1 ) {
      cout << "\r                  ";
      cout << "\r" << setw(6) << i1 << " / " << setw(6) << pending_f1_list.size()
	   << " / " << setw(6) << cur_num;
      cout.flush();
    }

    const TpgFault* f1 = mAnalyzer.fault(f1_id);

    GvalCnf gval_cnf(mMaxNodeId);
    FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
    SatEngine engine(string(), string(), NULL);

    // f1 を検出しない CNF を作成
    engine.make_fval_cnf(fval_cnf, f1, mAnalyzer.node_set(f1_id), kVal0);

    const vector<ymuint>& f_list = pending_list_array[f1_id];
    for (ymuint i2 = 0; i2 < f_list.size(); ++ i2) {
      ymuint f2_id = f_list[i2];
      if ( dom_flag[f2_id] ) {
	continue;
      }

      const FaultInfo& fi2 = mAnalyzer.fault_info(f2_id);
      const TpgFault* f2 = fi2.fault();

      ++ n_sat1;
      // これが f2 の十分割当のもとで成り立ったら支配しない
      if ( engine.check_sat(gval_cnf, fi2.sufficient_assignment()) == kB3True ) {
	++ n_nodom;
	continue;
      }
      if ( fi2.single_cube() ) {
	// これ以上のチェックは必要ない．
	dom_flag[f1_id] = true;
	++ n_dom2;
	-- cur_num;
	break;
      }

      ++ n_sat3;
      if ( check_fault_dominance(f2, f1) ) {
	dom_flag[f1_id] = true;
	++ n_dom3;
	-- cur_num;
	break;
      }
    }
  }

  ymuint n_single = 0;
  ymuint n_double = 0;
  ymuint n_triple = 0;
  // 支配されていない故障を dom_fault_list に入れる．
  dom_fault_list.clear();
  dom_fault_list.reserve(fault_num2);
  for (ymuint i = 0; i < fault_num2; ++ i) {
    ymuint f_id = fault_list2[i];
    if ( dom_flag[f_id] ) {
      mAnalyzer.clear_fault_info(f_id);
    }
    else {
      const TpgFault* fault = mAnalyzer.fault(f_id);
      dom_fault_list.push_back(fault);
      const FaultInfo& fi = mAnalyzer.fault_info(f_id);
      if ( fi.single_cube() ) {
	++ n_single;
      }
      else {
	ymuint n = fi.other_sufficient_assignment_num();
	if ( n == 1 ) {
	  ++ n_double;
	}
	else if ( n == 2 ) {
	  ++ n_triple;
	}
      }
    }
  }

  ymuint dom_fault_num = dom_fault_list.size();
  local_timer.stop();
  if ( mVerbose ) {
    cout << " --> " << setw(6) << dom_fault_num << ": " << setw(6) << n_dom3 << " / " << setw(6) << n_sat3 << endl;
    cout << "Total    " << fault_num << " original faults" << endl;
    cout << "Total    " << dom_fault_num << " dominator faults" << endl;
    cout << "Total    " << n_single << " single cube faults" << endl;
    cout << "Total    " << n_double << " double cube faults" << endl;
    cout << "Total    " << n_triple << " triple cube faults" << endl;
    cout << "Total    " << n_sat1 << " simple non-dominance test" << endl;
    cout << "Total    " << n_nodom  << " non-dominance" << endl;
    cout << "Total    " << n_sat2 + n_sat3 << " dominance test" << endl;
    cout << "CPU time for dominance test" << local_timer.time() << endl;
    cout << "  CPU time (success) " << mSuccessTime << "(MAX " << mSuccessMax << ")" << endl
	 << "  CPU time (failure) " << mFailureTime << "(MAX " << mFailureMax << ")" << endl
	 << "  CPU time (abort)   " << mAbortTime   << "(MAX " << mAbortMax << ")" << endl;
    cout << "         " << mDomCheckCount << " smart dom check" << endl;
  }
}

// @brief f1 が f2 を支配しているか調べる．
bool
DomChecker::check_fault_dominance(const TpgFault* f1,
				  const TpgFault* f2)
{
  const TpgNode* fnode1 = f1->node();
  const TpgNode* fnode2 = f2->node();
  const TpgNode* dom_node = common_node(fnode1, fnode2);

  if ( dom_node != NULL ) {
    ++ mDomCheckCount;

    SatEngine engine(string(), string(), NULL);
    GvalCnf gval_cnf(mMaxNodeId);

    NodeSet node_set0;
    node_set0.mark_region(mMaxNodeId, dom_node);

    NodeSet node_set1;
    node_set1.mark_region2(mMaxNodeId, fnode1, dom_node);

    NodeSet node_set2;
    node_set2.mark_region2(mMaxNodeId, fnode2, dom_node);

    FvalCnf fval_cnf0(mMaxNodeId, gval_cnf);
    FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
    FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);

    engine.make_fval_cnf2(fval_cnf0, fval_cnf1, fval_cnf2,
			  dom_node, f1, f2,
			  node_set0, node_set1, node_set2);

    Literal dlit1(fval_cnf1.dvar(dom_node));
    Literal dlit2(fval_cnf2.dvar(dom_node));

    vector<Literal> assumption(2);
    assumption[0] = dlit1;
    assumption[1] = ~dlit2;
    bool ans = ( engine.check_sat(assumption) == kB3False );
    if ( verify_dom_check ) {
      SatEngine engine(string(), string(), NULL);
      GvalCnf gval_cnf(mMaxNodeId);
      FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
      const NodeSet& _node_set1 = mAnalyzer.node_set(f1->id());
      engine.make_fval_cnf(fval_cnf1, f1, _node_set1, kVal1);

      const FaultInfo& fi2 = mAnalyzer.fault_info(f2->id());
      if ( false && fi2.single_cube() ) {
	// f2 を検出しない CNF を生成
	engine.add_negation(gval_cnf, fi2.sufficient_assignment());
      }
      else {
	FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);
	const NodeSet& node_set2 = mAnalyzer.node_set(f2->id());
	// f1 を検出して f2 を検出しない CNF を生成
	engine.make_fval_cnf(fval_cnf2, f2, node_set2, kVal0);
      }
      Bool3 sat_stat = engine.check_sat();
      bool ans2 = ( sat_stat == kB3False );
      if ( ans != ans2 ) {
	cout << endl;
	cout << "Error" << endl;
	cout << f1 << " " << f2 << endl;
	cout << " ans1 = " << ans << endl
	     << " ans2 = " << ans2 << endl;
	{
	  cout << "dom_node = " << dom_node->id() << endl;
	  cout << "NodeSet0" << endl;
	  for (ymuint i = 0; i < node_set0.tfo_size(); ++ i) {
	    cout << " " << node_set0.tfo_tfi_node(i)->id();
	  }
	  cout << endl;
	  cout << "NodeSet1" << endl;
	  for (ymuint i = 0; i < node_set1.tfo_size(); ++ i) {
	    cout << " " << node_set1.tfo_tfi_node(i)->id();
	  }
	  cout << endl;
	  cout << "NodeSet2" << endl;
	  for (ymuint i = 0; i < node_set2.tfo_size(); ++ i) {
	    cout << " " << node_set2.tfo_tfi_node(i)->id();
	  }
	  cout << endl;
	}
      }
    }

    return ans;
  }

  StopWatch timer;
  timer.start();

  SatEngine engine(string(), string(), NULL);
  GvalCnf gval_cnf(mMaxNodeId);
  FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
  const NodeSet& node_set1 = mAnalyzer.node_set(f1->id());
  engine.make_fval_cnf(fval_cnf1, f1, node_set1, kVal1);

  const FaultInfo& fi2 = mAnalyzer.fault_info(f2->id());
  if ( fi2.single_cube() ) {
    // f2 を検出しない CNF を生成
    engine.add_negation(gval_cnf, fi2.sufficient_assignment());
  }
  else {
    FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);
    const NodeSet& node_set2 = mAnalyzer.node_set(f2->id());
    // f1 を検出して f2 を検出しない CNF を生成
    engine.make_fval_cnf(fval_cnf2, f2, node_set2, kVal0);
  }
  Bool3 ans = engine.check_sat();
  timer.stop();
  USTime time = timer.time();
  if ( ans == kB3False ) {
    mSuccessTime += time;
    if ( mSuccessMax.usr_time_usec() < time.usr_time_usec() ) {
      mSuccessMax = time;
    }
    return true;
  }
  else if ( ans == kB3True ) {
    mFailureTime += time;
    if ( mFailureMax.usr_time_usec() < time.usr_time_usec() ) {
      mFailureMax = time;
    }
    return false;
  }
  else {
    mAbortTime += timer.time();
    if ( mAbortMax.usr_time_usec() < time.usr_time_usec() ) {
      mAbortMax = time;
    }
    return false;
  }
}

// @brief f1 と f2 が等価かどうか調べる．
bool
DomChecker::check_fault_equivalence(const TpgFault* f1,
				    const TpgFault* f2)
{
  const TpgNode* fnode1 = f1->node();
  const TpgNode* fnode2 = f2->node();
  const TpgNode* dom_node = common_node(fnode1, fnode2);

  if ( dom_node != NULL ) {
    ++ mDomCheckCount;

    SatEngine engine(string(), string(), NULL);
    GvalCnf gval_cnf(mMaxNodeId);

    NodeSet node_set0;
    node_set0.mark_region(mMaxNodeId, dom_node);

    NodeSet node_set1;
    node_set1.mark_region2(mMaxNodeId, fnode1, dom_node);

    NodeSet node_set2;
    node_set2.mark_region2(mMaxNodeId, fnode2, dom_node);

    FvalCnf fval_cnf0(mMaxNodeId, gval_cnf);
    FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
    FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);

    engine.make_fval_cnf2(fval_cnf0, fval_cnf1, fval_cnf2,
			  dom_node, f1, f2,
			  node_set0, node_set1, node_set2);

    Literal dlit1(fval_cnf1.dvar(dom_node));
    Literal dlit2(fval_cnf2.dvar(dom_node));

    vector<Literal> assumption(2);
    assumption[0] = dlit1;
    assumption[1] = ~dlit2;
    if ( engine.check_sat(assumption) != kB3False ) {
      return false;
    }
    assumption[0] = ~dlit1;
    assumption[1] = dlit2;
    if ( engine.check_sat(assumption) != kB3False ) {
      return false;
    }
    return true;
  }
  else {
    return check_fault_dominance2(f1, f2) && check_fault_dominance2(f2, f1);
  }
}

// @brief f1 が f2 を支配しているか調べる．
bool
DomChecker::check_fault_dominance2(const TpgFault* f1,
				   const TpgFault* f2)
{
  StopWatch timer;
  timer.start();

  SatEngine engine(string(), string(), NULL);
  GvalCnf gval_cnf(mMaxNodeId);

  const FaultInfo& fi1 = mAnalyzer.fault_info(f1->id());
  const FaultInfo& fi2 = mAnalyzer.fault_info(f2->id());

  if ( fi2.single_cube() ) {
    // f2 を検出しない CNF を生成
    engine.add_negation(gval_cnf, fi2.sufficient_assignment());
  }
  else {
    FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);
    const NodeSet& node_set2 = mAnalyzer.node_set(f2->id());
    // f2 を検出しない CNF を生成
    engine.make_fval_cnf(fval_cnf2, f2, node_set2, kVal0);
  }

  if ( engine.check_sat(gval_cnf, fi1.mandatory_assignment()) == kB3False ) {
    return true;
  }

  if ( fi1.single_cube() ) {
    // sufficient_assignment() == mandatory_assignment() なので答は出ている．
    return false;
  }

  FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
  const NodeSet& node_set1 = mAnalyzer.node_set(f1->id());
  // f1 を検出する CNF を生成
  engine.make_fval_cnf(fval_cnf1, f1, node_set1, kVal1);

  Bool3 sat_stat = engine.check_sat();

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

// @brief シミュレーション時の検出パタン数を返す．
ymuint
DomChecker::det_count(ymuint f_id)
{
  ASSERT_COND( f_id < mMaxFaultId );
  return mFaultDataArray[f_id].mDetCount;
}

// @brief 非支配故障候補数を返す．
ymuint
DomChecker::dom_cand_size(ymuint f_id)
{
  ASSERT_COND( f_id < mMaxFaultId );
  return mFaultDataArray[f_id].mDomCandList.size();
}

// @brief 支配故障候補数を返す．
ymuint
DomChecker::dom_cand2_size(ymuint f_id)
{
  ASSERT_COND( f_id < mMaxFaultId );
  return mFaultDataArray[f_id].mDomCandList2.size();
}

END_NAMESPACE_YM_SATPG

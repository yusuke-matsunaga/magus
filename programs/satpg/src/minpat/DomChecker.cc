
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
#include "NodeSet.h"

#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"

#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

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
  mDetFlag.resize(mMaxFaultId, false);

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
  get_pat_list(src_list);

  vector<const TpgFault*> rep_fault_list;
  get_rep_faults(src_list, rep_fault_list);

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
DomChecker::get_pat_list(const vector<const TpgFault*>& fault_list)
{
  StopWatch local_timer;
  local_timer.start();

  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);

  KDet2Op op(mFsim, fault_list);

  ymuint nf = fault_list.size();
  mEqClassList.push_back(vector<ymuint>());
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* fault = fault_list[i];
    mEqClassList[0].push_back(fault->id());
  }

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
      for (ymuint j = 0; j < kPvBitLen; ++ j) {
	const vector<ymuint>& det_list = op.det_list(j);
	record_pat(det_list, base + j);
      }
      cur_array.clear();
      op.clear_det_list();
      base += kPvBitLen;
    }
  }
  if ( !cur_array.empty() ) {
    mFsim.ppsfp(cur_array, op);
    for (ymuint j = 0; j < cur_array.size(); ++ j) {
      const vector<ymuint>& det_list = op.det_list(j);
      record_pat(det_list, base + j);
    }
    op.clear_det_list();
    base += cur_array.size();
    cur_array.clear();
  }

  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    TestVector* tv = mTvMgr.new_vector();
    cur_array.push_back(tv);
  }
  ymuint nochg_count = 0;
  for ( ; ; ) {
    for (ymuint i = 0; i < kPvBitLen; ++ i) {
      cur_array[i]->set_from_random(mRandGen);
    }
    mFsim.ppsfp(cur_array, op);
    ymuint nchg = 0;
    for (ymuint j = 0; j < kPvBitLen; ++ j) {
      const vector<ymuint>& det_list = op.det_list(j);
      nchg += record_pat(det_list, base + j);
    }
    cur_array.clear();
    op.clear_det_list();
    base += kPvBitLen;
    if ( nchg == 0 ) {
      ++ nochg_count;
      if ( nochg_count > 3 ) {
	break;
      }
    }
    else {
      nochg_count = 0;
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
    mTvMgr.delete_vector(cur_array[i]);
  }

  // mDomCandList の後始末．
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    const TpgFault* fault = fault_list[i];
    FaultData& fd = mFaultDataArray[fault->id()];
    if ( fd.mDomCandList.size() != fd.mDomCandListSize ) {
      fd.mDomCandList.erase(fd.mDomCandList.begin() + fd.mDomCandListSize, fd.mDomCandList.end());
    }
  }

  local_timer.stop();

  if ( mVerbose ) {
    cout << "CPU time (fault simulation)  " << local_timer.time() << endl
	 << "Total " << base << " patterns simulated" << endl;
  }

}

ymuint
DomChecker::record_pat(const vector<ymuint>& det_list,
		       ymuint pat_id)
{
  ymuint n = det_list.size();
  ymuint nchg = 0;

  for (ymuint i = 0; i < n; ++ i) {
    ymuint f_id = det_list[i];
    mDetFlag[f_id] = true;
  }

  // 検出結果を用いて等価故障のリストを更新する．
  ymuint ne = mEqClassList.size();
  for (ymuint i = 0; i < ne; ++ i) {
    const vector<ymuint>& src_list = mEqClassList[i];
    ASSERT_COND( !src_list.empty() );
    if ( src_list.size() == 1 ) {
      continue;
    }
    vector<ymuint> dst_list0;
    vector<ymuint> dst_list1;
    for (ymuint rpos = 0; rpos < src_list.size(); ++ rpos) {
      ymuint fid = src_list[rpos];
      if ( mDetFlag[fid] ) {
	dst_list1.push_back(fid);
      }
      else {
	dst_list0.push_back(fid);
      }
    }
    if ( dst_list1.empty() ) {
      ASSERT_COND( dst_list0.size() == src_list.size() );
      // なにもしない．
    }
    else if ( dst_list0.empty() ) {
      ASSERT_COND( dst_list1.size() == src_list.size() );
      // なにもしない．
    }
    else {
      mEqClassList[i] = dst_list1;
      mEqClassList.push_back(dst_list0);
    }
  }

  // 検出結果を用いて支配される故障の候補リストを作る．
  for (ymuint i = 0; i < n; ++ i) {
    ymuint f_id = det_list[i];
    FaultData& fd = mFaultDataArray[f_id];
    if ( fd.mDetCount == 0 ) {
      fd.mDomCandList.reserve(n - 1);
      // 初めて検出された場合
      // 構造的に独立でない故障を候補にする．
      const vector<ymuint>& input_list1 = mAnalyzer.input_list(f_id);
      for (ymuint j = 0; j < n; ++ j) {
	ymuint f_id2 = det_list[j];
	if ( f_id2 == f_id ) {
	  continue;
	}
	const vector<ymuint>& input_list2 = mAnalyzer.input_list(f_id2);
	bool intersect = check_intersect(input_list1, input_list2);
	if ( !intersect ) {
	  // 共通部分を持たない故障は独立
	  continue;
	}
	fd.mDomCandList.push_back(f_id2);
      }
      fd.mDomCandListSize = fd.mDomCandList.size();
    }
    else {
      // 二回目以降
      // 候補のうち，今回検出されなかった故障を外す．
      ymuint wpos = 0;
      for (ymuint j = 0; j < fd.mDomCandListSize; ++ j) {
	ymuint f_id2 = fd.mDomCandList[j];
	if ( mDetFlag[f_id2] ) {
	  if ( wpos != j ) {
	    fd.mDomCandList[wpos] = f_id2;
	  }
	  ++ wpos;
	}
      }
      if ( wpos < fd.mDomCandListSize ) {
	nchg += fd.mDomCandListSize - wpos;
	fd.mDomCandListSize = wpos;
      }
    }
    ++ fd.mDetCount;
  }

  for (ymuint i = 0; i < n; ++ i) {
    ymuint f_id = det_list[i];
    mDetFlag[f_id] = false;
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
  ymuint ne = mEqClassList.size();
  for (ymuint i = 0; i < ne; ++ i) {
    const vector<ymuint>& eq_list = mEqClassList[i];
    ymuint n = eq_list.size();
    for (ymuint i1 = 0; i1 < n; ++ i1) {
      ymuint f1_id = eq_list[i1];
      if ( mark[f1_id] ) {
	continue;
      }

      if ( mVerbose > 1 ) {
	cout << "\r"
	     << setw(6) << i << " / " << setw(6) << ne
	     << "  " << setw(6) << i1;
      }

      const FaultInfo& fi1 = mAnalyzer.fault_info(f1_id);
      const TpgFault* f1 = fi1.fault();
      rep_fault_list.push_back(f1);
      for (ymuint i2 = i1 + 1; i2 < n; ++ i2) {
	ymuint f2_id = eq_list[i2];
	if ( mark[f2_id] ) {
	  continue;
	}
	const FaultInfo& fi2 = mAnalyzer.fault_info(f2_id);
	const TpgFault* f2 = fi2.fault();
	++ n_check;
	if ( check_fault_equivalence(f1, f2) ) {
	  mark[f2_id] = true;
	  ++ n_success;
	  continue;
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

  // 非支配故障の候補から支配故障の候補を作る．
  for (ymuint i = 0; i < fault_num; ++ i) {
    ymuint f1_id = fault_list[i];
    const vector<ymuint>& cand_list = mFaultDataArray[f1_id].mDomCandList;
    for (ymuint j = 0; j < cand_list.size(); ++ j) {
      ymuint f2_id = cand_list[j];
      mFaultDataArray[f2_id].mDomCandList2.push_back(f1_id);
    }
  }

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
  }
}

// @brief f1 が f2 を支配しているか調べる．
bool
DomChecker::check_fault_dominance(const TpgFault* f1,
				  const TpgFault* f2)
{
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
  return engine.check_sat() == kB3False;
}

// @brief f1 と f2 が等価かどうか調べる．
bool
DomChecker::check_fault_equivalence(const TpgFault* f1,
				    const TpgFault* f2)
{
#if 0
  SatEngine engine(string(), string(), NULL);
  GvalCnf gval_cnf(mMaxNodeId);
  FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
  const NodeSet& node_set1 = mAnalyzer.node_set(f1->id());
  engine.make_fval_cnf(fval_cnf1, f1, node_set1, kValX);

  FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);
  const NodeSet& node_set2 = mAnalyzer.node_set(f2->id());
  engine.make_fval_cnf(fval_cnf2, f2, node_set2, kValX);

  Literal fd1lit(fval_cnf1.fd_var());
  Literal fd2lit(fval_cnf2.fd_var());
  engine.add_clause( fd1lit,  fd2lit);
  engine.add_clause(~fd1lit, ~fd2lit);
  return engine.check_sat() == kB3False;
#else
#if 1
  return check_fault_dominance(f1, f2) && check_fault_dominance(f2, f1);
#else
  const FaultInfo& fi1 = mAnalyzer.fault_info(f1->id());
  const FaultInfo& fi2 = mAnalyzer.fault_info(f2->id());

  {
    SatEngine engine(string(), string(), NULL);
    GvalCnf gval_cnf(mMaxNodeId);

    NodeValList suf_list1;
    if ( fi1.single_cube() ) {
      suf_list1 = fi1.sufficient_assignment();
    }
    else {
      FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
      const NodeSet& node_set1 = mAnalyzer.node_set(f1->id());
      engine.make_fval_cnf(fval_cnf1, f1, node_set1, kVal1);
    }

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
    if ( engine.check_sat(gval_cnf, suf_list1) != kB3False ) {
      return false;
    }
  }
  {
    SatEngine engine(string(), string(), NULL);
    GvalCnf gval_cnf(mMaxNodeId);

    NodeValList suf_list1;
    if ( fi2.single_cube() ) {
      suf_list1 = fi2.sufficient_assignment();
    }
    else {
      FvalCnf fval_cnf2(mMaxNodeId, gval_cnf);
      const NodeSet& node_set2 = mAnalyzer.node_set(f2->id());
      engine.make_fval_cnf(fval_cnf2, f2, node_set2, kVal1);
    }

    if ( fi1.single_cube() ) {
      // f1 を検出しない CNF を生成
      engine.add_negation(gval_cnf, fi1.sufficient_assignment());
    }
    else {
      FvalCnf fval_cnf1(mMaxNodeId, gval_cnf);
      const NodeSet& node_set1 = mAnalyzer.node_set(f1->id());
      // f2 を検出して f1 を検出しない CNF を生成
      engine.make_fval_cnf(fval_cnf1, f1, node_set1, kVal0);
    }
    if ( engine.check_sat(gval_cnf, suf_list1) == kB3False ) {
      return true;
    }
    return false;
  }
#endif
#endif
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

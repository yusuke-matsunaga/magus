
/// @file ConflictChecker.cc
/// @brief ConflictChecker の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ConflictChecker.h"

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
// クラス ConflictChecker
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ConflictChecker::ConflictChecker(FaultAnalyzer& analyzer,
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
  for (ymuint i = 0; i < mMaxFaultId; ++ i) {
    mFaultDataArray[i].mDetCount = 0;
  }
}

// @brief デストラクタ
ConflictChecker::~ConflictChecker()
{
}

// @brief verbose フラグを設定する．
// @param[in] verbose 表示を制御するフラグ
void
ConflictChecker::set_verbose(int verbose)
{
  mVerbose = verbose;
}

// @brief 故障間の衝突性を調べる．
void
ConflictChecker::analyze_conflict(const vector<const TpgFault*>& fault_list)
{
  StopWatch local_timer;
  local_timer.start();

  mConflictStats.conf_timer.reset();
  mConflictStats.conf1_timer.reset();
  mConflictStats.conf2_timer.reset();
  mConflictStats.conf3_timer.reset();
  mConflictStats.conf4_timer.reset();
  mConflictStats.int1_timer.reset();
  mConflictStats.int2_timer.reset();
  mConflictStats.conf_count = 0;
  mConflictStats.conf1_count = 0;
  mConflictStats.conf2_count = 0;
  mConflictStats.conf3_count = 0;
  mConflictStats.conf4_count = 0;
  mConflictStats.conf4_check_count = 0;
  mConflictStats.int1_count = 0;
  mConflictStats.int2_count = 0;

  // シミュレーション結果を用いてコンフリクトチェックのスクリーニングを行う．
  get_pat_list(fault_list);

  ymuint fault_num = fault_list.size();
  for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
    const TpgFault* f1 = fault_list[i1];

    if ( mVerbose > 1 ) {
      cout << "\r" << setw(6) << i1 << " / " << setw(6) << fault_num;
      cout.flush();
    }
    vector<ymuint>& f2_list = mFaultDataArray[f1->id()].mCandList;
    vector<const TpgFault*> conf_list;
    analyze_conflict(f1, f2_list, conf_list, false, false);
    for (ymuint i = 0; i < conf_list.size(); ++ i) {
      const TpgFault* f2 = conf_list[i];
      mFaultDataArray[f1->id()].mConflictList.push_back(f2->id());
      mFaultDataArray[f2->id()].mConflictList.push_back(f1->id());
    }
  }

  for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
    const TpgFault* f1 = fault_list[i1];
    FaultData& fd = mFaultDataArray[f1->id()];
    sort(fd.mConflictList.begin(), fd.mConflictList.end());
  }

  local_timer.stop();

  if ( mVerbose ) {
    cout << endl;
    print_conflict_stats(cout);
    cout << "Total CPU time " << local_timer.time() << endl;
  }
}

// @brief 衝突リストを得る．
const vector<ymuint>&
ConflictChecker::conflict_list(ymuint fid)
{
  ASSERT_COND( fid < mMaxFaultId );
  return mFaultDataArray[fid].mConflictList;
}

// @brief 故障間の衝突性を調べる．
void
ConflictChecker::estimate_conflict(const vector<const TpgFault*>& fault_list,
				   ymuint sample_num,
				   vector<double>& conf_prob_array)
{
  StopWatch local_timer;
  local_timer.start();

  mConflictStats.conf_timer.reset();
  mConflictStats.conf1_timer.reset();
  mConflictStats.conf2_timer.reset();
  mConflictStats.conf3_timer.reset();
  mConflictStats.conf4_timer.reset();
  mConflictStats.int1_timer.reset();
  mConflictStats.int2_timer.reset();
  mConflictStats.conf_count = 0;
  mConflictStats.conf1_count = 0;
  mConflictStats.conf2_count = 0;
  mConflictStats.conf3_count = 0;
  mConflictStats.conf4_count = 0;
  mConflictStats.conf4_check_count = 0;
  mConflictStats.int1_count = 0;
  mConflictStats.int2_count = 0;

  RandGen rg;

  conf_prob_array.clear();
  conf_prob_array.resize(mMaxFaultId);

  // シミュレーション結果を用いてコンフリクトチェックのスクリーニングを行う．

  get_pat_list(fault_list);
#if 0
  ymuint fault_num = fault_list.size();
  for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
    const TpgFault* f1 = fault_list[i1];

    if ( mVerbose > 1 ) {
      cout << "\r                  ";
      cout << "\r" << setw(6) << i1 << " / " << setw(6) << fault_num;
      cout.flush();
    }
    vector<const TpgFault*> f2_list;
    if ( sample_num < fault_num ) {
      f2_list.reserve(sample_num);
      for (ymuint i2 = 0; i2 < sample_num; ++ i2) {
	ymuint pos = rg.int32() % fault_num;
	const TpgFault* f2 = fault_list[pos];
	f2_list.push_back(f2);
      }
    }
    else {
      f2_list.reserve(fault_num);
      for (ymuint i2 = 0; i2 < fault_num; ++ i2) {
	const TpgFault* f2 = fault_list[i2];
	f2_list.push_back(f2);
      }
    }
    vector<const TpgFault*> conf_list;
    analyze_conflict(f1, f2_list, conf_list, true, false);
    conf_prob_array[f1->id()] = static_cast<double>(conf_list.size()) / sample_num;
  }

  local_timer.stop();

  if ( mVerbose > 1 ) {
    cout << endl;
  }
  if ( mVerbose > 0 ) {
    print_conflict_stats(cout);
    cout << "Total CPU time " << local_timer.time() << endl;
  }
#endif
}

// @brief 1つの故障と複数の故障間の衝突性を調べる．
void
ConflictChecker::analyze_conflict(const TpgFault* f1,
				  const vector<ymuint>& f2_list,
				  vector<const TpgFault*>& conf_list,
				  bool simple,
				  bool local_verbose)
{
  if ( mAnalyzer.fault_info(f1->id()).single_cube() ) {
    analyze_conflict2(f1, f2_list, conf_list, simple, local_verbose);
    return;
  }

  mConflictStats.conf_timer.start();

  GvalCnf gval_cnf(mMaxNodeId);
  FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
  SatEngine engine(string(), string(), NULL);

  engine.make_fval_cnf(fval_cnf, f1, mAnalyzer.node_set(f1->id()), kVal1);

  const FaultInfo& fi1 = mAnalyzer.fault_info(f1->id());
  const NodeValList& suf_list1 = fi1.sufficient_assignment();
  const NodeValList& pi_suf_list1 = fi1.pi_sufficient_assignment();
  const NodeValList& ma_list1 = fi1.mandatory_assignment();

  conf_list.reserve(f2_list.size());
  for (ymuint i2 = 0; i2 < f2_list.size(); ++ i2) {
    ymuint f2_id = f2_list[i2];
    if ( f1->id() > f2_id ) {
      continue;
    }

    const FaultInfo& fi2 = mAnalyzer.fault_info(f2_id);
    const TpgFault* f2 = fi2.fault();
    const NodeValList& suf_list2 = fi2.sufficient_assignment();
    const NodeValList& pi_suf_list2 = fi2.pi_sufficient_assignment();
    const NodeValList& ma_list2 = fi2.mandatory_assignment();

    mConflictStats.conf1_timer.start();
    if ( check_conflict(ma_list1, ma_list2) ) {
      // 必要割当そのものがコンフリクトしている．
      ++ mConflictStats.conf_count;
      ++ mConflictStats.conf1_count;
      conf_list.push_back(f2);
      mConflictStats.conf1_timer.stop();
      continue;
    }
    mConflictStats.conf1_timer.stop();

    mConflictStats.int1_timer.start();
    if ( !check_conflict(pi_suf_list1, pi_suf_list2) ) {
      // 十分割当が両立しているのでコンフリクトしない．
      ++ mConflictStats.int1_count;
      mConflictStats.int1_timer.stop();
      continue;
    }
    mConflictStats.int1_timer.stop();

    mConflictStats.int2_timer.start();
    Bool3 sat_stat = engine.check_sat(gval_cnf, suf_list2);
    if ( sat_stat == kB3True ) {
      // f2 の十分割当のもとで f1 が検出できれば f1 と f2 はコンフリクトしない．
      ++ mConflictStats.int2_count;
      mConflictStats.int2_timer.stop();
      continue;
    }
    mConflictStats.int2_timer.stop();

    if ( fi2.single_cube() ) {
      if ( sat_stat == kB3False ) {
	++ mConflictStats.conf_count;
	++ mConflictStats.conf3_count;
	conf_list.push_back(f2);
      }
      // f2 の十分割当と必要割当が等しければ上のチェックで終わり．
      continue;
    }

    mConflictStats.conf3_timer.start();
    if ( engine.check_sat(gval_cnf, ma_list2) == kB3False ) {
      // f2 の必要割当のもとで f1 が検出できなければ f1 と f2 はコンフリクトしている．
      ++ mConflictStats.conf_count;
      ++ mConflictStats.conf3_count;
      conf_list.push_back(f2);
      mConflictStats.conf3_timer.stop();
      continue;
    }
    mConflictStats.conf3_timer.stop();

    if ( simple ) {
      continue;
    }

    mConflictStats.conf4_timer.start();
    ++ mConflictStats.conf4_check_count;
    if ( check_fault_conflict(f1, f2) ) {
      ++ mConflictStats.conf_count;
      ++ mConflictStats.conf4_count;
      conf_list.push_back(f2);
    }
    mConflictStats.conf4_timer.stop();
  }

  mConflictStats.conf_timer.stop();

  if ( mVerbose > 0 && local_verbose ) {
    if ( mVerbose > 1 ) {
      cout << endl;
    }
    print_conflict_stats(cout);
  }
}

// @brief 1つの故障と複数の故障間の衝突性を調べる．
void
ConflictChecker::analyze_conflict2(const TpgFault* f1,
				   const vector<ymuint>& f2_list,
				   vector<const TpgFault*>& conf_list,
				   bool simple,
				   bool local_verbose)
{
  mConflictStats.conf_timer.start();

  GvalCnf gval_cnf(mMaxNodeId);
  SatEngine engine(string(), string(), NULL);

  const FaultInfo& fi1 = mAnalyzer.fault_info(f1->id());
  const NodeValList& suf_list1 = fi1.sufficient_assignment();
  const NodeValList& pi_suf_list1 = fi1.pi_sufficient_assignment();

  conf_list.reserve(f2_list.size());
  for (ymuint i2 = 0; i2 < f2_list.size(); ++ i2) {
    ymuint f2_id = f2_list[i2];

    const FaultInfo& fi2 = mAnalyzer.fault_info(f2_id);
    const TpgFault* f2 = fi2.fault();
    const NodeValList& suf_list2 = fi2.sufficient_assignment();
    const NodeValList& pi_suf_list2 = fi2.pi_sufficient_assignment();
    const NodeValList& ma_list2 = fi2.mandatory_assignment();

    mConflictStats.conf1_timer.start();
    if ( check_conflict(suf_list1, ma_list2) ) {
      // 必要割当そのものがコンフリクトしている．
      ++ mConflictStats.conf_count;
      ++ mConflictStats.conf1_count;
      conf_list.push_back(f2);
      mConflictStats.conf1_timer.stop();
      continue;
    }
    mConflictStats.conf1_timer.stop();

    mConflictStats.int1_timer.start();
    if ( !check_conflict(pi_suf_list1, pi_suf_list2) ) {
      // 十分割当が両立しているのでコンフリクトしない．
      ++ mConflictStats.int1_count;
      mConflictStats.int1_timer.stop();
      continue;
    }
    mConflictStats.int1_timer.stop();

    mConflictStats.int2_timer.start();
    Bool3 sat_stat = engine.check_sat(gval_cnf, suf_list1, suf_list2);
    if ( sat_stat == kB3True ) {
      // f2 の十分割当のもとで f1 が検出できれば f1 と f2 はコンフリクトしない．
      ++ mConflictStats.int2_count;
      mConflictStats.int2_timer.stop();
      continue;
    }
    mConflictStats.int2_timer.stop();

    if ( fi2.single_cube() ) {
      if ( sat_stat == kB3False ) {
	++ mConflictStats.conf_count;
	++ mConflictStats.conf3_count;
	conf_list.push_back(f2);
      }
      // f2 の十分割当と必要割当が等しければ上のチェックで終わり．
      continue;
    }

    mConflictStats.conf3_timer.start();
    if ( engine.check_sat(gval_cnf, suf_list1, ma_list2) == kB3False ) {
      // f2 の必要割当のもとで f1 が検出できなければ f1 と f2 はコンフリクトしている．
      ++ mConflictStats.conf_count;
      ++ mConflictStats.conf3_count;
      conf_list.push_back(f2);
      mConflictStats.conf3_timer.stop();
      continue;
    }
    mConflictStats.conf3_timer.stop();

    mConflictStats.conf4_timer.start();
    ++ mConflictStats.conf4_check_count;
    {
      GvalCnf gval_cnf(mMaxNodeId);
      FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
      SatEngine engine(string(), string(), NULL);

      engine.make_fval_cnf(fval_cnf, f2, mAnalyzer.node_set(f2->id()), kVal1);
      if ( engine.check_sat(gval_cnf, suf_list1) == kB3False ) {
	++ mConflictStats.conf_count;
	++ mConflictStats.conf4_count;
	conf_list.push_back(f2);
      }
    }
    mConflictStats.conf4_timer.stop();
  }

  mConflictStats.conf_timer.stop();

  if ( mVerbose > 0 && local_verbose ) {
    if ( mVerbose > 1 ) {
      cout << endl;
    }
    print_conflict_stats(cout);
  }
}

/// @brief f1 と f2 が衝突しているか調べる．
bool
ConflictChecker::check_fault_conflict(const TpgFault* f1,
				      const TpgFault* f2)
{
  GvalCnf gval_cnf2(mMaxNodeId);
  FvalCnf fval_cnf1(mMaxNodeId, gval_cnf2);
  FvalCnf fval_cnf2(mMaxNodeId, gval_cnf2);
  SatEngine engine(string(), string(), NULL);

  const NodeSet& node_set1 = mAnalyzer.node_set(f1->id());
  const NodeSet& node_set2 = mAnalyzer.node_set(f2->id());

  // f1 と f2 を検出する CNF を生成
  engine.make_fval_cnf(fval_cnf1, f1, node_set1, kVal1);
  engine.make_fval_cnf(fval_cnf2, f2, node_set2, kVal1);

  return engine.check_sat() == kB3False;
}

// @brief analyze_conflict の統計情報を出力する．
void
ConflictChecker::print_conflict_stats(ostream& s)
{
  s << "Total    " << setw(6) << mConflictStats.conf_count  << " conflicts" << endl;
  s << "Total    " << setw(6) << mConflictStats.conf1_count << " conflicts (ma_list)" << endl;
  s << "Total    " << setw(6) << mConflictStats.conf3_count << " conflicts (single ma_list)" << endl;
  s << "Total    " << setw(6) << mConflictStats.conf4_count << " conflicts (exact) / "
       << setw(6) << mConflictStats.conf4_check_count << endl;
  s << "Total    " << setw(6) << mConflictStats.int1_count  << " pi_suf_list intersection check" << endl;
  s << "Total    " << setw(6) << mConflictStats.int2_count  << " suf_list intersection check" << endl;
  s << "CPU time (conflict check)    " << mConflictStats.conf_timer.time() << endl;
  s << "CPU time (simple ma_list)    " << mConflictStats.conf1_timer.time() << endl;
  s << "CPU time (single conflict)   " << mConflictStats.conf3_timer.time() << endl;
  s << "CPU time (exact conflict)    " << mConflictStats.conf4_timer.time() << endl;
  s << "CPU time (siple pi_suf_list) " << mConflictStats.int1_timer.time() << endl;
  s << "CPU time (single suf_list)   " << mConflictStats.int2_timer.time() << endl;
}

// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
// @param[in] fault_list 故障リスト
void
ConflictChecker::get_pat_list(const vector<const TpgFault*>& fault_list)
{
  StopWatch local_timer;
  local_timer.start();

  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);

  KDet2Op op(mFsim, fault_list);

  ymuint npat = fault_list.size();
  ymuint base = 0;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
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
	record_pat(det_list, fault_list, base + j);
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
      record_pat(det_list, fault_list, base + j);
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
  for (ymuint c = 0; c < 1000; ++ c) {
    for (ymuint i = 0; i < kPvBitLen; ++ i) {
      cur_array[i]->set_from_random(mRandGen);
    }
    mFsim.ppsfp(cur_array, op);
    ymuint nchg = 0;
    for (ymuint j = 0; j < kPvBitLen; ++ j) {
      const vector<ymuint>& det_list = op.det_list(j);
      nchg += record_pat(det_list, fault_list, base + j);
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

  // mCandList の後始末．
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    const TpgFault* fault = fault_list[i];
    FaultData& fd = mFaultDataArray[fault->id()];
    if ( fd.mCandList.size() != fd.mCandListSize ) {
      fd.mCandList.erase(fd.mCandList.begin() + fd.mCandListSize, fd.mCandList.end());
    }
  }

  local_timer.stop();

  if ( mVerbose ) {
    cout << "CPU time (fault simulation)  " << local_timer.time() << endl
	 << "Total " << base << " patterns simulated" << endl;
  }

}

ymuint
ConflictChecker::record_pat(const vector<ymuint>& det_list,
			    const vector<const TpgFault*>& fault_list,
			    ymuint pat_id)
{
  ymuint n = det_list.size();
  ymuint nchg = 0;
  vector<bool> det_flag(mMaxFaultId, false);
  for (ymuint i = 0; i < n; ++ i) {
    ymuint f_id = det_list[i];
    det_flag[f_id] = true;
  }

  // 検出結果を用いて支配される故障の候補リストを作る．
  for (ymuint i = 0; i < n; ++ i) {
    ymuint f_id = det_list[i];
    FaultData& fd = mFaultDataArray[f_id];
    ++ fd.mDetCount;
    if ( fd.mDetCount == 1 ) {
      // 初めて検出された場合
      // 構造的に独立でない故障を候補にする．
      const vector<ymuint>& input_list1 = mAnalyzer.input_list(f_id);
      for (ymuint j = 0; j < fault_list.size(); ++ j) {
	const TpgFault* f2 = fault_list[j];
	ymuint f2_id = f2->id();
	if ( f2_id == f_id ) {
	  continue;
	}
	const vector<ymuint>& input_list2 = mAnalyzer.input_list(f2_id);
	bool intersect = check_intersect(input_list1, input_list2);
	if ( !intersect ) {
	  // 共通部分を持たない故障は独立
	  continue;
	}
	fd.mCandList.push_back(f2_id);
      }
      fd.mCandListSize = fd.mCandList.size();
    }
    else {
      // 二回目以降
      // 候補のうち，今回検出された故障を外す．
      ymuint wpos = 0;
      for (ymuint j = 0; j < fd.mCandListSize; ++ j) {
	ymuint f2_id = fd.mCandList[j];
	if ( !det_flag[f2_id] ) {
	  if ( wpos != j ) {
	    fd.mCandList[wpos] = f2_id;
	  }
	  ++ wpos;
	}
      }
      if ( wpos < fd.mCandListSize ) {
	nchg += fd.mCandListSize - wpos;
	fd.mCandListSize = wpos;
      }
    }
  }

  return nchg;
}

END_NAMESPACE_YM_SATPG

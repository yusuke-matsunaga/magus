
/// @file ConflictChecker.cc
/// @brief ConflictChecker の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ConflictChecker.h"

#include "FaultAnalyzer.h"

//#include "TpgNetwork.h"
//#include "TpgNode.h"
#include "TpgFault.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "Fsim.h"
#include "KDet2Op.h"
#include "NodeSet.h"
//#include "NodeValList.h"

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

// パタン番号リストの包含関係を調べる．
// 0 bit: list1 のみの要素がある．
// 1 bit: list2 のみの要素がある．
//
// リストの内容は昇順にソートされていると仮定する．
inline
ymuint
check_pat_list1(const vector<ymuint>& tv_list1,
		const vector<ymuint>& tv_list2)
{
  bool flag_10 = false;
  bool flag_01 = false;
  bool flag_11 = false;
  ymuint ans = 0U;
  ymuint n1 = tv_list1.size();
  ymuint n2 = tv_list2.size();
  ymuint p1 = 0;
  ymuint p2 = 0;
  ymuint v1 = tv_list1[p1];
  ymuint v2 = tv_list2[p2];
  for ( ; ; ) {
    if ( v1 < v2 ) {
      ans |= 1U;
      ++ p1;
      if ( p1 >= n1 ) {
	break;
      }
      v1 = tv_list1[p1];
    }
    else if ( v1 > v2 ) {
      ans |= 2U;
      ++ p2;
      if ( p2 >= n2 ) {
	break;
      }
      v2 = tv_list2[p2];
    }
    else {
      ++ p1;
      ++ p2;
      if ( p1 >= n1 ) {
	break;
      }
      v1 = tv_list1[p1];
      if ( p2 >= n2 ) {
	break;
      }
      v2 = tv_list2[p2];
    }
    if ( ans == 3U ) {
      return ans;
    }
  }
  if ( p1 < n1 ) {
    ans |= 1U;
  }
  if ( p2 < n2 ) {
    ans |= 2U;
  }
  return ans;
}

// パタン番号リストの包含関係を調べる．
// true  list1 と list2 に共通の要素がある．
//
// リストの内容は昇順にソートされていると仮定する．
inline
bool
check_pat_list2(const vector<ymuint>& tv_list1,
		const vector<ymuint>& tv_list2)
{
  bool flag_10 = false;
  bool flag_01 = false;
  bool flag_11 = false;
  ymuint n1 = tv_list1.size();
  ymuint n2 = tv_list2.size();
  ymuint p1 = 0;
  ymuint p2 = 0;
  ymuint v1 = tv_list1[p1];
  ymuint v2 = tv_list2[p2];
  for ( ; ; ) {
    if ( v1 < v2 ) {
      ++ p1;
      if ( p1 >= n1 ) {
	return false;
      }
      v1 = tv_list1[p1];
    }
    else if ( v1 > v2 ) {
      ++ p2;
      if ( p2 >= n2 ) {
	return false;
      }
      v2 = tv_list2[p2];
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
				 Fsim& fsim,
				 TvMgr& tvmgr) :
  mAnalyzer(analyzer),
  mFsim(fsim),
  mTvMgr(tvmgr)
{
  mVerbose = mAnalyzer.verbose();
  mMaxNodeId = mAnalyzer.max_node_id();
  mMaxFaultId = mAnalyzer.max_fault_id();
  mFaultDataArray.resize(mMaxFaultId);
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
ConflictChecker::analyze_conflict(const vector<TpgFault*>& fault_list)
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
    TpgFault* f1 = fault_list[i1];

    if ( mVerbose > 1 ) {
      cout << "\r" << setw(6) << i1 << " / " << setw(6) << fault_num;
      cout.flush();
    }
    vector<TpgFault*> f2_list;
    f2_list.reserve(fault_num - i1 - 1);
    for (ymuint i2 = i1 + 1; i2 < fault_num; ++ i2) {
      TpgFault* f2 = fault_list[i2];
      f2_list.push_back(f2);
    }
    vector<TpgFault*> conf_list;
    analyze_conflict(f1, f2_list, conf_list, false, false);
    for (ymuint i = 0; i < conf_list.size(); ++ i) {
      TpgFault* f2 = conf_list[i];
      mFaultDataArray[f1->id()].mConflictList.push_back(f2->id());
      mFaultDataArray[f2->id()].mConflictList.push_back(f1->id());
    }
  }

  for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
    TpgFault* f1 = fault_list[i1];
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
ConflictChecker::estimate_conflict(const vector<TpgFault*>& fault_list,
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

  ymuint fault_num = fault_list.size();
  for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
    TpgFault* f1 = fault_list[i1];

    if ( mVerbose > 1 ) {
      cout << "\r                  ";
      cout << "\r" << setw(6) << i1 << " / " << setw(6) << fault_num;
      cout.flush();
    }
    vector<TpgFault*> f2_list;
    if ( sample_num < fault_num ) {
      f2_list.reserve(sample_num);
      for (ymuint i2 = 0; i2 < sample_num; ++ i2) {
	ymuint pos = rg.int32() % fault_num;
	TpgFault* f2 = fault_list[pos];
	f2_list.push_back(f2);
      }
    }
    else {
      f2_list.reserve(fault_num);
      for (ymuint i2 = 0; i2 < fault_num; ++ i2) {
	TpgFault* f2 = fault_list[i2];
	f2_list.push_back(f2);
      }
    }
    vector<TpgFault*> conf_list;
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
}

// @brief 1つの故障と複数の故障間の衝突性を調べる．
void
ConflictChecker::analyze_conflict(TpgFault* f1,
				  const vector<TpgFault*>& f2_list,
				  vector<TpgFault*>& conf_list,
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

  fval_cnf.make_cnf(engine, f1, kVal1);

  const vector<ymuint>& input_list1 = mAnalyzer.input_list(f1->id());

  const FaultInfo& fi1 = mAnalyzer.fault_info(f1->id());
  const vector<ymuint>& tv_list1 = mFaultDataArray[f1->id()].mPatList;
  const NodeValList& suf_list1 = fi1.sufficient_assignment();
  const NodeValList& pi_suf_list1 = fi1.pi_sufficient_assignment();
  const NodeValList& ma_list1 = fi1.mandatory_assignment();

  conf_list.reserve(f2_list.size());
  for (ymuint i2 = 0; i2 < f2_list.size(); ++ i2) {
    TpgFault* f2 = f2_list[i2];

    const vector<ymuint>& input_list2 = mAnalyzer.input_list(f2->id());

    bool intersect = check_intersect(input_list1, input_list2);
    if ( !intersect ) {
      // 共通部分を持たない故障は独立
      continue;
    }

    const FaultInfo& fi2 = mAnalyzer.fault_info(f2->id());
    const vector<ymuint>& tv_list2 = mFaultDataArray[f2->id()].mPatList;

    bool stat = check_pat_list2(tv_list1, tv_list2);
    if ( stat ) {
      // f1 と f2 が同時に1なのでコンフリクトしない．
      continue;
    }

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
ConflictChecker::analyze_conflict2(TpgFault* f1,
				   const vector<TpgFault*>& f2_list,
				   vector<TpgFault*>& conf_list,
				   bool simple,
				   bool local_verbose)
{
  mConflictStats.conf_timer.start();

  GvalCnf gval_cnf(mMaxNodeId);
  SatEngine engine(string(), string(), NULL);

  const vector<ymuint>& input_list1 = mAnalyzer.input_list(f1->id());

  const FaultInfo& fi1 = mAnalyzer.fault_info(f1->id());
  const vector<ymuint>& tv_list1 = mFaultDataArray[f1->id()].mPatList;
  const NodeValList& suf_list1 = fi1.sufficient_assignment();
  const NodeValList& pi_suf_list1 = fi1.pi_sufficient_assignment();

  conf_list.reserve(f2_list.size());
  for (ymuint i2 = 0; i2 < f2_list.size(); ++ i2) {
    TpgFault* f2 = f2_list[i2];

    const vector<ymuint>& input_list2 = mAnalyzer.input_list(f2->id());

    bool intersect = check_intersect(input_list1, input_list2);
    if ( !intersect ) {
      // 共通部分を持たない故障は独立
      continue;
    }

    const FaultInfo& fi2 = mAnalyzer.fault_info(f2->id());
    const vector<ymuint>& tv_list2 = mFaultDataArray[f2->id()].mPatList;

    bool stat = check_pat_list2(tv_list1, tv_list2);
    if ( stat ) {
      // f1 と f2 が同時に1なのでコンフリクトしない．
      continue;
    }

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

    if ( simple ) {
      continue;
    }

    mConflictStats.conf4_timer.start();
    ++ mConflictStats.conf4_check_count;
    {
      GvalCnf gval_cnf(mMaxNodeId);
      FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
      SatEngine engine(string(), string(), NULL);
      fval_cnf.make_cnf(engine, f2, kVal1);
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
ConflictChecker::check_fault_conflict(TpgFault* f1,
				      TpgFault* f2)
{
  GvalCnf gval_cnf2(mMaxNodeId);
  FvalCnf fval_cnf1(mMaxNodeId, gval_cnf2);
  FvalCnf fval_cnf2(mMaxNodeId, gval_cnf2);
  SatEngine engine(string(), string(), NULL);

  const NodeSet& node_set1 = mAnalyzer.node_set(f1->id());
  const NodeSet& node_set2 = mAnalyzer.node_set(f2->id());

  fval_cnf1.make_cnf(engine, f1, node_set1, kVal1);
  fval_cnf2.make_cnf(engine, f2, node_set2, kVal1);

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
ConflictChecker::get_pat_list(const vector<TpgFault*>& fault_list)
{
  StopWatch local_timer;
  local_timer.start();

  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);

  KDet2Op op(mFsim, fault_list);

  ymuint npat = fault_list.size();
  ymuint base = 0;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    TpgFault* fault = fault_list[i];
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
  for (ymuint c = 0; c < 1000; ++ c) {
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
#if 0
  // mDomCandList の後始末．
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    TpgFault* fault = fault_list[i];
    FaultData& fd = mFaultDataArray[fault->id()];
    if ( fd.mDomCandList.size() != fd.mDomCandListSize ) {
      fd.mDomCandList.erase(fd.mDomCandList.begin() + fd.mDomCandListSize, fd.mDomCandList.end());
    }
  }
#endif
  local_timer.stop();

  if ( mVerbose ) {
    cout << "CPU time (fault simulation)  " << local_timer.time() << endl
	 << "Total " << base << " patterns simulated" << endl;
  }

}

ymuint
ConflictChecker::record_pat(const vector<ymuint>& det_list,
			    ymuint pat_id)
{
  ymuint n = det_list.size();
  ymuint nchg = 0;
  vector<bool> det_flag(mMaxFaultId, false);
  for (ymuint i = 0; i < n; ++ i) {
    ymuint f_id = det_list[i];
    det_flag[f_id] = true;
    mFaultDataArray[f_id].mPatList.push_back(pat_id);
  }
#if 0
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
	if ( det_flag[f_id2] ) {
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
#endif
  return nchg;
}

END_NAMESPACE_YM_SATPG

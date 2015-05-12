
/// @file MinPatBase.cc
/// @brief MinPatBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatBase.h"
#include "FgMgr2.h"
#include "Compactor.h"
#include "McCompactor.h"
#include "TpgNetwork.h"
#include "TvMgr.h"
#include "Verifier.h"
#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"
#include "NodeSet.h"
#include "ModelValMap.h"

#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス MinPatBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatBase::MinPatBase(bool group_dominance)
{
  mVerbose = 0;
  mGroupDominance = group_dominance;
}

// @brief デストラクタ
MinPatBase::~MinPatBase()
{
}

// @brief テストベクタの最小化を行なう．
// @param[in] network 対象のネットワーク
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
// @param[in] fsim3 3値の故障シミュレータ
// @param[in] exact 故障グループの両立性判定を厳密に行うときに true とする．
// @param[in] compaction 最後に圧縮を行うときに true とする．
// @param[in] fast_compaction 最後に高速圧縮を行うときに true とする．
// @param[in] mc_compaction 最後に最小被覆圧縮を行うときに true とする．
// @param[out] tv_list テストベクタのリスト
// @param[out] stats 実行結果の情報を格納する変数
void
MinPatBase::run(TpgNetwork& network,
		TvMgr& tvmgr,
		Fsim& fsim2,
		Fsim& fsim3,
		bool exact,
		bool compaction,
		bool fast_compaction,
		bool mc_compaction,
		bool has_thval,
		ymuint thval,
		vector<TestVector*>& tv_list,
		USTime& time)
{
  StopWatch total_timer;
  total_timer.start();

  mFast = !exact;

  mMaxNodeId = network.max_node_id();

  mAnalyzer.set_verbose(verbose());

  mAnalyzer.init(network, tvmgr);

  // 検出された故障番号のリスト
  const vector<ymuint>& fid_list = mAnalyzer.fid_list();

  // 故障のリストを作る(Fsim用)
  vector<const TpgFault*> fault_list;
  ymuint max_fault_id = 0;
  {
    ymuint nf = fid_list.size();
    fault_list.reserve(nf);
    for (ymuint i = 0; i < nf; ++ i) {
      ymuint fid = fid_list[i];
      if ( max_fault_id < fid ) {
	max_fault_id = fid;
      }
      const TpgFault* fault = mAnalyzer.fault(fid);
      fault_list.push_back(fault);
    }
  }

  // 故障シミュレータに故障リストをセットする．
  fsim2.set_faults(fault_list);
  fsim3.set_faults(fault_list);

  init(fid_list, tvmgr, fsim2);

  if ( false ) {
    vector<ymuint> dom_fid_list = this->fid_list();
    sort(dom_fid_list.begin(), dom_fid_list.end());
    for (ymuint i = 0; i < dom_fid_list.size(); ++ i) {
      ymuint fid = dom_fid_list[i];
      const FaultInfo& fi = mAnalyzer.fault_info(fid);
      vector<ymuint> eq_list = fi.eq_list();
      vector<ymuint> dom_list = fi.dom_list();
      sort(eq_list.begin(), eq_list.end());
      sort(dom_list.begin(), dom_list.end());
      cout << fid << endl
	   << " EQ:  ";
      for (ymuint j = 0; j < eq_list.size(); ++ j) {
	cout << " " << eq_list[j];
      }
      cout << endl;
      cout << " DOM: ";
      for (ymuint j = 0; j < dom_list.size(); ++ j) {
	cout << " " << dom_list[j];
      }
      cout << endl
	   << endl;
    }
  }

  StopWatch local_timer;
  local_timer.start();

  FgMgr2 fgmgr(mMaxNodeId, mAnalyzer);
  vector<ymuint> group_list;
  ymuint nf = fault_num();

  { // 最初の故障を選ぶ
    ymuint fid = get_first_fault();

    // 最初のグループを作る．
    ymuint gid = fgmgr.new_group(fid);
    group_list.push_back(gid);
  }

  // 未処理の故障がある限り以下の処理を繰り返す．
  for (ymuint c = 1; c < nf; ++ c) {

    if ( verbose() > 1 ) {
      cout << "\r   " << setw(6) << c << " / " << setw(6) << nf
	   << " : " << setw(6) << fgmgr.group_num();
      cout.flush();
    }

    // 故障を選ぶ．
    ymuint fid = get_next_fault(fgmgr, group_list);

#if 0
    // 故障を追加できるグループを見つける．
    ymuint gid = find_group(fgmgr, fid, group_list);
    if ( gid == fgmgr.group_num() ) {
      // 見つからなかった．
      // 新たなグループを作る．
      gid = fgmgr.new_group(fid);
      group_list.push_back(gid);
    }
    else {
      // 故障を追加する．
      fgmgr.add_fault(gid, fid);
    }
#else
    // 故障を追加できるグループを見つける．
    ymuint gid = fgmgr.find_group2(fid, group_list, mFast);
    if ( gid == fgmgr.group_num() ) {
      // 見つからなかった．
      // 新たなグループを作る．
      ymuint gid = fgmgr.new_group(fid);
      group_list.push_back(gid);
    }
#endif
  }

  local_timer.stop();
  if ( verbose() > 0 ) {
    if ( verbose() > 1 ) {
      cout << endl;
    }
    cout << " # of fault groups = " << setw(4) << group_list.size() << endl;
    cout << "CPU time (coloring)              " << local_timer.time() << endl;
    cout << "Total   " << setw(8) << fgmgr.mfault_num() << " exact compatibility check" << endl
	 << "Total   " << setw(8) << fgmgr.check_count() << " SAT checks" << endl
	 << "        " << setw(8) << fgmgr.found_count() << "  success" << endl
	 << "        " << fgmgr.check_time() << " CPU time for SAT checks" << endl
	 << "Avarage " << setw(8) << fgmgr.mfault_avg() << " faults per check" << endl
	 << "Max     " << setw(8) << fgmgr.mfault_max() << " faults" << endl;
  }

  fgmgr.clear_count();

  if ( compaction || fast_compaction ) {
    // 後処理
    local_timer.reset();
    local_timer.start();

    Compactor compactor;
    compactor.set_verbose(verbose());

    if ( has_thval ) {
      compactor.set_thval(thval);
    }

    vector<ymuint> new_group_list;
    compactor.run(fgmgr, mMaxNodeId, group_list, fast_compaction, new_group_list);
    group_list = new_group_list;

    local_timer.stop();
    if ( verbose() > 0 ) {
      cout << " # of fault groups = " << setw(4) << group_list.size() << endl;
      cout << "CPU time (compaction)              " << local_timer.time() << endl;
    }
  }

  // テストパタンを作る．
  local_timer.reset();
  local_timer.start();

  ymuint new_ng = group_list.size();
  tv_list.clear();
  tv_list.reserve(new_ng);
  for (ymuint i = 0; i < new_ng; ++ i) {
    ymuint gid = group_list[i];
    const NodeValList& suf_list = fgmgr.sufficient_assignment(gid);
    TestVector* tv = tvmgr.new_vector();
    make_testvector(network, suf_list, tv);
    tv_list.push_back(tv);
  }

  local_timer.stop();
  if ( verbose() > 0 ) {
    cout << "CPU time (testvector generation) " << local_timer.time() << endl;
  }

  if ( mc_compaction ) {
    local_timer.reset();
    local_timer.start();

    McCompactor compactor(fsim3);
    compactor.set_verbose(verbose());

    vector<TestVector*> new_tv_list;
    compactor.run(fault_list, tv_list, new_tv_list);

    tv_list = new_tv_list;

    local_timer.stop();
    if ( verbose() > 0 ) {
      cout << "# of Test Patterns = " << tv_list.size() << endl;
      cout << "CPU time (minimum covering) " << local_timer.time() << endl;
    }
  }

  if ( verbose() > 0 ) {
    cout << "Total   " << setw(8) << fgmgr.mfault_num() << " exact compatibility check" << endl
	 << "Total   " << setw(8) << fgmgr.check_count() << " SAT checks" << endl
	 << "        " << setw(8) << fgmgr.found_count() << "  success" << endl
	 << "        " << fgmgr.check_time() << " CPU time for SAT checks" << endl
	 << "Avarage " << setw(8) << fgmgr.mfault_avg() << " faults per check" << endl
	 << "Max     " << setw(8) << fgmgr.mfault_max() << " faults" << endl;
  }

  { // 検証しておく．
    Verifier ver;
    if ( ver.check(fsim2, fault_list, tv_list) ) {
      if ( verbose() > 0 ) {
	cout << "  No errors" << endl;
      }
    }
  }

  total_timer.stop();
  time = total_timer.time();
}

// @brief verbose フラグをセットする．
void
MinPatBase::set_verbose(int verbose)
{
  mVerbose = verbose;
}

// @brief verbose フラグを得る．
int
MinPatBase::verbose() const
{
  return mVerbose;
}

// @brief 故障を追加するグループを選ぶ．
// @param[in] fgmgr 故障グループを管理するオブジェクト
// @param[in] fid 故障番号
// @param[in] group_list 現在のグループリスト
//
// グループが見つからなければ fgmgr.group_num() を返す．
ymuint
MinPatBase::find_group(FgMgr& fgmgr,
		       ymuint fid,
		       const vector<ymuint>& group_list)
{
  if ( mGroupDominance ) {
    ymuint gid = fgmgr.find_dom_group(fid, group_list);
    if ( gid != fgmgr.group_num() ) {
      return gid;
    }
  }

  ymuint gid = fgmgr.find_group(fid, group_list, mFast);
  return gid;
}

// @brief 故障解析器を返す．
FaultAnalyzer&
MinPatBase::analyzer()
{
  return mAnalyzer;
}

// @brief テストパタンを作る．
// @param[in] gid グループ番号
// @param[in] network ネットワーク
// @param[in] tv テストベクタ
void
MinPatBase::make_testvector(TpgNetwork& network,
			    const NodeValList& suf_list,
			    TestVector* tv)
{
  GvalCnf gval_cnf(mMaxNodeId);

  SatEngine engine(string(), string(), NULL);

  vector<Bool3> sat_model;
  Bool3 sat_ans = engine.check_sat(gval_cnf, suf_list, sat_model);
  ASSERT_COND ( sat_ans == kB3True );

  const VidMap& var_map = gval_cnf.var_map();
  ModelValMap val_map(var_map, var_map, sat_model);
  ymuint ni = network.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* node = network.input(i);
    ymuint input_id = node->input_id();
    Val3 val;
    if ( var_map(node) == kVarIdIllegal ) {
      val = kVal0;
    }
    else {
      val = val_map.gval(node);
    }
    tv->set_val(input_id, val);
  }
}

END_NAMESPACE_YM_SATPG

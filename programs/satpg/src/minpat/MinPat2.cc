
/// @file MinPat2.cc
/// @brief MinPat2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPat2.h"
#include "MinPatStats.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "FaultMgr.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "Fsim.h"
#include "KDet2.h"
#include "NodeSet.h"
#include "NodeValList.h"
#include "Verifier.h"
#include "GcSolver.h"
#include "GcSolver2.h"
#include "GcSolver3.h"
#include "GcNode.h"
#include "TpgCnf1.h"
#include "TpgCnf0.h"

#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

extern
void
analyze_fault(TpgFault* f,
	      NodeValList& suf_list,
	      NodeValList& ma_list,
	      ymuint max_id);

extern
bool
check_dominance(TpgFault* f1,
		TpgFault* f2,
		ymuint max_id);

extern
bool
check_conflict(TpgFault* f1,
	       TpgFault* f2,
	       ymuint max_id);

// @brief インスタンスを生成する関数
MinPat*
new_MinPat2()
{
  return new MinPat2();
}

BEGIN_NONAMESPACE

struct FaultInfo
{

  NodeValList mSufList;

  NodeValList mMaList;
};

// パタン番号リストの包含関係を調べる．
// 0 bit: list1 のみの要素がある．
// 1 bit: list2 のみの要素がある．
// 2 bit: list1 と list2 に共通の要素がある．
//
// リストの内容は昇順にソートされていると仮定する．
ymuint
check_pat_list(const vector<ymuint>& tv_list1,
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
  while ( p1 < n1 && p2 < n2 ) {
    ymuint v1 = tv_list1[p1];
    ymuint v2 = tv_list2[p2];
    if ( v1 < v2 ) {
      ans |= 1U;
      ++ p1;
    }
    else if ( v1 > v2 ) {
      ans |= 2U;
      ++ p2;
    }
    else {
      ans |= 4U;
      ++ p1;
      ++ p2;
    }
    if ( ans == 7U ) {
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

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス MinPat2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPat2::MinPat2()
{
}

// @brief デストラクタ
MinPat2::~MinPat2()
{
}

// @brief テストベクタの最小化を行なう．
// @param[in] network 対象のネットワーク
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim2 2値の故障シミュレータ
// @param[in] fsim3 3値の故障シミュレータ
// @param[inout] tv_list テストベクタのリスト
// @param[out] stats 実行結果の情報を格納する変数
void
MinPat2::run(TpgNetwork& network,
	     TvMgr& tvmgr,
	     FaultMgr& fmgr,
	     Fsim& fsim2,
	     Fsim& fsim3,
	     vector<TestVector*>& tv_list,
	     MinPatStats& stats)
{
  StopWatch total_timer;
  StopWatch local_timer;

  total_timer.start();
  local_timer.start();

  ymuint orig_num = tv_list.size();

  Verifier ver(fmgr, fsim3);

  // 故障番号の最大値を求める．
  ymuint max_fault_id = 0;
  const vector<TpgFault*>& f_list = fmgr.det_list();
  ymuint fault_num = f_list.size();
  for (ymuint i = 0; i < fault_num; ++ i) {
    TpgFault* f = f_list[i];
    if ( max_fault_id < f->id() ) {
      max_fault_id = f->id();
    }
  }
  ++ max_fault_id;

  {
    cout << "# of faults:   " << fault_num << endl;
    cout << "# of patterns: " << orig_num << endl;
  }

  ymuint max_node_id = network.max_node_id();

  vector<vector<ymuint> > input_list_array(max_node_id);
  { // 各故障(に関連したノード)に関係する入力番号のリストを作る．
    // ここでいう関係とは TFI of TFO of fault location のこと
    // 計算には NodeSet を使う．
    NodeSet node_set;
    for (ymuint i = 0; i < fault_num; ++ i) {
      TpgFault* f = f_list[i];
      const TpgNode* node = f->node();
      vector<ymuint>& input_list = input_list_array[node->id()];
      if ( !input_list.empty() ) {
	continue;
      }
      node_set.mark_region(max_node_id, node);
      for (ymuint j = 0; j < node_set.tfo_tfi_size(); ++ j) {
	const TpgNode* node1 = node_set.tfo_tfi_node(j);
	if ( node1->is_input() ) {
	  input_list.push_back(node1->input_id());
	}
      }
      // ソートしておく．
      sort(input_list.begin(), input_list.end());
    }
  }

  // ドロップ無しの故障シミュレーター
  KDet2 kdet(fsim3, f_list);

  vector<vector<ymuint> > pat_list_array;
  ymuint npat0 = 1000;
  ymuint npat = orig_num + npat0;

  cout << "  fault simulation (npat = " << npat << ") starts."
       << endl;
  { // もともとのパタンに乱数パタンを npat0 個加える．
    // 結果は各故障ごとに検出パタン番号を pat_list_array に入れる．
    RandGen rg;
    vector<TestVector*> tmp_tv_list(npat);
    for (ymuint i = 0; i < orig_num; ++ i) {
      TestVector* tv = tv_list[i];
      tmp_tv_list[i] = tv;
    }
    for (ymuint i = 0; i < npat0; ++ i) {
      TestVector* tv = tvmgr.new_vector();
      tv->set_from_random(rg);
      tmp_tv_list[i + orig_num] = tv;
    }

    kdet.run(tmp_tv_list, pat_list_array);

    // 乱数パタンは削除しておく．
    for (ymuint i = 0; i < npat0; ++ i) {
      TestVector* tv = tmp_tv_list[i + orig_num];
      tvmgr.delete_vector(tv);
    }
  }

  cout << "  fault simulation ends." << endl;

  HashSet<ymuint> input_hash;

  ymuint n_sat1 = 0;

  vector<bool> dom_flag(max_fault_id, false);
  // シミュレーション結果から故障の支配関係のヒントを作り，
  // SAT で正確に判定する．
  {
    for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
      TpgFault* f1 = f_list[i1];

      // input_list1 の要素をハッシュに登録する．
      const vector<ymuint>& input_list1 = input_list_array[f1->node()->id()];
      input_hash.clear();
      for (ymuint i = 0; i < input_list1.size(); ++ i) {
	input_hash.add(input_list1[i]);
      }

      const vector<ymuint>& tv_list1 = pat_list_array[f1->id()];
      for (ymuint i2 = i1 + 1; i2 < fault_num; ++ i2) {
	TpgFault* f2 = f_list[i2];
	if ( dom_flag[f2->id()] ) {
	  continue;
	}

	// input_list2 の要素でハッシュに登録されている要素があれば
	// input_list1 と input_list2 は共通部分を持つ．
	const vector<ymuint>& input_list2 = input_list_array[f2->node()->id()];
	bool int_stat = false;
	for (ymuint i = 0; i < input_list2.size(); ++ i) {
	  if ( input_hash.check(input_list2[i]) ) {
	    int_stat = true;
	    break;
	  }
	}
	if ( !int_stat ) {
	  // 共通部分を持たない故障は独立
	  continue;
	}

	const vector<ymuint>& tv_list2 = pat_list_array[f2->id()];

	ymuint stat = check_pat_list(tv_list1, tv_list2);

	if ( (stat & 1U) == 0U ) {
	  // f1 が 0 のときは f2 も 0 だった．
	  ++ n_sat1;
	  if ( check_dominance(f1, f2, max_node_id) ) {
	    //cout << f2->str() << " is dominated by " << f1->str() << endl;
	    dom_flag[f2->id()] = true;
	  }
	}
	if ( !dom_flag[f2->id()] && (stat & 2U) == 0U ) {
	  // f2 が 0 のときは f1 も 0 だった．
	  ++ n_sat1;
	  if ( check_dominance(f2, f1, max_node_id) ) {
	    //cout << f1->str() << " is dominated by " << f2->str() << endl;
	    dom_flag[f1->id()] = true;
	    break;
	  }
	}
      }
    }
  }

  // 支配されていない故障を rep_fault_list に入れる．
  vector<TpgFault*> rep_fault_list;
  rep_fault_list.reserve(fault_num);
  for (ymuint i = 0; i < fault_num; ++ i) {
    TpgFault* f = f_list[i];
    if ( !dom_flag[f->id()] ) {
      rep_fault_list.push_back(f);
    }
  }
  ymuint rep_fault_num = rep_fault_list.size();

  local_timer.stop();

  cout << "Total    " << fault_num << " original faults" << endl;
  cout << "Total    " << rep_fault_num << " representative faults" << endl;
  cout << "Total    " << n_sat1 << " dominance test" << endl;
  cout << "CPU time for dominance test" << local_timer.time() << endl;

  local_timer.reset();
  local_timer.start();

  vector<FaultInfo> fault_info_array(max_fault_id);

  for (ymuint i = 0; i < rep_fault_num; ++ i) {
    TpgFault* f = rep_fault_list[i];
    NodeValList& suf_list = fault_info_array[f->id()].mSufList;
    NodeValList& ma_list = fault_info_array[f->id()].mMaList;;
    analyze_fault(f, suf_list, ma_list, max_node_id);
  }

  local_timer.stop();

  cout << "CPU time for fault analysis" << local_timer.time() << endl;

  local_timer.reset();
  local_timer.start();

  GcSolver2 gcsolver(rep_fault_list, network.max_node_id());

  StopWatch conf1_timer;
  StopWatch conf2_timer;
  StopWatch conf3_timer;
  StopWatch conf4_timer;
  StopWatch int1_timer;
  StopWatch int2_timer;

  ymuint n_sat2 = 0;
  ymuint n_conf = 0;
  ymuint n_conf1 = 0;
  ymuint n_conf2 = 0;
  ymuint n_conf3 = 0;
  ymuint n_conf4 = 0;
  ymuint n_int1 = 0;
  ymuint n_int2 = 0;
  { // シミュレーション結果を用いてコンフリクトチェックのスクリーニングを行う．
    for (ymuint i1 = 0; i1 < rep_fault_num; ++ i1) {
      TpgFault* f1 = rep_fault_list[i1];

      TpgCnf1 tpg_cnf(string(), string(), NULL);

      tpg_cnf.make_fval_cnf(f1, network.max_node_id());

      // input_list1 の要素をハッシュに登録する．
      const vector<ymuint>& input_list1 = input_list_array[f1->node()->id()];
      input_hash.clear();
      for (ymuint i = 0; i < input_list1.size(); ++ i) {
	input_hash.add(input_list1[i]);
      }

      const vector<ymuint>& tv_list1 = pat_list_array[f1->id()];
      FaultInfo& fi1 = fault_info_array[f1->id()];
      NodeValList& suf_list1 = fi1.mSufList;
      NodeValList& ma_list1 = fi1.mMaList;

      for (ymuint i2 = i1 + 1; i2 < rep_fault_num; ++ i2) {
	TpgFault* f2 = rep_fault_list[i2];

	// input_list2 の要素でハッシュに登録されている要素があれば
	// input_list1 と input_list2 は共通部分を持つ．
	const vector<ymuint>& input_list2 = input_list_array[f2->node()->id()];
	bool int_stat = false;
	for (ymuint i = 0; i < input_list2.size(); ++ i) {
	  if ( input_hash.check(input_list2[i]) ) {
	    int_stat = true;
	    break;
	  }
	}
	if ( !int_stat ) {
	  // 共通部分を持たない故障は独立
	  continue;
	}

	const vector<ymuint>& tv_list2 = pat_list_array[f2->id()];

	ymuint stat = check_pat_list(tv_list1, tv_list2);
	if ( (stat & 4U) != 0U ) {
	  // f1 と f2 が同時に1なのでコンフリクトしない．
	  continue;
	}

	FaultInfo& fi2 = fault_info_array[f2->id()];
	NodeValList& suf_list2 = fi2.mSufList;
	NodeValList& ma_list2 = fi2.mMaList;

	conf1_timer.start();
	if ( check_conflict(ma_list1, ma_list2) ) {
	  // 必要割当そのものがコンフリクトしている．
	  ++ n_conf;
	  ++ n_conf1;
	  gcsolver.connect(i1, i2);
	  conf1_timer.stop();
	  continue;
	}
	conf1_timer.stop();

	int2_timer.start();
	if ( !check_conflict(suf_list1, suf_list2) && tpg_cnf.check_intersect(suf_list2) ) {
	  // f2 の十分割当のもとで f1 が検出できれば f1 と f2 はコンフリクトしない．
	  ++ n_int2;
	  int2_timer.stop();
	  continue;
	}
	int2_timer.stop();

	conf3_timer.start();
	if ( tpg_cnf.check_conflict(ma_list2) ) {
	  // f2 の必要割当のもとで f1 が検出できなければ f1 と f2 はコンフリクトしている．
	  ++ n_conf;
	  ++ n_conf3;
	  gcsolver.connect(i1, i2);
	  conf3_timer.stop();
	  continue;
	}
	conf3_timer.stop();

	conf4_timer.start();
	// f1 と f2 が同時に 1 になることがない．
	++ n_sat2;
	if ( check_conflict(f1, f2, max_node_id) ) {
	  ++ n_conf;
	  ++ n_conf4;
	  gcsolver.connect(i1, i2);
	}
	conf4_timer.stop();
      }
    }
  }

  local_timer.stop();

  cout << "Total    " << setw(6) << n_conf  << " conflicts" << endl;
  cout << "Total    " << setw(6) << n_conf1 << " conflicts (ma_list)" << endl;
  cout << "Total    " << setw(6) << n_conf3 << " conflicts (single suf_list)" << endl;
  cout << "Total    " << setw(6) << n_conf4 << " conflicts (exact)" << endl;
  cout << "Total    " << setw(6) << n_sat2  << " exact test" << endl;
  cout << "Total    " << setw(6) << n_int2  << " suf_list intersection check" << endl;
  cout << "Total CPU time " << local_timer.time() << endl;
  cout << "CPU time (simple ma_list)    " << conf1_timer.time() << endl;
  cout << "CPU time (single conflict)   " << conf3_timer.time() << endl;
  cout << "CPU time (exact conflict)    " << conf4_timer.time() << endl;
  cout << "CPU time (single suf_list)   " << int2_timer.time() << endl;

  local_timer.reset();
  local_timer.start();
  cout << "coloring start" << endl;
  ymuint nc = gcsolver.coloring();
  cout << " # of fault groups = " << nc << endl;
  local_timer.stop();
  cout << "CPU time (coloring)          " << local_timer.time() << endl;

  // テストパタンを作る．
  TpgCnf0 tpg_cnf0(string(), string(), NULL);
  vector<TestVector*> new_tv_list;
  new_tv_list.reserve(nc);
  for (ymuint col = 1; col <= nc; ++ col) {
    const NodeValList& suf_list = gcsolver.suf_list(col);
    TestVector* tv = tvmgr.new_vector();
    bool stat = tpg_cnf0.get_testvector(network, suf_list, tv);
    ASSERT_COND( stat );
    new_tv_list.push_back(tv);
    if ( false ) {
      cout << "#" << col << endl
	   << " faults: ";
      const vector<TpgFault*>& fault_list = gcsolver.fault_list(col);
      for (ymuint i = 0; i < fault_list.size(); ++ i) {
	TpgFault* f = fault_list[i];
	cout << " " << f->str();
      }
      cout << endl;
      cout << tv << endl;
    }
  }
  tv_list = new_tv_list;
  {
    // 検証しておく．
    if ( ver.check(tv_list) ) {
      cout << "No Errors" << endl;
    }
  }

  total_timer.stop();
  USTime time = total_timer.time();

  stats.set(orig_num, tv_list.size(), time);
}


END_NAMESPACE_YM_SATPG

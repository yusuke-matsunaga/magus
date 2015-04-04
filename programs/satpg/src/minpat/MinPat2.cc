
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
#include "Verifier.h"
#include "GcSolver.h"
#include "GcNode.h"
#include "YmUtils/Graph.h"
#include "YmUtils/MinCov.h"
#include "YmUtils/RandGen.h"
#include "YmUtils/RandPermGen.h"
#include "YmUtils/StopWatch.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
MinPat*
new_MinPat2()
{
  return new MinPat2();
}

BEGIN_NONAMESPACE

// 2つのリストに共通要素があるときに true を返す．
// リストの内容は昇順にソートされていると仮定する．
bool
check_intersect(const vector<ymuint>& list1,
		const vector<ymuint>& list2)
{
  ymuint n1 = list1.size();
  ymuint n2 = list2.size();
  ymuint i1 = 0;
  ymuint i2 = 0;
  while ( i1 < n1 && i2 < n2 ) {
    ymuint v1 = list1[i1];
    ymuint v2 = list2[i2];
    if ( v1 < v2 ) {
      ++ i1;
    }
    else if ( v1 > v2 ) {
      ++ i2;
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
  StopWatch local_timer;

  local_timer.start();

  RandGen randgen;

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
      TpgNode* node = f->node();
      vector<ymuint>& input_list = input_list_array[node->id()];
      if ( !input_list.empty() ) {
	continue;
      }
      node_set.mark_region(max_node_id, vector<TpgNode*>(1, node));
      for (ymuint j = 0; j < node_set.tfo_tfi_size(); ++ j) {
	TpgNode* node1 = node_set.tfo_tfi_node(j);
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
	if ( (stat & 2U) == 0U ) {
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

  ymuint n_sat2 = 0;
  ymuint n_conf = 0;
  {
    for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
      TpgFault* f1 = f_list[i1];
      if ( dom_flag[f1->id()] ) {
	continue;
      }

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
	if ( (stat & 4U) == 0U ) {
	  // f1 と f2 が同時に 1 になることがない．
	  ++ n_sat2;
	  if ( check_conflict(f1, f2, max_node_id) ) {
	    //cout << f1->str() << " conflicts with " << f2->str() << endl;
	    ++ n_conf;
	  }
	}
      }
    }
  }

  ymuint nrep = 0;
  for (ymuint i = 0; i < fault_num; ++ i) {
    TpgFault* f = f_list[i];
    if ( !dom_flag[f->id()] ) {
      ++ nrep;
    }
  }
  cout << "Total " << fault_num << " original faults" << endl;
  cout << "Total " << nrep << " representative faults" << endl;
  cout << "Total " << n_sat1 << " dominance tespt" << endl;
  cout << "Total " << n_sat2 << " conflict test" << endl;
  cout << "Total " << n_conf << " conflicts" << endl;

#if 0
  // マージできないテストパタンの間に枝を持つグラフを作る．
  ymuint n = tv_list.size();
  GcSolver gcsolver(n);

  for (ymuint i1 = 1; i1 < n; ++ i1) {
    TestVector* tv1 = tv_list[i1];
    GcNode* node1 = gcsolver.node(i1);
    for (ymuint i2 = 0; i2 < i1; ++ i2) {
      TestVector* tv2 = tv_list[i2];
      GcNode* node2 = gcsolver.node(i2);
      if ( TestVector::is_conflict(*tv1, *tv2) ) {
	connect(node1, node2);
      }
    }
    node1->set_pat(tv1);
    for (vector<ymuint>::const_iterator p = det_list_array[i1].begin();
	 p != det_list_array[i1].end(); ++ p) {
      node1->add_fault(*p);
    }
  }
#endif
#if 0
  // このグラフ上での最小彩色問題を解くことで3値のパタンを圧縮する．
  vector<vector<ymuint> > color_group;
  ymuint nc = gcsolver.coloring(color_group);

  vector<TestVector*> new_tv_list;
  for (ymuint i = 0; i < nc; ++ i) {
    // 同じ色のグループのパタンを一つにマージする．
    TestVector* new_tv = tvmgr.new_vector();
    const vector<ymuint>& id_list = color_group[i];
    for (vector<ymuint>::const_iterator p = id_list.begin();
	 p != id_list.end(); ++ p) {
      ymuint id = *p;
      TestVector* tv = tv_list[id];
      bool stat = new_tv->merge(*tv);
      ASSERT_COND( stat );
    }
    // 残った X にランダムに 0/1 を割り当てる．
    new_tv->fix_x_from_random(randgen);

    new_tv_list.push_back(new_tv);
  }
  if ( false ) {
    // 検証しておく．
    if ( ver.check(new_tv_list) ) {
      cout << "No Errors(3)" << endl;
    }
  }
  cout << "Graph Coloring End: " << new_tv_list.size() << endl;

  // 最小被覆問題を解く．
  tv_list = new_tv_list;

  {
    KDet kdet(fsim3, f_list, max_fault_id);

    vector<vector<ymuint> > det_list_array;
    ymuint k = 40;
    kdet.run(tv_list, k, det_list_array);
    vector<bool> fmark(max_fault_id);
    vector<ymuint> fmap(max_fault_id);

    MinCov mincov;

    ymuint fnum = 0;
    ymuint pnum = tv_list.size();
    for (ymuint i = 0; i < pnum; ++ i) {
      const vector<ymuint>& det_list = det_list_array[i];
      for (ymuint j = 0; j < det_list.size(); ++ j) {
	ymuint f = det_list[j];
	if ( !fmark[f] ) {
	  fmark[f] = true;
	  fmap[f] = fnum;
	  ++ fnum;
	}
      }
    }

    mincov.set_size(fnum, pnum);
    for (ymuint i = 0; i < pnum; ++ i) {
      const vector<ymuint>& det_list = det_list_array[i];
      for (ymuint j = 0; j < det_list.size(); ++ j) {
	ymuint f = det_list[j];
	ymuint r = fmap[f];
	mincov.insert_elem(r, i);
      }
    }
    vector<ymuint32> pat_list;
    double cost = mincov.heuristic(pat_list, MinCov::kGreedy);
    if ( pat_list.size() < tv_list.size() ) {
      vector<TestVector*> tv_tmp_list(tv_list);
      tv_list.clear();
      for (ymuint i = 0; i < pat_list.size(); ++ i) {
	tv_list.push_back(tv_tmp_list[pat_list[i]]);
      }
    }
  }
  cout << "Minimum Covering End: " << tv_list.size() << endl;
#endif

#if 0
  {
    // 検証しておく．
    if ( ver.check(tv_list) ) {
      cout << "No Errors(4)" << endl;
    }
  }
#endif

  local_timer.stop();
  USTime time = local_timer.time();

  stats.set(orig_num, tv_list.size(), time);
}


END_NAMESPACE_YM_SATPG

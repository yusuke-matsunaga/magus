
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
#include "KDet.h"
#include "Verifier.h"
#include "GcSolver.h"
#include "GcNode.h"
#include "YmUtils/Graph.h"
#include "YmUtils/MinCov.h"
#include "YmUtils/RandGen.h"
#include "YmUtils/RandPermGen.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
MinPat*
new_MinPat2()
{
  return new MinPat2();
}

BEGIN_NONAMESPACE

void
dfs_mffc(TpgNode* node,
	 vector<bool>& mark,
	 vector<TpgFault*>& fault_list)
{
  mark[node->id()] = true;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( mark[inode->id()] == false && inode->imm_dom() != NULL ) {
      dfs_mffc(inode, mark, fault_list);
    }
  }

  if ( !node->is_output() ) {
    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      TpgFault* f = node->fault(i);
      if ( f->status() == kFsDetected ) {
	fault_list.push_back(f);
      }
    }
  }
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

  KDet kdet(fsim3, f_list);

  vector<vector<ymuint> > det_list_array;
  ymuint npat0 = 100;
  ymuint npat = orig_num + npat0;
  ymuint k = npat;

  cout << "  fault simulation (npat = " << npat << ", k = " << k << ") starts."
       << endl;
  {
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

    kdet.run(tmp_tv_list, k, det_list_array);

    for (ymuint i = 0; i < npat0; ++ i) {
      TestVector* tv = tmp_tv_list[i + orig_num];
      tvmgr.delete_vector(tv);
    }
  }

  cout << "  fault simulation ends." << endl;

  ymuint n_sat = 0;

  vector<bool> dom_flag(max_fault_id, false);
  // シミュレーション結果から故障の排他性と支配関係のヒントを作る．
  { // とりあえずナイーブな方法
    // 故障ごとに検出パタンのリストを作る．
    vector<vector<ymuint> > tv_list_array(max_fault_id);
    for (ymuint i = 0; i < npat; ++ i) {
      const vector<ymuint>& det_list = det_list_array[i];
      ymuint nf = det_list.size();
      for (ymuint j = 0; j < nf; ++ j) {
	ymuint f_id = det_list[j];
	tv_list_array[f_id].push_back(i);
      }
    }
    for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
      TpgFault* f1 = f_list[i1];
      const vector<ymuint>& tv_list1 = tv_list_array[f1->id()];
      for (ymuint i2 = i1 + 1; i2 < fault_num; ++ i2) {
	TpgFault* f2 = f_list[i2];
	if ( dom_flag[f2->id()] ) {
	  continue;
	}
	const vector<ymuint>& tv_list2 = tv_list_array[f2->id()];

	if ( false ) {
	  cout << f1->str() << ":";
	  for (ymuint i = 0; i < tv_list1.size(); ++ i) {
	    cout << " " << tv_list1[i];
	  }
	  cout << endl;
	  cout << f2->str() << ":";
	  for (ymuint i = 0; i < tv_list2.size(); ++ i) {
	    cout << " " << tv_list2[i];
	  }
	  cout << endl;
	}

	bool flag_10 = false;
	bool flag_01 = false;
	bool flag_11 = false;
	ymuint n1 = tv_list1.size();
	ymuint n2 = tv_list2.size();
	ymuint p1 = 0;
	ymuint p2 = 0;
	while ( p1 < n1 && p2 < n2 ) {
	  ymuint v1 = tv_list1[p1];
	  ymuint v2 = tv_list2[p2];
	  if ( v1 < v2 ) {
	    flag_10 = true;
	    ++ p1;
	  }
	  else if ( v1 > v2 ) {
	    flag_01 = true;
	    ++ p2;
	  }
	  else {
	    flag_11 = true;
	    ++ p1;
	    ++ p2;
	  }
	  if ( flag_10 && flag_01 && flag_11 ) {
	    break;
	  }
	}
	if ( p1 < n1 ) {
	  flag_10 = true;
	}
	if ( p2 < n2 ) {
	  flag_01 = true;
	}

	if ( !flag_10 ) {
	  // f1 が 0 のときは f2 も 0 だった．
	  ++ n_sat;
	  if ( check_dominance(f1, f2, max_node_id) ) {
	    cout << f2->str() << " is dominated by " << f1->str() << endl;
	    dom_flag[f2->id()] = true;
	  }
	}
	if ( !flag_01 ) {
	  // f2 が 0 のときは f1 も 0 だった．
	  ++ n_sat;
	  if ( check_dominance(f2, f1, max_node_id) ) {
	    cout << f1->str() << " is dominated by " << f2->str() << endl;
	    dom_flag[f1->id()] = true;
	    break;
	  }
	}
#if 0
	if ( !flag_11 ) {
	  // f1 と f2 が同時に 1 になることがない．
	  ++ n_sat;
	  if ( check_conflict(f1, f2, max_node_id) ) {
	    cout << f1->str() << " conflicts with " << f2->str() << endl;
	  }
	}
#endif
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
  cout << "Total " << n_sat << " sat invocation" << endl;

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


/// @file MinPatImpl.cc
/// @brief MinPatImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatImpl.h"
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
new_MinPat()
{
  return new MinPatImpl();
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

//////////////////////////////////////////////////////////////////////
// クラス MinPatImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatImpl::MinPatImpl()
{
}

// @brief デストラクタ
MinPatImpl::~MinPatImpl()
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
MinPatImpl::run(TpgNetwork& network,
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
  for (ymuint i = 0; i < f_list.size(); ++ i) {
    TpgFault* f = f_list[i];
    if ( max_fault_id < f->id() ) {
      max_fault_id = f->id();
    }
  }
  ++ max_fault_id;

  {
    ymuint n = network.active_node_num();
    ymuint nm = 0;
    for (ymuint i = 0; i < n; ++ i) {
      TpgNode* node = network.active_node(i);
      if ( node->imm_dom() == NULL ) {
	++ nm;
      }
    }
    cout << "# of faults:   " << f_list.size() << endl;
    cout << "# of MFFCs:    " << nm << endl;
    cout << "# of patterns: " << orig_num << endl;
  }

  {
    ymuint n = network.active_node_num();
    ymuint nm = 0;
    vector<bool> mark(network.max_node_id(), false);
    for (ymuint i = 0; i < n; ++ i) {
      TpgNode* node = network.active_node(i);
      if ( node->imm_dom() == NULL ) {
	vector<TpgFault*> fault_list;
	dfs_mffc(node, mark, fault_list);
	cout << "Fault Group: " << fault_list.size() << endl;
      }
    }
  }

  KDet kdet(fsim3, f_list, max_fault_id);

  cout << "  fault simulation starts." << endl;

  vector<vector<ymuint> > det_list_array;
  ymuint k = 2000;
  kdet.run(tv_list, k, det_list_array);

  cout << "  fault simulation ends." << endl;

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
      assert_cond( stat, __FILE__, __LINE__);
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

  {
    // 検証しておく．
    if ( ver.check(tv_list) ) {
      cout << "No Errors(4)" << endl;
    }
  }

  local_timer.stop();
  USTime time = local_timer.time();

  stats.set(orig_num, tv_list.size(), time);
}


END_NAMESPACE_YM_SATPG

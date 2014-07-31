
/// @file MinPatImpl.cc
/// @brief MinPatImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatImpl.h"
#include "MinPatStats.h"
#include "AtpgMgr.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "FaultMgr.h"
#include "TpgFault.h"
#include "Fsim.h"
#include "KDet.h"
#include "YmUtils/GcSolver.h"
#include "YmUtils/MinCov.h"
#include "YmUtils/RandGen.h"
#include "YmUtils/RandPermGen.h"
#include "Verifier.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
MinPat*
new_MinPat(AtpgMgr& mgr)
{
  return new MinPatImpl(mgr._tv_mgr(), mgr._fault_mgr(),
			mgr._fsim(), mgr._fsim3());
}


//////////////////////////////////////////////////////////////////////
// クラス MinPatImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim2 2値の故障シミュレータ
// @param[in] fsim3 3値の故障シミュレータ
MinPatImpl::MinPatImpl(TvMgr& tvmgr,
		       FaultMgr& fmgr,
		       Fsim& fsim2,
		       Fsim& fsim3) :
  mTvMgr(tvmgr),
  mFaultMgr(fmgr),
  mFsim2(fsim2),
  mFsim3(fsim3)
{
}

// @brief デストラクタ
MinPatImpl::~MinPatImpl()
{
}

// @brief テストベクタの最小化を行なう．
// @param[in] tv_list テストベクタのリスト
// @param[in] stats 実行結果の情報を格納する変数
void
MinPatImpl::run(vector<TestVector*>& tv_list,
		MinPatStats& stats)
{
  StopWatch local_timer;

  local_timer.start();

  RandGen randgen;

  ymuint orig_num = tv_list.size();

  Verifier ver(mFaultMgr, mFsim3);

  if ( 0 ) {
    if ( ver.check(tv_list) ) {
      cout << "No Errors(1)" << endl;
    }
  }


  ymuint max_fault_id = 0;
  const vector<TpgFault*>& f_list = mFaultMgr.det_list();
  for (ymuint i = 0; i < f_list.size(); ++ i) {
    TpgFault* f = f_list[i];
    if ( max_fault_id < f->id() ) {
      max_fault_id = f->id();
    }
  }
  ++ max_fault_id;
  {

    KDet kdet(mFsim3, f_list, max_fault_id);

    vector<vector<ymuint> > det_list_array;
    ymuint k = 200;
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
    double cost = mincov.heuristic(pat_list);
    ymuint orig_num = tv_list.size();
    if ( pat_list.size() < tv_list.size() ) {
      vector<TestVector*> tv_tmp_list(tv_list);
      tv_list.clear();
      for (ymuint i = 0; i < pat_list.size(); ++ i) {
	tv_list.push_back(tv_tmp_list[pat_list[i]]);
      }
    }
    cout << "Minimum Covering End: " << tv_list.size() << " / " << orig_num << endl;
  }

  // 3値のパタンを抜き出し tv3_list に入れる．
  // 2値のパタンは tv2_list に入れる．
  vector<TestVector*> tv2_list;
  vector<TestVector*> tv3_list;
  tv2_list.reserve(orig_num);
  tv3_list.reserve(orig_num);
  for (vector<TestVector*>::iterator p = tv_list.begin();
       p != tv_list.end(); ++ p) {
    TestVector* tv = *p;
    if ( tv->x_num() > 0 ) {
      tv3_list.push_back(tv);
    }
    else {
      tv2_list.push_back(tv);
    }
  }

  if ( 0 ) {
    // 検証しておく．
    if ( ver.check(tv3_list) ) {
      cout << "No Errors(2)" << endl;
    }
  }

  if ( tv3_list.empty() ) {
#if 0
    cout << "No x-patterns" << endl;
#endif
  }
  else {
    // 最小彩色問題を解くことで3値のパタンを圧縮する．
    GcSolver gcmgr;

    ymuint n = tv3_list.size();
    gcmgr.init(n);
    for (ymuint i1 = 1; i1 < n; ++ i1) {
      TestVector* tv1 = tv3_list[i1];
      for (ymuint i2 = 0; i2 < i1; ++ i2) {
	TestVector* tv2 = tv3_list[i2];
	if ( TestVector::is_conflict(*tv1, *tv2) ) {
	  gcmgr.connect(i1, i2);
	}
      }
    }
    vector<vector<ymuint> > color_group;
    ymuint nc = gcmgr.coloring(color_group);

#if 0
    cout << "coloring " << n << " --> " << nc << endl;
#endif

    for (ymuint i = 0; i < nc; ++ i) {
      // 同じ色のグループのパタンを一つにマージする．
      TestVector* new_tv = mTvMgr.new_vector();
      const vector<ymuint>& id_list = color_group[i];
      for (vector<ymuint>::const_iterator p = id_list.begin();
	   p != id_list.end(); ++ p) {
	ymuint id = *p;
	TestVector* tv = tv3_list[id];
	bool stat = new_tv->merge(*tv);
	assert_cond( stat, __FILE__, __LINE__);
      }
      // 残った X にランダムに 0/1 を割り当てる．
      new_tv->fix_x_from_random(randgen);

      tv2_list.push_back(new_tv);
    }
  }
  if ( 0 ) {
    // 検証しておく．
    if ( ver.check(tv2_list) ) {
      cout << "No Errors(3)" << endl;
    }
  }
  cout << "Graph Coloring End: " << tv2_list.size() << endl;

  // 最小被覆問題を解く．
  tv_list = tv2_list;

  {
    KDet kdet(mFsim3, f_list, max_fault_id);

    vector<vector<ymuint> > det_list_array;
    ymuint k = 400;
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
    double cost = mincov.heuristic(pat_list);
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

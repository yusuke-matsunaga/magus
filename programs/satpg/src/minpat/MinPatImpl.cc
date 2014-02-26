
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
#include "Fsim.h"
#include "Fop2MinPat.h"
#include "utils/GcSolver.h"
#include "utils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
MinPat*
new_MinPat(AtpgMgr& mgr)
{
  return new MinPatImpl(mgr._tv_mgr(), mgr._fault_mgr(),
			mgr._fsim(), mgr._fsimx());
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

  // tv2_list のパタンを用いて故障シミュレーションを行なう．
  cout << "Fault Simulation Start" << endl;
  Fop2MinPat op(mFsim2, mFaultMgr);
  vector<FsimOp2*> op_list(1, &op);

  op.clear_count();
  op.set_limit(10);

  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);

  for (vector<TestVector*>::iterator p = tv2_list.begin();
       p != tv2_list.end(); ++ p) {
    TestVector* tv = *p;
    cur_array.push_back(tv);
    if ( cur_array.size() == kPvBitLen ) {
      op.set_pattern(cur_array);
      mFsim2.ppsfp(cur_array, op_list);
      cur_array.clear();
    }
  }
  if ( !cur_array.empty() ) {
    op.set_pattern(cur_array);
    mFsim2.ppsfp(cur_array, op_list);
    cur_array.clear();
  }
  cout << "Fault Simulation End" << endl;

  tv_list = tv2_list;

  local_timer.stop();
  USTime time = local_timer.time();

  stats.set(orig_num, tv2_list.size(), time);
}


END_NAMESPACE_YM_SATPG

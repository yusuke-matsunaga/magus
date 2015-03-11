
/// @file GreedyMinPat.cc
/// @brief GreedyMinPat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GreedyMinPat.h"
#include "MinPatStats.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "FaultMgr.h"
#include "TpgFault.h"
#include "Fsim.h"
#include "KDet.h"
#include "Verifier.h"
#include "YmLogic/Bdd.h"
#include "YmLogic/BddMgr.h"
#include "YmUtils/Graph.h"
#include "YmUtils/MinCov.h"
#include "YmUtils/RandGen.h"
#include "YmUtils/RandPermGen.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
MinPat*
new_GreedyMinPat()
{
  return new GreedyMinPat();
}


BEGIN_NONAMESPACE

Bdd
tv_to_bdd(TestVector* tv,
	  BddMgr& bdd_mgr)
{
  Bdd ans = bdd_mgr.make_one();
  ymuint n = tv->input_num();
  for (ymuint i = 0; i < n; ++ i) {
    switch ( tv->val3(i) ) {
    case kValX:
      break;

    case kVal0:
      ans &= bdd_mgr.make_negaliteral(VarId(i));
      break;

    case kVal1:
      ans &= bdd_mgr.make_posiliteral(VarId(i));
      break;
    }
  }
  return ans;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス GreedyMinPat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GreedyMinPat::GreedyMinPat()
{
}

// @brief デストラクタ
GreedyMinPat::~GreedyMinPat()
{
}

// @brief テストベクタの最小化を行なう．
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim2 2値の故障シミュレータ
// @param[in] fsim3 3値の故障シミュレータ
// @param[inout] tv_list テストベクタのリスト
// @param[out] stats 実行結果の情報を格納する変数
void
GreedyMinPat::run(TvMgr& tvmgr,
		  FaultMgr& fmgr,
		  Fsim& fsim2,
		  Fsim& fsim3,
		  vector<TestVector*>& tv_list,
		  MinPatStats& stats)
{
  StopWatch local_timer;

  local_timer.start();

  BddMgr bdd_mgr("classic");

  RandGen randgen;

  ymuint orig_num = tv_list.size();

  Verifier ver(fmgr, fsim3);

  if ( 0 ) {
    if ( ver.check(tv_list) ) {
      cout << "No Errors(1)" << endl;
    }
  }


  ymuint max_fault_id = 0;
  const vector<TpgFault*>& f_list = fmgr.det_list();
  for (ymuint i = 0; i < f_list.size(); ++ i) {
    TpgFault* f = f_list[i];
    if ( max_fault_id < f->id() ) {
      max_fault_id = f->id();
    }
  }
  ++ max_fault_id;

  vector<Bdd> tf_array(max_fault_id);
  for (ymuint i = 0; i < max_fault_id; ++ i) {
    tf_array[i] = bdd_mgr.make_zero();
  }
  {

    KDet kdet(fsim3, f_list, max_fault_id);

    vector<vector<ymuint> > det_list_array;
    ymuint k = 10;
    kdet.run(tv_list, k, det_list_array);
    vector<bool> fmark(max_fault_id);
    vector<ymuint> fmap(max_fault_id);

    MinCov mincov;

    ymuint fnum = 0;
    ymuint pnum = tv_list.size();
    for (ymuint i = 0; i < pnum; ++ i) {
      cout << "pat#" << i << endl;
      TestVector* tv = tv_list[i];
      const vector<ymuint>& det_list = det_list_array[i];
      for (ymuint j = 0; j < det_list.size(); ++ j) {
	ymuint f = det_list[j];
	if ( !fmark[f] ) {
	  fmark[f] = true;
	  fmap[f] = fnum;
	  ++ fnum;
	}
	tf_array[f] |= tv_to_bdd(tv, bdd_mgr);
      }
    }
  }

#if 0
  // tf_array の maximal independent set を求める．
  vector<ymuint> mis;
  get_mis(tf_array, mis);

  for (vector<ymuint>::iterator p = mis.begin();
       p != mis.end(); ++ p) {
    ymuint fid = *p;
    Bdd f0 = tf_array[fid];
  }
#endif

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

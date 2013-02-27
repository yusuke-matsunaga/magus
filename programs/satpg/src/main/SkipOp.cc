
/// @file SkipOp.cc
/// @brief SkipOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "SkipOp.h"
#include "TpgFault.h"
#include "FaultMgr.h"
#include "TvMgr.h"
#include "Fsim.h"
#include "TestVector.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス SkipOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SkipOp::SkipOp(FaultMgr& fault_mgr,
	       TvMgr& tvmgr,
	       vector<TestVector*>& tv_list,
	       Fsim& fsim3,
	       ymuint threshold,
	       bool drop,
	       bool verify) :
  mFaultMgr(fault_mgr),
  mTvMgr(tvmgr),
  mTvList(tv_list),
  mFsim3(fsim3),
  mThreshold(threshold),
  mDrop(drop),
  mVerify(verify)
{
}

// @brief デストラクタ
SkipOp::~SkipOp()
{
  for (vector<TpgFault*>::iterator p = mUntestList.begin();
       p != mUntestList.end(); ++ p) {
    TpgFault* f = *p;
    f->clear_untest_num();
  }
  for (vector<TpgFault*>::iterator p = mSkipList.begin();
       p != mSkipList.end(); ++ p) {
    TpgFault* f = *p;
    f->clear_skip();
  }
}

// @brief テストパタンが見つかった場合に呼ばれる関数
// @param[in] f 故障
// @param[in] val_list "入力ノードの番号 x 2 + 値" のリスト
void
SkipOp::set_detected(TpgFault* f,
		     const vector<ymuint>& val_list)
{
  TestVector* tv = mTvMgr.new_vector();
  tv->init();
  for (vector<ymuint>::const_iterator p = val_list.begin();
       p != val_list.end(); ++ p) {
    ymuint tmp = *p;
    ymuint iid = tmp / 2;
    ymuint val = tmp % 2;
    if ( val == 1 ) {
      tv->set_val(iid, kVal1);
    }
    else {
      tv->set_val(iid, kVal0);
    }
  }

  mTvList.push_back(tv);

  mFaultMgr.set_status(f, kFsDetected);

  if ( mDrop ) {
    vector<TpgFault*> det_faults;
    mFsim3.run(tv, det_faults);
    for (vector<TpgFault*>::iterator p = det_faults.begin();
	 p != det_faults.end(); ++ p) {
      TpgFault* f = *p;
      mFaultMgr.set_status(f, kFsDetected);
    }
  }

  if ( mVerify ) {
    bool detect = mFsim3.run(tv, f);
    assert_cond( detect , __FILE__, __LINE__);
  }
}

// @brief 検出不能のときに呼ばれる関数
void
SkipOp::set_untestable(TpgFault* f)
{
  if ( f->untest_num() == 0 ) {
    // はじめて検出不能になった．
    mUntestList.push_back(f);
  }

  // 検出不能回数を1増やす．
  f->inc_untest_num();

  if ( f->untest_num() >= mThreshold ) {
    // 検出不能回数がしきい値を越えたのでスキップする．
    f->set_skip();
    mSkipList.push_back(f);
  }
}

END_NAMESPACE_YM_SATPG

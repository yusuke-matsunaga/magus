
/// @file atpg/src/main/Op2.cc
/// @brief Op2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "Op2.h"
#include "SaFault.h"
#include "FaultMgr.h"
#include "TvMgr.h"
#include "Fsim.h"
#include "TestVector.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス Op2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Op2::Op2(FaultMgr& fmgr,
	 vector<SaFault*>& skip_faults,
	 TvMgr& tvmgr,
	 vector<TestVector*>& tv_list,
	 Fsim& fsim3,
	 bool drop,
	 bool verify) :
  mFaultMgr(fmgr),
  mSkipFaults(skip_faults),
  mTvMgr(tvmgr),
  mTvList(tv_list),
  mFsim3(fsim3),
  mDrop(drop),
  mVerify(verify)
{
}

// @brief デストラクタ
Op2::~Op2()
{
}

// @brief テストパタンが見つかった場合に呼ばれる関数
// @param[in] f 故障
// @param[in] val_list "入力ノードの番号 x 2 + 値" のリスト
void
Op2::set_detected(SaFault* f,
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
  if ( mDrop ) {
    vector<SaFault*> det_faults;
    mFsim3.run(tv, det_faults);
    for (vector<SaFault*>::iterator p = det_faults.begin();
	 p != det_faults.end(); ++ p) {
      SaFault* f = *p;
      mFaultMgr.set_status(f, kFsDetected);
    }
  }
  if ( mVerify ) {
    bool detect = mFsim3.run(tv, f);
    assert_cond( detect , __FILE__, __LINE__);
  }

  mTvList.push_back(tv);

  mFaultMgr.set_status(f, kFsDetected);
}

// @brief 検出不能のときに呼ばれる関数
void
Op2::set_untestable(SaFault* f)
{
  mFaultMgr.set_status(f, kFsSkip);
  mSkipFaults.push_back(f);
}

END_NAMESPACE_YM_SATPG

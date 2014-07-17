
/// @file FaultMgr.cc
/// @brief FaultMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2012-2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FaultMgr.h"
#include "TpgFault.h"
#include "TpgNetwork.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
FaultMgr::FaultMgr()
{
  mChanged = false;
}

// @brief デストラクタ
FaultMgr::~FaultMgr()
{
}

// @brief クリアする．
void
FaultMgr::clear()
{
  mDetList.clear();
  mRemainList.clear();
  mUntestList.clear();

  mChanged = true;
}

// @brief network の故障を設定する．
void
FaultMgr::set_faults(TpgNetwork& network)
{
  clear();

  const vector<TpgFault*>& rep_faults = network.rep_faults();

  // 最初はすべての故障が mRemainList に属する．
  for (ymuint i = 0; i < rep_faults.size(); ++ i) {
    TpgFault* f = rep_faults[i];
    // ただし，外部出力に到達できない故障は検出不能となる．
    if ( f->node()->is_active() ) {
      mRemainList.push_back(f);
    }
    else {
      mUntestList.push_back(f);
    }
  }
}

// @brief fault の状態を変更する．
void
FaultMgr::set_status(TpgFault* fault,
		     FaultStatus stat)
{
  fault->mStatus = stat;
  mChanged = true;
}

// @brief 故障リストをスキャンして未検出リストを更新する．
void
FaultMgr::update()
{
  if ( mChanged ) {
    ymuint n = mRemainList.size();
    ymuint wpos = 0;
    for (ymuint rpos = 0; rpos < n; ++ rpos) {
      TpgFault* f = mRemainList[rpos];
      switch ( f->status() ) {
      case kFsUndetected:
	if ( wpos != rpos ) {
	  mRemainList[wpos] = f;
	}
	++ wpos;
	break;

      case kFsDetected:
	mDetList.push_back(f);
	break;

      case kFsUntestable:
	mUntestList.push_back(f);
	break;

      case kFsAborted:
	set_status(f, kFsUndetected);
	if ( wpos != rpos ) {
	  mRemainList[wpos] = f;
	}
	++ wpos;
	break;
      }
    }
    if ( wpos < n ) {
      mRemainList.erase(mRemainList.begin() + wpos, mRemainList.end());
    }
    mChanged = false;
  }
}

END_NAMESPACE_YM_SATPG

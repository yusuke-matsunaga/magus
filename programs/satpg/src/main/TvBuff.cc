
/// @file src/main/TvBuff.cc
/// @brief TvBuff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "TvBuff.h"
#include "AtpgMgr.h"
#include "PackedVal.h"
#include "SaFault.h"
#include "FaultMgr.h"
#include "fsim/Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
TvBuff::TvBuff(AtpgMgr* mgr) :
  mMgr(mgr)
{
  mBuff.reserve(kPvBitLen);
}

// @brief デストラクタ
TvBuff::~TvBuff()
{
}

// @brief バッファをクリアする．
void
TvBuff::clear()
{
  mBuff.clear();
}

// @brief パタンをバッファに入れる．
// @param[in] tv テストベクタ
// @note バッファが一杯になったら故障シミュレーションを行う．
void
TvBuff::put(TestVector* tv)
{
  mBuff.push_back(tv);
  if ( mBuff.size() == kPvBitLen ) {
    do_simulation();
  }
}

// @brief バッファに残っているパタンで故障シミュレーションを行う．
void
TvBuff::flush()
{
  if ( !mBuff.empty() ) {
    do_simulation();
  }
}

// @brief 故障シミュレーションを行う．
void
TvBuff::do_simulation()
{
#if 0
  MStopWatch& timer = mMgr->_timer();
  int old_timer_id = timer.change(TM_FSIM);
#endif
  ymuint n = mBuff.size();
  if ( n == 1 ) {
    vector<SaFault*> f_list;
    mMgr->fsim(mBuff[0], f_list);
  }
  else {
    vector<vector<SaFault*> > f_lists(n);
    mMgr->fsim(mBuff, f_lists);
  }
  mBuff.clear();
#if 0
  timer.change(old_timer_id);
#endif
}

END_NAMESPACE_YM_SATPG

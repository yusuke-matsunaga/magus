
/// @file SkipOp.cc
/// @brief SkipOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "SkipOp.h"
#include "TpgFault.h"


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
  OpBase(fault_mgr, tvmgr, tv_list, fsim3, drop, verify),
  mThreshold(threshold)
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

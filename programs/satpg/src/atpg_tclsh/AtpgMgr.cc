
/// @file AtpgMgr.cc
/// @brief AtpgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgMgr.h"
#include "TpgNetwork.h"
#include "TpgFault.h"
#include "FaultMgr.h"
#include "TvMgr.h"
#include "Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// AtpgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AtpgMgr::AtpgMgr() :
  mTimer(TM_SIZE, TM_MISC)
{
  mFaultMgr = new FaultMgr();
  mTvMgr = new TvMgr();

  mFsim = new_Fsim2();
  mFsim3 = new_Fsim3();

  mNetwork = NULL;
}

// @brief デストラクタ
AtpgMgr::~AtpgMgr()
{
  delete mFaultMgr;
  delete mTvMgr;
  delete mFsim;
  delete mFsim3;
  delete mNetwork;
}

// @brief ネットワークを設定する．
void
AtpgMgr::set_network(TpgNetwork* network)
{
  delete mNetwork;
  mNetwork = network;

  after_set_network();
}

// @brief ファイル読み込みに関わる時間を得る．
USTime
AtpgMgr::read_time() const
{
  return mTimer.time(TM_READ);
}

// @brief DTPG に関わる時間を得る．
USTime
AtpgMgr::dtpg_time() const
{
  return mTimer.time(TM_DTPG);
}

// @brief 故障シミュレーションに関わる時間を得る．
USTime
AtpgMgr::fsim_time() const
{
  return mTimer.time(TM_FSIM);
}

// @brief SAT に関わる時間を得る．
USTime
AtpgMgr::sat_time() const
{
  return mTimer.time(TM_SAT);
}

// @brief その他の時間を得る．
USTime
AtpgMgr::misc_time() const
{
  return mTimer.time(TM_MISC);
}

// @brief ネットワークをセットした後に呼ぶ関数
void
AtpgMgr::after_set_network()
{
  mFaultMgr->set_faults(*mNetwork);

  mTvMgr->clear();
  mTvMgr->init(mNetwork->input_num2());

  mFsim->set_network(*mNetwork);
  mFsim3->set_network(*mNetwork);
}

END_NAMESPACE_YM_SATPG

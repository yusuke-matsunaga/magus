
/// @file src/main/Dtpg.cc
/// @brief AtpgMgr の実装ファイル (Dtpgを行う部分)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "Dtpg.h"
#include "AtpgMgr.h"
#include "dtpg/DtpgSat.h"
#include "ym_utils/StopWatch.h"

#include "SaFault.h"
#include "FaultMgr.h"
#include "TestVector.h"
#include "TvMgr.h"
#include "TvBuff.h"

#include "ym_networks/TgNode.h"


BEGIN_NAMESPACE_YM_SATPG

// DTPG の結果を故障シミュレーションで検証する時に 1 にする．
const int verify_dtpg = 0;


// @brief コンストラクタ
Dtpg::Dtpg(AtpgMgr* mgr) :
  mAtpgMgr(mgr),
  mDtpgSat(new DtpgSat()),
  mTvBuff(mgr),
  mTmpTv(NULL)
{
}

// @brief デストラクタ
Dtpg::~Dtpg()
{
}

// @brief 未検出の故障に対してテストパタン生成を行う．
// @param[in] sim_mode シミュレーションモード
void
Dtpg::operator()(tSimMode sim_mode)
{
  const FaultMgr& fmgr = mAtpgMgr->_fault_mgr();
  const vector<SaFault*>& flist = fmgr.remain_list();

  if ( flist.empty() ) {
    mStats.mDetectNum = 0;
    mStats.mUntestNum = 0;
    mStats.mAbortNum = 0;
    mStats.mPatNum = 0;
    mStats.mTime = USTime(0.0, 0.0, 0.0);
    return;
  }

  TvMgr& tvmgr = mAtpgMgr->_tv_mgr();
  vector<TestVector*>& tv_list = mAtpgMgr->_tv_list();


  mTmpTv = tvmgr.new_vector();

#if 0
  MStopWatch& timer = _timer();
  int old_tm_id = timer.change(TM_DTPG);
#endif

  StopWatch local_timer;
  local_timer.start();

  ymuint d_num0 = fmgr.det_num();
  ymuint r_num0 = fmgr.untest_num();
  ymuint p_num0 = tv_list.size();

  mStats.mDtpgNum = 0;

  mTvBuff.clear();

  dtpg2(sim_mode);

  mTvBuff.flush();

  fmgr.update();

  tvmgr.delete_vector(mTmpTv);

  mStats.mDetectNum = fmgr.det_num() - d_num0;
  mStats.mUntestNum = fmgr.untest_num() - r_num0;
  mStats.mAbortNum = fmgr.remain_num();
  mStats.mPatNum = tv_list.size() - p_num0;
  local_timer.stop();
  mStats.mTime = local_timer.time();

#if 0
  timer.change(old_tm_id);
#endif
}

// @brief 未検出の故障に対してテストパタン生成を行う．
// @param[in] sim_mode シミュレーションモード
void
Dtpg::dtpg2(tSimMode sim_mode)
{
  FaultMgr& fmgr = mAtpgMgr->_fault_mgr();
  vector<TestVector*>& tv_list = mAtpgMgr->_tv_list();

  const vector<SaFault*>& flist = fmgr.remain_list();

  mCurFaults.clear();
  mCurFaults.reserve(flist.size());
  for (vector<SaFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    mCurFaults.push_back(*p);
  }

  mTvBuff.clear();

  mStats.mBtNum = 0;

  switch ( sim_mode ) {
  case kSimNone:
    for (ymuint i = 0; i < mCurFaults.size(); ++ i) {
      SaFault* f = mCurFaults[i];
      TestVector* cur_tv = dtpg_sat(f);
      if ( cur_tv ) {
	tv_list.push_back(cur_tv);
      }
    }
    break;

  case kSimSingle:
    for (ymuint i = 0; i < mCurFaults.size(); ++ i) {
      SaFault* f = mCurFaults[i];
      TestVector* cur_tv = dtpg_sat(f);
      if ( cur_tv ) {
	mTvBuff.put(cur_tv);
	mTvBuff.flush();
	tv_list.push_back(cur_tv);
      }
    }
    break;

  case kSimPpsfp:
    {
      ymuint cf_num = mCurFaults.size();
      while ( cf_num > 0 ) {
	// ランダムに故障を抜き出す．
	ymuint pos = ymuint(mFselGen.int32() % cf_num);
	SaFault* f = mCurFaults[pos];
	-- cf_num;
	if ( pos < cf_num ) {
	  mCurFaults[pos] = mCurFaults[cf_num];
	}

	TestVector* cur_tv = dtpg_sat(f);
	if ( cur_tv ) {
	  mTvBuff.put(cur_tv);
	  tv_list.push_back(cur_tv);
	}
      }
    }
    break;
  }
}

// @brief f に対するテストパタン生成を行う．
// @return パタンを返す．
TestVector*
Dtpg::dtpg_sat(SaFault* f)
{
  if ( f->status() != kFsUndetected ) {
    // すでに処理済みになっていた．
    return NULL;
  }

  FaultMgr& fmgr = _fault_mgr();

  ++ mStats.mDtpgNum;

  MStopWatch& timer = _timer();
  int old_timer_id = timer.change(TM_SAT);
  TgNetwork& network = _network();
  tStat stat = mDtpgSat->run(network, f, mTmpTv);
  mStats.mBtNum += mDtpgSat->stats().mDecisionNum;
  timer.change(old_timer_id);

  if ( stat == kUntest ) {
    // 検出不能故障
    fmgr.set_status(f, kFsUntestable);
    return NULL;;
  }
  if ( stat == kAbort ) {
    // アボート
    fmgr.set_status(f, kFsAbort);
    return NULL;
  }

  assert_cond( stat == kDetect, __FILE__, __LINE__);

  fmgr.set_status(f, kFsDetected);

  TvMgr& tvmgr = _tv_mgr();
  TestVector* cur_tv = tvmgr.new_vector();
  cur_tv->set_from_random(mPatGen);
  cur_tv->copy(*mTmpTv);

#if 0
  if ( verify_dtpg ) {
    Fsim3& fsim = _fsim3();
    list<SaFault*> flist;
    fsim.run(mTmpTv, flist);

    bool found = false;
    for (list<SaFault*>::iterator p = flist.begin();
	 p != flist.end(); ++ p) {
      SaFault* f1 = *p;
      if ( f1 == f ) {
	found = true;
	break;
      }
    }
    if ( !found ) {
      cerr << "dtpg error : tv is not a test vector for "
	   << f->str() << endl;
      mTmpTv->dump_bin(cerr);
      cerr << endl;
      assert_not_reached(__FILE__, __LINE__);
    }
  }
#endif

  return cur_tv;
}

// @brief 直前の実行結果を返す．
const DtpgStats&
Dtpg::stats() const
{
  return mStats;
}

// @brief SAT エンジンの指定を行う．
void
Dtpg::set_sat_mode(int mode)
{
  mDtpgSat->set_mode(mode);
}


//////////////////////////////////////////////////////////////////////
// クラス DtpgStats
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DtpgStats::DtpgStats()
{
  mDetectNum = 0;
  mUntestNum = 0;
  mAbortNum = 0;
  mPatNum = 0;
  mDtpgNum = 0;
  mBtNum = 0;
}

// @brief デストラクタ
DtpgStats::~DtpgStats()
{
}

// @brief 検出した故障数を得る．
ymuint
DtpgStats::detected_faults() const
{
  return mDetectNum;
}

// @brief 検出不能と判断した故障数を得る．
ymuint
DtpgStats::untestable_faults() const
{
  return mUntestNum;
}

// @brief アボートした故障数を得る．
ymuint
DtpgStats::aborted_faults() const
{
  return mAbortNum;
}

// @brief 生成したパタン数を得る．
ymuint
DtpgStats::generated_patterns() const
{
  return mPatNum;
}

// @brief 起動回数を得る．
ymuint
DtpgStats::call_count() const
{
  return mDtpgNum;
}

// @brief バックトラック数を得る．
ymuint64
DtpgStats::backtrack_count() const
{
  return mBtNum;
}

// @brief 計算時間を得る．
USTime
DtpgStats::time() const
{
  return mTime;
}

END_NAMESPACE_YM_SATPG

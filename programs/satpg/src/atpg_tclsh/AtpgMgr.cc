
/// @file AtpgMgr.cc
/// @brief AtpgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgMgr.h"
#include "RtpgStats.h"
#include "TpgNetwork.h"
#include "TpgFault.h"
#include "FaultMgr.h"
#include "TvMgr.h"
#include "Fsim.h"
#include "DtpgDriver.h"
#include "DtpgEngine.h"
#include "Rtpg.h"
#include "MinPat.h"
#include "DetectOp.h"


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

  mRtpg = new_Rtpg();
  mMinPat = new_MinPat();

  mNetwork = NULL;
}

// @brief デストラクタ
AtpgMgr::~AtpgMgr()
{
  delete mFaultMgr;
  delete mTvMgr;
  delete mFsim;
  delete mFsim3;
  delete mRtpg;
  delete mMinPat;
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

// @brief 乱数生成器を初期化する．
void
AtpgMgr::rtpg_init(ymuint32 seed)
{
  mRtpg->init(seed);
}

// @brief 乱数パタンを用いた故障シミュレーションを行なう．
// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
// @param[in] max_i 故障検出できないシミュレーション回数の上限
// @param[in] max_pat 最大のパタン数
// @param[in] stats 実行結果の情報を格納する変数
void
AtpgMgr::rtpg(ymuint min_f,
	      ymuint max_i,
	      ymuint max_pat,
	      RtpgStats& stats)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_FSIM);

  mFsim->set_faults(mFaultMgr->remain_list());

  mRtpg->run(*mFaultMgr, *mTvMgr, *mFsim, min_f, max_i, max_pat, mTvList, stats);

  mTimer.change(old_id);
}

// @brief テストパタン生成を行なう．
void
AtpgMgr::dtpg(DtpgMode mode,
	      tDtpgPoMode po_mode,
	      const string& option_str,
	      const string& sat_type,
	      const string& sat_option,
	      ostream* outp,
	      bool timer_enable,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop,
	      DtpgStats& stats)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_DTPG);

  mFsim3->set_faults(mFaultMgr->remain_list());

  DtpgDriver* dtpg = new_DtpgDriver();

  DtpgEngine* engine = NULL;
  if ( mode.engine_type() == "single" ) {
    engine = new_SatEngineSingle(sat_type, sat_option, outp, *mNetwork, bt, dop, uop);
  }
  else if ( mode.engine_type() == "single2" ) {
    engine = new_SatEngineSingle2(mode.val(), sat_type, sat_option, outp, *mNetwork, bt, dop, uop);
  }
  else if ( mode.engine_type() == "multi" ) {
    engine = new_SatEngineMulti(sat_type, sat_option, outp, *mNetwork, bt, dop, uop, false);
  }
  else if ( mode.engine_type() == "multi_forget" ) {
    engine = new_SatEngineMulti(sat_type, sat_option, outp, *mNetwork, bt, dop, uop, true);
  }
  else if ( mode.engine_type() == "multi2" ) {
    engine = new_SatEngineMulti2(mode.val(), sat_type, sat_option, outp, *mNetwork, bt, dop, uop, false);
  }
  else if ( mode.engine_type() == "multi2_forget" ) {
    engine = new_SatEngineMulti2(mode.val(), sat_type, sat_option, outp, *mNetwork, bt, dop, uop, true);
  }
  else if ( mode.engine_type() == "smt_single" ) {
    engine = new_SmtEngineSingle(sat_type, sat_option, outp, *mNetwork, bt, dop, uop);
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }

  engine->set_option(option_str);
  engine->timer_enable(timer_enable);

  dtpg->run(*mNetwork, mode.mode(), po_mode, *engine, stats);

  delete dtpg;
  delete engine;

  mTimer.change(old_id);
}

// @brief テストパタン圧縮を行なう．
void
AtpgMgr::minpat(MinPatStats& stats)
{
  mMinPat->run(*mTvMgr, *mFaultMgr, *mFsim, *mFsim3, _tv_list(), stats);
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


/// @file RtpgCmd.cc
/// @brief RtpgCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "RtpgCmd.h"
#include "Rtpg.h"
#include "RtpgStats.h"
#include "FaultMgr.h"
#include "Fsim.h"
#include "YmTclpp/TclPopt.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 故障シミュレーションを実行するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RtpgCmd::RtpgCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  mPoptNpat = new TclPoptInt(this, "npat",
			     "specify number of patterns to be simulated");
  mPoptMin = new TclPoptInt(this, "min_fault",
			    "specify the minimum limit of detected faults at a time");
  mPoptMax = new TclPoptInt(this, "max_interval",
			    "specify the maximum limit of non-detecting interval");
  mPoptSeed = new TclPoptUint(this, "seed",
			      "sepcify the random seed");
  mPoptFile = new TclPoptStr(this, "file",
			     "specify the file name containg patterns");
  mPoptPrintStats = new TclPopt(this, "print_stats",
				"print statistics");
}

// @brief デストラクタ
RtpgCmd::~RtpgCmd()
{
}

// コマンド処理関数
int
RtpgCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  Rtpg* rtpg = new_Rtpg();

  bool n_flag = false;
  ymuint max_pat = 100000;
  ymuint max_i = 4;
  ymuint min_f = 0;

  if ( mPoptNpat->is_specified() ) {
    max_pat = mPoptNpat->val();
    n_flag = true;
  }

  if ( mPoptMin->is_specified() ) {
    min_f = mPoptMin->val();
  }

  if ( mPoptMax->is_specified() ) {
    max_i = mPoptMax->val();
  }

  if ( mPoptSeed->is_specified() ) {
    rtpg->init(mPoptSeed->val());
  }

  if ( mPoptFile->is_specified() ) {
    // 未実装
  }

  bool print_stats = mPoptPrintStats->is_specified();

  if ( n_flag ) {
    min_f = 0;
    max_i = 0;
  }

  FaultMgr& fmgr = _fault_mgr();
  Fsim& fsim = _fsim();
  TvMgr& tvmgr = _tv_mgr();
  vector<TestVector*>& tv_list = _tv_list();
  RtpgStats stats;
  fsim.set_faults(fmgr.remain_list());
  rtpg->run(fmgr, tvmgr, fsim, min_f, max_i, max_pat, tv_list, stats);

  after_update_faults();

  delete rtpg;

  if ( print_stats ) {
    cout << "********** rtpg **********" << endl
	 << setw(10) << stats.detected_faults()
	 << ": # of detected faults" << endl
	 << setw(10) << stats.simulated_patterns()
	 << ": # of simulated patterns" << endl
	 << setw(10) << stats.effective_patterns()
	 << ": # of effective patterns" << endl
	 << stats.time() << endl;
  }

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

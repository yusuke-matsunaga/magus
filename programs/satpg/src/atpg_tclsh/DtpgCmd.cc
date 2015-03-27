
/// @file DtpgCmd.cc
/// @brief DtpgCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgCmd.h"
#include "YmTclpp/TclPopt.h"
#include "AtpgMgr.h"
#include "TpgNetwork.h"
#include "DtpgStats.h"
#include "DtpgEngine.h"
#include "FaultMgr.h"
#include "Fsim.h"
#include "BackTracer.h"
#include "DetectOp.h"
#include "DopList.h"
#include "UntestOp.h"
#include "UopList.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// テストパタン生成を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DtpgCmd::DtpgCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  mPoptVerbose = new TclPoptInt(this, "verbose",
				"specify verbose level (0, 1, 2, ...)");
  mPoptSat = new TclPopt(this, "sat",
			 "SAT mode");
  mPoptSatOption = new TclPoptStr(this, "sat-option",
			 "SAT option <STRING>");
  mPoptSatRec = new TclPopt(this, "satrec",
			 "SATREC mode");
  mPoptMiniSat = new TclPopt(this, "minisat",
			     "MINISAT mode");
  mPoptMiniSat2 = new TclPopt(this, "minisat2",
			     "MINISAT-2 mode");
  mPoptPrintStats = new TclPopt(this, "print_stats",
				"print statistics");
  mPoptSingle = new TclPopt(this, "single",
			    "single mode");
  mPoptSingle2 = new TclPoptInt(this, "single2",
				"single2 mode <INT>");
  mPoptSingle3 = new TclPopt(this, "single3",
			     "single3 mode");
  mPoptMulti = new TclPopt(this, "multi",
			   "multi mode");
  mPoptMulti2 = new TclPoptInt(this, "multi2",
			       "multi2 mode <INT>");
  mPoptConcurrent = new TclPopt(this, "concurrent",
				"concurrent mode");
  mPoptSmtSingle = new TclPopt(this, "smt_single",
			       "smt_single mode");
  mPoptX = new TclPoptInt(this, "x",
			  "X-extract mode [0-2]");
  mPoptDrop = new TclPopt(this, "drop",
			  "with fault drop");
  mPoptKDet = new TclPoptInt(this, "k_det",
			     "detection count");
  mPoptOpt = new TclPoptStr(this, "option",
			    "specify option string <STR>");
  mPoptVerify = new TclPopt(this, "verify",
			    "verify generated pattern");
  mPoptNoPat = new TclPopt(this, "no_pat",
			   "do not generate patterns");
  mPoptTimer = new TclPopt(this, "timer",
			   "enable timer");
  mPoptNoTimer = new TclPopt(this, "notimer",
			     "disable timer");

  new_popt_group(mPoptSat, mPoptMiniSat, mPoptMiniSat2, mPoptSatRec);
  new_popt_group(mPoptSingle, mPoptSingle2, mPoptMulti, mPoptMulti2, mPoptSmtSingle);
  new_popt_group(mPoptTimer, mPoptNoTimer);
}

// @brief デストラクタ
DtpgCmd::~DtpgCmd()
{
}

// コマンド処理関数
int
DtpgCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  // verbose level の設定
  if ( mPoptVerbose->is_specified() ) {
#if 0
    int verbose_level = mPoptVerbose->val();
    mDtpg.set_verbose_level(verbose_level);
#endif
  }

  // SAT mode の設定
  string sat_type;
  string sat_option;
  ostream* outp = NULL;
  if ( mPoptSatOption->is_specified() ) {
    sat_option = mPoptSatOption->val();
  }
  if ( mPoptSat->is_specified() ) {
    sat_type = "";
  }
  else if ( mPoptSatRec->is_specified() ) {
    sat_type = "satrec";
    outp = &cout;
  }
  else if ( mPoptMiniSat->is_specified() ) {
    sat_type = "minisat";
  }
  else if ( mPoptMiniSat2->is_specified() ) {
    sat_type = "minisat2";
  }
  else {
    sat_type = "";
  }

  bool print_stats = mPoptPrintStats->is_specified();

  string engine_type;
  ymuint mode_val = 0;
  ymuint kdet_val = 0;
  if ( mPoptSingle->is_specified() ) {
    if ( mPoptKDet->is_specified() ) {
      engine_type = "single_kdet";
      kdet_val = mPoptKDet->val();
    }
    else {
      engine_type = "single";
    }
  }
  else if ( mPoptSingle2->is_specified() ) {
    engine_type = "single2";
    mode_val = mPoptSingle2->val();
  }
  else if ( mPoptSingle3->is_specified() ) {
    engine_type = "single3";
  }
  else if ( mPoptMulti->is_specified() ) {
    engine_type = "multi";
  }
  else if ( mPoptMulti2->is_specified() ) {
    engine_type = "multi2";
    mode_val = mPoptMulti2->val();
  }
  else if ( mPoptConcurrent->is_specified() ) {
    engine_type = "concurrent";
  }
#if 0
  else if ( mPoptSmtSingle->is_specified() ) {
    engine_type = "smt_single";
  }
#endif

  string option_str = mPoptOpt->val();

  DopList dop_list;
  UopList uop_list;

  if ( !mPoptNoPat->is_specified() ) {
    dop_list.add(new_DopTvList(_tv_mgr(), _tv_list()));
  }
  dop_list.add(new_DopBase(_fault_mgr()));
  uop_list.add(new_UopBase(_fault_mgr()));

  ymuint xmode = 0;
  if ( mPoptX->is_specified() ) {
    xmode = mPoptX->val();
  }

  BackTracer* bt = NULL;
  switch ( xmode ) {
  case 1: bt = new_BtJust1(); break;
  case 2: bt = new_BtJust2(); break;
  case 3: bt = new_BtJust3(); break;
  default: bt = new_BtSimple(); break;
  }
  if ( bt != NULL ) {
    bt->set_max_id(_network().max_node_id());
  }

  if ( mPoptDrop->is_specified() ) {
    dop_list.add(new_DopDrop(_fault_mgr(), _fsim3()));
  }
  if ( mPoptVerify->is_specified() ) {
    dop_list.add(new_DopVerify(_fsim3()));
  }

  bool timer_enable = true;
  if ( mPoptNoTimer->is_specified() ) {
    timer_enable = false;
  }

  Fsim& fsim3 = _fsim3();
  fsim3.set_faults(_fault_mgr().remain_list());

  DtpgEngine* engine = NULL;
  if ( engine_type == "single" ) {
    engine = new_DtpgSatS(sat_type, sat_option, outp, *bt, dop_list, uop_list);
  }
  else if ( engine_type == "single_kdet" ) {
    engine = new_DtpgSatKDet(sat_type, sat_option, outp, *bt, dop_list, uop_list, kdet_val);
  }
  else if ( engine_type == "single2" ) {
    engine = new_DtpgSatS2(mode_val, sat_type, sat_option, outp, *bt, dop_list, uop_list);
  }
  else if ( engine_type == "single3" ) {
    engine = new_DtpgSatS3(sat_type, sat_option, outp, *bt, dop_list, uop_list);
  }
  else if ( engine_type == "multi" ) {
    engine = new_DtpgSatM(sat_type, sat_option, outp, *bt, dop_list, uop_list);
  }
  else if ( engine_type == "multi2" ) {
    engine = new_DtpgSatM2(mode_val, sat_type, sat_option, outp, *bt, dop_list, uop_list);
  }
  else if ( engine_type == "concurrent" ) {
    engine = new_DtpgSatC(sat_type, sat_option, outp, *bt, dop_list, uop_list);
  }
#if 0
  else if ( engine_type == "smt_single" ) {
    engine = new_SmtEngineSingle(sat_type, sat_option, outp, *bt, dop_list, uop_list);
  }
#endif
  else {
    // デフォルトフォールバック
    engine = new_DtpgSatS(sat_type, sat_option, outp, *bt, dop_list, uop_list);
  }

  engine->set_option(option_str);
  engine->timer_enable(timer_enable);

  DtpgStats stats;
  engine->run(_network(), stats);

  delete engine;

  after_update_faults();

  // -print_stats オプションの処理
  if ( print_stats ) {
    ios::fmtflags save = cout.flags();
    cout.setf(ios::fixed, ios::floatfield);
    if ( stats.mCnfGenCount > 0 ) {
      cout << "CNF generation" << endl
	   << "  " << setw(10) << stats.mCnfGenCount
	   << "  " << stats.mCnfGenTime
	   << "  " << setw(8) << stats.mCnfGenTime.usr_time_usec() / stats.mCnfGenCount
	   << "u usec"
	   << "  " << setw(8) << stats.mCnfGenTime.sys_time_usec() / stats.mCnfGenCount
	   << "s usec" << endl;
    }
    if ( stats.mDetCount > 0 ) {
      cout << endl
	   << "*** SAT instances (" << stats.mDetCount << ") ***" << endl
	   << "Total CPU time  (s)            = " << setw(10) << stats.mDetTime.usr_time() << "u"
	   << " " << setw(8) << stats.mDetTime.sys_time() << "s" << endl
	   << "Ave. CPU time (usec)           = "
	   << setw(10) << stats.mDetTime.usr_time_usec() / stats.mDetCount
	   << "u"
	   << " " << setw(8) << stats.mDetTime.sys_time_usec() / stats.mDetCount
	   << "s" << endl

	   << "# of restarts (Ave./Max)       = "
	   << setw(10) << (double) stats.mDetStats.mRestart / stats.mDetCount
	   << " / " << setw(8) << stats.mDetStatsMax.mRestart << endl

	   << "# of conflicts (Ave./Max)      = "
	   << setw(10) << (double) stats.mDetStats.mConflictNum / stats.mDetCount
	   << " / " << setw(8) << stats.mDetStatsMax.mConflictNum << endl

	   << "# of decisions (Ave./Max)      = "
	   << setw(10) << (double) stats.mDetStats.mDecisionNum / stats.mDetCount
	   << " / " << setw(8) << stats.mDetStatsMax.mDecisionNum << endl

	   << "# of implications (Ave./Max)   = "
	   << setw(10) << (double) stats.mDetStats.mPropagationNum / stats.mDetCount
	   << " / " << setw(8) << stats.mDetStatsMax.mPropagationNum << endl;
    }
    if ( stats.mRedCount > 0 ) {
      cout << endl
	   << "*** UNSAT instances (" << stats.mRedCount << ") ***" << endl
	   << "Total CPU time  (s)            = " << setw(10) << stats.mRedTime.usr_time() << "u"
	   << " " << setw(8) << stats.mRedTime.sys_time() << "s" << endl
	   << "Ave. CPU time (usec)           = "
	   << setw(10) << stats.mRedTime.usr_time_usec() / stats.mRedCount
	   << "u"
	   << " " << setw(8) << stats.mRedTime.sys_time_usec() / stats.mRedCount
	   << "s" << endl

	   << "# of restarts (Ave./Max)       = "
	   << setw(10) << (double) stats.mRedStats.mRestart / stats.mRedCount
	   << " / " << setw(8) << stats.mRedStatsMax.mRestart << endl

	   << "# of conflicts (Ave./Max)      = "
	   << setw(10) << (double) stats.mRedStats.mConflictNum / stats.mRedCount
	   << " / " << setw(8) << stats.mRedStatsMax.mConflictNum << endl

	   << "# of decisions (Ave./Max)      = "
	   << setw(10) << (double) stats.mRedStats.mDecisionNum / stats.mRedCount
	   << " / " << setw(8) << stats.mRedStatsMax.mDecisionNum << endl

	   << "# of implications (Ave./Max)   = "
	   << setw(10) << (double) stats.mRedStats.mPropagationNum / stats.mRedCount
	   << " / " << setw(8) << stats.mRedStatsMax.mPropagationNum << endl;
    }
    if ( stats.mPartRedCount > 0 ) {
      cout << endl
	   << "*** Partial UNSAT instances (" << stats.mPartRedCount << ") ***" << endl
	   << "Total CPU time  (s)            = " << setw(10) << stats.mPartRedTime.usr_time() << "u"
	   << " " << setw(8) << stats.mPartRedTime.sys_time() << "s" << endl
	   << "Ave. CPU time (usec)           = "
	   << setw(10) << stats.mPartRedTime.usr_time_usec() / stats.mPartRedCount
	   << "u"
	   << " " << setw(8) << stats.mPartRedTime.sys_time_usec() / stats.mPartRedCount
	   << "s" << endl

	   << "# of restarts (Ave./Max)       = "
	   << setw(10) << (double) stats.mPartRedStats.mRestart / stats.mPartRedCount
	   << " / " << setw(8) << stats.mPartRedStatsMax.mRestart << endl

	   << "# of conflicts (Ave./Max)      = "
	   << setw(10) << (double) stats.mPartRedStats.mConflictNum / stats.mPartRedCount
	   << " / " << setw(8) << stats.mPartRedStatsMax.mConflictNum << endl

	   << "# of decisions (Ave./Max)      = "
	   << setw(10) << (double) stats.mPartRedStats.mDecisionNum / stats.mPartRedCount
	   << " / " << setw(8) << stats.mPartRedStatsMax.mDecisionNum << endl

	   << "# of implications (Ave./Max)   = "
	   << setw(10) << (double) stats.mPartRedStats.mPropagationNum / stats.mPartRedCount
	   << " / " << setw(8) << stats.mPartRedStatsMax.mPropagationNum << endl;
    }
    if ( stats.mAbortCount > 0 ) {
      cout << endl
	   << "*** ABORT instances ***" << endl
	   << "  " << setw(10) << stats.mAbortCount
	   << "  " << stats.mAbortTime
	   << "  " << setw(8) << stats.mAbortTime.usr_time_usec() / stats.mAbortCount
	   << "u usec"
	   << "  " << setw(8) << stats.mAbortTime.sys_time_usec() / stats.mAbortCount
	   << "s usec" << endl;
    }
    cout.flags(save);
  }

  // stats の結果を TCL 変数にセットする．
  TclObj base("::atpg::dtpg_stats");
  set_var(base, "cnf_count",
	  stats.mCnfGenCount,
	  TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
  set_var(base, "cnf_time",
	  stats.mCnfGenTime.usr_time(),
	  TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
  set_var(base, "det_count",
	  stats.mDetCount,
	  TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
  set_var(base, "det_time",
	  stats.mDetTime.usr_time(),
	  TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
  set_var(base, "part_red_count",
	  stats.mPartRedCount,
	  TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
  set_var(base, "part_red_time",
	  stats.mPartRedTime.usr_time(),
	  TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
  set_var(base, "red_count",
	  stats.mRedCount,
	  TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
  set_var(base, "red_time",
	  stats.mRedTime.usr_time(),
	  TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

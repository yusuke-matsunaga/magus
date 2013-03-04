
/// @file src/atpg_tclsh/DtpgCmd.cc
/// @brief DtpgCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgCmd.h"
#include "ym_tclpp/TclPopt.h"
#include "AtpgMgr.h"
#include "DtpgStats.h"


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
  mPoptSatRec = new TclPopt(this, "satrec",
			 "SATREC mode");
  mPoptMiniSat = new TclPopt(this, "minisat",
			     "MINISAT mode");
  mPoptFsim = new TclPoptStr(this, "fsim",
			     "specify FSIM mode (none, single, ppsfp)");
  mPoptPrintStats = new TclPopt(this, "print_stats",
				"print statistics");
  mPoptSingle = new TclPopt(this, "single",
			    "dual mode");
  mPoptDual = new TclPopt(this, "dual",
			  "dual mode");
  mPoptNode = new TclPopt(this, "node",
			  "node mode");
  mPoptFFR = new TclPopt(this, "ffr",
			  "FFR mode");
  mPoptMFFC = new TclPopt(this, "mffc",
			  "MFFC mode");
  mPoptAll = new TclPopt(this, "all",
			 "all mode");
  mPoptPo = new TclPopt(this, "po",
			"po-split mode");
  mPoptRpo = new TclPopt(this, "rpo",
			 "po-split (reverse order) mode");
  mPoptSkip = new TclPoptInt(this, "skip",
			     "specify skip count <INT>");
  mPoptX = new TclPoptInt(this, "x",
			  "X-extract mode [0-1]");
  mPoptDrop = new TclPopt(this, "drop",
			  "with fault drop");
  mPoptVerify = new TclPopt(this, "verify",
			    "verify generated pattern");
  mPoptTimer = new TclPopt(this, "timer",
			   "enable timer");

  new_popt_group(mPoptSat, mPoptMiniSat, mPoptSatRec);
  new_popt_group(mPoptSingle, mPoptDual, mPoptNode, mPoptFFR, mPoptMFFC, mPoptAll);
  new_popt_group(mPoptPo, mPoptRpo);
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
  if ( mPoptSat->is_specified() ) {
    mgr().set_dtpg_mode("", "classic");
  }
  else if ( mPoptSatRec->is_specified() ) {
    mgr().set_dtpg_mode("satrec");
  }
  else if ( mPoptMiniSat->is_specified() ) {
    mgr().set_dtpg_mode("minisat");
  }

  // fsim_mode の設定
  tSimMode fsim_mode = kSimPpsfp;
  if ( mPoptFsim->is_specified() ) {
    string str = mPoptFsim->val();
    if ( str == "none" ) {
      fsim_mode = kSimNone;
    }
    else if ( str == "single" ) {
      fsim_mode = kSimSingle;
    }
    else if ( str == "ppsfp" ) {
      fsim_mode = kSimPpsfp;
    }
    else {
      print_usage();
      return TCL_ERROR;
    }
  }

  // backtrack limit の設定
#if 0
  if ( mPoptBacktrack->is_specified() ) {
    int btnum = mPoptBacktrack->val();
    mDtpg.set_backtrack_limit(btnum);
  }
  else {
    mDtpg.set_default_backtrack_limit();
  }
#endif

  bool print_stats = mPoptPrintStats->is_specified();

  tDtpgMode mode = kDtpgSingle;
  if ( mPoptSingle->is_specified() ) {
    mode = kDtpgSingle;
  }
  else if ( mPoptDual->is_specified() ) {
    mode = kDtpgDual;
  }
  else if ( mPoptNode->is_specified() ) {
    mode = kDtpgNode;
  }
  else if ( mPoptFFR->is_specified() ) {
    mode = kDtpgFFR;
  }
  else if ( mPoptMFFC->is_specified() ) {
    mode = kDtpgMFFC;
  }
  else if ( mPoptAll->is_specified() ) {
    mode = kDtpgAll;
  }

  bool po_mode = mPoptPo->is_specified();
  bool rpo_mode = mPoptRpo->is_specified();
  int skip_count = 0;
  if ( mPoptSkip->is_specified() ) {
    skip_count = mPoptSkip->val();
  }

  if ( mPoptX->is_specified() ) {
    mgr().set_dtpg_xmode(mPoptX->val());
  }

  mgr().set_dtpg_drop_mode(mPoptDrop->is_specified());

  mgr().set_dtpg_verify_mode(mPoptVerify->is_specified());

  mgr().set_dtpg_timer(mPoptTimer->is_specified());

  mgr().clear_stats();

  string dtpg_option = "";
  if ( po_mode ) {
    dtpg_option = "po";
  }
  if ( rpo_mode ) {
    dtpg_option = "rpo";
  }

  mgr().dtpg(mode, skip_count, dtpg_option);

  after_update_faults();

  if ( print_stats ) {
    DtpgStats stats;
    mgr().get_stats(stats);
#if 0
    const DtpgStats& stats = mDtpg.stats();
    cout << "********** dtpg **********" << endl
	 << setw(10) << stats.detected_faults()
	 << ": # of detected faults" << endl
	 << setw(10) << stats.untestable_faults()
	 << ": # fo redundant faults" << endl
	 << setw(10) << stats.aborted_faults()
	 << ": # of aborted faults" << endl
	 << setw(10) << stats.generated_patterns()
	 << ": # of generated patterns" << endl
	 << setw(10) << stats.call_count()
	 << ": # of dtpg runs" << endl
	 << setw(10) << stats.backtrack_count()
	 << ": # of total backtracks" << endl
	 << stats.time() << endl;
#endif
  }

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

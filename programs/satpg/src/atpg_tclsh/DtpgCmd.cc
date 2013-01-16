
/// @file src/atpg_tclsh/DtpgCmd.cc
/// @brief DtpgCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgCmd.h"
#include "ym_tclpp/TclPopt.h"
#include "AtpgMgr.h"
#include "ym_networks/TgNode.h"


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
  mPoptFFR = new TclPopt(this, "ffr",
			  "FFR mode");
  mPoptMFFC = new TclPopt(this, "mffc",
			  "MFFC mode");
  mPoptAll = new TclPopt(this, "all",
			 "all mode");
  mPoptPo = new TclPopt(this, "po",
			"po-split mode");
  mPoptSkip = new TclPopt(this, "skip",
			  "skip mode");
  mPoptX = new TclPoptInt(this, "x",
			  "X-extract mode [0-1]");
  mPoptVerify = new TclPopt(this, "verify",
			    "verify generated pattern");
  new_popt_group(mPoptSat, mPoptMiniSat, mPoptSatRec);
  new_popt_group(mPoptDual, mPoptFFR, mPoptMFFC, mPoptAll);
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

  bool single_mode = mPoptSingle->is_specified();
  bool dual_mode = mPoptDual->is_specified();
  bool ffr_mode = mPoptFFR->is_specified();
  bool mffc_mode = mPoptMFFC->is_specified();
  bool all_mode = mPoptAll->is_specified();
  bool po_mode = mPoptPo->is_specified();
  bool skip_mode = mPoptSkip->is_specified();

  if ( mPoptX->is_specified() ) {
    mgr().set_dtpg_xmode(mPoptX->val());
  }

  if ( mPoptVerify->is_specified() ) {
    mgr().set_dtpg_verify_mode(true);
  }

  mgr().clear_stats();

  string dtpg_option;
  if ( ffr_mode ) {
    dtpg_option = "ffr";
  }
  else if ( mffc_mode ) {
    dtpg_option = "mffc";
  }
  else if ( all_mode ) {
    dtpg_option = "all";
  }
  else if ( dual_mode ) {
    dtpg_option = "dual";
  }
  else {
    single_mode = true;
  }
  if ( single_mode ) {
    dtpg_option = "single";
  }
  if ( po_mode ) {
    dtpg_option += "_po";
  }
  if ( skip_mode ) {
    dtpg_option += "_skip";
  }

  mgr().dtpg(dtpg_option);

  after_update_faults();

  if ( print_stats ) {
    mgr().get_stats();
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

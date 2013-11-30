
/// @file src/atpg_tclsh/DtpgCmd.cc
/// @brief DtpgCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgCmd.h"
#include "ym_tclpp/TclPopt.h"
#include "AtpgMgr.h"
#include "TpgNetwork.h"
#include "DtpgStats.h"
#include "BackTracer.h"
#include "DetectOp.h"
#include "UntestOp.h"


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
  mPoptFFR2 = new TclPoptInt(this, "ffr2",
			     "FFR2 mode");
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
			  "X-extract mode [0-2]");
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

  bool print_stats = mPoptPrintStats->is_specified();

  tDtpgMode mode = kDtpgSingle;
  ymuint ffr2_limit = 0;
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
  else if ( mPoptFFR2->is_specified() ) {
    mode = kDtpgFFR2;
    ffr2_limit = mPoptFFR2->val();
  }
  else if ( mPoptMFFC->is_specified() ) {
    mode = kDtpgMFFC;
  }
  else if ( mPoptAll->is_specified() ) {
    mode = kDtpgAll;
  }

  vector<DetectOp*> dop_list;
  vector<UntestOp*> uop_list;

  dop_list.push_back(new_DopTvList(mgr()));
  dop_list.push_back(new_DopBase(mgr()));

  bool po_flag = mPoptPo->is_specified();
  bool rpo_flag = mPoptRpo->is_specified();
  int skip_count = 0;
  if ( (po_flag || rpo_flag) && mPoptSkip->is_specified() ) {
    skip_count = mPoptSkip->val();
  }
  if ( skip_count > 0 ) {
    uop_list.push_back(new_UopSkip(mgr(), skip_count));
  }
  else {
    uop_list.push_back(new_UopBase(mgr()));
  }

  ymuint xmode = 0;
  if ( mPoptX->is_specified() ) {
    xmode = mPoptX->val();
  }
  TvMgr& tvmgr = mgr()._tv_mgr();
  ymuint max_id = mgr()._network().node_num();
  BackTracer* bt = NULL;
  switch ( xmode ) {
  case 1: bt = new_BtJust1(tvmgr, max_id); break;
  case 2: bt = new_BtJust2(tvmgr, max_id); break;
  case 3: bt = new_BtZdd(tvmgr, max_id); break;
  default: bt = new_BtSimple(tvmgr, max_id); break;
  }

  if ( mPoptDrop->is_specified() ) {
    dop_list.push_back(new_DopDrop(mgr()));
  }
  if ( mPoptVerify->is_specified() ) {
    dop_list.push_back(new_DopVerify(mgr()));
  }

#if 0
  mgr().set_dtpg_timer(mPoptTimer->is_specified());
#else
  mgr().set_dtpg_timer(print_stats);
#endif

  tDtpgPoMode po_mode = kDtpgPoNone;
  if ( po_flag ) {
    po_mode = kDtpgPoInc;
  }
  if ( rpo_flag ) {
    po_mode = kDtpgPoDec;
  }

  DtpgStats stats;

  mgr().dtpg(DtpgMode(mode, ffr2_limit), po_mode, *bt, dop_list, uop_list, stats);

  for (vector<DetectOp*>::iterator p = dop_list.begin();
       p != dop_list.end(); ++ p) {
    delete *p;
  }
  for (vector<UntestOp*>::iterator p = uop_list.begin();
       p != uop_list.end(); ++ p) {
    delete *p;
  }

  after_update_faults();

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
      cout << "SAT" << endl
	   << "  " << setw(10) << stats.mDetCount
	   << "  " << stats.mDetTime
	   << "  " << setw(8) << stats.mDetTime.usr_time_usec() / stats.mDetCount
	   << "u usec"
	   << "  " << setw(8) << stats.mDetTime.sys_time_usec() / stats.mDetCount
	   << "s usec" << endl;
    }
    if ( stats.mRedCount > 0 ) {
      cout << "UNSAT" << endl
	   << "  " << setw(10) << stats.mRedCount
	   << "  " << stats.mRedTime
	   << "  " << setw(8) << stats.mRedTime.usr_time_usec() / stats.mRedCount
	   << "u usec"
	   << "  " << setw(8) << stats.mRedTime.sys_time_usec() / stats.mRedCount
	   << "s usec" << endl;
    }
    if ( stats.mAbortCount > 0 ) {
      cout << "ABORT" << endl
	   << "  " << setw(10) << stats.mAbortCount
	   << "  " << stats.mAbortTime
	   << "  " << setw(8) << stats.mAbortTime.usr_time_usec() / stats.mAbortCount
	   << "u usec"
	   << "  " << setw(8) << stats.mAbortTime.sys_time_usec() / stats.mAbortCount
	   << "s usec" << endl;
    }
    cout.flags(save);

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

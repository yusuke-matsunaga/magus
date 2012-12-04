
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
  new_popt_group(mPoptSat, mPoptMiniSat, mPoptSatRec);
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
    mgr().set_dtpg_mode();
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

  // GO!
  //mDtpg(fsim_mode);
  FaultMgr& fmgr = mgr()._fault_mgr();
  const vector<SaFault*>& flist = fmgr.remain_list();
  TvMgr& tvmgr = mgr()._tv_mgr();
  TestVector* tmp_tv = tvmgr.new_vector();
  vector<TestVector*>& tv_list = mgr()._tv_list();

  vector<SaFault*> cur_flist(flist.begin(), flist.end());
  for (vector<SaFault*>::const_iterator p = cur_flist.begin();
       p != cur_flist.end(); ++ p) {
    SaFault* f = *p;
    if ( f->status() != kFsUndetected ) {
      continue;
    }

    const TgNode* fnode = f->node();
    tStat stat;
    if ( f->is_input_fault() ) {
      ymuint pos = f->pos();
      const TgNode* fsrc = fnode->fanin(pos);
      stat = mgr().dtpg_single(fnode, pos, fsrc, f->val(), tmp_tv);
    }
    else {
      stat = mgr().dtpg_single(fnode, f->val(), tmp_tv);
    }
    if ( stat == kUntest ) {
      // 検出不能
      fmgr.set_status(f, kFsUntestable);
    }
    else if ( stat == kDetect ) {
      // 検出
      fmgr.set_status(f, kFsDetected);
      tv_list.push_back(tmp_tv);
      tmp_tv = tvmgr.new_vector();
    }
  }

  tvmgr.delete_vector(tmp_tv);

  fmgr.update();
  after_update_faults();

  if ( print_stats ) {
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

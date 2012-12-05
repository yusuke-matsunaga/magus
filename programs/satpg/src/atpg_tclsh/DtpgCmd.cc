
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

BEGIN_NONAMESPACE

void
update_stat(tStat stat,
	    FaultMgr& fmgr,
	    SaFault* f,
	    TestVector* tv,
	    vector<TestVector*>& tv_list,
	    TvMgr& tvmgr)
{
  if ( stat == kUntest ) {
    // 検出不能
    fmgr.set_status(f, kFsUntestable);
  }
  else if ( stat == kDetect ) {
    // 検出
    fmgr.set_status(f, kFsDetected);
    TestVector* tmp_tv = tvmgr.new_vector();
    tmp_tv->copy(*tv);
    tv_list.push_back(tmp_tv);
  }
}

END_NONAMESPACE

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
  mPoptDual = new TclPopt(this, "dual",
			  "dual mode");
  mPoptFFR = new TclPopt(this, "ffr",
			  "FFR mode");
  new_popt_group(mPoptSat, mPoptMiniSat, mPoptSatRec);
  new_popt_group(mPoptDual, mPoptFFR);
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

  bool dual_mode = mPoptDual->is_specified();
  bool ffr_mode = mPoptFFR->is_specified();

  FaultMgr& fmgr = mgr()._fault_mgr();
  const vector<SaFault*>& flist = fmgr.remain_list();
  TvMgr& tvmgr = mgr()._tv_mgr();
  TestVector* tmp_tv = tvmgr.new_vector();
  TestVector* tmp_tv1 = tvmgr.new_vector();
  TestVector* tmp_tv2[2] = {tmp_tv, tmp_tv1};
  vector<TestVector*>& tv_list = mgr()._tv_list();

  if ( ffr_mode ) {
    const vector<TgFFR*>& ffr_list = mgr()._ffr_list();
    for (vector<TgFFR*>::const_iterator p = ffr_list.begin();
	 p != ffr_list.end(); ++ p) {
      TgFFR* ffr = *p;

      const vector<const TgNode*>& node_list = ffr->node_list();
      vector<SaFault*> flist;
      for (vector<const TgNode*>::const_iterator q = node_list.begin();
	   q != node_list.end(); ++ q) {
	const TgNode* node = *q;
	SaFault* fo0 = fmgr.find_ofault(node, 0);
	if ( fo0->rep() == fo0 && fo0->status() == kFsUndetected ) {
	  flist.push_back(fo0);
	}
	SaFault* fo1 = fmgr.find_ofault(node, 1);
	if ( fo1->rep() == fo1 && fo1->status() == kFsUndetected ) {
	  flist.push_back(fo1);
	}
	ymuint ni = node->ni();
	for (ymuint i = 0; i < ni; ++ i) {
	  SaFault* fi0 = fmgr.find_ifault(node, i, 0);
	  if ( fi0->rep() == fi0 && fi0->status() == kFsUndetected ) {
	    flist.push_back(fi0);
	  }
	  SaFault* fi1 = fmgr.find_ifault(node, i, 1);
	  if ( fi1->rep() == fi1 && fi1->status() == kFsUndetected ) {
	    flist.push_back(fi1);
	  }
	}
      }
      ymuint nf = flist.size();
      vector<TestVector*> tv_list1(nf);
      vector<tStat> stat_list(nf);
      for (ymuint i = 0; i < nf; ++ i) {
	tv_list1[i] = tvmgr.new_vector();
      }

      mgr().dtpg_ffr(ffr, flist, tv_list1, stat_list);

      for (ymuint i = 0; i < nf; ++ i) {
	update_stat(stat_list[i], fmgr, flist[i], tv_list1[i], tv_list, tvmgr);
	tvmgr.delete_vector(tv_list1[i]);
      }
    }
  }
  else {
    vector<SaFault*> cur_flist(flist.begin(), flist.end());
    for (vector<SaFault*>::const_iterator p = cur_flist.begin();
	 p != cur_flist.end(); ++ p) {
      SaFault* f = *p;
      if ( f->status() != kFsUndetected ) {
	continue;
      }

      const TgNode* fnode = f->node();
      SaFault* alt_f = fmgr.find_alt_fault(f);
      if ( dual_mode && alt_f->status() == kFsUndetected ) {
	pair<tStat, tStat> stats;
	if ( f->is_input_fault() ) {
	  ymuint pos = f->pos();
	  const TgNode* fsrc = fnode->fanin(pos);
	  stats = mgr().dtpg_dual(fnode, pos, fsrc, tmp_tv2);
	}
	else {
	  stats = mgr().dtpg_dual(fnode, tmp_tv2);
	}
	SaFault* f0;
	SaFault* f1;
	if ( f->val() == 0 ) {
	  f0 = f;
	  f1 = alt_f;
	}
	else {
	  f0 = alt_f;
	  f1 = f;
	}
	update_stat(stats.first, fmgr, f0, tmp_tv2[0], tv_list, tvmgr);
	update_stat(stats.second, fmgr, f1, tmp_tv2[1], tv_list, tvmgr);
      }
      else {
	tStat stat;
	if ( f->is_input_fault() ) {
	  ymuint pos = f->pos();
	  const TgNode* fsrc = fnode->fanin(pos);
	  stat = mgr().dtpg_single(fnode, pos, fsrc, f->val(), tmp_tv);
	}
	else {
	  stat = mgr().dtpg_single(fnode, f->val(), tmp_tv);
	}
	update_stat(stat, fmgr, f, tmp_tv, tv_list, tvmgr);
      }
    }
  }

  tvmgr.delete_vector(tmp_tv);
  tvmgr.delete_vector(tmp_tv1);

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

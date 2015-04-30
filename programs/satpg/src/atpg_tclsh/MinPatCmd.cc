
/// @file MinPatCmd.cc
/// @brief MinPatCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatCmd.h"
#include "MinPat.h"
#include "MinPatStats.h"
#include "YmTclpp/TclPopt.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 故障シミュレーションを実行するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatCmd::MinPatCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  mPoptSimple = new TclPopt(this, "simple",
			    "simple heuristic");
  mPoptSimple2 = new TclPopt(this, "simple2",
			     "simple2 heuristic");
  mPoptDsatur = new TclPopt(this, "dsatur",
			    "Dsatur heuristic");
  mPoptDsatur2 = new TclPopt(this, "dsatur2",
			     "Dsatur-2 heuristic");
  mPoptGroupDominance = new TclPopt(this, "group-dominance",
				    "use group dominance");
  mPoptFaultDominance = new TclPopt(this, "fault-dominance",
				    "use fault dominance");
  mPoptPrintStats = new TclPopt(this, "print_stats",
				"print statistics");
  mPoptVerbose = new TclPoptInt(this, "verbose",
				"<int>: specify verbose-level");
  mPoptDomMethod = new TclPoptInt(this, "dom-method",
				  "<int>: specify dominance check method(0-2)");
  mPoptExact = new TclPopt(this, "exact",
			   "exact fault grouping");
  mPoptCompaction = new TclPopt(this, "compaction",
				"do compaction");
}

// @brief デストラクタ
MinPatCmd::~MinPatCmd()
{
}

// コマンド処理関数
int
MinPatCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  bool print_stats = mPoptPrintStats->is_specified();

  bool simple = mPoptSimple->is_specified();
  bool simple2 = mPoptSimple2->is_specified();
  bool dsatur = mPoptDsatur->is_specified();
  bool dsatur2 = mPoptDsatur2->is_specified();
  int verbose = mPoptVerbose->is_specified() ? mPoptVerbose->val() : 0;
  bool group_dominance = mPoptGroupDominance->is_specified();
  bool exact = mPoptExact->is_specified();
  bool compaction = mPoptCompaction->is_specified();

  MinPat* minpat = NULL;

  if ( simple ) {
    minpat = new_MinPatSimple(group_dominance);
  }
  else if ( simple2 ) {
    minpat = new_MinPatSimple2(group_dominance);
  }
  else if ( dsatur ) {
    minpat = new_MinPatDsatur();
  }
  else if ( dsatur2 ) {
    minpat = new_MinPatDsatur2();
  }
  else {
    minpat = new_MinPat(group_dominance);
  }

  minpat->set_verbose(verbose);

  if ( mPoptDomMethod->is_specified() ) {
    minpat->set_dom_method(mPoptDomMethod->val());
  }

  USTime time;
  minpat->run(_network(), _tv_mgr(),  _fsim(), exact, compaction, _tv_list(), time);

  delete minpat;

  if ( print_stats ) {
    cout << " # of patterns: "
	 << setw(10) << _tv_list().size()
	 << ": " << time << endl;
  }

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG


/// @file MinPat2Cmd.cc
/// @brief MinPat2Cmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPat2Cmd.h"
#include "MinPat.h"
#include "MinPatStats.h"
#include "YmTclpp/TclPopt.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 故障シミュレーションを実行するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPat2Cmd::MinPat2Cmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  mPoptPrintStats = new TclPopt(this, "print_stats",
				"print statistics");
}

// @brief デストラクタ
MinPat2Cmd::~MinPat2Cmd()
{
}

// コマンド処理関数
int
MinPat2Cmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  bool print_stats = mPoptPrintStats->is_specified();

  MinPat* minpat = new_MinPat2();

  MinPatStats stats;
  minpat->run(_network(), _tv_mgr(), _fault_mgr(), _fsim(), _fsim3(), _tv_list(), stats);

  delete minpat;

  if ( print_stats ) {
    cout << "********** minpat **********" << endl
	 << setw(10) << stats.original_patterns()
	 << ": # of original patterns" << endl
	 << setw(10) << stats.minimized_patterns()
	 << ": # of minimized patterns" << endl
	 << stats.time() << endl;
  }

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

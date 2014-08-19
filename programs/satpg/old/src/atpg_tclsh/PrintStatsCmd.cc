
/// @file src/atpg_tclsh/PrintStatsCmd.cc
/// @brief ランダムパタン生成を行うコマンド
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "PrintStatsCmd.h"
#include "AtpgMgr.h"
#include "FaultMgr.h"
#include "TvMgr.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintStatsCmd::PrintStatsCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  mStopWatch.start();
}

// @brief デストラクタ
PrintStatsCmd::~PrintStatsCmd()
{
}

// コマンド処理関数
int
PrintStatsCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  USTime lap = mStopWatch.time();

  USTime r_time = mgr().read_time();
  USTime d_time = mgr().dtpg_time();
  USTime f_time = mgr().fsim_time();
  USTime s_time = mgr().sat_time();
  USTime m_time = mgr().misc_time();

  FaultMgr& fmgr = _fault_mgr();
  ymuint num = mgr()._tv_list().size();

  fprintf(stdout, "#A: # of total faults       = %7d\n", fmgr.all_rep_num());
  fprintf(stdout, "#B: # of detected faults    = %7d\n", fmgr.det_num());
  fprintf(stdout, "#C: # of redundant faults   = %7d\n", fmgr.untest_num());
  fprintf(stdout, "#D: # of undetected faults  = %7d\n", fmgr.remain_num());
  fprintf(stdout, "#E: # of generated patterns = %7d\n", num);
  fprintf(stdout, "#F:   Total CPU time        = %7.2fu %7.2fs\n",
	  lap.usr_time(), lap.sys_time());
  fprintf(stdout, "#G:    (read time           = %7.2f)\n",
	  r_time.usr_time());
  fprintf(stdout, "#H:    (dtpg time           = %7.2f)\n",
	  d_time.usr_time());
  fprintf(stdout, "#I:    (fsim time           = %7.2f)\n",
	  f_time.usr_time());
  fprintf(stdout, "#J:    (sat  time           = %7.2f)\n",
	  s_time.usr_time());
  fprintf(stdout, "#K:    (misc time           = %7.2f)\n",
	  m_time.usr_time());

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

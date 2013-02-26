
/// @file src/atpg_tclsh/FaultCmd.cc
/// @brief FaultCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "FaultCmd.h"
#include "AtpgMgr.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 故障の設定を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SetFault::SetFault(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
}

// コマンド処理関数
int
SetFault::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();

  if ( objc > 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  const TgNetwork& network = _network();
  FaultMgr& fault_mgr = _fault_mgr();
  fault_mgr.set_ssa_fault(network);

  after_update_faults();

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

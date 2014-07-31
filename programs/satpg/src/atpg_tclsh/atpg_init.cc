
/// @file atpg_init.cc
/// @brief satpg のパッケージ初期化
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include <tcl.h>
#include "YmTclpp/TclCmd.h"
#include "AtpgMgr.h"
#include "AtpgCmd.h"
#include "NtwkIoCmd.h"
#include "RtpgCmd.h"
#include "DtpgCmd.h"
#include "MinPatCmd.h"
#include "PrintFaultCmd.h"
#include "PrintPatCmd.h"
#include "PrintPatStatsCmd.h"
#include "PrintStatsCmd.h"
#include "TimeCmd.h"


BEGIN_NAMESPACE_YM_SATPG

int
atpg_init(Tcl_Interp* interp)
{
  //////////////////////////////////////////////////////////////////////
  // atpg という namespace を作っておく．
  //////////////////////////////////////////////////////////////////////
  if ( Tcl_Eval(interp, "namespace eval atpg {}") == TCL_ERROR ) {
    return TCL_ERROR;
  }


  //////////////////////////////////////////////////////////////////////
  // サブモジュールの初期化
  //////////////////////////////////////////////////////////////////////

  AtpgMgr* mgr = new AtpgMgr();

  TclCmdBinder1<ReadBlif, AtpgMgr*>::reg(interp, mgr, "::atpg::read_blif");
  TclCmdBinder1<ReadIscas89, AtpgMgr*>::reg(interp, mgr, "::atpg::read_iscas89");
  TclCmdBinder1<PrintFaultCmd, AtpgMgr*>::reg(interp, mgr, "::atpg::print_fault");
  TclCmdBinder1<PrintStatsCmd, AtpgMgr*>::reg(interp, mgr, "::atpg::print_stats");
  TclCmdBinder1<PrintPatCmd, AtpgMgr*>::reg(interp, mgr, "::atpg::print_pat");
  TclCmdBinder1<PrintPatStatsCmd, AtpgMgr*>::reg(interp, mgr, "::atpg::print_pat_stats");
  TclCmdBinder1<RtpgCmd, AtpgMgr*>::reg(interp, mgr, "::atpg::rtpg");
  TclCmdBinder1<DtpgCmd, AtpgMgr*>::reg(interp, mgr, "::atpg::dtpg");
  TclCmdBinder1<MinPatCmd, AtpgMgr*>::reg(interp, mgr, "::atpg::minpat");
  TclCmdBinder<StopwatchCls>::reg(interp, "atpg::stopwatch");
  TclCmdBinder<TimeCmd>::reg(interp,     "atpg::time");


  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  ostringstream buf;
  buf << "namespace eval tclreadline {" << endl
      << "namespace eval atpg {" << endl
      << "proc complete(read_blif) { t s e l p m } { return \"\" }" << endl
      << "proc complete(read_iscas89) { t s e l p m } { return \"\" }" << endl
      << "proc complete(print_fault) { t s e l p m } { return \"\" }" << endl
      << "proc complete(print_stats) { t s e l p m } { return \"\" }" << endl
      << "proc complete(print_pat) { t s e l p m } { return \"\" }" << endl
      << "proc complete(print_pat_stats) { t s e l p m } { return \"\" }" << endl
      << "proc complete(rtpg) { t s e l p m } { return \"\" }" << endl
      << "proc complete(dtpg) { t s e l p m } { return \"\" }" << endl
      << "proc complete(minpat) { t s e l p m } { return \"\" }" << endl
      << "proc complete(stopwatch) { t s e l p m } { return \"\" }" << endl
      << "proc complete(time) { t s e l p m } { return \"\" }" << endl
      << "}" << endl
      << "}" << endl;
  string str = buf.str();
  if ( Tcl_Eval(interp, str.c_str()) == TCL_ERROR ) {
    return TCL_ERROR;
  }


  // もしも ATPG_LIBRARY という環境変数が定義されていたらそれを auto_path
  // に加える．そうでなければ configure 時に指定されたパスを auto_path
  // に加える．
  // 同時にそのパスを ::atpg::libdir という変数にセットする．
  // ATPG_DATA と ATPG_BIN も同様
  const char* script =
    "namespace eval ::atpg { }\n"
    "if [info exists env(ATPG_LIBRARY)] {"
    "    set ::atpg::libdir $env(ATPG_LIBRARY)"
    "} else {"
    "    set ::atpg::libdir " ATPG_LIBDIR
    "}\n"
    "lappend auto_path $::atpg::libdir\n"
    "if [info exists env(ATPG_DATA)] {"
    "    set ::atpg::datadir $env(ATPG_DATA)"
    "} else {"
    "    set ::atpg::datadir " ATPG_DATADIR
    "}\n"
    "if [info exists env(ATPG_BIN)] {"
    "    set ::atpg::bindir $env(ATPG_BIN)"
    "} else {"
    "    set ::atpg::bindir " ATPG_BINDIR
    "}\n";
  if (Tcl_Eval(interp, script) != TCL_OK) {
    return TCL_ERROR;
  }

  //////////////////////////////////////////////////////////////////////
  // パッケージ宣言
  //////////////////////////////////////////////////////////////////////
  if ( Tcl_PkgProvide(interp, "Atpg", SATPG_VERSION) != TCL_OK ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

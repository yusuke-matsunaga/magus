
/// @file magus/src/logbase/logbase_init.cc
/// @brief コアパッケージの初期化を行う．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateObj.cc 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <tcl.h>

#include "MagMgr.h"
#include "MvnReadVerilog.h"


BEGIN_NAMESPACE_MAGUS

// Magus の初期化関数
int
logbase_init(Tcl_Interp* interp,
	     MagMgr* mgr)
{
  //////////////////////////////////////////////////////////////////////
  // コマンドの登録
  //////////////////////////////////////////////////////////////////////

  // ネットワークのファイル入出力コマンド
  TclCmdBinder1<MvnReadVerilog, MagMgr*>::reg(interp, mgr, "magus::read_verilog");


  //////////////////////////////////////////////////////////////////////
  // デフォルト値を入れる変数の初期化
  //////////////////////////////////////////////////////////////////////
  // 直接 Tcl のスクリプトを実行させる．
  const char* const script =
    "set ::magus::default(arrival_slope)  0.0\n"
    "set ::magus::default(slew_slope)     0.0\n"
    "set ::magus::default(required_slope) 0.0\n"
    "set ::magus::default(fanout_no)        1\n"
    "set ::magus::default(timescale)  1.0e-12\n"
    "set ::magus::default(elim_limit) 1000\n"
    "set ::magus::default(elim_autolimit) true\n"
    "set ::magus::default(xsim_pat_num) 1000\n";

  if ( Tcl_Eval(interp, (char*) script) != TCL_OK ) {
    return TCL_ERROR;
  }

  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  const char* completer =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "proc complete(read_verilog) { t s e l p m } { return \"\" }\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, completer) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS

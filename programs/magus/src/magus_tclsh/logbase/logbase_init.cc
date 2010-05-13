
/// @file magus/src/logbase/logbase_init.cc
/// @brief コアパッケージの初期化を行う．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GateObj.cc 347 2007-01-16 02:37:45Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include <tcl.h>

#include <NetMgr.h>
#include "NtwkBaseCmd.h"
#include "NtwkInfoCmd.h"
#include "NtwkIoCmd.h"
#include "ElimCmd.h"
#include "NtwkNdCmd.h"


BEGIN_NAMESPACE_MAGUS

// Magus の初期化関数
int
logbase_init(Tcl_Interp* interp,
	     NetMgr* mgr)
{
  //////////////////////////////////////////////////////////////////////
  // コマンドの登録
  //////////////////////////////////////////////////////////////////////

  // ネットワークの生成／削除／複写コマンド
  TclCmdBinder1<NewNtwk, NetMgr*>::reg(interp, mgr,  "magus::new_network");
  TclCmdBinder1<DelNtwk, NetMgr*>::reg(interp, mgr,  "magus::delete_network");
  TclCmdBinder1<CopyNtwk, NetMgr*>::reg(interp, mgr, "magus::::copy_network");
  TclCmdBinder1<ClrNtwk, NetMgr*>::reg(interp, mgr,  "magus::::clear_network");

  // 操作対象のネットワークを指定するコマンド
  TclCmdBinder1<PushNtwk, NetMgr*>::reg(interp, mgr, "magus::push_current_network");
  TclCmdBinder1<PopNtwk, NetMgr*>::reg(interp, mgr,  "magus::pop_current_network");
  TclCmdBinder1<ChgNtwk, NetMgr*>::reg(interp, mgr, "magus::change_current_network");
  TclCmdBinder1<CurNtwk, NetMgr*>::reg(interp, mgr,  "magus::current_network");

  // ネットワーク名の列挙を行うコマンド
  TclCmdBinder1<ListNtwk, NetMgr*>::reg(interp, mgr, "magus::network_list");
  TclCmdBinder1<ForNtwk, NetMgr*>::reg(interp, mgr,  "magus::foreach_network");
  
#if 0
  // ネットワークトレースの設定／解除を行うコマンド
  TclCmdBinder1<SetNetTrace, NetMgr*>::reg(interp, mgr, "magus::network_trace");
#endif
  
  // ネットワークの諸元を取り出すコマンド
  TclCmdBinder1<NtwkInfo, NetMgr*>::reg(interp, mgr,   "magus::network_info");
  TclCmdBinder1<NtwkAllInfo, NetMgr*>::reg(interp, mgr, "magus::network_allinfo");
  TclCmdBinder1<NtwkPrintStats, NetMgr*>::reg(interp, mgr, "magus::print_stats");

  // ネットワークのファイル入出力コマンド
  TclCmdBinder1<ReadBlif, NetMgr*>::reg(interp, mgr, "magus::read_blif");
  TclCmdBinder1<ReadIscas89, NetMgr*>::reg(interp, mgr, "magus::read_iscas89");
  TclCmdBinder1<WriteBlif, NetMgr*>::reg(interp, mgr, "magus::write_blif");
  TclCmdBinder1<WriteEqu, NetMgr*>::reg(interp, mgr, "magus::write_equ");
  TclCmdBinder1<WriteVerilog, NetMgr*>::reg(interp, mgr, "magus::write_verilog");

  // ネットワーク上での処理コマンド(その2)
  TclCmdBinder1<ElimCmd, NetMgr*>::reg(interp, mgr,    "magus::eliminate");
  TclCmdBinder1<SimplifyCmd, NetMgr*>::reg(interp, mgr, "magus::simplify");
  TclCmdBinder1<SweepCmd, NetMgr*>::reg(interp, mgr,      "magus::sweep");
  TclCmdBinder1<CleanCmd, NetMgr*>::reg(interp, mgr,      "magus::clean_up");
  TclCmdBinder1<DecompCmd, NetMgr*>::reg(interp, mgr,     "magus::decomp");
  TclCmdBinder1<GateDecompCmd, NetMgr*>::reg(interp, mgr, "magus::gate_decomp");
  TclCmdBinder1<SopDecompCmd, NetMgr*>::reg(interp, mgr,  "magus::sop_decomp");
  TclCmdBinder1<ConvCombiCmd, NetMgr*>::reg(interp, mgr,  "magus::conv_combi");
  
  // ノードに対するコマンド
  TclCmdBinder1<NodeInfo, NetMgr*>::reg(interp, mgr,   "magus::node_info");
  TclCmdBinder1<ListNode, NetMgr*>::reg(interp, mgr,   "magus::list_node");
  TclCmdBinder1<ElimNode, NetMgr*>::reg(interp, mgr,   "magus::eliminate_node");
  TclCmdBinder1<DelNode, NetMgr*>::reg(interp, mgr,    "magus::delete_node");
  TclCmdBinder1<SimplifyNode, NetMgr*>::reg(interp, mgr, "magus::simplify_node");

  
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

  // デフォルトネットワークを作成する．
  if ( Tcl_Eval(interp, "magus::new_network default_network")
       == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // それをカレントネットワークにする．
  if ( Tcl_Eval(interp, "magus::push_current_network default_network")
      == TCL_ERROR ) {
    return TCL_ERROR;
  }


  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  const char* completer =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "proc complete(stopwatch) { t s e l p m } { return \"\" }\n"
    "proc complete(time) { t s e l p m } { return \"\" }\n"
    "proc complete(random) { t s e l p m } { return \"\" }\n"
    "proc complete(new_network) { t s e l p m } { return \"\" }\n"
    "proc complete(delete_network) { t s e l p m } { return \"\" }\n"
    "proc complete(copy_network) { t s e l p m } { return \"\" }\n"
    "proc complete(clear_network) { t s e l p m } { return \"\" }\n"
    "proc complete(push_current_network) { t s e l p m } { return \"\" }\n"
    "proc complete(pop_current_network) { t s e l p m } { return \"\" }\n"
    "proc complete(change_current_network) { t s e l p m } { return \"\" }\n"
    "proc complete(current_network) { t s e l p m } { return \"\" }\n"
    "proc complete(network_list) { t s e l p m } { return \"\" }\n"
    "proc complete(foreach_network) { t s e l p m } { return \"\" }\n"
    "proc complete(network_trace) { t s e l p m } { return \"\" }\n"
    "proc complete(network_info) { t s e l p m } { return \"\" }\n"
    "proc complete(network_allinfo) { t s e l p m } { return \"\" }\n"
    "proc complete(print_stats) { t s e l p m } { return \"\" }\n"
    "proc complete(read_blif) { t s e l p m } { return \"\" }\n"
    "proc complete(read_iscas89) { t s e l p m } { return \"\" }\n"
    "proc complete(write_blif) { t s e l p m } { return \"\" }\n"
    "proc complete(write_equ) { t s e l p m } { return \"\" }\n"
    "proc complete(write_verilog) { t s e l p m } { return \"\" }\n"
    "proc complete(eliminate) { t s e l p m } { return \"\" }\n"
    "proc complete(sweep) { t s e l p m } { return \"\" }\n"
    "proc complete(clean_up) { t s e l p m } { return \"\" }\n"
    "proc complete(decomp) { t s e l p m } { return \"\" }\n"
    "proc complete(gate_decomp) { t s e l p m } { return \"\" }\n"
    "proc complete(sop_decomp) { t s e l p m } { return \"\" }\n"
    "proc complete(node_info) { t s e l p m } { return \"\" }\n"
    "proc complete(list_node) { t s e l p m } { return \"\" }\n"
    "proc complete(eliminate_node) { t s e l p m } { return \"\" }\n"
    "proc complete(delete_node) { t s e l p m } { return \"\" }\n"
    "proc complete(equiv) { t s e l p m } { return \"\" }\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, completer) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS

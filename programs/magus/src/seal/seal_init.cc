
/// @file magus/src/seal/seal_init.cc
/// @brief Seal パッケージの初期化ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: seal_init.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "SealCmd.h"
#include "TMRCmd.h"
#include "DupCmd.h"
#include "McaCmd.h"


BEGIN_NAMESPACE_MAGUS

int
seal_init(Tcl_Interp* interp,
	  NetMgr* mgr)
{
  TclCmdBinder1<nsSeal::SealCmd, NetMgr*>::reg(interp, mgr, "magus::seal");
  TclCmdBinder1<nsSeal::TMRCmd, NetMgr*>::reg(interp, mgr, "magus::tmr");
  TclCmdBinder1<nsSeal::DupCmd, NetMgr*>::reg(interp, mgr, "magus::dup_node");
  TclCmdBinder1<nsSeal::McaCmd, NetMgr*>::reg(interp, mgr, "magus::mc_analysis");

  const char* init =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "proc complete(seal) { text start end line pos mod } { return \"\" }\n"
    "proc complete(tmr) { text start end line pos mod } { return \"\" }\n"
    "proc complete(dup_node) { text start end line pos mod } { return \"\" }\n"
    "proc complete(mc_analysis) { text start end line pos mod } { return \"\" }\n"
    "}\n"
    "}\n";

  if ( Tcl_Eval(interp, (char*) init) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  return TCL_OK;

}

END_NAMESPACE_MAGUS

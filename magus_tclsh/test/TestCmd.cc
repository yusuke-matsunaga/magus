﻿
/// @file src/test/TestCmd.cc
/// @brief TestCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TestCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TestCmd.h"
#include "YmNetworks/BNetwork.h"
#include "YmNetworks/BNetBlifReader.h"
#include "YmNetworks/BNetVerilogWriter.h"

#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"


BEGIN_NAMESPACE_MAGUS

// @brief コンストラクタ
TestCmd::TestCmd(MagMgr* mgr) :
  MagCmd(mgr)
{
}

int
TestCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc != 2 ) {
    TclObj emsg;
    emsg << "USAGE : " << arg0() << " <file-name>";
    set_result(emsg);
    return TCL_ERROR;
  }

  string file = objv[1];
  // ファイル名の展開を行う．
  string ex_file_name;
  bool stat = tilde_subst(file, ex_file_name);
  if ( !stat ) {
    // ファイル名の文字列中に誤りがあった．
    return TCL_ERROR;
  }

  StreamMsgHandler msg_handler(&cerr);
  MsgMgr::reg_handler(&msg_handler);

  BNetBlifReader read;
  BNetwork network;
  if ( !read(ex_file_name, network) ) {
    return TCL_ERROR;
  }

  BNetVerilogWriter writer;
  writer.dump(cout, network);

  return TCL_OK;
}


int
test_init(Tcl_Interp* interp,
	  MagMgr* mgr)
{
  using nsYm::nsTclpp::TclCmdBinder1;

  TclCmdBinder1<TestCmd, MagMgr*>::reg(interp, mgr,
				       "magus::rwtest");

  const char* init =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "proc complete(rwtest) { text start end line pos mod } { return \"\" }\n"
    "}\n"
    "}\n";

  if ( Tcl_Eval(interp, (char*) init) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS


/// @file src/pkg_init.cc
/// @brief Magus のパッケージ初期化
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: magus_init.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <tcl.h>
#include "NetMgr.h"

//#define USE_TEST_PACKAGE

BEGIN_NAMESPACE_MAGUS

int
misc_init(Tcl_Interp* interp);

int
logbase_init(Tcl_Interp* interp,
	     NetMgr* mgr);

int
equiv_init(Tcl_Interp* interp,
	   NetMgr* mgr);

int
lutmap_init(Tcl_Interp* interp,
	    NetMgr* mgr);

int
seal_init(Tcl_Interp* interp,
	  NetMgr* mgr);

#if defined(USE_TEST_PACKAGE)
int
test_init(Tcl_Interp* interp,
	  NetMgr* mgr);
#endif

END_NAMESPACE_MAGUS


int
magus_init(Tcl_Interp* interp)
{
  using namespace nsYm::nsMagus;
  
  //////////////////////////////////////////////////////////////////////
  // magus という namespace を作っておく．
  //////////////////////////////////////////////////////////////////////
  if ( Tcl_Eval(interp, "namespace eval ::magus {}") == TCL_ERROR ) {
    return TCL_ERROR;
  }

  
  //////////////////////////////////////////////////////////////////////
  // NetMgr の生成
  //////////////////////////////////////////////////////////////////////

  NetMgr* mgr = new NetMgr;
  
  if ( misc_init(interp) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  if ( logbase_init(interp, mgr) == TCL_ERROR ) {
    return TCL_ERROR;
  }
  
  if ( equiv_init(interp, mgr) == TCL_ERROR ) {
    return TCL_ERROR;
  }
  
  if ( lutmap_init(interp, mgr) == TCL_ERROR ) {
    return TCL_ERROR;
  }
  
  if ( seal_init(interp, mgr) == TCL_ERROR ) {
    return TCL_ERROR;
  }

#if defined(USE_TEST_PACKAGE)
  if ( test_init(interp, mgr) == TCL_ERROR ) {
    return TCL_ERROR;
  }
#endif

  // もしも MAGUS_LIBRARY という環境変数が定義されていたらそれを auto_path
  // に加える．そうでなければ configure 時に指定されたパスを auto_path
  // に加える．
  // 同時にそのパスを ::magus::libdir という変数にセットする．
  // MAGUS_DATA と MAGUS_BIN も同様
  const char* script =
    "namespace eval ::magus { }\n"
    "if [info exists env(MAGUS_LIBRARY)] {"
    "    set ::magus::libdir $env(MAGUS_LIBRARY)"
    "} else {"
    "    set ::magus::libdir " MAGUS_LIBDIR
    "}\n"
    "lappend auto_path $::magus::libdir\n"
    "if [info exists env(MAGUS_DATA)] {"
    "    set ::magus::datadir $env(MAGUS_DATA)"
    "} else {"
    "    set ::magus::datadir " MAGUS_DATADIR
    "}\n"
    "if [info exists env(MAGUS_BIN)] {"
    "    set ::magus::bindir $env(MAGUS_BIN)"
    "} else {"
    "    set ::magus::bindir " MAGUS_BINDIR
    "}\n";
  if (Tcl_Eval(interp, script) != TCL_OK) {
    return TCL_ERROR;
  }

  
  //////////////////////////////////////////////////////////////////////
  // パッケージ宣言
  //////////////////////////////////////////////////////////////////////
  if ( Tcl_PkgProvide(interp, "Magus", MAGUS_VERSION) != TCL_OK ) {
    return TCL_ERROR;
  }
  
  return TCL_OK;
}

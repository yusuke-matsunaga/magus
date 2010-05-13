
/// @file magus/cellmap/CellmapCmd2.cc
/// @brief CellmapCmd2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellmapCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "CellmapCmd.h"
#include "NetMgr.h"
#include "SbjGraph.h"
#include "CellNetwork.h"


BEGIN_NAMESPACE_MAGUS_CELLMAP

//////////////////////////////////////////////////////////////////////
// LUT mapping の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellmapCmd::CellmapCmd(NetMgr* mgr,
		       CellmapData* data) :
  MagCmd(mgr),
  mData(data)
{
}

// @brief デストラクタ
CellmapCmd::~CellmapCmd()
{
}

// @brief SbjGraph を得る．
SbjGraph&
CellmapCmd::sbjgraph()
{
  return mData->mSbjGraph;
}

// @brief CellNetwork を得る．
CnGraph&
CellmapCmd::lutnetwork()
{
  return mData->mCellNetwork;
}

END_NAMESPACE_MAGUS_CELLMAP

BEGIN_NAMESPACE_MAGUS

int
cellmap_init(Tcl_Interp* interp,
	     NetMgr* mgr)
{
  nsCellmap::CellmapData* data = new nsCellmap::CellmapData;

  TclCmdBinder2<nsCellmap::Conv2SbjCmd,
    NetMgr*, nsCellmap::CellmapData*>::reg(interp, mgr, data,
					 "magus::cellmap::conv2sbj");
  TclCmdBinder2<nsCellmap::DumpSbjCmd,
    NetMgr*, nsCellmap::CellmapData*>::reg(interp, mgr, data,
					 "magus::cellmap::dump_sbjgraph");
  TclCmdBinder2<nsCellmap::AreaMapCmd,
    NetMgr*, nsCellmap::CellmapData*>::reg(interp, mgr, data,
					 "magus::cellmap::area_map");
  TclCmdBinder2<nsCellmap::DelayMapCmd,
    NetMgr*, nsCellmap::CellmapData*>::reg(interp, mgr, data,
					 "magus::cellmap::delay_map");
  TclCmdBinder2<nsCellmap::DumpCellCmd,
    NetMgr*, nsCellmap::CellmapData*>::reg(interp, mgr, data,
					 "magus::cellmap::dump_cellnetwork");
  TclCmdBinder2<nsCellmap::Conv2BNetCmd,
    NetMgr*, nsCellmap::CellmapData*>::reg(interp, mgr, data,
					 "magus::cellmap::conv2bnet");

  const char* init =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "namespace eval cellmap {\n"
    "proc complete(conv2sbj) { text start end line pos mod } { return \"\" }\n"
    "proc complete(dump_sbjgraph) { text start end line pos mod } { return \"\" }\n"
    "proc complete(area_map) { text start end line pos mod } { return \"\" }\n"
    "proc complete(delay_map) { text start end line pos mod } { return \"\" }\n"
    "proc complete(dump_cellnetwork) { text start end line pos mod } { return \"\" }\n"
    "proc complete(conv2bnet) { text start end line pos mod } { return \"\" }\n"
    "}\n"
    "}\n"
    "}\n";

  if ( Tcl_Eval(interp, (char*) init) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS

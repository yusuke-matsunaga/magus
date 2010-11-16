
/// @file magus/cellmap/AreaMapCmd.cc
/// @brief AreaMapCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AreaMapCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "CellmapCmd.h"
#include "MagMgr.h"
#include "SbjGraph.h"
#include "CellNetwork.h"
#include "CutHolder.h"
#include "MapRecord.h"
#include "DagACover.h"
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_MAGUS_CELLMAP

//////////////////////////////////////////////////////////////////////
// area map コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AreaMapCmd::AreaMapCmd(MagMgr* mgr,
		       CellmapData* data) :
  CellmapCmd(mgr, data)
{
  mPoptMethod = new TclPoptStr(this, "method",
			       "specify covering method",
			       "tree|df|dag|fo");
  mPoptVerbose = new TclPopt(this, "verbose",
			     "verbose mode");
  set_usage_string("<#inputs>[=INT]");
}

// @brief デストラクタ
AreaMapCmd::~AreaMapCmd()
{
}
  
// @brief コマンドを実行する仮想関数
int
AreaMapCmd::cmd_proc(TclObjVector& objv)
{
  bool verbose = mPoptVerbose->is_specified();
  tCutMode cutmode = kCmTree;
  
  string fargs = "dag";

  if ( mPoptMethod->is_specified() ) {
    string method = mPoptMethod->val();
    if ( method == "tree" ) {
      cutmode = kCmTree;
      fargs = "tree";
    }
    else if ( method == "df" ) {
      cutmode = kCmDF;
      fargs = "df";
    }
    else if ( method == "dag" ) {
      cutmode = kCmAll;
      fargs = "dag";
    }
    else if ( method == "fo" ) {
      cutmode = kCmAll;
      fargs = "fo";
    }
    else {
      string emsg = method + ": unknown method";
      set_result(emsg);
      return TCL_ERROR;
    }
  }

  size_t objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  try {
    int limit;
    int code = int_conv(objv[1], limit);
    if ( code != TCL_OK ) {
      return code;
    }

    DagACoverFactory factory;
    DagACover* dagcov = factory(fargs);
    CutHolder ch;
    ch.enum_cut(sbjgraph(), limit, cutmode);
    MapRecord maprec;
    dagcov->record_cuts(sbjgraph(), ch, maprec);
    int cell_num;
    maprec.gen_mapgraph(sbjgraph(), lutnetwork(), cell_num);
    delete dagcov;
    set_var("::magus::cellmap_stats", "cell_num",
	    cell_num,
	    TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
    return TCL_OK;
  }
  catch ( AssertError x ) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "Assertion Error";
    set_result(emsg);
    return TCL_ERROR;
  }
  
  return TCL_OK;
}

END_NAMESPACE_MAGUS_CELLMAP

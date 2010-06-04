
/// @file magus/cellmap/DelayMapCmd.cc
/// @brief DelayMapCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DelayMapCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
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
#include "CutHolder.h"
#include "MapRecord.h"
#include "DagDCover.h"
#include <ym_tclpp/TclPopt.h>

#include <ym_utils/StopWatch.h>


BEGIN_NAMESPACE_MAGUS_CELLMAP

//////////////////////////////////////////////////////////////////////
// delay map コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DelayMapCmd::DelayMapCmd(NetMgr* mgr,
			 CellmapData* data) :
  CellmapCmd(mgr, data)
{
  mPoptMethod = new TclPoptStr(this, "method",
			       "specify covering method",
			       "dag|fo");
  mPoptSlack = new TclPoptInt(this, "slack",
			      "specify slack value");
  mPoptVerbose = new TclPopt(this, "verbose",
			     "verbose mode");
  set_usage_string("<#inputs>[=INT]");
}

// @brief デストラクタ
DelayMapCmd::~DelayMapCmd()
{
}
  
// @brief コマンドを実行する仮想関数
int
DelayMapCmd::cmd_proc(TclObjVector& objv)
{
  int slack = 0;
  bool verbose = false;
  
  string fargs = "dag";

  if ( mPoptMethod->is_specified() ) {
    string method = mPoptMethod->val();
    if ( method == "dag" ) {
      fargs = "dag";
    }
    else if ( method == "fo" ) {
      fargs = "fo";
    }
    else {
      string emsg = method + ": unknown method";
      set_result(emsg);
      return TCL_ERROR;
    }
  }

  if ( mPoptSlack->is_specified() ) {
    slack = mPoptSlack->val();
  }

  if ( mPoptVerbose->is_specified() ) {
    verbose = true;
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

    DagDCoverFactory ddfactory;
    CutHolder ch;
    ch.enum_cut(sbjgraph(), limit, kCmAll);
    DagDCover* dagdcov = ddfactory(fargs);
    MapRecord maprec;
    dagdcov->record_cuts(sbjgraph(), ch, slack, maprec);
    
    int cell_num;
    maprec.gen_mapgraph(sbjgraph(), lutnetwork(), cell_num);
    delete dagdcov;
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

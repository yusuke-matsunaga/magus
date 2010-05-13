
/// @file magus/lutmap/DelayMapCmd.cc
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

#include "LutmapCmd.h"
#include "SbjGraph.h"
#include "LutNetwork.h"
#include "CutHolder.h"
#include "MapRecord.h"
#include "CutResub.h"
#include "DagDCover.h"
#include <ym_tclpp/TclPopt.h>

#include <ym_utils/StopWatch.h>


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// delay map コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DelayMapCmd::DelayMapCmd(NetMgr* mgr,
			 LutmapData* data) :
  LutmapCmd(mgr, data)
{
  mPoptMethod = new TclPoptStr(this, "method",
			       "specify covering method",
			       "dag|fo");
  mPoptSlack = new TclPoptInt(this, "slack",
			      "specify slack value");
  mPoptResub = new TclPopt(this, "resub",
			   "do cut resubstitution");
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

  if ( mPoptResub->is_specified() ) {
    fargs += "-r";
  }

  if ( mPoptVerbose->is_specified() ) {
    verbose = true;
  }
  
  ymuint objc = objv.size();
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
    DagDCover* dagdcov = ddfactory(fargs);
    MapRecord maprec;
    dagdcov->record_cuts(sbjgraph(), limit, slack, maprec);
    
    int lut_num;
    int depth;
    maprec.gen_mapgraph(sbjgraph(), lutnetwork(), lut_num, depth);
    delete dagdcov;
    set_var("::magus::lutmap_stats", "lut_num",
	    lut_num,
	    TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
    set_var("::magus::lutmap_stats", "level",
	    depth,
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

END_NAMESPACE_MAGUS_LUTMAP


/// @file magus/lutmap/AreaMapCmd.cc
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

#include "LutmapCmd.h"
#include "SbjGraph.h"
#include "LutNetwork.h"
#include "CutHolder.h"
#include "MapRecord.h"
#include "CutResub.h"
#include "DagACover.h"
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// area map コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AreaMapCmd::AreaMapCmd(NetMgr* mgr,
		       LutmapData* data) :
  LutmapCmd(mgr, data)
{
  mPoptMethod = new TclPoptStr(this, "method",
			       "specify covering method",
			       "tree|df|dag|fo");
  mPoptResub = new TclPopt(this, "resub",
			   "do cut resubstitution");
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
  bool resub = mPoptResub->is_specified();
  bool verbose = mPoptVerbose->is_specified();
  
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

    DagACoverFactory factory;
    DagACover* dagcov = factory(fargs);
    CutHolder ch;
    ch.enum_cut(sbjgraph(), limit);
    MapRecord maprec;
    dagcov->record_cuts(sbjgraph(), ch, maprec);
    if ( resub ) {
      CutResub cutresub;
      cutresub(sbjgraph(), ch, maprec);
    }
    int lut_num;
    int depth;
    maprec.gen_mapgraph(sbjgraph(), lutnetwork(), lut_num, depth);
    delete dagcov;
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

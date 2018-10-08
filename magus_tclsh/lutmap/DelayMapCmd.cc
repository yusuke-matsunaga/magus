
/// @file magus/lutmap/DelayMapCmd.cc
/// @brief DelayMapCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DelayMapCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DelayMapCmd.h"
#include "ym/TclPopt.h"

#include "LutMap.h"

#include "ym/MvnMgr.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// delay map コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DelayMapCmd::DelayMapCmd(MagMgr* mgr,
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

  ymuint mode = 0;
  if ( mPoptMethod->is_specified() ) {
    string method = mPoptMethod->val();
    if ( method == "dag" ) {
      mode = 1;
    }
    else if ( method == "fo" ) {
      mode = 0;
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
    mode |= 2;
  }

  if ( mPoptVerbose->is_specified() ) {
    verbose = true;
  }

  ymuint objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  int limit;
  int code = int_conv(objv[1], limit);
  if ( code != TCL_OK ) {
    return code;
  }

  LutMap lutmap;

  int lut_num;
  int depth;

  NetHandle* neth = cur_nethandle();
  switch ( neth->type() ) {
  case NetHandle::kMagBn:
    {
      lutmap.delay_map(*neth->bnetwork(), limit, slack, mode,
		       *neth->_bnetwork(), lut_num, depth);
    }
    break;

  case NetHandle::kMagMvn:
    {
#if 0
      const MvnMgr& mvn = *neth->mvn();
      MvnBnConv conv;
      BdnMgr tmp_network;
      MvnBnMap mvnode_map(mvn.max_node_id());
      conv(mvn, tmp_network, mvnode_map);
      lutmap.delay_map(tmp_network, limit, slack, mode,
		       lutnetwork(), lut_num, depth);
#endif
    }
    break;
  }

  set_var("::magus::lutmap_stats", "lut_num",
	  lut_num,
	  TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
  set_var("::magus::lutmap_stats", "level",
	  depth,
	  TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
  return TCL_OK;

  return TCL_OK;
}

END_NAMESPACE_MAGUS

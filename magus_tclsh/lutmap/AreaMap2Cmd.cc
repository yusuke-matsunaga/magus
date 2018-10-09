
/// @file magus/lutmap/AreaMap2Cmd.cc
/// @brief AreaMap2Cmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AreaMap2Cmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "AreaMap2Cmd.h"
#include "ym/TclPopt.h"

#include "lutmap_nsdef.h"

#include "ym/MvnMgr.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// area map コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AreaMap2Cmd::AreaMap2Cmd(MagMgr* mgr,
		       LutmapData* data) :
  LutmapCmd(mgr, data)
{
  mPoptMethod = new TclPoptStr(this, "method",
			       "specify covering method",
			       "dag|fo");
  mPoptResub = new TclPopt(this, "resub",
			   "do cut resubstitution");
  mPoptCount = new TclPoptInt(this, "count",
			      "sepcify count limit <INT>");
  mPoptVerbose = new TclPopt(this, "verbose",
			     "verbose mode");
  set_usage_string("<#inputs>[=INT]");
}

// @brief デストラクタ
AreaMap2Cmd::~AreaMap2Cmd()
{
}

// @brief コマンドを実行する仮想関数
int
AreaMap2Cmd::cmd_proc(TclObjVector& objv)
{
  bool verbose = mPoptVerbose->is_specified();

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
  if ( mPoptResub->is_specified() ) {
    mode |= 2;
  }

  ymuint count = 10000;
  if ( mPoptCount->is_specified() ) {
    count = mPoptCount->val();
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

  int lut_num;
  int depth;

  string mode_str;
  NetHandle* neth = cur_nethandle();
  switch ( neth->type() ) {
  case NetHandle::kMagBn:
    {
      lutmap(*neth->bnetwork(), limit, -1, mode_str, *neth->_bnetwork(), lut_num, depth);
    }
    break;

  case NetHandle::kMagMvn:
    {
      const MvnMgr& mvn = *neth->mvn();
#if 0
      MvnBnetConv conv;
      BnNetwork tmp_network;
      MvnBnMap mvnode_map(mvn.max_node_id());
      conv(mvn, tmp_network, mvnode_map);
      lutmap.area_map(tmp_network, limit, mode, lutnetwork(), lut_num, depth);
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
}

END_NAMESPACE_MAGUS

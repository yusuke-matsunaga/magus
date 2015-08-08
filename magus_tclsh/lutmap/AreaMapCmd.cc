﻿
/// @file magus/lutmap/AreaMapCmd.cc
/// @brief AreaMapCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AreaMapCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "AreaMapCmd.h"
#include "YmTclpp/TclPopt.h"

#include "LutMap.h"

#include "YmNetworks/BNetBdnConv.h"

#include "YmNetworks/MvnMgr.h"
#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/MvnBdnConv.h"
#include "YmNetworks/MvnBdnMap.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// area map コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AreaMapCmd::AreaMapCmd(MagMgr* mgr,
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

  ymuint lut_num;
  ymuint depth;

  NetHandle* neth = cur_nethandle();
  switch ( neth->type() ) {
  case NetHandle::kMagBNet:
    {
      BNetBdnConv conv;

      BdnMgr tmp_network;
      conv(*neth->bnetwork(), tmp_network);
      lutmap.area_map(tmp_network, limit, mode, lutnetwork(), lut_num, depth);
    }
    break;

  case NetHandle::kMagBdn:
    lutmap.area_map(*neth->bdn(), limit, mode, lutnetwork(), lut_num, depth);
    break;

  case NetHandle::kMagMvn:
    {
      const MvnMgr& mvn = *neth->mvn();
      MvnBdnConv conv;
      BdnMgr tmp_network;
      MvnBdnMap mvnode_map(mvn.max_node_id());
      conv(mvn, tmp_network, mvnode_map);
      lutmap.area_map(tmp_network, limit, mode, lutnetwork(), lut_num, depth);
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

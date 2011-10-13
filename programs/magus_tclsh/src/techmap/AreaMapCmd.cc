
/// @file AreaMapCmd.cc
/// @brief AreaMapCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "AreaMapCmd.h"
#include "ym_tclpp/TclPopt.h"

#include "ym_networks/BNetBdnConv.h"
#include "ym_techmap/CellMap.h"
#include "ym_networks/MvnMgr.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/MvnBdnConv.h"
#include "ym_networks/MvnBdnMap.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
// area map コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AreaMapCmd::AreaMapCmd(MagMgr* mgr,
		       CmnMgr& cmnmgr) :
  TechmapCmd(mgr, cmnmgr)
{
  mPoptMethod = new TclPoptStr(this, "method",
			       "specify covering method",
			       "tree|df|dag|fo");
  mPoptResub = new TclPopt(this, "resub",
			   "do cut resubstitution");
  mPoptVerbose = new TclPopt(this, "verbose",
			     "verbose mode");
  set_usage_string("");
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
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  NetHandle* neth = cur_nethandle();
  CellMap mapper;
  switch ( neth->type() ) {
  case NetHandle::kMagBNet:
    {
      BNetBdnConv conv;

      BdnMgr tmp_network;
      conv(*neth->bnetwork(), tmp_network);

      mapper.area_map(*cur_cell_library(), tmp_network, 0, cmnmgr());
    }
    break;

  case NetHandle::kMagBdn:
    mapper.area_map(*cur_cell_library(), *neth->bdn(), 0, cmnmgr());
    break;

  case NetHandle::kMagMvn:
    {
      const MvnMgr& mvn = *neth->mvn();
      MvnBdnConv conv;
      BdnMgr tmp_network;
      MvnBdnMap mvnode_map(mvn.max_node_id());
      conv(mvn, tmp_network, mvnode_map);

      mapper.area_map(*cur_cell_library(), tmp_network, 0, cmnmgr());
    }
    break;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS_TECHMAP


/// @file AreaMapCmd.cc
/// @brief AreaMapCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "AreaMapCmd.h"
#include "ym/TclPopt.h"

#include "CellMap.h"

#include "ym/MvnMgr.h"
#include "ym/BnNetwork.h"
#include "ym/MvnBnConv.h"
#include "ym/MvnBnMap.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
// area map コマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AreaMapCmd::AreaMapCmd(MagMgr* mgr) :
  TechmapCmd(mgr)
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

  if ( cur_cell_library() == nullptr ) {
    TclObj emsg;
    emsg << "Cell Library is not set.";
    set_result(emsg);
    return TCL_ERROR;
  }

  NetHandle* neth = cur_nethandle();
  CellMap mapper;
  switch ( neth->type() ) {
  case NetHandle::kMagBn:
    {
      mapper.area_map(*cur_cell_library(), *neth->bnetwork(), 0, cmnmgr());
    }
    break;

  case NetHandle::kMagMvn:
    {
#if 0
      const MvnMgr& mvn = *neth->mvn();
      MvnBdnConv conv;
      BdnMgr tmp_network;
      MvnBdnMap mvnode_map(mvn.max_node_id());
      conv(mvn, tmp_network, mvnode_map);

      mapper.area_map(*cur_cell_library(), tmp_network, 0, cmnmgr());
#endif
    }
    break;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS_TECHMAP

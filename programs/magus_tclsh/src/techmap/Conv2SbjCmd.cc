
/// @file techmap/Conv2SbjCmd.cc
/// @brief Conv2SbjCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Conv2SbjCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Conv2SbjCmd.h"
#include "ym_networks/BNet2Sbj.h"
#include "ym_networks/MvnMgr.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/MvnBdnConv.h"
#include "ym_networks/MvnBdnMap.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
// BNetwork から SbjGraph に変換するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Conv2SbjCmd::Conv2SbjCmd(MagMgr* mgr,
			 TechmapData* data) :
  TechmapCmd(mgr, data)
{
}

// @brief デストラクタ
Conv2SbjCmd::~Conv2SbjCmd()
{
}

// @brief コマンドを実行する仮想関数
int
Conv2SbjCmd::cmd_proc(TclObjVector& objv)
{
  if ( objv.size() != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  bool stat = false;
  NetHandle* neth = cur_nethandle();
  switch ( neth->type() ) {
  case NetHandle::kMagBNet:
    {
      ostringstream err_out;
      const BNetwork& src_network = *neth->bnetwork();
      BNet2Sbj conv;
      stat = conv(src_network, sbjgraph(), err_out);
      TclObj emsg = err_out.str();
      set_result(emsg);
    }
    break;

  case NetHandle::kMagBdn:
    break;

  case NetHandle::kMagMvn:
    {
      const MvnMgr& mgr = *neth->mvn();
      MvnBdnConv conv;
      MvnBdnMap mvnode_map(mgr.max_node_id());
      BdnMgr bdnetwork;
      conv(mgr, bdnetwork, mvnode_map);
#warning "TODO: code is broken"
      stat = true;
    }
    break;
  }

  return stat ? TCL_OK : TCL_ERROR;
}

END_NAMESPACE_MAGUS_TECHMAP

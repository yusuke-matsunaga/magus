
/// @file magus/lutmap/Conv2SbjCmd.cc
/// @brief Conv2SbjCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Conv2SbjCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapCmd.h"
#include "SbjGraph.h"
#include "ym_bnet/bnet_nsdef.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

bool
bnet2sbj(const BNetwork& network,
	 SbjGraph& sbjgraph,
	 ostream& err_out);

//////////////////////////////////////////////////////////////////////
// BNetwork から SbjGraph に変換するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Conv2SbjCmd::Conv2SbjCmd(NetMgr* mgr,
			 LutmapData* data) :
  LutmapCmd(mgr, data)
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

  try {
    ostringstream err_out;
    BNetwork& src_network = *cur_network();
    bool stat = bnet2sbj(src_network, sbjgraph(), err_out);
    TclObj emsg = err_out.str();
    set_result(emsg);
    return stat ? TCL_OK : TCL_ERROR;
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

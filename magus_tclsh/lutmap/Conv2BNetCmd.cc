
/// @file magus/lutmap/Conv2BNetCmd.cc
/// @brief Conv2BNetCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Conv2BNetCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapCmd.h"
#include "NetHandle.h"
#include "ym/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// マッピングを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Conv2BNetCmd::Conv2BNetCmd(MagMgr* mgr,
			   LutmapData* data) :
  LutmapCmd(mgr, data)
{
  mPoptDstNetwork = new TclPoptStr(this, "dst_network",
				   "specify destination network",
				   "<network-name>");
}

// @brief デストラクタ
Conv2BNetCmd::~Conv2BNetCmd()
{
}

// @brief コマンドを実行する仮想関数
int
Conv2BNetCmd::cmd_proc(TclObjVector& objv)
{
  NetHandle* neth = nullptr;

  if ( mPoptDstNetwork->is_specified() ) {
    neth = find_or_new_nethandle(mPoptDstNetwork->val(), NetHandle::kMagBn);
    if ( neth == nullptr ) {
      // 見付からなかった
      // エラーメッセージは find_network() の中でセットされている．
      return TCL_ERROR;
    }
  }

  if ( objv.size() != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  try {
    if ( neth == nullptr ) {
      neth = cur_nethandle();
    }
    BnNetwork& dst_network = *neth->_bnetwork();
    // 実はただのコピー
    dst_network.copy(lutnetwork());
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

END_NAMESPACE_MAGUS

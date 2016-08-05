
/// @file TechmapCmd.cc
/// @brief TechmapCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TechmapCmd.h"
#include "AreaMapCmd.h"
#include "ym/TclPopt.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
// LUT mapping の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TechmapCmd::TechmapCmd(MagMgr* mgr) :
  MagCmd(mgr)
{
  mPoptDstNetwork = new TclPoptStr(this, "dst_network",
				   "specify destination network",
				   "<network-name>");
}

// @brief デストラクタ
TechmapCmd::~TechmapCmd()
{
}

#if 0
// @brief 結果を納めるネットワーク
BnNetwork&
TechmapCmd::dst_network()
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
}
#endif

END_NAMESPACE_MAGUS_TECHMAP

BEGIN_NAMESPACE_MAGUS

int
techmap_init(Tcl_Interp* interp,
	     MagMgr* mgr)
{
  using namespace nsTechmap;

  TclCmdBinder1<AreaMapCmd, MagMgr*>::reg(interp, mgr, "magus::techmap::area_map");


  const char* init =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "namespace eval techmap {\n"
    "proc complete(load_pat) { text start end line pos mod } { return \"\" }\n"
    "proc complete(dump_pat) { text start end line pos mod } { return \"\" }\n"
    "proc complete(area_map) { text start end line pos mod } { return \"\" }\n"
    "proc complete(dump_cngraph) { text start end line pos mod } { return \"\" }\n"
    "}\n"
    "}\n"
    "}\n";

  if ( Tcl_Eval(interp, (char*) init) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS

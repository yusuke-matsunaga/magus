
/// @file magus/logbase/MagBNetCmd.cc
/// @brief MagBNetCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MagBNetCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetCmd.h"
#include "YmTclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// Magus のコマンドオブジェクトの基底クラス MagBNetCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr Magus の管理オブジェクト
// @param[in] new_bnet_enable -new_bnet オプションを使用するとき true
BNetCmd::BNetCmd(MagMgr* mgr,
		 bool new_bnet_enable) :
  NetCmd(mgr, new_bnet_enable, false, false)
{
}

// @brief デストラクタ
BNetCmd::~BNetCmd()
{
}

// @brief カレントネットワークの取得
// @note カレントネットワークが BNetwork でない場合には nullptr を返す．
BNetwork*
BNetCmd::cur_network() const
{
  return cur_nethandle()->_bnetwork();
}

// コマンド行の引数を解析しネットワークとライブラリをセットする．
// エラーが起きた場合にはインタプリタにメッセージをセットし，
// false を返す．
// ネットワークおよびライブラリが指定されていた場合には，
// それぞれ mNetworkSpecified, mLibrarySpecified に true を入れる．
int
BNetCmd::before_cmd_proc(TclObjVector& objv)
{
  if ( NetCmd::before_cmd_proc(objv) == TCL_OK ) {
    if ( cur_nethandle()->type() != NetHandle::kMagBNet ) {
      TclObj emsg;
      emsg << "Network type mismatch. BNetwork type assumed.";
      set_result(emsg);
      return TCL_ERROR;
    }
  }

  return TCL_OK;
}

void
BNetCmd::after_cmd_proc()
{
  NetCmd::after_cmd_proc();
}

END_NAMESPACE_MAGUS

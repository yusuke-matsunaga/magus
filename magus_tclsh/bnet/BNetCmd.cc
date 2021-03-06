﻿
/// @file magus/logbase/MagBNetCmd.cc
/// @brief MagBNetCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetCmd.h"
#include "ym/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// Magus のコマンドオブジェクトの基底クラス MagBNetCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr Magus の管理オブジェクト
// @param[in] new_bnet_enable -new_bnet オプションを使用するとき true
BNetCmd::BNetCmd(MagMgr* mgr,
		 bool new_bnet_enable) :
  NetCmd(mgr, new_bnet_enable, false)
{
}

// @brief デストラクタ
BNetCmd::~BNetCmd()
{
}

// @brief カレントネットワークの取得
// @note カレントネットワークが BnNetwork でない場合には nullptr を返す．
BnNetwork*
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
    if ( cur_nethandle()->type() != NetHandle::kMagBn ) {
      TclObj emsg;
      emsg << "Network type mismatch. BnNetwork type assumed.";
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

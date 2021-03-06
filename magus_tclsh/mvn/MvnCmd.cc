﻿
/// @file src/mvn/MvnCmd.cc
/// @brief MvnCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2017 Yusuke Matsunaga
/// All rights reserved.


#include "MvnCmd.h"
#include "ym/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// Magus のコマンドオブジェクトの基底クラス MvnCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnCmd::MvnCmd(MagMgr* mgr,
	       bool new_mvn_enable) :
  NetCmd(mgr, false, new_mvn_enable)
{
}

// @brief デストラクタ
MvnCmd::~MvnCmd()
{
}

// @brief カレントネットワークの取得
// @note カレントネットワークが MvNetwork でない場合には nullptr を返す．
MvnMgr*
MvnCmd::cur_network() const
{
  return cur_nethandle()->_mvn();
}

// コマンド行の引数を解析しネットワークとライブラリをセットする．
// エラーが起きた場合にはインタプリタにメッセージをセットし，
// false を返す．
// ネットワークおよびライブラリが指定されていた場合には，
// それぞれ mNetworkSpecified, mLibrarySpecified に true を入れる．
int
MvnCmd::before_cmd_proc(TclObjVector& objv)
{
  if ( NetCmd::before_cmd_proc(objv) == TCL_OK ) {
    if ( cur_nethandle()->type() != NetHandle::kMagMvn ) {
      TclObj emsg;
      emsg << "Network type mismatch. MvNetwork type assumed.";
      set_result(emsg);
      return TCL_ERROR;
    }
  }

  return TCL_OK;
}

void
MvnCmd::after_cmd_proc()
{
  NetCmd::after_cmd_proc();
}

END_NAMESPACE_MAGUS

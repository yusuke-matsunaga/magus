
/// @file src/core/CurNetwork.cc
/// @brief CurNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CurNetwork.h"
#include "NetHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// カレントネットワーク名を返すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CurNtwk::CurNtwk(MagMgr* mgr) :
  MagCmd(mgr)
{
}

// @brief デストラクタ
CurNtwk::~CurNtwk()
{
}

// コマンドを実行する．
int
CurNtwk::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  NetHandle* neth = cur_nethandle();
  string name = neth->name();
  set_result(name);
  return TCL_OK;
}

END_NAMESPACE_MAGUS

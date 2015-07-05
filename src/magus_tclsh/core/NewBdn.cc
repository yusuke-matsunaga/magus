﻿
/// @file src/core/NewBdn.cc
/// @brief NewBdn の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NewBdn.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NewBdn.h"
#include "MagMgr.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// 新しいネットワークを作成するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NewBdn::NewBdn(MagMgr* mgr) :
  MagCmd(mgr)
{
  set_usage_string("?<network-name>?");
}

// @brief デストラクタ
NewBdn::~NewBdn()
{
}

// コマンドを実行する．
int
NewBdn::cmd_proc(TclObjVector& objv)
{
  // ネットワーク名
  string name;

  ymuint objc = objv.size();
  if ( objc > 2 ) {
    // 引数が多すぎる．
    print_usage();
    return TCL_ERROR;
  }
  if ( objc == 2 ) {
    // ネットワーク名が残っていた．
    name = objv[1];
  }
  else {
    // ネットワーク名がない．
    name = mgr()->new_tmpname();
    // 自動的に名前をつける．
    // まさかとは思うけど念のため
    if ( name == string() ) {
      set_result("No more available name for temporary network.");
      return TCL_ERROR;
    }
  }

  // 新たなネットワークを生成する．
  NetHandle* neth = new_bdnhandle(name);
  if ( !neth ) {
    // 何らかのエラーが起こった．
    return TCL_ERROR;
  }

  // ネットワーク名を返り値に入れておく．
  set_result(name);

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS

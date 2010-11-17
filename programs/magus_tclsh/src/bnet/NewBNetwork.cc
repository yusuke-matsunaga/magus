
/// @file src/bnet/NewBNetwork.cc
/// @brief src/bnet/NewBNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NtwkBaseCmd.h"
//#include "MagMgr.h"
//#include "NetHandle.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// 新しい BNetwork を作成するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NewBNetwork::NewBNetwork(MagMgr* mgr) :
  MagCmd(mgr)
{
  mPoptTrace = new TclPoptBool(this, "trace",
			       "set network trace");
  set_usage_string("?<network-name>?");
}

// @brief デストラクタ
NewBNetwork::~NewBNetwork()
{
}

// コマンドを実行する．
int
NewBNetwork::cmd_proc(TclObjVector& objv)
{
  // trace を付けるかどうか(デフォルトでは付ける)
  bool trace = true;
  // ネットワーク名
  string name;

  if ( mPoptTrace->is_specified() ) {
    trace = mPoptTrace->val();
  }

  size_t objc = objv.size();
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
    // 名無しのネットワークにはデフォルトではトレースを付けない．
    if ( mPoptTrace->count() == 0 ) {
      trace = false;
    }
  }

  // 新たなネットワークを生成し，登録する．
  NetHandle* neth = new_bnethandle(name);
  if ( neth == NULL ) {
    // 何らかのエラーが起こった．
    return TCL_ERROR;
  }

  // ネットワーク名を返り値に入れておく．
  set_result(name);

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS

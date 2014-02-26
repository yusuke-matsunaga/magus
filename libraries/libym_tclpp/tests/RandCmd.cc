
/// @file RandCmd.cc
/// @brief RandCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// 乱数を発生させるコマンドクラス


#include "RandCmd.h"


BEGIN_NAMESPACE_YM_TCLPP


//////////////////////////////////////////////////////////////////////
// 乱数発生コマンド
//////////////////////////////////////////////////////////////////////

// usage を出力する関数
void
RandCmd::usage()
{
  const char* usage =
    "USAGE: random ?range?";
  set_result(usage);
}


// 乱数発生コマンド
int
RandCmd::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  if ( objc > 2 ) {
    usage();
    return TCL_ERROR;
  }

  unsigned long limit = 0;
  if ( objc == 2 ) {
    int stat = ulong_conv(objv[1], limit);
    if ( stat != TCL_OK ) {
      return stat;
    }
  }
  long rand = mRandGen.int32();
  if ( limit ) {
    rand = rand % limit;
  }

  set_result(rand);

  return TCL_OK;
}

END_NAMESPACE_YM_TCLPP

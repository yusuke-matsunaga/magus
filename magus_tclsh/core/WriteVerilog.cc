﻿
/// @file src/core/WriteVerilog.cc
/// @brief WriteVerilog の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "WriteVerilog.h"

#include "ym/MvnVerilogWriter.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ファイルに Verilog 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
WriteVerilog::WriteVerilog(MagMgr* mgr) :
  NetCmd(mgr, false, false)
{
  set_usage_string("?<filename>?");
}

// @brief デストラクタ
WriteVerilog::~WriteVerilog()
{
}

// コマンドを実行する．
int
WriteVerilog::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();

  // このコマンドはファイル名のみを引数に取る．
  // 引数がなければ標準出力に出す．
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  // 出力先のファイルを開く
  ostream* osp = &cout;
  ofstream ofs;
  if ( objc == 2 ) {
    string file_name = objv[1];
    if ( !open_ofile(ofs, file_name) ) {
      // ファイルが開けなかった．
      return TCL_ERROR;
    }
    osp = &ofs;
  }

  int stat = TCL_ERROR;  // 念のため TCL_ERROR を入れておく．

  NetHandle* neth = cur_nethandle();
  switch ( neth->type() ) {
  case NetHandle::kMagBn:
    {
      // BNetwork の場合
#if 0
      BNetVerilogWriter writer;
      writer.dump(*osp, *neth->bnetwork());
#endif
      // この関数はfailしない．
      stat = TCL_OK;
    }
    break;

  case NetHandle::kMagMvn:
    {
      // Mvn の場合
      MvnVerilogWriter writer;
      writer(*osp, *neth->mvn());
      // この関数はfailしない．
      stat = TCL_OK;
    }
    break;
  }

  return stat;
}

END_NAMESPACE_MAGUS

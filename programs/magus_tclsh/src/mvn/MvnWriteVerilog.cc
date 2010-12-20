
/// @file magus/lutmap/MvnWriteVerilog.cc
/// @brief MvnWriteVerilog の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MvnWriteVerilog.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnWriteVerilog.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// verilog から SbjGraph に変換するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnWriteVerilog::MvnWriteVerilog(MagMgr* mgr) :
  MvnCmd(mgr)
{
  set_usage_string("?<filename>?");
}

// @brief デストラクタ
MvnWriteVerilog::~MvnWriteVerilog()
{
}

// @brief コマンドを実行する仮想関数
int
MvnWriteVerilog::cmd_proc(TclObjVector& objv)
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

  MvMgr* mgr = cur_network();
  if ( mgr == NULL ) {
    TclObj emsg;
    emsg << "Current network is not a MvNetwork.";
    set_result(emsg);
    return TCL_ERROR;
  }

  dump_verilog(*osp, *mgr);

  return TCL_OK;
}

END_NAMESPACE_MAGUS


/// @file ReadLibrary.cc
/// @brief ReadLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ReadLibrary.h"

#include "MagMgr.h"
#include "utils/MsgMgr.h"
#include "tclpp/TclObjMsgHandler.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス ReadLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadLibrary::ReadLibrary(MagMgr* mgr) :
  MagCmd(mgr)
{
}

// @brief デストラクタ
ReadLibrary::~ReadLibrary()
{
}

// コマンドを実行する．
int
ReadLibrary::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();

  // このコマンドはファイル名を引数としてとる．
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  // コマンド行を解析してファイル名を取って来て，ファイルを開く
  string file_name = objv[1];
  // ファイル名の展開を行う．
  string ex_file_name;
  bool stat = tilde_subst(file_name, ex_file_name);
  if ( !stat ) {
    // ファイル名文字列の中に誤りがあった．
    return TCL_ERROR;
  }

  TclObjMsgHandler mh;
  MsgMgr::reg_handler(&mh);

  // 実際の読み込みを行う．
  const CellLibrary* lib = read_library(ex_file_name);
  if ( lib == NULL ) {
    TclObj emsg = mh.msg_obj();
    emsg << "Error occurred in reading " << objv[1];
    set_result(emsg);
    return TCL_ERROR;
  }

  mgr()->set_cur_cell_library(lib);

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS


/// @file ReadMislib.cc
/// @brief ReadMislib の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ReadMislib.h"

#include "MagMgr.h"
#include "ym_cell/CellMislibReader.h"
#include "ym_utils/MsgMgr.h"
#include "TclObjMsgHandler.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス ReadMislib
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadMislib::ReadMislib(MagMgr* mgr) :
  MagCmd(mgr)
{
  set_usage_string("<filename>");
}

// @brief デストラクタ
ReadMislib::~ReadMislib()
{
}

// コマンドを実行する．
int
ReadMislib::cmd_proc(TclObjVector& objv)
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
  CellMislibReader read;
  const CellLibrary* lib = read(ex_file_name);
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


/// @file DumpLibrary.cc
/// @brief DumpLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DumpLibrary.h"
#include "ym/CellLibrary.h"
#include "ym/FileODO.h"


BEGIN_NAMESPACE_MAGUS

// @brief コンストラクタ
DumpLibrary::DumpLibrary(MagMgr* mgr) :
  MagCmd(mgr)
{
  set_usage_string("output-filename");
}

// @brief デストラクタ
DumpLibrary::~DumpLibrary()
{
}

// @brief コマンドを実行する仮想関数
int
DumpLibrary::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  string dst_filename = objv[1];
  // ファイル名の '~' 置換を行なう．
  string ex_name;
  bool stat = tilde_subst(dst_filename, ex_name);
  if ( !stat ) {
    // ファイル名文字列の中に誤り
    return TCL_ERROR;
  }
  FileODO bo;
  if ( !bo.open(ex_name) ) {
    // 開けなかった．
    TclObj errmsg;
    errmsg << ex_name << ": " << posix_error();
    set_result(errmsg);
    return TCL_ERROR;
  }

  cur_cell_library()->dump(bo);

  return TCL_OK;
}

END_NAMESPACE_MAGUS

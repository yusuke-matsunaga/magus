
/// @file DumpLibrary.cc
/// @brief DumpLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DumpLibrary.h"
#include "ym_cell/CellDumper.h"


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
  ofstream os;
  if ( !open_ofile(os, dst_filename, ios::binary) ) {
    // エラーメッセージは open_ofile() がセットしている．
    return TCL_ERROR;
  }

  CellDumper dump;

  dump(os, *cur_cell_library());

  return TCL_OK;
}

END_NAMESPACE_MAGUS

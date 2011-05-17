
/// @file magus/src/techmap/DumpPatCmd.cc
/// @brief DumpPatCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DumpPatCmd.h"

#include "ym_cell/CellMislibReader.h"
#include "ym_techmap/CellMap.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

// @brief コンストラクタ
DumpPatCmd::DumpPatCmd(MagMgr* mgr,
		       TechmapData* data) :
  TechmapCmd(mgr, data)
{
  set_usage_string("<genlib-file> <pat-file>");
}

// @brief デストラクタ
DumpPatCmd::~DumpPatCmd()
{
}

// @brief コマンドを実行する仮想関数
int
DumpPatCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 3 ) {
    print_usage();
    return TCL_ERROR;
  }

  try {
    string src_filename;
    bool stat1 = tilde_subst(objv[1], src_filename);
    if ( !stat1 ) {
      return TCL_ERROR;
    }

    CellMislibReader reader;
    const CellLibrary* library = reader.read(src_filename);
    if ( library == NULL ) {
      TclObj emsg;
      emsg << "Error occured in reading " << src_filename;
      set_result(emsg);
      return TCL_ERROR;
    }

    string dst_filename = objv[2];
    ofstream os;
    if ( !open_ofile(os, dst_filename, ios::binary) ) {
      // エラーメッセージは open_ofile() がセットしている．
      return TCL_ERROR;
    }

    techmap().dump_library(os, *library);

    return TCL_OK;
  }
  catch ( AssertError x ) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "Assertion Error";
    set_result(emsg);
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS_TECHMAP

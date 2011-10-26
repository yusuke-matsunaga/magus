
/// @file DumpPatCmd.cc
/// @brief DumpPatCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DumpPatCmd.h"

#include "ym_cell/CellMislibReader.h"
#include "ym_cell/CellDotlibReader.h"
#include "ym_cell/CellLibrary.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

// @brief コンストラクタ
DumpPatCmd::DumpPatCmd(MagMgr* mgr) :
  MagCmd(mgr)
{
  mMislibOpt = new TclPoptStr(this, "mislib",
			      "read 'mislib' library file",
			      "mislib-filename");
  mDotlibOpt = new TclPoptStr(this, "dotlib",
			      "read '.lib' library file",
			      "dotlib(liberty)-filename");
  new_popt_group(mMislibOpt, mDotlibOpt);

  set_usage_string("output-filename");
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
  if ( objc != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  bool mislib = mMislibOpt->is_specified();
  bool dotlib = mDotlibOpt->is_specified();

  string arg1;
  if ( mislib ) {
    arg1 = mMislibOpt->val();
  }
  else if ( dotlib ) {
    arg1 = mDotlibOpt->val();
  }
  else {
    print_usage();
    return TCL_ERROR;
  }

  string src_filename;
  bool stat1 = tilde_subst(arg1, src_filename);
  if ( !stat1 ) {
    return TCL_ERROR;
  }

  const CellLibrary* library = NULL;
  if ( mislib ) {
    CellMislibReader reader;
    library = reader.read(src_filename);
  }
  else if ( dotlib ) {
    CellDotlibReader reader;
    library = reader.read(src_filename);
  }

  if ( library == NULL ) {
    TclObj emsg;
    emsg << "Error occured in reading " << src_filename;
    set_result(emsg);
    return TCL_ERROR;
  }

  string dst_filename = objv[1];
  ofstream os;
  if ( !open_ofile(os, dst_filename, ios::binary) ) {
    // エラーメッセージは open_ofile() がセットしている．
    return TCL_ERROR;
  }

  library->dump(os);

  return TCL_OK;
}

END_NAMESPACE_MAGUS_TECHMAP

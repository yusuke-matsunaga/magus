
/// @file magus/cellmap/DumpSbjCmd.cc
/// @brief DumpSbjCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DumpSbjCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "CellmapCmd.h"
#include "MagMgr.h"
#include "SbjGraph.h"
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_MAGUS_CELLMAP

//////////////////////////////////////////////////////////////////////
// SbjGraph の内容をダンプするコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DumpSbjCmd::DumpSbjCmd(MagMgr* mgr,
		       CellmapData* data) :
  CellmapCmd(mgr, data)
{
  mPoptBlif = new TclPopt(this, "blif",
			  "blif mode");
  set_usage_string("?<filename>?");
}

// @brief デストラクタ
DumpSbjCmd::~DumpSbjCmd()
{
}
  
// @brief コマンドを実行する仮想関数
int
DumpSbjCmd::cmd_proc(TclObjVector& objv)
{
  bool blif = mPoptBlif->is_specified();
  size_t objc = objv.size();
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  try {
    ostream* outp = &cout;
    ofstream ofs;
    if ( objc == 2 ) {
      string file_name = objv[1];
      if ( !open_ofile(ofs, file_name) ) {
	// ファイルが開けなかった．
	return TCL_ERROR;
      }
      outp = &ofs;
    }
    if ( blif ) {
      write_blif(*outp, sbjgraph());
    }
    else {
      dump(*outp, sbjgraph());
    }
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

END_NAMESPACE_MAGUS_CELLMAP

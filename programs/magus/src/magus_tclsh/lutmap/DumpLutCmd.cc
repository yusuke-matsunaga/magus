
/// @file magus/lutmap/DumpLutCmd.cc
/// @brief DumpLutCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DumpLutCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "LutmapCmd.h"
#include "LutNetwork.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// マッピングを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DumpLutCmd::DumpLutCmd(NetMgr* mgr,
		       LutmapData* data) :
  LutmapCmd(mgr, data)
{
  set_usage_string("?<filename>");
}

// @brief デストラクタ
DumpLutCmd::~DumpLutCmd()
{
}
  
// @brief コマンドを実行する仮想関数
int
DumpLutCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
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
    lutnetwork().dump(*outp);
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

END_NAMESPACE_MAGUS_LUTMAP


/// @file DumpCnCmd.cc
/// @brief DumpCnCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DumpCnCmd.h"
#include "YmTclpp/TclPopt.h"
#include "YmNetworks/CmnDumper.h"
#include "YmNetworks/CmnBlifWriter.h"
#include "YmNetworks/CmnVerilogWriter.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
// マッピングを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DumpCnCmd::DumpCnCmd(MagMgr* mgr,
		     CmnMgr& cmnmgr) :
  TechmapCmd(mgr, cmnmgr)
{
  mPoptVerilog = new TclPopt(this, "verilog",
			     "verilog mode");
  mPoptBlif = new TclPopt(this, "blif",
			  "blif mode");
  new_popt_group(mPoptVerilog, mPoptBlif);
  set_usage_string("?<filename>");
}

// @brief デストラクタ
DumpCnCmd::~DumpCnCmd()
{
}

// @brief コマンドを実行する仮想関数
int
DumpCnCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }
  bool verilog = mPoptVerilog->is_specified();
  bool blif = mPoptBlif->is_specified();

#if !defined(YM_DEBUG)
  try {
#endif
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
    if ( verilog ) {
      CmnVerilogWriter dump;
      dump(*outp, cmnmgr());
    }
    else if ( blif ) {
      CmnBlifWriter dump;
      dump(*outp, cmnmgr());
    }
    else {
      CmnDumper dump;
      dump(*outp, cmnmgr());
    }
    return TCL_OK;
#if !defined(YM_DEBUG)
  }
  catch ( AssertError x ) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "Assertion Error";
    set_result(emsg);
    return TCL_ERROR;
  }
#endif

  return TCL_OK;
}

END_NAMESPACE_MAGUS_TECHMAP


/// @file magus/logbase/BNetInfoCmd.cc
/// @brief BNetInfoCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetInfoCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BNetInfoCmd.h"
#include "ym/BnNetwork.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ネットワークの諸元を取り出すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BNetInfo::BNetInfo(MagMgr* mgr) :
  BNetCmd(mgr)
{
  const char* usage =
    "<ATTR> ?<value>?\n"
    "\t<ATTR> is model_name, input_num, output_num, logic_num,\n"
    "\t\tdff_num, and library_name";
  set_usage_string(usage);
}

// @brief デストラクタ
BNetInfo::~BNetInfo()
{
}

// コマンドを実行する．
int
BNetInfo::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  // このコマンドは属性を表す引数をとる．
  if ( objc != 2 && objc != 3 ) {
    // 引数の数が合わない．
    print_usage();
    // 異常終了
    return TCL_ERROR;
  }

  BnNetwork* network = cur_network();

  string attr = objv[1];
  TclObj result;
  if ( attr == "model_name" ) {
    result = network->name();
  }
  else {
    if ( attr == "input_num" ) {
      result = network->input_num();
    }
    else if ( attr == "output_num" ) {
      result = network->output_num();
    }
    else if ( attr == "logic_num" ) {
      result = network->logic_num();
    }
    else if ( attr == "dffnum" ) {
      result = network->dff_num();
    }
    else if ( attr == "library_name" ) {
#if defined(USE_LIBCELL)
      const hf_CCellMgr* cell_mgr = network->CellMgr();
      if (cell_mgr == nullptr) {
	result = "not mapped";
      }
      else {
	result = FindLibraryName(cell_mgr);
      }
#else
      result = "not mapped";
#endif
    }
    else {
      TclObj emsg;
      emsg << attr << " : illegal attribute";
      set_result(emsg);
      return TCL_ERROR;
    }
    if ( objc == 3 ) {
      // この属性は読み出し専用
      TclObj emsg;
      emsg << attr << " is read only";
      set_result(emsg);
      return TCL_ERROR;
    }
  }
 set_result(result);

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ネットワークの諸元を一括して取り出して連想配列に入れるコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BNetAllInfo::BNetAllInfo(MagMgr* mgr) :
  BNetCmd(mgr)
{
  set_usage_string("<varname>");
}

// @brief デストラクタ
BNetAllInfo::~BNetAllInfo()
{
}

// コマンドを実行する．
int
BNetAllInfo::cmd_proc(TclObjVector& objv)
{
  size_t objc = objv.size();
  // このコマンドは連想配列名のみを引数にとる．
  if ( objc != 2 ) {
    // 引数の数が合わない．
    print_usage();
    // 異常終了
    return TCL_ERROR;
  }

  TclObj base = objv[1];

  BnNetwork* network = cur_network();

  // モデル名
  set_var(base, "model_name", network->name(), 0);
  // 入力数
  set_var(base, "input_num", network->input_num(), 0);
  // 出力数
  set_var(base, "output_num", network->output_num(), 0);
  // 中間変数の数
  set_var(base, "logic_num", network->logic_num(), 0);
  // FF の数
  set_var(base, "dff_num", network->dff_num(), 0);
  // ライブラリ名
#if defined(USE_LIBCELL)
  const hf_CCellMgr* cell_mgr = network->CellMgr();
  if (cell_mgr == nullptr) {
    set_var(base, "library_name", "not mapped", 0);
  }
  else {
    string libname = FindLibraryName(cell_mgr);
    set_var(base, "library_name", libname, 0);
  }
#else
  set_var(base, "library_name", "not mapped", 0);
#endif
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// ネットワークの緒元を出力するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BNetPrintStats::BNetPrintStats(MagMgr* mgr) :
  BNetCmd(mgr)
{
}

// @brief デストラクタ
BNetPrintStats::~BNetPrintStats()
{
}

// コマンド処理関数
int
BNetPrintStats::cmd_proc(TclObjVector& objv)
{
  BnNetwork* network = cur_network();
  TclObj msg;
  msg << network->name()
      << "\tpi=" << TclObj(network->input_num())
      << "\tpo=" << TclObj(network->output_num())
      << "\tnodes=" << TclObj(network->logic_num())
      << "\tlatches=" << TclObj(network->dff_num());
  set_result(msg);

  return TCL_OK;
}

END_NAMESPACE_MAGUS

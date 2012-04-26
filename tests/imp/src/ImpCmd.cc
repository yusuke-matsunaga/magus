
/// @file ImpCmd.cc
/// @brief ImpCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpCmd.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnBlifReader.h"
#include "ym_networks/BdnIscas89Reader.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/StopWatch.h"
#include "ym_tclpp/TclObjMsgHandler.h"
#include "ym_tclpp/TclPopt.h"

#include "StrImp.h"
#include "ContraImp.h"
#include "SatImp.h"
#include "RlImp.h"
#include "NaImp.h"
#include "ConstImp.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] imp_data 共通のデータ
ImpCmd::ImpCmd(ImpData* imp_data) :
  mData(imp_data)
{
}


// @brief デストラクタ
ImpCmd::~ImpCmd()
{
}

// @brief ImpMgr を得る．
ImpMgr&
ImpCmd::mgr()
{
  return mData->mImpMgr;
}


//////////////////////////////////////////////////////////////////////
// クラス ReadBlifCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] imp_data 共通のデータ
ReadBlifCmd::ReadBlifCmd(ImpData* imp_data) :
  ImpCmd(imp_data)
{
  set_usage_string("<filename>");
}

// @brief デストラクタ
ReadBlifCmd::~ReadBlifCmd()
{
}

// @brief コマンドを実行する仮想関数
int
ReadBlifCmd::cmd_proc(TclObjVector& objv)
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
  BdnMgr bdn_network;

  BdnBlifReader bdn_read;
  if ( !bdn_read(ex_file_name, bdn_network) ) {
    TclObj emsg = mh.msg_obj();
    emsg << "Error occured in reading " << objv[1];
    set_result(emsg);
    return TCL_ERROR;
  }

  // BDN の情報を ImpMgr にコピーする．
  mgr().set(bdn_network);

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// クラス ReadIscas89
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] imp_data 共通のデータ
ReadIscas89Cmd::ReadIscas89Cmd(ImpData* imp_data) :
  ImpCmd(imp_data)
{
  set_usage_string("<filename>");
}

// @brief デストラクタ
ReadIscas89Cmd::~ReadIscas89Cmd()
{
}

// @brief コマンドを実行する仮想関数
int
ReadIscas89Cmd::cmd_proc(TclObjVector& objv)
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
  BdnMgr bdn_network;

  BdnIscas89Reader bdn_read;
  if ( !bdn_read(ex_file_name, bdn_network) ) {
    TclObj emsg = mh.msg_obj();
    emsg << "Error occured in reading " << objv[1];
    set_result(emsg);
    return TCL_ERROR;
  }

  // BDN の情報を ImpMgr にコピーする．
  mgr().set(bdn_network);

  return TCL_OK;
}

// @brief コンストラクタ
// @param[in] imp_data 共通のデータ
LearningCmd::LearningCmd(ImpData* imp_data) :
  ImpCmd(imp_data)
{
  mPoptMethod = new TclPoptStr(this, "method",
			       "specify learning algorithm",
			       "direct|contra|recursive|naive|naive2|exact");
  mPoptLevel = new TclPoptInt(this, "level",
			      "specify recursive learing level",
			      "integer");
  mPoptDump = new TclPopt(this, "dump",
			  "dump indirect implications");
  mPoptVerify = new TclPopt(this, "verify",
			    "verify indirect implications");
}

// @brief デストラクタ
LearningCmd::~LearningCmd()
{
}

// @brief コマンドを実行する仮想関数
int
LearningCmd::cmd_proc(TclObjVector& objv)
{
  StopWatch timer;

  timer.start();

  string method = "direct";
  if ( mPoptMethod->is_specified() ) {
    method = mPoptMethod->val();
  }

  ImpInfo imp_info;
  if ( method == "direct" ) {
    StrImp imp;
    imp.learning(mgr(), imp_info);
  }
  else if ( method == "contra" ) {
    ContraImp imp;
    imp.learning(mgr(), imp_info);
  }
  else if ( method == "recursive" ) {
    RlImp imp;
    if ( mPoptLevel->is_specified() ) {
      int level = mPoptLevel->val();
      imp.set_learning_level(level);
    }
    imp.learning(mgr(), imp_info);
  }
  else if ( method == "naive" ) {
    NaImp imp;
    imp.learning(mgr(), imp_info);
  }
  else if ( method == "naive2" ) {
    NaImp imp;
    imp.use_contra(false);
    imp.learning(mgr(), imp_info);
  }
  else if ( method == "exact" ) {
    SatImp imp;
    imp.learning(mgr(), imp_info);
  }
  else {
    string emsg = method + ": unknown method";
    set_result(emsg);
    return TCL_ERROR;
  }

  timer.stop();
  cout << setw(10) << imp_info.imp_num(mgr()) << " implications"
       << "\t" << timer.time()
       << endl;

  if ( mPoptVerify->is_specified() ) {
    verify(mgr(), imp_info);
  }
  if ( mPoptDump->is_specified() ) {
    imp_info.print(cout);
  }

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// クラス CheckConstCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] imp_data 共通のデータ
CheckConstCmd::CheckConstCmd(ImpData* imp_data) :
  ImpCmd(imp_data)
{
}

// @brief デストラクタ
CheckConstCmd::~CheckConstCmd()
{
}

// @brief コマンドを実行する仮想関数
int
CheckConstCmd::cmd_proc(TclObjVector& objv)
{
  ConstImp imp;
  imp.learning(mgr());
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// クラス PrintCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] imp_data 共通のデータ
PrintCmd::PrintCmd(ImpData* imp_data) :
  ImpCmd(imp_data)
{
  set_usage_string("?<filename>?");
}

// @brief デストラクタ
PrintCmd::~PrintCmd()
{
}

// @brief コマンドを実行する仮想関数
int
PrintCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();

  // このコマンドはファイル名のみを引数にとる．
  // 引数がなければ標準出力に出す．
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  // 出力先のファイルを開く
  ostream* osp = &cout;
  ofstream ofs;
  if ( objc == 2 ) {
    string filename = objv[1];
    if ( !open_ofile(ofs, filename) ) {
      // ファイルが開けなかった．
      return TCL_ERROR;
    }
    osp = &ofs;
  }

  mgr().print_network(*osp);

  return TCL_OK;
}

END_NAMESPACE_YM_NETWORKS


//////////////////////////////////////////////////////////////////////
// imp モジュールの初期化を行う関数
//////////////////////////////////////////////////////////////////////

int
imp_init(Tcl_Interp* interp)
{
  using namespace nsYm::nsNetworks;
  using namespace nsYm;

  //////////////////////////////////////////////////////////////////////
  // imp という namespace を作っておく．
  //////////////////////////////////////////////////////////////////////
  if ( Tcl_Eval(interp, "namespace eval ::imp {}") == TCL_ERROR ) {
    return TCL_ERROR;
  }


  //////////////////////////////////////////////////////////////////////
  // コマンドの登録
  //////////////////////////////////////////////////////////////////////

  ImpData* data = new ImpData;

  TclCmdBinder1<ReadBlifCmd, ImpData*>::reg(interp, data, "imp::read_blif");
  TclCmdBinder1<ReadIscas89Cmd, ImpData*>::reg(interp, data, "imp::read_iscas89");
  TclCmdBinder1<LearningCmd, ImpData*>::reg(interp, data, "imp::learning");
  TclCmdBinder1<CheckConstCmd, ImpData*>::reg(interp, data, "imp::check_const");
  TclCmdBinder1<PrintCmd, ImpData*>::reg(interp, data, "imp::print_network");


  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  const char* completer =
    "namespace eval tclreadline {\n"
    "namespace eval imp {\n"
    "proc complete(read_blif) { t s e l p m } { return \"\" }\n"
    "proc complete(read_iscas89) { t s e l p m } { return \"\" }\n"
    "proc complete(learning) { t s e l p m } { return \"\" }\n"
    "proc complete(check_const) { t s e l p m } { return \"\" }\n"
    "proc complete(print_network) { t s e l p m } { return \"\" }\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, completer) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}


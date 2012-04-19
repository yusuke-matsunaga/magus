
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
#include "ym_tclpp/TclObjMsgHandler.h"
#include "ImpCmd.h"


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



  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  const char* completer =
    "namespace eval tclreadline {\n"
    "namespace eval imp {\n"
    "proc complete(read_blif) { t s e l p m } { return \"\" }\n"
    "proc complete(read_iscas89) { t s e l p m } { return \"\" }\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, completer) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}


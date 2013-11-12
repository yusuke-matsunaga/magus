
/// @file RecCmd.cc
/// @brief RecCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RecCmd.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnBlifReader.h"
#include "ym_networks/BdnIscas89Reader.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/StopWatch.h"
#include "ym_tclpp/TclObjMsgHandler.h"
#include "ym_tclpp/TclPopt.h"

#include "ym_logic/NpnMgr.h"

#include "ym_utils/FileIDO.h"
#include "ym_utils/FileODO.h"

#include "TopDown.h"
#include "FuncMgr.h"
#include "FuncRec.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス RecCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func_mgr FuncMgr
RecCmd::RecCmd(FuncMgr& func_mgr) :
  mFuncMgr(func_mgr)
{
}


// @brief デストラクタ
RecCmd::~RecCmd()
{
}

// @brief FuncMgr を得る．
FuncMgr&
RecCmd::mgr()
{
  return mFuncMgr;
}


//////////////////////////////////////////////////////////////////////
// クラス ReadCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func_mgr FuncMgr
ReadCmd::ReadCmd(FuncMgr& func_mgr) :
  RecCmd(func_mgr)
{
  mMinCutSize = new TclPoptInt(this, "min_cut_size",
			       "specify minimum cut size",
			       "integer");
  mMaxCutSize = new TclPoptInt(this, "max_cut_size",
			       "specify maximum cut size",
			       "integer");
}

// @brief デストラクタ
ReadCmd::~ReadCmd()
{
}

// @brief カット列挙を行い関数の登録を行う．
// @param[in] network 対象のネットワーク
int
ReadCmd::record(const BdnMgr& network)
{
  ymuint min_cut_size = 0;
  ymuint max_cut_size = 0;

  if ( mMinCutSize->is_specified() ) {
    min_cut_size = mMinCutSize->val();
  }
  if ( mMaxCutSize->is_specified() ) {
    max_cut_size = mMaxCutSize->val();
  }
  if ( min_cut_size == 0 ) {
    if ( max_cut_size == 0 ) {
      min_cut_size = 4;
      max_cut_size = 4;
    }
    else {
      min_cut_size = max_cut_size;
    }
  }
  else {
    if ( max_cut_size == 0 ) {
      max_cut_size = min_cut_size;
    }
  }

  FuncRec op(mgr());
  TopDown enumcut;

  op.set_min_size(min_cut_size);
  op.set_debug_level(1);

  enumcut(network, max_cut_size, &op);

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// クラス ReadBlifCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func_mgr FuncMgr
ReadBlifCmd::ReadBlifCmd(FuncMgr& func_mgr) :
  ReadCmd(func_mgr)
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

  int cmd_ok = record(bdn_network);

  return cmd_ok;
}


//////////////////////////////////////////////////////////////////////
// クラス ReadIscas89
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func_mgr FuncMgr
ReadIscas89Cmd::ReadIscas89Cmd(FuncMgr& func_mgr) :
  ReadCmd(func_mgr)
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

  int cmd_ok = record(bdn_network);

  return cmd_ok;
}


//////////////////////////////////////////////////////////////////////
// クラス DumpCmd
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
// @param[in] func_mgr FuncMgr
DumpCmd::DumpCmd(FuncMgr& func_mgr) :
  RecCmd(func_mgr)
{
}

// @brief デストラクタ
DumpCmd::~DumpCmd()
{
}

// @brief コマンドを実行する仮想関数
int
DumpCmd::cmd_proc(TclObjVector& objv)
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

  FileODO bo(ex_file_name);
  if ( !bo ) {
    TclObj emsg;
    emsg << "Could not create " << file_name;
    set_result(emsg);
    return TCL_ERROR;
  }

  mgr().dump(bo);

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// クラス DumpRepCmd
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
// @param[in] func_mgr FuncMgr
DumpRepCmd::DumpRepCmd(FuncMgr& func_mgr) :
  RecCmd(func_mgr)
{
}

// @brief デストラクタ
DumpRepCmd::~DumpRepCmd()
{
}

// @brief コマンドを実行する仮想関数
int
DumpRepCmd::cmd_proc(TclObjVector& objv)
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

  FileODO bo(ex_file_name);
  if ( !bo ) {
    TclObj emsg;
    emsg << "Could not create " << file_name;
    set_result(emsg);
    return TCL_ERROR;
  }

  mgr().dump_rep(bo);

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// クラス RestoreCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func_mgr FuncMgr
RestoreCmd::RestoreCmd(FuncMgr& func_mgr) :
  RecCmd(func_mgr)
{
}

// @brief デストラクタ
RestoreCmd::~RestoreCmd()
{
}

// @brief コマンドを実行する仮想関数
int
RestoreCmd::cmd_proc(TclObjVector& objv)
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

  FileIDO bi(ex_file_name);
  if ( !bi ) {
    TclObj emsg;
    emsg << "Could not open " << file_name;
    set_result(emsg);
    return TCL_ERROR;
  }

  mgr().restore(bi);

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// クラス PrintCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func_mgr FuncMgr
PrintCmd::PrintCmd(FuncMgr& func_mgr) :
  RecCmd(func_mgr)
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

  vector<TvFunc> func_list;
  mgr().func_list(func_list);

  ymuint min_n = 0;
  ymuint max_n = 0;
  for (vector<TvFunc>::const_iterator p = func_list.begin();
       p != func_list.end(); ++ p) {
    const TvFunc& func = *p;
    ymuint ni = func.ni();
    if ( max_n < ni ) {
      max_n = ni;
    }
    if ( min_n == 0 || min_n > ni ) {
      min_n = ni;
    }
  }

#if 0
  NpnMgr npn_mgr;

  ymuint nfall = 0;
  ymuint nrall = 0;
  for (ymuint i = min_n; i <= max_n; ++ i) {
    vector<TvFunc> func_list;
    mgr().func_list(i, func_list);
    hash_set<TvFunc> rep_hash;
    ymuint nrep = 0;
    for (vector<TvFunc>::const_iterator p = func_list.begin();
	 p != func_list.end(); ++ p) {
      const TvFunc& f = *p;
      NpnMap cmap;
      npn_mgr.cannonical(f, cmap);
      TvFunc rep = f.xform(cmap);
      if ( rep_hash.count(rep) == 0 ) {
	rep_hash.insert(rep);
	++ nrep;
      }
    }
    *osp << "Total " << setw(12) << func_list.size() << " " << setw(2) << i << " input functions"
	 << "      " << setw(10) << nrep << " representative functions" << endl;
    nfall += func_list.size();
    nrall += nrep;
  }
  *osp << "Total " << setw(12) << nfall << "          functions"
       << "      " << setw(10) << nrall << " representative functions" << endl;
#else
  for (ymuint i = min_n; i <= max_n; ++ i) {
    vector<TvFunc> func_list;
    mgr().func_list(i, func_list);
    *osp << "Total " << setw(12) << func_list.size() << " " << setw(2) << i << " input functions" << endl;
  }
  *osp << "Total " << setw(12) << func_list.size() << "          functions" << endl;

  vector<TvFunc> rep_func_list;
  mgr().rep_func_list(rep_func_list);
  for (ymuint i = min_n; i <= max_n; ++ i) {
    vector<TvFunc> func_list;
    mgr().rep_func_list(i, func_list);
    *osp << "Total " << setw(12) << func_list.size() << " " << setw(2) << i << " input representative functions" << endl;
  }
  *osp << "Total " << setw(12) << rep_func_list.size() << "          representative functions" << endl;
#endif

  return TCL_OK;
}

END_NAMESPACE_YM_NETWORKS


//////////////////////////////////////////////////////////////////////
// rec モジュールの初期化を行う関数
//////////////////////////////////////////////////////////////////////

int
rec_init(Tcl_Interp* interp)
{
  using namespace nsYm::nsNetworks;
  using namespace nsYm;

  //////////////////////////////////////////////////////////////////////
  // rec という namespace を作っておく．
  //////////////////////////////////////////////////////////////////////
  if ( Tcl_Eval(interp, "namespace eval ::rec {}") == TCL_ERROR ) {
    return TCL_ERROR;
  }


  //////////////////////////////////////////////////////////////////////
  // コマンドの登録
  //////////////////////////////////////////////////////////////////////

  FuncMgr* func_mgr_p = new FuncMgr;
  FuncMgr& func_mgr = *func_mgr_p;

  TclCmdBinder1<ReadBlifCmd, FuncMgr&>::reg(interp, func_mgr, "rec::read_blif");
  TclCmdBinder1<ReadIscas89Cmd, FuncMgr&>::reg(interp, func_mgr, "rec::read_iscas89");
  TclCmdBinder1<DumpCmd, FuncMgr&>::reg(interp, func_mgr, "rec::dump");
  TclCmdBinder1<DumpRepCmd, FuncMgr&>::reg(interp, func_mgr, "rec::dump_rep");
  TclCmdBinder1<RestoreCmd, FuncMgr&>::reg(interp, func_mgr, "rec::restore");
  TclCmdBinder1<PrintCmd, FuncMgr&>::reg(interp, func_mgr, "rec::print");


  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の処理
  // すごい手抜き
  //////////////////////////////////////////////////////////////////////
  const char* completer =
    "namespace eval tclreadline {\n"
    "namespace eval rec {\n"
    "proc complete(read_blif) { t s e l p m } { return \"\" }\n"
    "proc complete(read_iscas89) { t s e l p m } { return \"\" }\n"
    "proc complete(dump) { t s e l p m } { return \"\" }\n"
    "proc complete(dump_rep) { t s e l p m } { return \"\" }\n"
    "proc complete(restore) { t s e l p m } { return \"\" }\n"
    "proc complete(print) { t s e l p m } { return \"\" }\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, completer) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}

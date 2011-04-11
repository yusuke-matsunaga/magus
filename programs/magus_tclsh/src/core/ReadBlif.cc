
/// @file src/core/ReadBlif.cc
/// @brief ReadBlif の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetIoCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ReadBlif.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadBlif::ReadBlif(MagMgr* mgr) :
  NetCmd(mgr, true, true, false)
{
  mReader.add_msg_handler(&mMsgHandler);
  set_usage_string("<filename>");
}

// @brief デストラクタ
ReadBlif::~ReadBlif()
{
}

// コマンドを実行する．
int
ReadBlif::cmd_proc(TclObjVector& objv)
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

  // 実際の読み込みを行う．
  NetHandle* neth = cur_nethandle();
  switch ( neth->type() ) {
  case NetHandle::kMagBNet:
    {
      bool result = mReader.read(ex_file_name, *neth->_bnetwork());

      // エラーが起きていないか調べる．
      if ( !result ) {
	TclObj emsg = mMsgHandler.msg_obj();
	mMsgHandler.clear();
	emsg << "Error occurred in reading " << objv[1];
	set_result(emsg);
	return TCL_ERROR;
      }
    }
    break;

  case NetHandle::kMagBdn:
    {
    }
    break;

  case NetHandle::kMagMvn:
    {
    }
    break;
  }

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS


/// @file src/core/ReadBlif.cc
/// @brief ReadBlif の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ReadBlif.h"

#include "ym/BnNetwork.h"
#include "ym/MsgMgr.h"
#include "ym/TclObjMsgHandler.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadBlif::ReadBlif(MagMgr* mgr) :
  NetCmd(mgr, true, false)
{
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

  TclObjMsgHandler mh;
  MsgMgr::attach_handler(&mh);

  // 実際の読み込みを行う．
  NetHandle* neth = cur_nethandle();
  switch ( neth->type() ) {
  case NetHandle::kMagBn:
    {
      *neth->_bnetwork() = BnNetwork::read_blif(ex_file_name, cur_cell_library());

      // エラーが起きていないか調べる．
      if ( neth->_bnetwork()->node_num() == 0 ) {
	TclObj emsg = mh.msg_obj();
	emsg << "Error occurred in reading " << objv[1];
	set_result(emsg);
	return TCL_ERROR;
      }
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

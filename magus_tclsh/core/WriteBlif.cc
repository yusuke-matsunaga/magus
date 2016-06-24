
/// @file WriteBlif.cc
/// @brief WriteBlif の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2017 Yusuke Matsunaga
/// All rights reserved.


#include "WriteBlif.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ファイルに blif 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
WriteBlif::WriteBlif(MagMgr* mgr) :
  NetCmd(mgr, false, false)
{
  set_usage_string("?<filename>?");
}

// @brief デストラクタ
WriteBlif::~WriteBlif()
{
}

// コマンドを実行する．
int
WriteBlif::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();

  // このコマンドはファイル名のみを引数に取る．
  // 引数がなければ標準出力に出す．
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  // 出力先のファイルを開く
  ostream* osp = &cout;
  ofstream ofs;
  if ( objc == 2 ) {
    string file_name = objv[1];
    if ( !open_ofile(ofs, file_name) ) {
      // ファイルが開けなかった．
      return TCL_ERROR;
    }
    osp = &ofs;
  }

  int stat = TCL_ERROR;  // 念のため TCL_ERROR を入れておく．

  NetHandle* neth = cur_nethandle();
  switch ( neth->type() ) {
  case NetHandle::kMagBn:
    {
#if 0
      BNetBlifWriter writer;
      writer.dump(*osp, *neth->bnetwork());
#endif
      // この関数はfailしない．
      stat = TCL_OK;
    }
    break;

  case NetHandle::kMagMvn:
    {
      TclObj emsg;
      emsg << "Network type mismatch.";
      set_result(emsg);
      stat = TCL_ERROR;
    }
    break;
  }
  return stat;
}

END_NAMESPACE_MAGUS


/// @file WriteEqu.cc
/// @brief WriteEqu の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2017 Yusuke Matsunaga
/// All rights reserved.


#include "WriteEqu.h"

#include "YmNetworks/BNetEquWriter.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ファイルに EQU 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
WriteEqu::WriteEqu(MagMgr* mgr) :
  BNetCmd(mgr)
{
  set_usage_string("?<filename>?");
}

// @brief デストラクタ
WriteEqu::~WriteEqu()
{
}

// コマンドを実行する．
int
WriteEqu::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();

  // 引き数の数をチェックする．
  if( objc > 2 ){
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

  bool result = false;  // 念のため FALSE を入れておく．

  // 実際の書き出しを行う．
  BNetEquWriter writer;
  writer.dump(*osp, *(cur_network()));
  // この関数はfailしない．
  result = true;

  // 結果がエラーでないか調べる．今はエラーとはならない．
  if ( !result ) {
    TclObj msg;
    msg << "Error occured in writing " << objv[1];
    set_result(msg);
    return TCL_ERROR;
  }

  // 正常終了
  return TCL_OK;
}

END_NAMESPACE_MAGUS

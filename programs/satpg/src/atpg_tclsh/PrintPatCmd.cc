
/// @file PrintPatCmd.cc
/// @brief PrintPatCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PrintPatCmd.h"
#include "AtpgMgr.h"
#include "TestVector.h"
#include "YmTclpp/TclPopt.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintPatCmd::PrintPatCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  mPoptHex = new TclPopt(this, "hex",
			 "print in hex mode");
  mPoptNum = new TclPopt(this, "num",
			 "print with index numbering");
  set_usage_string("?filename?");
}

// @brief デストラクタ
PrintPatCmd::~PrintPatCmd()
{
}

// コマンド処理関数
int
PrintPatCmd::cmd_proc(TclObjVector& objv)
{
  // このコマンドはファイル名のみを引数に取る．
  // 引数がなければ標準出力に出す．
  ymuint objc = objv.size();
  if ( objc > 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  // 出力先のストリームを開く
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

  // 参照を使いたいのでめんどくさいことをやっている．
  ostream& out = *osp;

  bool hex_flag = mPoptHex->is_specified();
  bool num_flag = mPoptNum->is_specified();

  vector<TestVector*>& tvlist = _tv_list();
  ymuint n = tvlist.size();
  for (ymuint i = 0; i < n; ++ i) {
    TestVector* tv = tvlist[i];
    if ( num_flag ) {
      out << setw(5) << setfill('0') << (i + 1) << ": ";
    }
    if ( hex_flag ) {
      out << tv->hex_str();
    }
    else {
      out << tv->bin_str();
    }
    out << endl;
  }

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

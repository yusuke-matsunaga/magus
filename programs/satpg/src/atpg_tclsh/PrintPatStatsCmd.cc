
/// @file PrintPatStatsCmd.cc
/// @brief PrintPatStatsCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PrintPatStatsCmd.h"
#include "AtpgMgr.h"
#include "TestVector.h"
#include "YmTclpp/TclPopt.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrintPatStatsCmd::PrintPatStatsCmd(AtpgMgr* mgr) :
  AtpgCmd(mgr)
{
  mPoptHist = new TclPopt(this, "histgram",
			  "print histgram mode");
  set_usage_string("?filename?");
}

// @brief デストラクタ
PrintPatStatsCmd::~PrintPatStatsCmd()
{
}

// コマンド処理関数
int
PrintPatStatsCmd::cmd_proc(TclObjVector& objv)
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

  bool hist_mode = mPoptHist->is_specified();

  vector<TestVector*>& tvlist = _tv_list();
  ymuint n = tvlist.size();
  if ( n == 0 ) {
    out << "No patterns" << endl;
  }
  else {
    ymuint ni = tvlist[0]->input_num();
    vector<ymuint> hist(ni + 1, 0);
    for (ymuint i = 0; i < n; ++ i) {
      TestVector* tv = tvlist[i];
      ymuint nx = tv->x_num();
      ASSERT_COND( nx <= ni );
      ++ hist[nx];
    }
    out << endl
	 << "X-num" << endl;
    ymuint total_num = 0;
    for (ymuint i = 0; i <= ni; ++ i) {
      if ( hist[i] > 0 ) {
	total_num += hist[i] * i;
	if ( hist_mode ) {
	  out << setw(8) << i << ": " << setw(8) << hist[i] << endl;
	}
      }
    }
    double ave = (double)total_num / n;
    out << "Average: " << ave << " / " << ni << endl;
  }

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

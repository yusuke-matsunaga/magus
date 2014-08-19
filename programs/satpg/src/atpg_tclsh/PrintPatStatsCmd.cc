
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
}

// @brief デストラクタ
PrintPatStatsCmd::~PrintPatStatsCmd()
{
}

// コマンド処理関数
int
PrintPatStatsCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  bool hist_mode = mPoptHist->is_specified();

  vector<TestVector*>& tvlist = _tv_list();
  ymuint n = tvlist.size();
  if ( n == 0 ) {
    cout << "No patterns" << endl;
  }
  else {
    ymuint ni = tvlist[0]->input_num();
    vector<ymuint> hist(ni + 1, 0);
    for (ymuint i = 0; i < n; ++ i) {
      TestVector* tv = tvlist[i];
      ymuint nx = tv->x_num();
      assert_cond( nx <= ni, __FILE__, __LINE__);
      ++ hist[nx];
    }
    cout << endl
	 << "X-num" << endl;
    ymuint total_num = 0;
    for (ymuint i = 0; i <= ni; ++ i) {
      if ( hist[i] > 0 ) {
	total_num += hist[i] * i;
	if ( hist_mode ) {
	  cout << setw(8) << i << ": " << setw(8) << hist[i] << endl;
	}
      }
    }
    double ave = (double)total_num / n;
    cout << "Average: " << ave << " / " << ni << endl;
  }

  return TCL_OK;
}

END_NAMESPACE_YM_SATPG

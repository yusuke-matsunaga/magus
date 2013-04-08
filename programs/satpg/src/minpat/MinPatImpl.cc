
/// @file MinPatImpl.cc
/// @brief MinPatImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatImpl.h"
#include "TestVector.h"
#include "GcMgr.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス MinPatImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim2 2値の故障シミュレータ
// @param[in] fsim3 3値の故障シミュレータ
MinPatImpl::MinPatImpl(Fsim& fsim2,
		       Fsim& fsim3) :
  mFsim2(fsim2),
  mFsim3(fsim3)
{
}

// @brief デストラクタ
MinPatImpl::~MinPatImpl()
{
}

// @brief テストベクタの最小化を行なう．
// @param[in] tv_list テストベクタのリスト
// @param[in] stats 実行結果の情報を格納する変数
void
MinPatImpl::run(vector<TestVector*>& tv_list,
		MinPatStats& stats)
{
  // 3値のパタンを抜き出し tv3_list に入れる．
  // 2値のパタンは tv2_list に入れる．
  vector<TestVector*> tv2_list;
  vector<TestVector*> tv3_list;
  tv3_list.reserve(tv_list.size());
  for (vector<TestVector*>::iterator p = tv_list.begin();
       p != tv_list.end(); ++ p) {
    TestVector* tv = *p;
    if ( tv->x_num() > 0 ) {
      tv3_list.push_back(tv);
    }
    else {
      tv2_list.push_back(tv);
    }
  }

  if ( !tv3_list.empty() ) {
    // 最小彩色問題を解くことで3値のパタンを圧縮する．
    GcMgr gcmgr;

    ymuint n = tv3_list.size();
    ymuint ni = tv3_list[0]->input_num();
    gcmgr.init(n);
    for (ymuint i1 = 1; i1 < n; ++ i1) {
      TestVector* tv1 = tv3_list[i1];
      for (ymuint i2 = 0; i2 < i1; ++ i2) {
	TestVector* tv2 = tv3_list[i2];
	bool conflict = false;
	for (ymuint j = 0; j < ni; ++ j) {
	  Val3 v1 = tv1->val3(j);
	  Val3 v2 = tv2->val3(j);
	  if ( v1 != kValX && v2 != kValX && v1 != v2 ) {
	    conflict = true;
	    break;
	  }
	}
	if ( conflict ) {
	  gcmgr.connect(i1, i2);
	}
      }
    }
    vector<vector<ymuint> > color_group;
    ymuint nc = gcmgr.coloring(color_group);


  }

}


END_NAMESPACE_YM_SATPG

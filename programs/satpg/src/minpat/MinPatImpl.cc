
/// @file MinPatImpl.cc
/// @brief MinPatImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatImpl.h"


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
}


END_NAMESPACE_YM_SATPG


/// @file DopTvList.cc
/// @brief DopTvList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DopTvList.h"
#include "AtpgMgr.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
// @param[in] mgr AtpgMgr
DetectOp*
new_DopTvList(AtpgMgr& mgr)
{
  return new DopTvList(mgr._tv_list());
}


//////////////////////////////////////////////////////////////////////
// クラス DopTvList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tvlist テストベクタのリスト
DopTvList::DopTvList(vector<TestVector*>& tvlist) :
  mTvList(tvlist)
{
}

// @brief デストラクタ
DopTvList::~DopTvList()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] tv テストパタン
void
DopTvList::operator()(TpgFault* f,
		      TestVector* tv)
{
  mTvList.push_back(tv);
}

END_NAMESPACE_YM_SATPG

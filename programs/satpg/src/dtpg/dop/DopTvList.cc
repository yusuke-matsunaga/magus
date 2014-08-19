
/// @file DopTvList.cc
/// @brief DopTvList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DopTvList.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
// @param[in] tvlist テストベクタのリスト
DetectOp*
new_DopTvList(vector<TestVector*>& tvlist)
{
  return new DopTvList(tvlist);
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

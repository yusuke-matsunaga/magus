
/// @file DopTvList.cc
/// @brief DopTvList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DopTvList.h"
#include "TvMgr.h"
#include "NodeValList.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
// @param[in] tvmgr テストベクタのマネージャ
// @param[in] tvlist テストベクタのリスト
DetectOp*
new_DopTvList(TvMgr& tvmgr,
	      vector<TestVector*>& tvlist)
{
  return new DopTvList(tvmgr, tvlist);
}


//////////////////////////////////////////////////////////////////////
// クラス DopTvList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tvmgr テストベクタのマネージャ
// @param[in] tvlist テストベクタのリスト
DopTvList::DopTvList(TvMgr& tvmgr,
		     vector<TestVector*>& tvlist) :
  mTvMgr(tvmgr),
  mTvList(tvlist)
{
}

// @brief デストラクタ
DopTvList::~DopTvList()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] assign_list 値割当のリスト
void
DopTvList::operator()(TpgFault* f,
		      const NodeValList& assign_list)
{
  TestVector* tv = mTvMgr.new_vector();
  ymuint n = assign_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    Assign as = assign_list[i];
    ymuint id = as.node_id();
    if ( as.val() ) {
      tv->set_val(id, kVal1);
    }
    else {
      tv->set_val(id, kVal0);
    }
  }
  mTvList.push_back(tv);
}

END_NAMESPACE_YM_SATPG

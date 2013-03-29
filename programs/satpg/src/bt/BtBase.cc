
/// @file BtBase.cc
/// @brief BtBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "BtBase.h"
#include "TvMgr.h"
#include "TestVector.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス BtBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tvmgr TvMgr
BtBase::BtBase(TvMgr& tvmgr) :
  mTvMgr(tvmgr)
{
}

// @brief デストラクタ
BtBase::~BtBase()
{
}

// @brief SAT の値割り当て用ベクタを返す．
vector<Bool3>&
BtBase::model()
{
  return mModel;
}

// @brief テストベクタを生成する．
// @note 結果は mCurPattern に格納される．
TestVector*
BtBase::new_vector()
{
  mCurPattern = mTvMgr.new_vector();

  return mCurPattern;
}

// @brief 入力ノードの値を記録する．
// @param[in] node 対象の外部入力ノード
// @note node の値を mCurPattern に記録する．
void
BtBase::record_value(TpgNode* node)
{
  assert_cond( node->is_input(), __FILE__, __LINE__);

  Bool3 v = node_gval(node);
  ymuint iid = node->input_id();
  if ( v == kB3False ) {
    mCurPattern->set_val(iid, kVal0);
  }
  else if ( v == kB3True ) {
    mCurPattern->set_val(iid, kVal1);
  }
}

END_NAMESPACE_YM_SATPG

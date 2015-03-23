
/// @file BtBase.cc
/// @brief BtBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
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

// @brief ノードID番号の最大値を設定する．
/// @param[in] max_id ID番号の最大値
//
// このクラスの実装ではなにもしない．
void
BtBase::set_max_id(ymuint max_id)
{
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
// @param[in] model SAT の割り当て結果
// @note node の値を mCurPattern に記録する．
void
BtBase::record_value(TpgNode* node,
		     const vector<Bool3>& model)
{
  ASSERT_COND( node->is_input() );

  Bool3 v = node_gval(node, model);
  ymuint iid = node->input_id();
  if ( v == kB3False ) {
    mCurPattern->set_val(iid, kVal0);
  }
  else if ( v == kB3True ) {
    mCurPattern->set_val(iid, kVal1);
  }
}

END_NAMESPACE_YM_SATPG

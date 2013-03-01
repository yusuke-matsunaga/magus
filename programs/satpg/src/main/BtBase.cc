
/// @file BtBase.cc
/// @brief BtBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "BtBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス BtBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_id ノードの最大 ID + 1 ( = TpgNetwork::node_num() )
BtBase::BtBase(ymuint max_id)
{
  mValList.reserve(max_id);
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

// @brief バックトレースの結果の割り当てリストを返す．
const vector<ymuint32>&
BtBase::val_list()
{
  return mValList;
}

// @brief 入力ノードの値を記録する．
// @param[in] node 対象の外部入力ノード
// @note node の値を mValList に記録する．
// @note 単純だが mModel 上のインデックスと mValList の符号化は異なる．
void
BtBase::record_value(TpgNode* node)
{
  assert_cond( node->is_input(), __FILE__, __LINE__);

  // 今のところ model には 0 か 1 しか設定されていないはず．
  Bool3 v = node_gval(node);
  ymuint iid = node->input_id();
  ymuint packed_val = iid * 2;
  if ( v == kB3True ) {
    packed_val += 1;
  }
  mValList.push_back(packed_val);
}

END_NAMESPACE_YM_SATPG

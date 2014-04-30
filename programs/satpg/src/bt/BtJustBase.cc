
/// @file BtJustBase.cc
/// @brief BtJustBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtJustBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス BtJust1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tvmgr TvMgr
// @param[in] max_id ノードの最大 ID + 1 ( = TpgNetwork::node_num() )
BtJustBase::BtJustBase(TvMgr& tvmgr,
		       ymuint max_id) :
  BtBase(tvmgr),
  mJustifiedMarkArray(max_id)
{
  mJustifiedNodeList.reserve(max_id);
}

// @brief デストラクタ
BtJustBase::~BtJustBase()
{
}

// @brief justified マークを消す．
// @param[in] node 対象のノード
void
BtJustBase::clear_justified()
{
  for (vector<TpgNode*>::iterator p = mJustifiedNodeList.begin();
       p != mJustifiedNodeList.end(); ++ p) {
    TpgNode* node = *p;
    cout << "clear_justfied(Node#" << node->id() << ")" << endl;
    assert_cond( node->id() < mJustifiedMarkArray.size(), __FILE__, __LINE__);
    mJustifiedMarkArray[node->id()] = false;
    clear_justified_hook(node);
  }
  mJustifiedNodeList.clear();
}

// @brief clear_justified() 中で呼ばれるフック関数
// @note デフォルトの実装はなにもしない．
void
BtJustBase::clear_justified_hook(TpgNode* node)
{
}

END_NAMESPACE_YM_SATPG

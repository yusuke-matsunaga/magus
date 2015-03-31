
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
BtJustBase::BtJustBase()
{
}

// @brief デストラクタ
BtJustBase::~BtJustBase()
{
}

// @brief ノードID番号の最大値を設定する．
// @param[in] max_id ID番号の最大値
void
BtJustBase::set_max_id(ymuint max_id)
{
  mJustifiedMarkArray.clear();
  mJustifiedMarkArray.resize(max_id, false);
  mJustifiedNodeList.reserve(max_id);
}

// @brief justified マークを消す．
// @param[in] node 対象のノード
void
BtJustBase::clear_justified()
{
  for (vector<TpgNode*>::iterator p = mJustifiedNodeList.begin();
       p != mJustifiedNodeList.end(); ++ p) {
    TpgNode* node = *p;
    ASSERT_COND( node->id() < mJustifiedMarkArray.size() );
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


/// @file ImpNode.cc
/// @brief ImpNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] handle0 ファンイン0のハンドル
// @param[in] handle1 ファンイン1のハンドル
ImpNode::ImpNode(ImpNodeHandle handle0,
		 ImpNodeHandle handle1)
{
  mFanins[0].set(handle0, this, 0);
  ImpNode* node0 = handle0.node();
  if ( node0 != NULL ) {
    node0->mFanouts.push_back(&mFanins[0]);
  }
  mFanins[1].set(handle1, this, 1);
  ImpNode* node1 = handle1.node();
  if ( node1 != NULL ) {
    node1->mFanouts.push_back(&mFanins[1]);
  }
}

// @brief デストラクタ
ImpNode::~ImpNode()
{
}

// @brief 外部入力のときに true を返す．
bool
ImpNode::is_input() const
{
  return false;
}

// @brief AND タイプのときに true を返す．
bool
ImpNode::is_and() const
{
  return false;
}

END_NAMESPACE_YM_NETWORKS

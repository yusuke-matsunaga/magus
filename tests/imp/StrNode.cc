
/// @file StrNode.cc
/// @brief StrNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "StrNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス StrNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] node0 ファンイン0のノード
// @param[in] inv0 ファンイン0の極性
// @param[in] node1 ファンイン1のノード
// @param[in] inv1 ファンイン1の極性
StrNode::StrNode(ymuint id,
		 StrNode* node0,
		 bool inv0,
		 StrNode* node1,
		 bool inv1)
{
  mId = id;
  if ( node0 != NULL ) {
    mFanins[0].set(node0, inv0, this, 0);
    node0->mFanouts.push_back(&mFanins[0]);
  }
  if ( node1 != NULL ) {
    mFanins[1].set(node1, inv1, this, 1);
    node1->mFanouts.push_back(&mFanins[1]);
  }
}

// @brief デストラクタ
StrNode::~StrNode()
{
}

// @brief 外部入力のときに true を返す．
bool
StrNode::is_input() const
{
  return false;
}

// @brief AND タイプのときに true を返す．
bool
StrNode::is_and() const
{
  return false;
}

// @brief XOR タイプのときに true を返す．
bool
StrNode::is_xor() const
{
  return false;
}

END_NAMESPACE_YM_NETWORKS

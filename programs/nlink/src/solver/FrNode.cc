
/// @file FrNode.cc
/// @brief FrNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FrNode.h"

BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス FrNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] edge_id 枝番号
// @param[in] fr フロンティアの情報
FrNode::FrNode(ymuint edge_id,
	       const FrontierInfo& fr) :
  mEdgeId(edge_id),
  mFrontierInfo(fr)
{
}

// @brief デストラクタ
FrNode::~FrNode()
{
}

// @brief 0終端ノードを作る．
FrNode*
FrNode::const0()
{
  FrNode* node = new FrNode(0, FrontierInfo());
  node->mConstFlag = 0;
  return node;
}

// @brief 1終端ノードを作る．
FrNode*
FrNode::const1()
{
  FrNode* node = new FrNode(0, FrontierInfo());
  node->mConstFlag = 1;
  return node;
}

// @brief 0終端ノードの時 true を返す．
bool
FrNode::is_const0() const
{
  return mConstFlag == 0;
}

// @brief 1終端ノードの時 true を返す．
bool
FrNode::is_const1() const
{
  return mConstFlag == 1;
}

// @brief 枝番号を返す．
ymuint
FrNode::edge_id() const
{
  return mEdgeId;
}

// @brief フロンティア情報を返す．
const FrontierInfo&
FrNode::frontier_info() const
{
  return mFrontierInfo;
}

// @brief 0枝の先のノードを返す．
FrNode*
FrNode::edge0() const
{
  return mEdge0;
}

// @brief 0枝の先のノードを返す．
FrNode*
FrNode::edge1() const
{
  return mEdge1;
}

// @brif 0枝と1枝をセットする．
void
FrNode::set_child(FrNode* node0,
		  FrNode* node1)
{
  mEdge0 = node0;
  mEdge1 = node1;
}

END_NAMESPACE_YM_NLINK

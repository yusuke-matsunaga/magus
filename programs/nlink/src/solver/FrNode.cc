
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
FrNode::FrNode(ymuint edge_id) :
  mEdgeId(edge_id)
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
  FrNode* node = new FrNode(0);
  node->mConstFlag = 0;
  return node;
}

// @brief 1終端ノードを作る．
FrNode*
FrNode::const1()
{
  FrNode* node = new FrNode(0);
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

// @brief フロンティア情報を返す．
FrontierInfo&
FrNode::frontier_info()
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

// @brif 0枝をセットする．
void
FrNode::set_edge0(FrNode* node)
{
  mEdge0 = node;
}

// @brif 1枝をセットする．
void
FrNode::set_edge1(FrNode* node)
{
  mEdge1 = node;
}

END_NAMESPACE_YM_NLINK

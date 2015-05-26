
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

END_NAMESPACE_YM_NLINK

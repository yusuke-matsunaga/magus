
/// @file FrgGraph.cc
/// @brief FrgGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FrgGraph.h"
#include "FrgNode.h"
#include "FrgEdge.h"
#include "NlProblem.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス FrgGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] problem 問題
FrgGraph::FrgGraph(const NlProblem& problem)
{
  mWidth = problem.width();
  mHeight = problem.height();

  // 節点の生成
  ymuint nn = mWidth * mHeight;
  mNodeList.resize(nn);
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      FrgNode* node = new FrgNode;
      node->mId = x * mHeight + y;
      node->mX = x;
      node->mY = y;
      node->mTermId = 0;
    }
  }

  // 枝の生成
  ymuint ne = (mHeight - 1) * mWidth + mHeight * (mWidth - 1);
  ymuint id = 0;
  for (ymuint x = 0; x < mWidth; ++ x) {
    // 縦の枝を生成
    for (ymuint y = 0; y < mHeight - 1; ++ y) {
      FrgEdge* edge = new FrgEdge;
      edge->mId = id;
      ++ id;
      FrgNode* node1 = node(x, y);
      FrgNode* node2 = node(x, y + 1);
      edge->mNode1 = node1;
      edge->mNode2 = node2;
      node1->mEdgeList.push_back(edge);
      node2->mEdgeList.push_back(edge);
    }
    if ( x < mWidth - 1 ) {
      // 横の枝を生成
      for (ymuint y = 0; y < mHeight; ++ y) {
	FrgEdge* edge = new FrgEdge;
	edge->mId = id;
	++ id;
	FrgNode* node1 = node(x, y);
	FrgNode* node2 = node(x + 1, y);
	edge->mNode1 = node1;
	edge->mNode2 = node2;
	node1->mEdgeList.push_back(edge);
	node2->mEdgeList.push_back(edge);
      }
    }
  }

  // 端点の情報を設定
  for (ymuint i = 0; i < problem.elem_num(); ++ i) {
    NlPoint start_point = problem.start_point(i);
    FrgNode* start_node = node(start_point.x(), start_point.y());
    start_node->mTermId = i + 1;

    NlPoint end_point = problem.end_point(i);
    FrgNode* end_node = node(end_point.x(), end_point.y());
    end_node->mTermId = i + 1;
  }
}

// @brief デストラクタ
FrgGraph::~FrgGraph()
{
  for (ymuint i = 0; i < mNodeList.size(); ++ i) {
    delete mNodeList[i];
  }
  for (ymuint i = 0; i < mEdgeList.size(); ++ i) {
    delete mEdgeList[i];
  }
}

// @brief 幅を返す．
ymuint
FrgGraph::width() const
{
  return mWidth;
}

// @brief 高さを返す．
ymuint
FrgGraph::height() const
{
  return mHeight;
}

// @brief ノード数を返す．
ymuint
FrgGraph::node_num() const
{
  return mNodeList.size();
}

// @brief ノード番号からノードを返す．
// @param[in] id ノード番号
FrgNode*
FrgGraph::node(ymuint id) const
{
  ASSERT_COND( id < node_num() );
  return mNodeList[id];
}

// @brief 座標からノードを返す．
// @param[in] x, y 座標
FrgNode*
FrgGraph::node(ymuint x,
	       ymuint y) const
{
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y < mHeight );
  return mNodeList[x * mHeight + y];
}

// @brief 枝数を返す．
ymuint
FrgGraph::edge_num() const
{
  return mEdgeList.size();
}

// @brief 枝番号から枝を返す．
FrgEdge*
FrgGraph::edge(ymuint id) const
{
  ASSERT_COND( id < edge_num() );
  return mEdgeList[id];
}

END_NAMESPACE_YM_NLINK

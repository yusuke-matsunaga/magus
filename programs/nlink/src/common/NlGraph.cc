
/// @file NlGraph.cc
/// @brief NlGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlGraph.h"
#include "NlNode.h"
#include "NlEdge.h"
#include "NlProblem.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlGraph::NlGraph()
{
}

// @brief デストラクタ
NlGraph::~NlGraph()
{
  clear();
}

// @brief 内容をクリアする．
void
NlGraph::clear()
{
  for (ymuint i = 0; i < mNodeArray.size(); ++ i) {
    delete mNodeArray[i];
  }
  mNodeArray.clear();

  for (ymuint i = 0; i < mEdgeArray.size(); ++ i) {
    delete mEdgeArray[i];
  }
  mEdgeArray.clear();

  mTermArray.clear();
}

// @brief 問題を設定する．
// @param[in] problem 問題
void
NlGraph::set_problem(const NlProblem& problem)
{
  ymuint width = problem.width();
  ymuint height = problem.height();
  ymuint num = problem.elem_num();

  ASSERT_COND( width > 0 );
  ASSERT_COND( height > 0 );
  ASSERT_COND( num > 0 );

  clear();

  mWidth = width;
  mHeight = height;
  mNum = num;

  mTermArray.resize(num * 2);

  // 横の枝の数
  ymuint nh = (width - 1) * height;
  // 縦の枝の数
  ymuint nv = (height - 1) * width;
  // 枝の数
  mMaxEdgeId = nh + nv;
  mEdgeArray.resize(mMaxEdgeId);

  // 横の枝を作る．
  for (ymuint x = 0; x < mWidth - 1; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      ymuint id = x * mHeight + y;
      ostringstream buf;
      buf << "H(" << x << ", " << y << ")";
      NlEdge* edge = new NlEdge(id, buf.str());
      mEdgeArray[id] = edge;
    }
  }

  // 縦の枝を作る．
  for (ymuint y = 0; y < mHeight - 1; ++ y) {
    for (ymuint x = 0; x < mWidth; ++ x) {
      ymuint id = y * mWidth + x + nh;
      ostringstream buf;
      buf << "V(" << x << ", " << y << ")";
      NlEdge* edge = new NlEdge(id, buf.str());
      mEdgeArray[id] = edge;
    }
  }


  // ノードを作る．
  mMaxNodeId = width * height;
  mNodeArray.resize(mMaxNodeId);
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      ymuint id = x * mHeight + y;
      NlNode* node = new NlNode(id, x, y);
      mNodeArray[id] = node;
    }
  }

  // ノードを作り，枝を接続する．
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      ymuint id = x * mHeight + y;
      NlNode* node = new NlNode(id, x, y);
      mNodeArray[id] = node;
      vector<const NlEdge*> tmp_list;
      tmp_list.reserve(4);
      if ( x > 0 ) {
	// 左に辺がある．
	NlEdge* edge = mEdgeArray[(x - 1) * mHeight + y];
	tmp_list.push_back(edge);
	node->mLeftEdge = edge;
	edge->mNode2 = node;
      }
      if ( x < mWidth - 1 ) {
	// 右に辺がある．
	NlEdge* edge = mEdgeArray[x * mHeight + y];
	tmp_list.push_back(edge);
	node->mRightEdge = edge;
	edge->mNode1 = node;
      }
      if ( y > 0 ) {
	// 上に辺がある．
	NlEdge* edge = mEdgeArray[(y - 1) * mWidth + x + nh];
	tmp_list.push_back(edge);
	node->mUpperEdge = edge;
	edge->mNode2 = node;
      }
      if ( y < mHeight - 1 ) {
	// 下に辺がある．
	NlEdge* edge = mEdgeArray[y * mWidth + x + nh];
	tmp_list.push_back(edge);
	node->mLowerEdge = edge;
	edge->mNode1 = node;
      }
      node->mEdgeList = tmp_list;
    }
  }

  // 始点と終点に印をつける．
  for (ymuint k = 0; k < num; ++ k) {
    NlConnection con = problem.connection(k);
    {
      NlPoint start_point = con.start_point();
      ymuint x1 = start_point.x();
      ymuint y1 = start_point.y();
      NlNode* node1 = _node(x1, y1);
      node1->mTermId = k + 1;
      mTermArray[k * 2 + 0] = node1;
    }
    {
      NlPoint end_point = con.end_point();
      ymuint x2 = end_point.x();
      ymuint y2 = end_point.y();
      NlNode* node2 = _node(x2, y2);
      node2->mTermId = k + 1;
      mTermArray[k * 2 + 1] = node2;
    }
  }
}

END_NAMESPACE_YM_NLINK

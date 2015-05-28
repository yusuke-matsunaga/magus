
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

  // 横の辺の数
  ymuint nh = (width - 1) * height;
  mHbase = 1;

  // 縦の辺の数
  ymuint nv = (height - 1) * width;
  mVbase = nh + 1;

  // 辺の数
  mMaxEdgeId = nh + nv;

  // ノードを作る．
  mMaxNodeId = width * height;
  mNodeArray.resize(mMaxNodeId);
  for (ymuint i = 0; i < mMaxNodeId; ++ i) {
    NlNode* node = new NlNode;
    mNodeArray[i] = node;
    node->mId = i;
    node->mTermId = 0;
  }

  // 辺の隣接関係を作る．
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      NlNode* node = _node(x, y);
      node->mX = x;
      node->mY = y;
      node->mEdgeList.reserve(4);
      if ( x > 0 ) {
	// 左に辺がある．
	ymuint edge = left_edge(x, y);
	node->mEdgeList.push_back(edge);
	node->mLeftEdge = edge;
      }
      if ( x < mWidth - 1 ) {
	// 右に辺がある．
	ymuint edge = right_edge(x, y);
	node->mEdgeList.push_back(edge);
	node->mRightEdge = edge;
      }
      if ( y > 0 ) {
	// 上に辺がある．
	ymuint edge = upper_edge(x, y);
	node->mEdgeList.push_back(edge);
	node->mUpperEdge = edge;
      }
      if ( y < mHeight - 1 ) {
	// 下に辺がある．
	ymuint edge = lower_edge(x, y);
	node->mEdgeList.push_back(edge);
	node->mLowerEdge = edge;
      }
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

// @brief 枝番号から枝を表す文字列を返す．
// @param[in] edge_id 枝番号
string
NlGraph::edge_str(ymuint edge_id) const
{
  if ( edge_id < mHbase ) {
    return "---";
  }
  if ( edge_id < mVbase ) {
    ymuint tmp = edge_id - mHbase;
    ymuint x = tmp / mHeight;
    ymuint y = tmp % mHeight;
    ostringstream buf;
    buf << "H(" << x << ", " << y << ")";
    return buf.str();
  }
  ymuint tmp = edge_id - mVbase;
  ymuint x = tmp % mWidth;
  ymuint y = tmp / mWidth;
  ostringstream buf;
  buf << "V(" << x << ", " << y << ")";
  return buf.str();
}

END_NAMESPACE_YM_NLINK

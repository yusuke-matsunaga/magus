
/// @file GcSolver.cc
/// @brief GcSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GcSolver.h"
#include "GcNode.h"
#include "NodeHeap.h"
#include "YmUtils/Graph.h"


BEGIN_NAMESPACE_YM

// @brief 彩色問題を解く
// @param[in] graph 対象のグラフ
// @param[out] color_group 同じ色のノード番号のリストの配列
// @return 彩色数を返す．
ymuint
coloring(const Graph& graph,
	 vector<vector<ymuint> >& color_group)
{
  GcSolver gcsolver(graph);
  return gcsolver.coloring(color_group);
}


BEGIN_NONAMESPACE

// @brief GcSolver 用の比較関数
class GcComp
{
public:

  /// @brief ノードの比較関数
  int
  operator()(GcNode* node1,
	     GcNode* node2);

};

// @brief ノードの比較関数
int
GcComp::operator()(GcNode* node1,
		   GcNode* node2)
{
  if ( node1->sat_degree() < node2->sat_degree() ) {
    return 1;
  }
  if ( node1->sat_degree() > node2->sat_degree() ) {
    return -1;
  }
  if ( node1->adj_degree() < node2->adj_degree() ) {
    return 1;
  }
  if ( node1->adj_degree() > node2->adj_degree() ) {
    return -1;
  }
  return 0;
}


// @brief ノードに彩色して情報を更新する．
void
color_node(GcNode* node,
	   ymuint color,
	   NodeHeap<GcNode, GcComp>& node_heap)
{
  // node に color の色を割り当てる．
  node->set_color(color);

  // node に隣接するノードの SAT degree を更新する．
  const vector<GcNode*>& node_list = node->link_list();
  for (vector<GcNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    GcNode* node1 = *p;
    if ( node1->color() == 0 ) {
      // node1 が未着色の場合
      if ( !node1->check_adj_color(color) ) {
	// node1 にとって color は新規の隣り合う色だった．
	node1->add_adj_color(color);

	// SAT degree が変わったのでヒープ上の位置も更新する．
	node_heap.update(node1);
      }
    }
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス GcSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] graph 対象のグラフ
GcSolver::GcSolver(const Graph& graph)
{
  mNodeNum = graph.node_num();
  if ( mNodeNum > 0 ) {
    ymuint vectlen = (mNodeNum + 63) / 64;
    mNodeArray = new GcNode[mNodeNum];
    for (ymuint i = 0; i < mNodeNum; ++ i) {
      GcNode* node = &mNodeArray[i];
      node->init(i, vectlen);
    }
  }
  else {
    mNodeArray = NULL;
  }

  for (ymuint i = 0; i < graph.edge_num(); ++ i) {
    pair<ymuint, ymuint> p = graph.edge(i);
    ymuint id1 = p.first;
    ymuint id2 = p.second;

    ASSERT_COND( id1 < graph.node_num() );
    ASSERT_COND( id2 < graph.node_num() );

    GcNode* node1 = &mNodeArray[id1];
    GcNode* node2 = &mNodeArray[id2];

    connect(node1, node2);
  }
}

// @brief デストラクタ
GcSolver::~GcSolver()
{
  delete [] mNodeArray;
}

// @brief 彩色する．
// @param[in] color_group 同じ色のノード番号のリストの配列
// @return 彩色数を返す．
ymuint
GcSolver::coloring(vector<vector<ymuint> >& color_group)
{
  // dsatur アルゴリズムを用いる．

  NodeHeap<GcNode, GcComp>  node_heap(mNodeNum);

  for (ymuint i = 0; i < mNodeNum; ++ i) {
    node_heap.put_node(&mNodeArray[i]);
  }

  mMaxColor = 1;

  // 1: 隣接するノード数が最大のノードを選び彩色する．
  //    ソートしているので先頭のノードを選べば良い．
  GcNode* max_node = node_heap.get_min();
  color_node(max_node, mMaxColor, node_heap);

  // 2: saturation degree が最大の未彩色ノードを選び最小の色番号で彩色する．
  while ( !node_heap.empty() ) {
    GcNode* max_node = node_heap.get_min();
    // max_node につけることのできる最小の色番号を求める．
    clear_count();
    ymuint cnum = 0;
    const vector<GcNode*>& node_list = max_node->link_list();
    for (vector<GcNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      GcNode* node1 = *p;
      ymuint c = node1->color();
      if ( c > 0 ) {
	if ( !mCountArray[c - 1] ) {
	  mCountArray[c - 1] = true;
	  ++ cnum;
	  if ( cnum == mMaxColor ) {
	    break;
	  }
	}
      }
    }
    ymuint min_col = 0;
    for (ymuint c = 0; c < mMaxColor; ++ c) {
      if ( !mCountArray[c] ) {
	min_col = c + 1;
	break;
      }
    }
    if ( min_col == 0 ) {
      ++ mMaxColor;
      min_col = mMaxColor;
    }
    color_node(max_node, min_col, node_heap);
  }

  // 検証
  // もちろん最小色数ではないが，同じ色が隣接していないことを確認する．
  // また，未彩色のノードがないことも確認する．
  // 違反が見つかったら例外を送出する．
  if ( true ) {
    for (ymuint i = 0; i < mNodeNum; ++ i) {
      GcNode* node = &mNodeArray[i];
      ymuint c0 = node->color();
      ASSERT_COND( c0 > 0 );

      const vector<GcNode*>& node_list = node->link_list();
      for (vector<GcNode*>::const_iterator p = node_list.begin();
	   p != node_list.end(); ++ p) {
	GcNode* node1 = *p;
	ymuint c1 = node1->color();
	ASSERT_COND( c0 != c1 );
      }
    }
  }

  // 結果を color_group に入れる．
  color_group.clear();
  color_group.resize(mMaxColor);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    GcNode* node = &mNodeArray[i];
    ymuint c = node->color();

    ASSERT_COND( c > 0 );
    ASSERT_COND( c <= mMaxColor );

    color_group[c - 1].push_back(i);
  }

  return mMaxColor;
}

// @brief mCountArray をクリアする．
void
GcSolver::clear_count()
{
  mCountArray.clear();
  mCountArray.resize(mMaxColor, false);
}


//////////////////////////////////////////////////////////////////////
// クラス GcNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GcNode::GcNode()
{
  mColorSet = NULL;
  mSatDegree = 0;
  mColor = 0;
}

// @brief デストラクタ
GcNode::~GcNode()
{
  delete [] mColorSet;
}

// @brief 初期化する．
void
GcNode::init(ymuint id,
	     ymuint vectlen)
{
  mId = id;
  mColorSet = new ymuint64[vectlen];
  for (ymuint i = 0; i < vectlen; ++ i) {
    mColorSet[i] = 0UL;
  }
}

END_NAMESPACE_YM

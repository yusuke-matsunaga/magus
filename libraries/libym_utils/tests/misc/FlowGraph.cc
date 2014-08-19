
/// @file FlowGraph.cc
/// @brief FlowGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "FlowGraph.h"
#include "FlowNode.h"
#include "FlowEdge.h"
#include "FlowNodeHeap.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FlowGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FlowGraph::FlowGraph()
{
  mSource = new_node();
  mSink = new_node();
}

// @brief デストラクタ
FlowGraph::~FlowGraph()
{
}

// @brief ソースノードを返す．
FlowNode*
FlowGraph::source_node()
{
  return mSource;
}

// @brief シンクノードを返す．
FlowNode*
FlowGraph::sink_node()
{
  return mSink;
}

// @brief ノードを追加する．
// @return 作成したノードを返す．
FlowNode*
FlowGraph::new_node()
{
  FlowNode* node = new FlowNode(mNodeList.size());
  mNodeList.push_back(node);

  return node;
}

// @brief 枝を追加する．
// @param[in] from 入力元のノード
// @param[in] to 出力先のノード
// @param[in] cost コスト
FlowEdge*
FlowGraph::new_edge(FlowNode* from,
		    FlowNode* to,
		    ymuint cost)
{
  FlowEdge* edge = new FlowEdge(mEdgeList.size(), from, to, cost);
  mEdgeList.push_back(edge);

  from->mOutEdgeList.push_back(edge);
  to->mInEdgeList.push_back(edge);

  return edge;
}

// @brief 最小コスト最大フローを求める．
void
FlowGraph::max_flow()
{
  for ( ; ; ) {
    // mSource - mSink を結ぶ最小コストパスを求める．
    vector<FlowEdge*> edge_list;
    bool stat = pfs(edge_list);
    if ( stat ) {
      ymuint n = edge_list.size();
      for (ymuint i = 0; i < n; ++ i) {
	FlowEdge* edge = edge_list[n - i - 1];
	edge->mSelected = !edge->mSelected;
      }
    }
    else {
      break;
    }
  }
}

bool
FlowGraph::pfs(vector<FlowEdge*>& edge_list)
{
  vector<bool> mark(mNodeList.size(), false);
  FlowNodeHeap heap;

  mSource->mDistance = 0;
  heap.put(mSource);

  while ( !heap.empty() ) {
    FlowNode* node = heap.getmin();

    const vector<FlowEdge*>& edge_list1 = node->outedge_list();
    for (vector<FlowEdge*>::const_iterator p = edge_list1.begin();
	 p != edge_list1.end(); ++ p) {
      FlowEdge* edge = *p;
      if ( edge->mSelected ) {
	continue;
      }

      FlowNode* node1 = edge->to();
      ymint d = node->mDistance + edge->cost();
      if ( mark[node1->id()] ) {
	if ( node1->mDistance > d ) {
	  node1->mDistance = d;
	  node1->mParent = edge;
	  heap.move(node1->mPos);
	}
      }
      else {
	mark[node1->id()] = true;
	node1->mDistance = d;
	node1->mParent = edge;
	heap.put(node1);
      }
    }

    const vector<FlowEdge*>& edge_list2 = node->inedge_list();
    for (vector<FlowEdge*>::const_iterator p = edge_list2.begin();
	 p != edge_list2.end(); ++ p) {
      FlowEdge* edge = *p;
      if ( !edge->mSelected ) {
	continue;
      }

      FlowNode* node1 = edge->from();
      ymint d = node->mDistance - edge->cost();
      if ( mark[node1->id()] ) {
	if ( node1->mDistance > d ) {
	  node1->mDistance = d;
	  node1->mParent = edge;
	  heap.move(node1->mPos);
	}
      }
      else {
	mark[node1->id()] = true;
	node1->mDistance = d;
	node1->mParent = edge;
	heap.put(node1);
      }
    }
  }

  if ( mark[mSink->id()] && mSink->mDistance > 0 ) {
    for (FlowEdge* edge = mSink->mParent; ; ) {
      edge_list.push_back(edge);
      FlowNode* node = NULL;
      if ( edge->mSelected ) {
	node = edge->from();
      }
      else {
	node = edge->to();
      }
      if ( node == mSource ) {
	break;
      }
      edge = node->mParent;
    }
    return true;
  }

  return false;
}

END_NAMESPACE_YM

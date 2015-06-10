
/// @file NlSolverDfs.cc
/// @brief NlSolverDfs の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolverDfs.h"
#include "NlSolution.h"
#include "NlGraph.h"
#include "NlNode.h"
#include "NlEdge.h"

#include "YmUtils/HashMap.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_NLINK

Ban::Ban(const NlGraph& graph) :
  mGraph(graph),
  mEdgeArray(graph.max_edge_id()),
  mNodeArray(graph.max_node_id())
{
  for (ymuint edge_id = 0; edge_id < graph.max_edge_id(); ++ edge_id) {
    const NlEdge* edge = graph.edge(edge_id);
    const NlNode* node1 = edge->node1();
    const NlNode* node2 = edge->node2();
    if ( node1->terminal_id() > 0 && node2->terminal_id() > 0 &&
	 node1->terminal_id() != node2->terminal_id() ) {
      // この枝は選べない
      mEdgeArray[edge_id] = 2;
    }
    else {
      mEdgeArray[edge_id] = 0;
    }
  }

  for (ymuint node_id = 0; node_id < graph.max_node_id(); ++ node_id) {
    const NlNode* node = graph.node(node_id);
    ymuint term_id = node->terminal_id();
    if ( term_id > 0 ) {
      mNodeArray[node_id] = - term_id;
    }
    else {
      mNodeArray[node_id] = node_id;
    }
  }
}

bool
Ban::select(ymuint edge_id)
{
  ASSERT_COND( mEdgeArray[edge_id] == 0 );

  mEdgeArray[edge_id] = 1;

  const NlEdge* edge = mGraph.edge(edge_id);
  const NlNode* node1 = edge->node1();
  const NlNode* node2 = edge->node2();

  int val1 = mNodeArray[node1->id()];
  int val2 = mNodeArray[node2->id()];

  if ( val1 < 0 ) {
    if ( val2 < 0 ) {
      if ( val1 != val2 ) {
	// 異なる線分がつながった．
	return false;
      }
    }
    else {
      // val2 を val1 に変更
      for (ymuint i = 0; i < mNodeArray.size(); ++ i) {
	if ( mNodeArray[i] == val2 ) {
	  mNodeArray[i] = val1;
	}
      }
    }
  }
  else if ( val2 < 0 ) {
    // val1 を val2 に変更
    for (ymuint i = 0; i < mNodeArray.size(); ++ i) {
      if ( mNodeArray[i] == val1 ) {
	mNodeArray[i] = val2;
      }
    }
  }
  else { // val1 >= 0 && val2 >= 0
    if ( val1 == val2 ) {
      // サイクル発生
      return false;
    }
    if ( val1 < val2 ) {
      // val2 を val1 に変更
      for (ymuint i = 0; i < mNodeArray.size(); ++ i) {
	if ( mNodeArray[i] == val2 ) {
	  mNodeArray[i] = val1;
	}
      }
    }
    else {
      // val1 を val2 に変更
      for (ymuint i = 0; i < mNodeArray.size(); ++ i) {
	if ( mNodeArray[i] == val1 ) {
	  mNodeArray[i] = val2;
	}
      }
    }
  }
  return true;
}

bool
Ban::unselect(ymuint edge_id)
{
  ASSERT_COND( mEdgeArray[edge_id] == 0 );

  mEdgeArray[edge_id] = 2;

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス NlSolverDfs
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlSolverDfs::NlSolverDfs()
{
}

// @brief デストラクタ
NlSolverDfs::~NlSolverDfs()
{
}

// @brief 問題を解く
// @param[in] graph 問題のグラフ
// @param[in] verbose verbose フラグ
// @param[out] solution 解
void
NlSolverDfs::solve(const NlGraph& graph,
		   bool verbose,
		   NlSolution& solution)
{
  mSolution.init(graph);

  ymuint ne = graph.max_edge_id();
  vector<const NlEdge*> edge_list;

  Ban ban0(graph);

  bool stat = dfs_recur(ban0);
  if ( stat ) {
    cout << "FOUND" << endl;
    solution = mSolution;
  }
}

// @brief 探索を行う．
bool
NlSolverDfs::dfs_recur(const Ban& ban)
{
  Ban ban1(ban);

  if ( false ) {
    ymuint n = 0;
    for (ymuint i = 0; i < ban.mNodeArray.size(); ++ i) {
      if ( ban.mNodeArray[i] >= 0 ) {
	++ n;
      }
    }
    cout << "dfs_recur[" << n << "]" << endl;
  }

  // 必須割当を行う．
  for ( ; ; ) {
    bool no_change = true;
    for (ymuint node_id = 0; node_id < ban1.mGraph.max_node_id(); ++ node_id) {
      const NlNode* node = ban1.mGraph.node(node_id);
      const vector<const NlEdge*>& edge_list = node->edge_list();
      ymuint ne = edge_list.size();
      vector<ymuint> tmp_list;
      tmp_list.reserve(ne);
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (ymuint i = 0; i < ne; ++ i) {
	const NlEdge* edge = edge_list[i];
	ymuint edge_id = edge->id();
	switch ( ban1.mEdgeArray[edge_id] ) {
	case 0:
	  tmp_list.push_back(edge_id);
	  ++ n0;
	  break;

	case 1:
	  ++ n1;
	  break;
	}
      }
      if ( node->terminal_id() > 0 ) {
	if ( n1 == 0 ) {
	  if ( n0 == 1 ) {
	    // 唯一の枝を選ぶ．
	    ymuint edge_id = tmp_list[0];
	    if ( !ban1.select(edge_id) ) {
	      return false;
	    }
	    no_change = false;
	  }
	}
	else if ( n1 == 1 ) {
	  if ( n0 > 0 ) {
	    // 残りの枝を非選択にする．
	    for (ymuint i = 0; i < n0; ++ i) {
	      ymuint edge_id = tmp_list[i];
	      if ( !ban1.unselect(edge_id) ) {
		return false;
	      }
	    }
	    no_change = false;
	  }
	}
	else {
	  // エラー
	  return false;
	}
      }
      else {
	if ( n1 == 0 ) {
	  ;
	}
	else if ( n1 == 1 ) {
	  if ( n0 == 0 ) {
	    // エラー
	    return false;
	  }
	  else if ( n0 == 1 ) {
	    // 唯一の枝を選ぶ．
	    ymuint edge_id = tmp_list[0];
	    if ( !ban1.select(edge_id) ) {
	      return false;
	    }
	    no_change = false;
	  }
	  else {
	    ;
	  }
	}
	else if ( n1 == 2 ) {
	  if ( n0 > 0 ) {
	    // 残りの枝を非選択にする．
	    for (ymuint i = 0; i < n0; ++ i) {
	      ymuint edge_id = tmp_list[i];
	      if ( !ban1.unselect(edge_id) ) {
		return false;
	      }
	      ban1.mEdgeArray[edge_id] = 2;
	    }
	    no_change = false;
	  }
	}
	else { // n1 > 2
	  // エラー
	  return false;
	}
      }
    }
    if ( no_change ) {
      break;
    }
  }

  // 次に処理するノードを選ぶ．
  const NlNode* next_node = NULL;
  for (ymuint node_id = 0; node_id < ban1.mGraph.max_node_id(); ++ node_id) {
    const NlNode* node = ban1.mGraph.node(node_id);
    const vector<const NlEdge*>& edge_list = node->edge_list();
    ymuint ne = edge_list.size();
    vector<ymuint> tmp_list;
    tmp_list.reserve(ne);
    ymuint n0 = 0;
    ymuint n1 = 0;
    for (ymuint i = 0; i < ne; ++ i) {
      const NlEdge* edge = edge_list[i];
      ymuint edge_id = edge->id();
      switch ( ban1.mEdgeArray[edge_id] ) {
      case 0:
	tmp_list.push_back(edge_id);
	++ n0;
	break;

      case 1:
	++ n1;
	break;
      }
    }
    if ( n0 > 0 ) {
      int val = ban1.mNodeArray[node->id()];
      if ( val < 0 ) {
	next_node = node;
	break;
      }
    }
  }
  if ( next_node == NULL ) {
    // 全ての線分がつながった．
    ymuint w = ban1.mGraph.width();
    ymuint h = ban1.mGraph.height();
    for (ymuint x = 0; x < w; ++ x) {
      for (ymuint y = 0; y < h; ++ y) {
	if ( mSolution.get(x, y) < 0 ) {
	  continue;
	}
	int val = ban1.mNodeArray[x * h + y];
	mSolution.set(x, y, - val);
      }
    }
    return true;
  }

  const vector<const NlEdge*>& edge_list = next_node->edge_list();
  for (ymuint i = 0; i < edge_list.size(); ++ i) {
    const NlEdge* edge = edge_list[i];
    ymuint edge_id = edge->id();
    if ( ban1.mEdgeArray[edge_id] != 0 ) {
      continue;
    }

    Ban ban2(ban1);
    if ( ban2.select(edge_id) ) {
      bool stat = dfs_recur(ban2);
      if ( stat ) {
	return true;
      }
    }
  }

  return false;
}

END_NAMESPACE_YM_NLINK

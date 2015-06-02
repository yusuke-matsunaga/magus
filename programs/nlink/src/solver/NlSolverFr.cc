
/// @file NlSolverFr.cc
/// @brief NlSolverFr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolverFr.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "NlGraph.h"
#include "NlNode.h"
#include "NlEdge.h"
#include "FrontierInfo.h"
#include "FrNode.h"

#include "YmUtils/HashMap.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM

template<>
struct
HashFunc<vector<ymuint> >
{
  ymuint
  operator()(const vector<ymuint>& key) const
  {
    ymuint ans = 0U;
    for (ymuint i = 0; i < key.size(); ++ i) {
      ans += ans * 117 + ans;
    }
    return ans;
  }
};

END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolverFr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlSolverFr::NlSolverFr()
{
}

// @brief デストラクタ
NlSolverFr::~NlSolverFr()
{
}

// @brief 問題を解く
// @param[in] problem 問題
// @param[in] verbose verbose フラグ
// @param[out] solution 解
void
NlSolverFr::solve(const NlProblem& problem,
		  bool verbose,
		  NlSolution& solution)
{
  NlGraph graph;

  graph.set_problem(problem);

  ymuint ne = graph.max_edge_id();
  vector<const NlEdge*> edge_list;

  // 枝の順序付けを行う．
  ordering(graph, edge_list);

  // 各レベルごとの解集合
  vector<vector<FrNode*> > node_list_array(ne + 1);

  FrNode* root_node = new FrNode(0, FrontierInfo());
  node_list_array[0].push_back(root_node);

  FrNode* const0_node = FrNode::const0();
  FrNode* const1_node = FrNode::const1();

  for (ymuint i = 0; i < ne; ++ i) {
    const NlEdge* edge = edge_list[i];
    const vector<FrNode*>& node_list = node_list_array[i];
    vector<FrNode*>& new_node_list = node_list_array[i + 1];
    cout << endl;
    cout << "Level#" << i << ": " << node_list.size() << endl;
    cout << "  edge = " << edge->str() << endl;
    HashMap<string, FrNode*> node_hash;
    for (ymuint j = 0; j < node_list.size(); ++ j) {
      FrNode* node = node_list[j];
#if 0
      cout << endl;
      cout << "FR:" << node->mBits << endl;
      node->frontier_info().dump(cout);
#endif

      // edge を選ばないノードを作る．
      FrNode* node0 = NULL;
      // 親のフロンティアをコピーする．
      FrontierInfo fr0 = node->frontier_info();

      // フロンティアの更新
      // edge が最後の未処理枝だったノードを削除し
      // edge が最初の処理枝だったノードを追加する．
      // その際に，削除されるノードが条件を満たさなければ false を返す．
      bool stat0 = update_frontier(fr0, edge, false, graph);
#if 0
      cout << " FR0:" << node->mBits << "0" << endl;
      fr0.dump(cout);
#endif
      if ( !stat0 ) {
	// node の 0 枝は0
	node0 = const0_node;
      }
      else {
	string sig0 = fr0.signature();
	if ( node_hash.find(sig0, node0) ) {
	  ;
	}
	else {
	  // node の 0 枝に node0 をセットする．
	  if ( i < ne - 1 ) {
	    node0 = new FrNode(i + 1, fr0);
	    node0->mBits = node->mBits + "0";
	  }
	  else {
	    node0 = const1_node;
	  }

	  new_node_list.push_back(node0);
	  node_hash.add(sig0, node0);
	}
      }

      // edge を選ぶノードを作る．
      FrNode* node1 = NULL;
      // 親のフロンティアをコピーする．
      FrontierInfo fr1 = node->frontier_info();

      // フロンティアの更新
      // edge が最後の未処理枝だったノードを削除し
      // edge が最初の処理枝だったノードを追加する．
      // その際に，削除されるノードが条件を満たさなければ false を返す．
      bool stat1 = update_frontier(fr1, edge, true, graph);
#if 0
      cout << " FR1:" << node->mBits << "1" << endl;
      fr1.dump(cout);
#endif
      if ( !stat1 ) {
	// node の 1 枝は NULL
	node1 = const0_node;
      }
      else {
	string sig1 = fr1.signature();
	if ( node_hash.find(sig1, node1) ) {
	  ;
	}
	else {
	  if ( i < ne -  1 ) {
	    node1 = new FrNode(i + 1, fr1);
	    node1->mBits = node->mBits + "1";
	  }
	  else {
	    node1 = const1_node;
	  }

	  new_node_list.push_back(node1);
	  node_hash.add(sig1, node1);
	}
      }
      node->set_child(node0, node1);
    }
  }

  solution.init(problem);
}

// @brief 枝の順序付けを行う．
void
NlSolverFr::ordering(const NlGraph& graph,
		     vector<const NlEdge*>& edge_list)
{
  ymuint ne = graph.max_edge_id();
  edge_list.reserve(ne);
  mEdgeOrder.clear();
  mEdgeOrder.resize(ne);
  ymuint w = graph.width();
  ymuint h = graph.height();
  if ( w >= h ) {
    for (ymuint y0 = 0; y0 < h - 1; ++ y0) {
      for (ymuint x = 0; x < w; ++ x) {
	if ( x > y0 ) {
	  break;
	}
	ymuint y = y0 - x;
	const NlNode* node = graph.node(x, y);
	{
	  const NlEdge* edge = node->lower_edge();
	  ymuint order = edge_list.size();
	  edge_list.push_back(edge);
	  mEdgeOrder[edge->id()] = order;
	}
	{
	  const NlEdge* edge = node->right_edge();
	  ymuint order = edge_list.size();
	  edge_list.push_back(edge);
	  mEdgeOrder[edge->id()] = order;
	}
      }
    }
    for (ymuint x0 = 1; x0 < w; ++ x0) {
      for (ymuint dy = 0; dy < h - 1; ++ dy) {
	ymuint x = x0 + dy;
	ymuint y = h - dy - 1;
	if ( x >= w ) {
	  break;
	}
	const NlNode* node = graph.node(x, y);
	{
	  const NlEdge* edge = node->left_edge();
	  ymuint order = edge_list.size();
	  edge_list.push_back(edge);
	  mEdgeOrder[edge->id()] = order;
	}
	{
	  const NlEdge* edge = node->upper_edge();
	  ymuint order = edge_list.size();
	  edge_list.push_back(edge);
	  mEdgeOrder[edge->id()] = order;
	}
      }
    }
  }
  else {
    for (ymuint x0 = 0; x0 < w - 1; ++ x0) {
      for (ymuint y = 0; y < h; ++ y) {
	if ( y > x0 ) {
	  break;
	}
	ymuint x = x0 - y;
	const NlNode* node = graph.node(x, y);
	{
	  const NlEdge* edge = node->lower_edge();
	  ymuint order = edge_list.size();
	  edge_list.push_back(edge);
	  mEdgeOrder[edge->id()] = order;
	}
	{
	  const NlEdge* edge = node->right_edge();
	  ymuint order = edge_list.size();
	  edge_list.push_back(edge);
	  mEdgeOrder[edge->id()] = order;
	}
      }
    }
    for (ymuint y0 = 1; y0 < h; ++ y0) {
      for (ymuint dx = 0; dx < w - 1; ++ dx) {
	ymuint x = w - dx - 1;
	ymuint y = y0 + dx;
	if ( y >= h ) {
	  break;
	}
	const NlNode* node = graph.node(x, y);
	{
	  const NlEdge* edge = node->left_edge();
	  ymuint order = edge_list.size();
	  edge_list.push_back(edge);
	  mEdgeOrder[edge->id()] = order;
	}
	{
	  const NlEdge* edge = node->upper_edge();
	  ymuint order = edge_list.size();
	  edge_list.push_back(edge);
	  mEdgeOrder[edge->id()] = order;
	}
      }
    }
  }
}

bool
NlSolverFr::update_frontier(FrontierInfo& fr,
			    const NlEdge* edge,
			    bool selected,
			    const NlGraph& graph)
{
  // ノードIDをキーにしてフロンティア内の位置を保持するマップ
  vector<int> pos_map(graph.max_node_id(), -1);
  vector<ymuint> del_list;
  vector<ymuint> pos_list;
  del_list.reserve(fr.node_num());
  pos_list.reserve(fr.node_num());
  for (ymuint i = 0; i < fr.node_num(); ++ i) {
    ymuint id = fr.node_id(i);
    pos_map[id] = i;
    const NlNode* node = graph.node(id);
    const vector<const NlEdge*>& edge_list = node->edge_list();
    bool found = false;
    for (ymuint j = 0; j < edge_list.size(); ++ j) {
      const NlEdge* edge1 = edge_list[j];
      if ( mEdgeOrder[edge1->id()] > mEdgeOrder[edge->id()] ) {
	// まだ未処理の枝がある．
	found = true;
	break;
      }
    }
    if ( !found ) {
      // 全て処理した．
      del_list.push_back(id);
      pos_list.push_back(i);
    }
  }

  const NlNode* node1 = edge->node1();
  int pos1 = pos_map[node1->id()];
  if ( pos1 >= 0 ) {
    if ( selected ) {
      // deg を増やす
      fr.inc_deg(pos1);
      ymuint deg = fr.deg(pos1);
      if ( node1->terminal_id() > 0 ) {
	if ( deg > 1 ) {
#if 0
	  cout << "C: node(" << node1->x() << ", " << node1->y() << ").deg = " << deg << endl;
#endif
	  return false;
	}
      }
      else {
	if ( deg > 2 ) {
#if 0
	  cout << "D: node(" << node1->x() << ", " << node1->y() << ").deg = " << deg << endl;
#endif
	  return false;
	}
      }
    }
  }
  else {
    // node1 が新たにフロンティアに加わった．
    ymuint deg = selected ? 1 : 0;
    int comp_id = node1->id();
    if ( node1->terminal_id() > 0 ) {
      comp_id = - node1->terminal_id();
    }
    pos1 = fr.node_num();
    fr.add_node(node1->id(), deg, comp_id);
  }

  const NlNode* node2 = edge->node2();
  int pos2 = pos_map[node2->id()];
  if ( pos2 >= 0 ) {
    if ( selected ) {
      // deg を増やす．
      fr.inc_deg(pos2);
      ymuint deg = fr.deg(pos2);
      if ( node2->terminal_id() > 0 ) {
	if ( deg > 1 ) {
#if 0
	  cout << "E: node(" << node2->x() << ", " << node2->y() << ").deg = " << deg << endl;
#endif
	  return false;
	}
      }
      else {
	if ( deg > 2 ) {
#if 0
	  cout << "F: node(" << node2->x() << ", " << node2->y() << ").deg = " << deg << endl;
#endif
	  return false;
	}
      }
    }
  }
  else {
    // node2 が新たにフロンティアに加わった．
    ymuint deg = selected ? 1 : 0;
    int comp_id = node2->id();
    if ( node2->terminal_id() > 0 ) {
      comp_id = - node2->terminal_id();
    }
    pos2 = fr.node_num();
    fr.add_node(node2->id(), deg, comp_id);
  }

  for (ymuint i = 0; i < del_list.size(); ++ i) {
    ymuint id = del_list[i];
    const NlNode* node = graph.node(id);
    ymuint deg = fr.deg(pos_list[i]);
    if ( node->terminal_id() > 0 ) {
      // 終端ノードの場合，deg は 1 でなければならない．
      if ( deg != 1 ) {
#if 0
	cout << "A: node(" << node->x() << ", " << node->y() << ").deg = " << deg << endl;
#endif
	return false;
      }
    }
    else {
      // 中間ノードの場合，deg は 0 または 2 でなければならない．
      if ( deg != 0 && deg != 2 ) {
#if 0
	cout << "B: node(" << node->x() << ", " << node->y() << ").deg = " << deg << endl;
#endif
	return false;
      }
    }
  }

  if ( selected ) {
    int comp1 = fr.comp(pos1);
    int comp2 = fr.comp(pos2);
    if ( comp1 == comp2 && comp1 >= 0 ) {
      // サイクルができた．
#if 0
      cout << "G: node1(" << node1->x() << ", " << node1->y() << ").comp = " << comp1 << endl;
      cout << "G: node2(" << node2->x() << ", " << node2->y() << ").comp = " << comp2 << endl;
#endif
      return false;
    }
    if ( comp1 < 0 && comp2 < 0 && comp1 != comp2 ) {
      // 異なる端子番号の先がつながった．
#if 0
      cout << "H: node1(" << node1->x() << ", " << node1->y() << ").comp = " << comp1 << endl;
      cout << "H: node2(" << node2->x() << ", " << node2->y() << ").comp = " << comp2 << endl;
#endif
      return false;
    }
    if ( comp1 < comp2 ) {
      // comp2 を comp1 に書き換え．
      fr.replace_comp(comp2, comp1);
    }
    else {
      // comp1 を comp2 に書き換え．
      fr.replace_comp(comp1, comp2);
    }
  }

  fr.delete_nodes(del_list);

  return true;
}

END_NAMESPACE_YM_NLINK

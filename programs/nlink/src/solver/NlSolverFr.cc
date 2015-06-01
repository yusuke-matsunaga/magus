
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
  edge_list.reserve(ne);

  // 枝の順序付けを行う．
  mEdgeOrder.clear();
  mEdgeOrder.resize(ne);
  ymuint w = graph.width();
  ymuint h = graph.height();
  for (ymuint y0 = 0; y0 < h - 1; ++ y0) {
    for (ymuint x = 0; x < w; ++ x) {
      if ( x > y0 ) {
	break;
      }
      ymuint y = y0 - x;
      cout << "(" << x << ", " << y << ")" << endl;
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
      cout << "(" << x << ", " << y << ")" << endl;
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

  // 各レベルごとの解集合
  vector<vector<FrNode*> > node_list_array(ne + 1);

  FrNode* root_node = new FrNode(0);
  node_list_array[0].push_back(root_node);

  FrNode* const0_node = FrNode::const0();
  FrNode* const1_node = FrNode::const1();

  for (ymuint i = 0; i < ne; ++ i) {
    const NlEdge* edge = edge_list[i];
    const vector<FrNode*>& node_list = node_list_array[i];
    vector<FrNode*>& new_node_list = node_list_array[i + 1];
    cout << "Level#" << i << ": " << node_list.size() << endl;
    for (ymuint j = 0; j < node_list.size(); ++ j) {
      FrNode* node = node_list[j];
#if 0
      cout << "FR" << endl;
      node->frontier_info().dump(cout);
#endif

      // edge を選ばないノードを作る．
      FrNode* node0 = (i < ne - 1) ? new FrNode(i + 1) : const1_node;
      FrontierInfo& fr0 = node0->frontier_info();
      // 親のフロンティアをコピーする．
      fr0 = node->frontier_info();

      // フロンティアの更新
      // edge が最後の未処理枝だったノードを削除し
      // edge が最初の処理枝だったノードを追加する．
      // その際に，削除されるノードが条件を満たさなければ false を返す．
      bool stat0 = update_frontier(fr0, edge, false, graph);
#if 0
      cout << " FR0" << endl;
      fr0.dump(cout);
#endif
      if ( !stat0 ) {
	// node の 0 枝は NULL
	node->set_edge0(const0_node);

	delete node0;
      }
      else {
	// node の 0 枝に node0 をセットする．
	node->set_edge0(node0);

	new_node_list.push_back(node0);
      }

      // edge を選ぶノードを作る．
      FrNode* node1 = (i < ne - 1) ? new FrNode(i + 1) : const1_node;
      FrontierInfo& fr1 = node1->frontier_info();
      // 親のフロンティアをコピーする．
      fr1 = node->frontier_info();

      // フロンティアの更新
      // edge が最後の未処理枝だったノードを削除し
      // edge が最初の処理枝だったノードを追加する．
      // その際に，削除されるノードが条件を満たさなければ false を返す．
      bool stat1 = update_frontier(fr1, edge, true, graph);
#if 0
      cout << " FR1" << endl;
      fr1.dump(cout);
#endif
      if ( !stat1 ) {
	// node の 1 枝は NULL
	node->set_edge1(const0_node);

	delete node1;
      }
      else {
	node->set_edge1(node1);

	new_node_list.push_back(node1);
      }
    }
  }

  solution.init(problem);
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
  del_list.reserve(fr.node_num());
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
      ymuint deg = fr.deg(i);
      if ( node->terminal_id() > 0 ) {
	// 終端ノードの場合，deg は 1 でなければならない．
	if ( deg != 1 ) {
	  return false;
	}
      }
      else {
	// 中間ノードの場合，deg は 0 または 2 でなければならない．
	if ( deg != 0 && deg != 2 ) {
	  return false;
	}
      }
      del_list.push_back(id);
    }
  }
  fr.delete_nodes(del_list);

  const NlNode* node1 = edge->node1();
  int pos1 = pos_map[node1->id()];
  if ( pos1 >= 0 ) {
    if ( selected ) {
      // deg を増やす
      fr.inc_deg(pos1);
      ymuint deg = fr.deg(pos1);
      if ( node1->terminal_id() > 0 ) {
	if ( deg > 1 ) {
	  return false;
	}
      }
      else {
	if ( deg > 2 ) {
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
	  return false;
	}
      }
      else {
	if ( deg > 2 ) {
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

  if ( selected ) {
    int comp1 = fr.comp(pos1);
    int comp2 = fr.comp(pos2);
    if ( comp1 == comp2 && comp1 >= 0 ) {
      // サイクルができた．
      return false;
    }
    if ( comp1 < 0 && comp2 < 0 ) {
      // 異なる端子番号の先がつながった．
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

  return true;
}

END_NAMESPACE_YM_NLINK

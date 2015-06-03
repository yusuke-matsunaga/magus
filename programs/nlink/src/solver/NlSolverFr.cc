
/// @file NlSolverFr.cc
/// @brief NlSolverFr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolverFr.h"
#include "NlSolution.h"
#include "NlGraph.h"
#include "NlNode.h"
#include "NlEdge.h"
#include "FrontierInfo.h"
#include "FrNode.h"

#include "YmUtils/HashMap.h"
#include "YmUtils/HashSet.h"


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
// @param[in] graph 問題のグラフ
// @param[in] verbose verbose フラグ
// @param[out] solution 解
void
NlSolverFr::solve(const NlGraph& graph,
		  bool verbose,
		  NlSolution& solution)
{
  solution.init(graph);

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

  // フロンティアノードのリスト
  vector<const NlNode*> frontier_nodes;

  for (ymuint i = 0; i < ne; ++ i) {
    const NlEdge* edge = edge_list[i];

    // 新しいフロンティアノードを求める．
    vector<ymuint> del_list;
    int pos1;
    int pos2;
    calc_frontier(frontier_nodes, edge, graph.max_node_id(), del_list, pos1, pos2);

    const vector<FrNode*>& node_list = node_list_array[i];
    vector<FrNode*>& new_node_list = node_list_array[i + 1];
    cout << endl;
    cout << "Level#" << i << ": " << node_list.size()
	 << "  edge = " << edge->str() << endl;
#if 0
    {
      cout << "Frontiers = ";
      for (ymuint i = 0; i < frontier_nodes.size(); ++ i) {
	cout << " " << frontier_nodes[i]->str();
      }
      cout << endl;
    }
#endif
    ymuint orig_num = 0;
    HashMap<string, FrNode*> node_hash;
    for (ymuint j = 0; j < node_list.size(); ++ j) {
      FrNode* node = node_list[j];
#if 0
      cout << endl;
      cout << "FR:" << node->mBits << endl;
      node->frontier_info().dump(cout);
#endif

      // edge を選ばないノードを作る．
      // 親のフロンティアをコピーする．
      FrontierInfo fr0 = node->frontier_info();

      // 削除されるノードの次数をチェックする．
      bool ng1 = false;
      for (ymuint j = 0; j < del_list.size(); ++ j) {
	ymuint dpos = del_list[j];
	const NlNode* node = graph.node(fr0.node_id(dpos));
	ymuint deg = fr0.deg(dpos);
	if ( node->terminal_id() > 0 ) {
	  if ( deg != 1 ) {
	    ng1 = true;
#if 0
	    cout << "A: " << node->str() << ".deg = " << deg
		 << ": 1 is expected." << endl;
#endif
	    break;
	  }
	}
	else {
	  if ( deg != 0 && deg != 2 ) {
	    ng1 = true;
#if 0
	    cout << "B: " << node->str() << ".deg = " << deg
		 << ": 0 or 2 is expected." << endl;
#endif
	    break;
	  }
	}
      }

      FrNode* node0;
      if ( !ng1 ) {

	// 追加されるノードの処理を行う．
	if ( pos1 == -1 ) {
	  const NlNode* node1 = edge->node1();
	  int comp_id = node1->id();
	  if ( node1->terminal_id() > 0 ) {
	    comp_id = - node1->terminal_id();
	  }
	  fr0.add_node(node1->id(), 0, comp_id);
	}
	if ( pos2 == -1 ) {
	  const NlNode* node2 = edge->node2();
	  int comp_id = node2->id();
	  if ( node2->terminal_id() > 0 ) {
	    comp_id = - node2->terminal_id();
	  }
	  fr0.add_node(node2->id(), 0, comp_id);
	}

	fr0.delete_nodes(del_list);
#if 0
	cout << endl;
	cout << " FR0:" << node->mBits << "0" << endl;
	fr0.dump(cout);
#endif
	++ orig_num;
	string sig0 = fr0.signature();
	if ( node_hash.find(sig0, node0) ) {
	  // ハッシュに同じノードが登録されていた．
	}
	else {
	  // node の 0 枝に node0 をセットする．
	  if ( i < ne - 1 ) {
	    node0 = new FrNode(i + 1, fr0);
	    node0->mBits = node->mBits + "0";

	    new_node_list.push_back(node0);
	    node_hash.add(sig0, node0);
	  }
	  else {
	    node0 = const1_node;
	  }
	}
      }
      else {
	node0 = const0_node;
      }

      // edge を選ぶノードを作る．
      // 親のフロンティアをコピーする．
      FrontierInfo fr1 = node->frontier_info();

      bool ng2 = false;

      FrNode* node1;
      ymuint new_pos1 = pos1;
      ymuint new_pos2 = pos2;
      if ( !ng2 ) {
	// 追加されるノードの処理を行う．
	const NlNode* node1 = edge->node1();
	ymuint term_id1 = node1->terminal_id();
	if ( pos1 == -1 ) {
	  int comp_id = node1->id();
	  if ( term_id1 > 0 ) {
	    comp_id = - term_id1;
	  }
	  new_pos1 = fr1.node_num();
	  fr1.add_node(node1->id(), 1, comp_id);
	}
	else {
	  fr1.inc_deg(pos1);
	  ymuint deg = fr1.deg(pos1);
	  if ( term_id1 > 0 ) {
	    if ( deg > 1 ) {
	      ng2 = true;
#if 0
	      cout << "E: " << node1->str() << ".deg = " << deg
		   << ": 1 is expected." << endl;
#endif
	    }
	  }
	  else {
	    if ( deg > 2 ) {
	      ng2 = true;
#if 0
	      cout << "F: " << node1->str() << ".deg = " << deg
		   << ": 0 or 2 is expected." << endl;
#endif
	    }
	  }
	}
      }
      if ( !ng2 ) {
	// 削除されるノードの次数をチェックする．
	for (ymuint j = 0; j < del_list.size(); ++ j) {
	  ymuint pos = del_list[j];
	  const NlNode* node = graph.node(fr1.node_id(pos));
	  ymuint deg = fr1.deg(pos);
	  if ( node->terminal_id() > 0 ) {
	    if ( deg != 1 ) {
	      ng2 = true;
#if 0
	      cout << "C: " << node->str() << ".deg = " << deg
		   << ": 1 is expected." << endl;
#endif
	      break;
	    }
	  }
	  else {
	    if ( deg != 0 && deg != 2 ) {
	      ng2 = true;
#if 0
	      cout << "D: " << node->str() << ".deg = " << deg
		   << ": 0 or 2 is expected." << endl;
#endif
	      break;
	    }
	  }
	}
      }
      if ( !ng2 ) {
	const NlNode* node2 = edge->node2();
	ymuint term_id2 = node2->terminal_id();
	if ( pos2 == -1 ) {
	  int comp_id = node2->id();
	  if ( term_id2 > 0 ) {
	    comp_id = - term_id2;
	  }
	  new_pos2 = fr1.node_num();
	  fr1.add_node(node2->id(), 1, comp_id);
	}
	else {
	  fr1.inc_deg(pos2);
	  ymuint deg = fr1.deg(pos2);
	  if ( term_id2 > 0 ) {
	    if ( deg > 1 ) {
	      ng2 = true;
#if 0
	      cout << "E2: " << node2->str() << ".deg = " << deg
		   << ": 1 is expected." << endl;
#endif
	    }
	  }
	  else {
	    if ( deg > 2 ) {
	      ng2 = true;
#if 0
	      cout << "F2: " << node2->str() << ".deg = " << deg
		   << ": 0 or 2 is expected." << endl;
#endif
	    }
	  }
	}
      }
      if ( !ng2 ) {
	int comp1 = fr1.comp(new_pos1);
	int comp2 = fr1.comp(new_pos2);
	if ( comp1 == comp2 && comp1 >= 0 ) {
	  // サイクルができた．
#if 0
	  const NlNode* node1 = edge->node1();
	  const NlNode* node2 = edge->node2();
	  cout << "G: node1(" << node1->x() << ", " << node1->y() << ").comp = " << comp1 << endl;
	  cout << "G: node2(" << node2->x() << ", " << node2->y() << ").comp = " << comp2 << endl;
#endif
	  ng2 = true;
	}
	else if ( comp1 < 0 && comp2 < 0 && comp1 != comp2 ) {
	  // 異なる端子番号の先がつながった．
#if 0
	  const NlNode* node1 = edge->node1();
	  const NlNode* node2 = edge->node2();
	  cout << "H: node1(" << node1->x() << ", " << node1->y() << ").comp = " << comp1 << endl;
	  cout << "H: node2(" << node2->x() << ", " << node2->y() << ").comp = " << comp2 << endl;
#endif
	  ng2 = true;
	}
	else if ( comp1 < comp2 ) {
	  // comp2 を comp1 に書き換え．
	  fr1.replace_comp(comp2, comp1);
	}
	else {
	  // comp1 を comp2 に書き換え．
	  fr1.replace_comp(comp1, comp2);
	}
      }
      if ( !ng2 ) {
	fr1.delete_nodes(del_list);

#if 0
	cout << endl;
	cout << " FR1:" << node->mBits << "1" << endl;
	fr1.dump(cout);
#endif
	++ orig_num;
	string sig1 = fr1.signature();
	if ( node_hash.find(sig1, node1) ) {
	  // ハッシュに同じノードが登録されていた．
	}
	else {
	  // node の 0 枝に node0 をセットする．
	  if ( i < ne - 1 ) {
	    node1 = new FrNode(i + 1, fr1);
	    node1->mBits = node->mBits + "1";

	    new_node_list.push_back(node1);
	    node_hash.add(sig1, node1);
	  }
	  else {
	    node1 = const1_node;
	  }
	}
      }
      else {
	node1 = const0_node;
      }

      node->set_child(node0, node1);
    }
    cout << "orig_num = " << orig_num
	 << ", hashed num = " << new_node_list.size() << endl;
  }
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

  ymuint nall = 0;
  for (ymuint y0 = 0; y0 < h - 1; ++ y0) {
    ymuint n = 0;
    for (ymuint d = 0; d <= y0; ++ d) {
      ymuint x = d;
      ymuint y = y0 - d;
      if ( x >= w ) {
	continue;
      }
      cout << " (" << x << ", " << y << ")";
      const NlNode* node = graph.node(x, y);
      {
	++ n;
	const NlEdge* edge = node->lower_edge();
	ASSERT_COND( edge != NULL );
	ymuint order = edge_list.size();
	edge_list.push_back(edge);
	mEdgeOrder[edge->id()] = order;
      }
      if ( x < w - 1 ) {
	++ n;
	const NlEdge* edge = node->right_edge();
	ASSERT_COND( edge != NULL );
	ymuint order = edge_list.size();
	edge_list.push_back(edge);
	mEdgeOrder[edge->id()] = order;
      }
    }
    cout << "               " << n << endl;
    nall += n;
  }
  for (ymuint x0 = 1; x0 < w; ++ x0) {
    ymuint n = 0;
    for (ymuint d = 0; d < h; ++ d) {
      ymuint x = x0 + d;
      ymuint y = h - d - 1;
      if ( x >= w ) {
	break;
      }
      cout << " (" << x << ", " << y << ")";
      const NlNode* node = graph.node(x, y);
      {
	++ n;
	const NlEdge* edge = node->left_edge();
	ASSERT_COND( edge != NULL );
	ymuint order = edge_list.size();
	edge_list.push_back(edge);
	mEdgeOrder[edge->id()] = order;
      }
      if ( y > 0 ) {
	++ n;
	const NlEdge* edge = node->upper_edge();
	ASSERT_COND( edge != NULL );
	ymuint order = edge_list.size();
	edge_list.push_back(edge);
	mEdgeOrder[edge->id()] = order;
      }
    }
    cout << "               " << n << endl;
    nall += n;
  }
  cout << "nall = " << nall << ", ne = " << ne << endl;

  ASSERT_COND( edge_list.size() == ne );
}

// @brief フロンティアの更新を行う．
// @param[inout] frontier_nodes フロンティアノードのリスト
// @param[in] edge 新しく加わる枝
// @param[in] del_list 削除されるノードのフロンティアリスト上の位置
// @param[out] pos1, pos2 枝の両端のノードの位置
void
NlSolverFr::calc_frontier(vector<const NlNode*>& frontier_nodes,
			  const NlEdge* edge,
			  ymuint max_node_id,
			  vector<ymuint>& del_list,
			  int& pos1,
			  int& pos2)
{
  // ノードIDをキーにしてフロンティア内の位置を保持するマップ
  vector<int> pos_map(max_node_id, -1);

  del_list.reserve(frontier_nodes.size());
  for (ymuint i = 0; i < frontier_nodes.size(); ++ i) {
    const NlNode* node = frontier_nodes[i];
    pos_map[node->id()] = i;

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
      // すべての枝を処理した．
      del_list.push_back(i);
    }
  }

  const NlNode* node1 = edge->node1();
  pos1 = pos_map[node1->id()];

  const NlNode* node2 = edge->node2();
  pos2 = pos_map[node2->id()];

  // frontier_nodes を更新する．
  ymuint rpos = 0;
  ymuint wpos = 0;
  for (ymuint i = 0; i < del_list.size(); ++ i) {
    ymuint dpos = del_list[i];
    for ( ; rpos < dpos; ++ rpos) {
      if ( wpos < rpos ) {
	frontier_nodes[wpos] = frontier_nodes[rpos];
      }
      ++ wpos;
    }
    ++ rpos;
  }
  for ( ; rpos < frontier_nodes.size(); ++ rpos) {
    if ( wpos < rpos ) {
      frontier_nodes[wpos] = frontier_nodes[rpos];
    }
    ++ wpos;
  }
  frontier_nodes.erase(frontier_nodes.begin() + wpos, frontier_nodes.end());

  if ( pos1 == -1 ) {
    frontier_nodes.push_back(node1);
  }
  if ( pos2 == -1 ) {
    frontier_nodes.push_back(node2);
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


/// @file NpGraph.cc
/// @brief NpGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NpGraph.h"
#include "NpNode.h"
#include "NpEdge.h"
#include "NlGraph.h"
#include "NlNode.h"
#include "NlEdge.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NpGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] solver SATソルバ
// @param[in] src_graph もとになるグラフ
// @param[in] edge_map 枝の変数番号の配列
NpGraph::NpGraph(SatSolver& solver,
		 const NlGraph& src_graph,
		 const vector<VarId>& edge_map) :
  mNodeArray(src_graph.max_node_id())
{
  for (ymuint i = 0; i < src_graph.max_node_id(); ++ i) {
    NpNode* node = new NpNode(i);
    mNodeArray[i] = node;
  }

  mOrigEdgeList.reserve(src_graph.max_edge_id());
  for (ymuint i = 0; i < src_graph.max_edge_id(); ++ i) {
    const NlEdge* src_edge = src_graph.edge(i);
    const NlNode* src_node1 = src_edge->node1();
    const NlNode* src_node2 = src_edge->node2();
    NpNode* node1 = mNodeArray[src_node1->id()];
    NpNode* node2 = mNodeArray[src_node2->id()];
    connect(node1, node2, edge_map[i], false);
  }

  ymuint num = src_graph.num();
  // 端子間を結ぶ枝を作る．
  for (ymuint i1 = 0; i1 < num; ++ i1) {
    const NlNode* src_node1_1 = src_graph.start_node(i1);
    const NlNode* src_node1_2 = src_graph.end_node(i1);
    NpNode* node1_1 = mNodeArray[src_node1_1->id()];
    NpNode* node1_2 = mNodeArray[src_node1_2->id()];

    //add_equality(solver, node1_1, node1_2, false);

    for (ymuint i2 = i1 + 1; i2 < num; ++ i2) {
      const NlNode* src_node2_1 = src_graph.start_node(i2);
      const NlNode* src_node2_2 = src_graph.end_node(i2);
      NpNode* node2_1 = mNodeArray[src_node2_1->id()];
      NpNode* node2_2 = mNodeArray[src_node2_2->id()];

      add_equality(solver, node1_1, node2_1, true);
      add_equality(solver, node1_1, node2_2, true);
      add_equality(solver, node1_2, node2_1, true);
      add_equality(solver, node1_2, node2_2, true);
    }
  }

  // 対角線に枝を張る．
  ymuint w = src_graph.width();
  ymuint h = src_graph.height();
  for (ymuint x = 0; x < w - 1; ++ x) {
    for (ymuint y = 0; y < h - 1; ++ y) {
      add_shortcut(solver, src_graph, x,     y,     x + 1, y + 1);
      add_shortcut(solver, src_graph, x + 1, y,     x,     y + 1);
    }
  }

  //make_chordal(solver);

}

// @brief デストラクタ
NpGraph::~NpGraph()
{
  for (ymuint i = 0; i < mNodeArray.size(); ++ i) {
    delete mNodeArray[i];
  }

  for (ymuint i = 0; i < mOrigEdgeList.size(); ++ i) {
    delete mOrigEdgeList[i];
  }

  for (ymuint i = 0; i < mAuxEdgeList.size(); ++ i) {
    delete mAuxEdgeList[i];
  }
}

// @brief 経路を反例に加える．
// @param[in] solver SATソルバ
// @param[in] id1, id2 経路の両端のノード番号
void
NpGraph::add_path(SatSolver& solver,
		  const vector<Bool3>& model,
		  ymuint id1,
		  ymuint id2)
{
  NpNode* node1 = mNodeArray[id1];
  NpNode* node2 = mNodeArray[id2];

  ASSERT_COND( find_edge(node1, node2) == NULL );

  // node1, node2 間の最短経路を求める．
  vector<int> label(mNodeArray.size(), -1);
  vector<NpNode*> node_list;
  node_list.reserve(mNodeArray.size());
  vector<NpEdge*> edge_list;
  edge_list.reserve(mNodeArray.size());
  node_list.push_back(node1);
  label[node1->id()] = 0;

  bool reached = false;
  ymuint rpos = 0;
  for (ymuint l = 0; ; ++ l) {
    ymuint endmark = node_list.size();
    for ( ; rpos < endmark; ++ rpos) {
      NpNode* node = node_list[rpos];
      if ( node == node2 ) {
	// ラベルが到達した．
	reached = true;
	// バックトレースする．
	ASSERT_COND( label[node->id()] == l );
	edge_list.resize(l);
	for ( ; ; ) {
	  if ( node == node1 ) {
	    break;
	  }
	  const vector<NpNode*>& adj_list = node->adj_node_list();
	  const vector<NpEdge*>& edge_list = node->edge_list();
	  for (ymuint i = 0; i < adj_list.size(); ++ i) {
	    NpEdge* edge = edge_list[i];
	    if ( model[edge->var().val()] != kB3True ) {
	      continue;
	    }
	    NpNode* prev_node = adj_list[i];
	    if ( label[prev_node->id()] == l - 1 ) {
	      edge_list[l - 1] = edge;
	      node = prev_node;
	      break;
	    }
	  }
	}
	break;
      }
      const vector<NpNode*>& adj_list = node->adj_node_list();
      const vector<NpEdge*>& edge_list = node->edge_list();
      for (ymuint i = 0; i < adj_list.size(); ++ i) {
	NpEdge* edge = edge_list[i];
	if ( model[edge->var().val()] != kB3True ) {
	  continue;
	}
	NpNode* next_node = adj_list[i];
	if ( label[next_node->id()] >= 0 ) {
	  continue;
	}
	node_list.push_back(next_node);
	label[next_node->id()] = l + 1;
      }
    }
  }

  // edge_list を三角分割する．
  split_path(solver, edge_list);
}

Literal
NpGraph::split_path(SatSolver& solver,
		    const vector<NpEdge*>& edge_list)
{
  ymuint n = edge_list.size();
  ASSERT_COND( n > 1 );

  if ( n == 2 ) {
    NpEdge* edge1 = edge_list[0];
    NpNode* node1_1 = edge1->node1();
    NpNode* node1_2 = edge1->node2();

    NpEdge* edge2 = edge_list[1];
    NpNode* node2_1 = edge2->node1();
    NpNode* node2_2 = edge2->node2();

    NpNode* s_node;
    NpNode* e_node;

    if ( node1_1 == node2_1 ) {
      s_node = node1_2;
      e_node = node2_2;
    }
    else if ( node1_1 == node2_2 ) {
      s_node = node1_2;
      e_node = node2_1;
    }
    else if ( node1_2 == node2_1 ) {
      s_node = node1_1;
      e_node = node2_2;
    }
    else { // node1_2 == node2_2
      s_node = node1_1;
      e_node = node2_1;
    }

    VarId var3 = solver.new_var();
    connect(s_node, e_node, var3, true);

    Literal lit1(edge1->var());
    Literal lit2(edge2->var());
    Literal lit3(var);

    solver.add_clause(~lit1, ~lit2,  lit3);
    solver.add_clause(~lit1,  lit2, ~lit3);
    solver.add_clause( lit1, ~lit2, ~lit3);

    return lit3;
  }
  else {
    ymuint nl = n / 2;
    ymuint nr = n - nl;
    vector<NpEdge*> l_list(nl);
    for (ymuint i = 0; i < nl; ++ i) {
      l_list[i] = edge_list[i];
    }
    Literal lit1 = split_path(solver, l_list);

    vector<NpEdge*> r_list(nr);
    for (ymuint i = 0; i < nr; ++ i) {
      r_list[i] = edge_list[i + nl];
    }
    Literal lit2 = split_path(solver, r_list);


  }
}

// @brief chordal graph に変形する．
// @param[in] solver SAT ソルバ
void
NpGraph::make_chordal(SatSolver& solver)
{
  for (ymuint id = 0; id < mNodeArray.size(); ++ id) {
    NpNode* node = mNodeArray[id];
    const vector<NpNode*>& adj_list = node->adj_node_list();
    const vector<NpEdge*>& edge_list = node->edge_list();
    vector<NpNode*> tmp_list;
    vector<NpEdge*> tmp_edge_list;
    tmp_list.reserve(adj_list.size());
    tmp_edge_list.reserve(adj_list.size());
    for (ymuint i = 0; i < adj_list.size(); ++ i) {
      NpNode* node1 = adj_list[i];
      if ( node1->id() < id ) {
	continue;
      }
      tmp_list.push_back(node1);
      tmp_edge_list.push_back(edge_list[i]);
    }
    for (ymuint i1 = 0; i1 < tmp_list.size(); ++ i1) {
      NpNode* node1 = tmp_list[i1];
      NpEdge* edge1 = tmp_edge_list[i1];
      const vector<NpNode*>& adj_list1 = node1->adj_node_list();
      const vector<NpEdge*>& edge_list1 = node1->edge_list();
      for (ymuint i2 = i1 + 1; i2 < tmp_list.size(); ++ i2) {
	NpNode* node2 = tmp_list[i2];
	NpEdge* edge2 = tmp_edge_list[i2];
	NpEdge* edge3 = find_edge(node1, node2);
	VarId var3;
	if ( edge3 == NULL ) {
	  var3 = solver.new_var();
	  connect(node1, node2, var3, true);
	}
	else {
	  var3 = edge3->var();
	}

	// edge1->var(), edge2->var(), var3 の間の推移律を制約に加える．
	Literal lit1(edge1->var());
	Literal lit2(edge2->var());
	Literal lit3(var3);
	solver.add_clause(~lit1, ~lit2,  lit3);
	solver.add_clause(~lit1,  lit2, ~lit3);
	solver.add_clause( lit1, ~lit2, ~lit3);
      }
    }
  }
  cout << "make_chordal end: # of nodes          = " << mNodeArray.size() << endl
       << "                  # of original edges = " << mOrigEdgeList.size() << endl
       << "                  # of auxially edges = " << mAuxEdgeList.size() << endl;
}

// @brief short-cut 枝を追加する．
// @param[in] solver SATソルバ
// @param[in] src_graph もとになるグラフ
// @param[in] x1, y1 ノード1の座標
// @param[in] x1, y2 ノード2の座標
void
NpGraph::add_shortcut(SatSolver& solver,
		      const NlGraph& src_graph,
		      ymuint x1,
		      ymuint y1,
		      ymuint x2,
		      ymuint y2)
{
  const NlNode* src_node1 = src_graph.node(x1, y1);
  const NlNode* src_node2 = src_graph.node(x2, y2);
  NpNode* node1 = mNodeArray[src_node1->id()];
  NpNode* node2 = mNodeArray[src_node2->id()];

  if ( find_edge(node1, node2) == NULL ) {
    VarId var = solver.new_var();
    connect(node1, node2, var, true);
  }
}

// @brief 等価制約を追加する．
// @param[in] solver SATソルバ
// @param[in] node1, node2 両端のノード
// @param[in] disequality 非等価制約の時 true にするフラグ
void
NpGraph::add_equality(SatSolver& solver,
		      NpNode* node1,
		      NpNode* node2,
		      bool disequality)
{
  NpEdge* edge = find_edge(node1, node2);
  VarId dvar;
  if ( edge == NULL ) {
    dvar = solver.new_var();
    connect(node1, node2, dvar, true);
  }
  else {
    dvar = edge->var();
  }
  Literal dlit(dvar, disequality);
  solver.add_clause(dlit);
 }

// @brief node1 と node2 を結ぶ枝を探す．
// @param[in] node1, node2 両端のノード
//
// なければ NULL を返す．
NpEdge*
NpGraph::find_edge(NpNode* node1,
		   NpNode* node2) const
{
  ASSERT_COND( node1 != node2 );

  const vector<NpNode*>& adj_list1 = node1->adj_node_list();
  const vector<NpNode*>& adj_list2 = node2->adj_node_list();
  if ( adj_list1.size() < adj_list2.size() ) {
    for (ymuint i = 0; i < adj_list1.size(); ++ i) {
      NpNode* node3 = adj_list1[i];
      if ( node3 == node2 ) {
	return node1->edge_list()[i];
      }
    }
  }
  else {
    for (ymuint i = 0; i < adj_list2.size(); ++ i) {
      NpNode* node3 = adj_list2[i];
      if ( node3 == node1 ) {
	return node2->edge_list()[i];
      }
    }
  }

  return NULL;
}

// @brief node1 と node2 を結ぶ枝を作る．
// @param[in] node1, node2 両端のノード
// @param[in] var 変数番号
// @param[in] aux 追加枝の場合に true にするフラグ
void
NpGraph::connect(NpNode* node1,
		 NpNode* node2,
		 VarId var,
		 bool aux)
{
  NpEdge* edge = new NpEdge(node1, node2, var);
  node1->mEdgeList.push_back(edge);
  node1->mAdjNodeList.push_back(node2);
  node2->mEdgeList.push_back(edge);
  node2->mAdjNodeList.push_back(node1);
  if ( aux ) {
    mAuxEdgeList.push_back(edge);
  }
  else {
    mOrigEdgeList.push_back(edge);
  }
}

END_NAMESPACE_YM_NLINK


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

#if 0
  ymuint num = src_graph.num();
  // 異なる端子が隣接している場合にその枝を禁止する．
  for (ymuint i1 = 0; i1 < num; ++ i1) {
    const NlNode* src_node1_1 = src_graph.start_node(i1);
    const NlNode* src_node1_2 = src_graph.end_node(i1);
    NpNode* node1_list[2];
    node1_list[0] = mNodeArray[src_node1_1->id()];
    node1_list[1] = mNodeArray[src_node1_2->id()];
    for (ymuint i2 = i1 + 1; i2 < num; ++ i2) {
      const NlNode* src_node2_1 = src_graph.start_node(i2);
      const NlNode* src_node2_2 = src_graph.end_node(i2);
      NpNode* node2_list[2];
      node2_list[0] = mNodeArray[src_node2_1->id()];
      node2_list[1] = mNodeArray[src_node2_2->id()];

      for (ymuint x = 0; x < 4; ++ x) {
	NpNode* node1 = node1_list[(x >> 1) & 1];
	NpNode* node2 = node2_list[x & 1];
	NpEdge* edge = find_edge(node1, node2);
	if ( edge != NULL ) {
	  Literal lit(edge->var());
	  solver.add_clause(~lit);
	}
      }
    }
  }
#endif

#if 1
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
#endif

  // 対角線に枝を張る．
  ymuint w = src_graph.width();
  ymuint h = src_graph.height();
  for (ymuint x = 0; x < w - 1; ++ x) {
    for (ymuint y = 0; y < h - 1; ++ y) {
      add_shortcut(solver, src_graph, x,     y,     x + 1, y + 1);
      add_shortcut(solver, src_graph, x + 1, y,     x,     y + 1);
    }
  }

  make_chordal(solver);
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
NpGraph::add_path_constr(SatSolver& solver,
			 const vector<Bool3>& model,
			 ymuint id1,
			 ymuint id2)
{
  NpNode* node1 = mNodeArray[id1];
  NpNode* node2 = mNodeArray[id2];

  cout << "add_path_constr(" << id1 << ", " << id2 << ")" << endl;

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
  for (ymuint l = 0; !reached; ++ l) {
    ymuint endmark = node_list.size();
    for ( ; rpos < endmark; ++ rpos) {
      NpNode* node = node_list[rpos];
      if ( node == node2 ) {
	// ラベルが到達した．
	reached = true;
	// バックトレースする．
	ASSERT_COND( label[node->id()] == l );
	edge_list.resize(l);
	for ( ; ; -- l) {
	  if ( node == node1 ) {
	    break;
	  }
	  bool prev_found = false;
	  const vector<NpEdge*>& node_edge_list = node->edge_list();
	  for (ymuint i = 0; i < node_edge_list.size(); ++ i) {
	    NpEdge* edge = node_edge_list[i];
	    if ( model[edge->var().val()] != kB3True ) {
	      continue;
	    }
	    NpNode* prev_node = edge->alt_node(node);
	    if ( label[prev_node->id()] == l - 1 ) {
	      edge_list[l - 1] = edge;
	      node = prev_node;
	      prev_found = true;
	      break;
	    }
	  }
	  ASSERT_COND( prev_found );
	}
	break;
      }
      const vector<NpEdge*>& edge_list = node->edge_list();
      for (ymuint i = 0; i < edge_list.size(); ++ i) {
	NpEdge* edge = edge_list[i];
	if ( model[edge->var().val()] != kB3True ) {
	  continue;
	}
	NpNode* next_node = edge->alt_node(node);
	if ( label[next_node->id()] >= 0 ) {
	  continue;
	}
	node_list.push_back(next_node);
	label[next_node->id()] = l + 1;
      }
    }
  }

  // edge_list の枝が同時に選ばれないような制約を加える．
  // ただし edge_list の要素数が2以上の時は経路を分割して
  // ここのセグメントごとの制約にする．
  ymuint n = edge_list.size();

  ASSERT_COND( n > 0 );

  if ( n == 1 ) {
    Literal lit(edge_list[0]->var());
    solver.add_clause(~lit);
  }
  else { // n > 2
    ymuint nh = n / 2;
    // 前半部分を表すリテラル
    Literal lit1 = make_path_literal(solver, edge_list, 0, nh);
    // 後半部分を表すリテラル
    Literal lit2 = make_path_literal(solver, edge_list, nh, n);
    // これが同時に選ばれてはいけない．
    solver.add_clause(~lit1, ~lit2);
  }
}

BEGIN_NONAMESPACE

inline
void
analyze_path(NpEdge* edge1,
	     NpEdge* edge2,
	     NpNode*& s_node,
	     NpNode*& m_node,
	     NpNode*& e_node)
{
  NpNode* node1_1 = edge1->node1();
  NpNode* node1_2 = edge1->node2();

  NpNode* node2_1 = edge2->node1();
  NpNode* node2_2 = edge2->node2();

  if ( node1_1 == node2_1 ) {
    s_node = node1_2;
    m_node = node1_1;
    e_node = node2_2;
  }
  else if ( node1_1 == node2_2 ) {
    s_node = node1_2;
    m_node = node1_1;
    e_node = node2_1;
  }
  else if ( node1_2 == node2_1 ) {
    s_node = node1_1;
    m_node = node1_2;
    e_node = node2_2;
  }
  else { // node1_2 == node2_2
    s_node = node1_1;
    m_node = node1_2;
    e_node = node2_1;
  }
}

END_NONAMESPACE

// @brief 経路を表すリテラルを求める．
// @param[in] solver SATソルバ
// @param[in] edge_list 枝のリスト
// @param[in] start_pos 始点
// @param[in] end_pos 終点
Literal
NpGraph::make_path_literal(SatSolver& solver,
			   const vector<NpEdge*>& edge_list,
			   ymuint start_pos,
			   ymuint end_pos)
{
  ymuint n = end_pos - start_pos;

  if ( n == 1 ) {
    NpEdge* edge = edge_list[start_pos];
    return Literal(edge->var());
  }

  // n は 2以上

  // edge_list を中央で2分割する．
  ymuint nl = n / 2;
  Literal lit1 = make_path_literal(solver, edge_list, start_pos, start_pos + nl);
  Literal lit2 = make_path_literal(solver, edge_list, start_pos + nl, end_pos);

  // 経路の先頭と末尾を求める．
  NpNode* start_node = NULL;
  {
    NpNode* m_node;
    NpNode* e_node;
    analyze_path(edge_list[start_pos], edge_list[start_pos + 1],
		 start_node, m_node, e_node);
  }
  NpNode* end_node = NULL;
  {
    NpNode* s_node;
    NpNode* m_node;
    analyze_path(edge_list[end_pos - 2], edge_list[end_pos - 1],
		 s_node, m_node, end_node);
  }

  // (start_node, end_node) という枝を見つける．
  VarId var3;
  NpEdge* edge3 = find_edge(start_node, end_node);
  if ( edge3 != NULL ) {
    var3 = edge3->var();
  }
  else {
    // なかったら新たに作る．
    var3 = solver.new_var();
    connect(start_node, end_node, var3, true);
  }
  Literal lit3(var3);

  // lit1, lit2, lit3 の間の推移律に関する制約を追加する．
  solver.add_clause(~lit1, ~lit2,  lit3);
  solver.add_clause(~lit1,  lit2, ~lit3);
  solver.add_clause( lit1, ~lit2, ~lit3);

  return lit3;
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


/// @file NpGraph2.cc
/// @brief NpGraph2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NpGraph2.h"
#include "NpNode.h"
#include "NpEdge.h"
#include "NlGraph.h"
#include "NlNode.h"
#include "NlEdge.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NpGraph2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] solver SATソルバ
// @param[in] src_graph もとになるグラフ
// @param[in] edge_map 枝の変数番号の配列
NpGraph2::NpGraph2(SatSolver& solver,
		   const NlGraph& src_graph,
		   ymuint group_num,
		   const vector<ymuint>& group_map,
		   const vector<VarId>& edge_map)
{
  mGroupNum = group_num;

  // ノードを作る．
  ymuint nn = src_graph.max_node_id() * mGroupNum;
  mNodeArray.resize(nn);
  for (ymuint i = 0; i < nn; ++ i) {
    NpNode* node = new NpNode(i);
    mNodeArray[i] = node;
  }

  // オリジナルの枝を作る．
  ymuint ne = src_graph.max_edge_id() * mGroupNum;
  mOrigEdgeList.reserve(ne);
  for (ymuint i = 0; i < src_graph.max_edge_id(); ++ i) {
    const NlEdge* src_edge = src_graph.edge(i);
    const NlNode* src_node1 = src_edge->node1();
    const NlNode* src_node2 = src_edge->node2();
    for (ymuint g = 0; g < mGroupNum; ++ g) {
      NpNode* node1 = _node(src_node1->id(), g);
      NpNode* node2 = _node(src_node2->id(), g);
      connect(node1, node2, edge_map[src_edge->id() * mGroupNum + g], false);
    }
  }

  ymuint num = src_graph.num();
  // 端子間を結ぶ枝を作る．
  for (ymuint i1 = 0; i1 < num; ++ i1) {
    ymuint g1 = group_map[i1];
    const NlNode* src_node1_1 = src_graph.start_node(i1);
    const NlNode* src_node1_2 = src_graph.end_node(i1);
    NpNode* node1_1 = _node(src_node1_1->id(), g1);
    NpNode* node1_2 = _node(src_node1_2->id(), g1);

    for (ymuint i2 = i1 + 1; i2 < num; ++ i2) {
      ymuint g2 = group_map[i2];
      if ( g2 != g1 ) {
	continue;
      }

      const NlNode* src_node2_1 = src_graph.start_node(i2);
      const NlNode* src_node2_2 = src_graph.end_node(i2);
      NpNode* node2_1 = _node(src_node2_1->id(), g1);
      NpNode* node2_2 = _node(src_node2_2->id(), g1);

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
      for (ymuint g = 0; g < mGroupNum; ++ g) {
	add_shortcut(solver, src_graph, x,     y,     x + 1, y + 1, g);
	add_shortcut(solver, src_graph, x + 1, y,     x,     y + 1, g);
      }
    }
  }

  make_chordal(solver);
}

// @brief デストラクタ
NpGraph2::~NpGraph2()
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

// @brief chordal graph に変形する．
// @param[in] solver SAT ソルバ
void
NpGraph2::make_chordal(SatSolver& solver)
{
  for (ymuint id = 0; id < mNodeArray.size(); ++ id) {
    NpNode* node = mNodeArray[id];
    const vector<NpEdge*>& edge_list = node->edge_list();
    vector<NpEdge*> tmp_edge_list;
    tmp_edge_list.reserve(edge_list.size());
    for (ymuint i = 0; i < edge_list.size(); ++ i) {
      NpEdge* edge1 = edge_list[i];
      NpNode* node1 = edge1->alt_node(node);
      if ( node1->id() < id ) {
	continue;
      }
      tmp_edge_list.push_back(edge1);
    }
    for (ymuint i1 = 0; i1 < tmp_edge_list.size(); ++ i1) {
      NpEdge* edge1 = tmp_edge_list[i1];
      NpNode* node1 = edge1->alt_node(node);
      for (ymuint i2 = i1 + 1; i2 < tmp_edge_list.size(); ++ i2) {
	NpEdge* edge2 = tmp_edge_list[i2];
	NpNode* node2 = edge2->alt_node(node);

	NpEdge* edge3 = find_edge(node1, node2);
	if ( edge3 == NULL ) {
	  VarId var = solver.new_var();
	  edge3 = connect(node1, node2, var, true);
	}

	// edge1->var(), edge2->var(), edge3 の間の推移律を制約に加える．
	Literal lit1(edge1->var());
	Literal lit2(edge2->var());
	Literal lit3(edge3->var());
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
NpGraph2::add_shortcut(SatSolver& solver,
		       const NlGraph& src_graph,
		       ymuint x1,
		       ymuint y1,
		       ymuint x2,
		       ymuint y2,
		       ymuint g)
{
  const NlNode* src_node1 = src_graph.node(x1, y1);
  const NlNode* src_node2 = src_graph.node(x2, y2);
  NpNode* node1 = _node(src_node1->id(), g);
  NpNode* node2 = _node(src_node2->id(), g);

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
NpGraph2::add_equality(SatSolver& solver,
		       NpNode* node1,
		       NpNode* node2,
		       bool disequality)
{
  NpEdge* edge = find_edge(node1, node2);
  if ( edge == NULL ) {
    VarId var = solver.new_var();
    edge = connect(node1, node2, var, true);
  }

  VarId dvar = edge->var();
  Literal dlit(dvar, disequality);
  solver.add_clause(dlit);
 }

// @brief node1 と node2 を結ぶ枝を探す．
// @param[in] node1, node2 両端のノード
//
// なければ NULL を返す．
NpEdge*
NpGraph2::find_edge(NpNode* node1,
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
NpEdge*
NpGraph2::connect(NpNode* node1,
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
  return edge;
}

// @brief ノードを取り出す．
// @param[in] node_id ノード番号
// @param[in] group グループ番号
NpNode*
NpGraph2::_node(ymuint node_id,
		ymuint group)
{
  return mNodeArray[node_id * mGroupNum + group];
}

END_NAMESPACE_YM_NLINK

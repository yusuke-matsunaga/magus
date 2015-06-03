
/// @file EqSolver.cc
/// @brief EqSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "EqSolver.h"
#include "EqNode.h"
#include "EqEdge.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス EqSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_num 変数の数
EqSolver::EqSolver(ymuint var_num) :
  mNodeArray(var_num)
{
  for (ymuint i = 0; i < var_num; ++ i) {
    EqNode* node = new EqNode(i);
    mNodeArray[i] = node;
  }
}

// @brief デストラクタ
EqSolver::~EqSolver()
{
  for (ymuint i = 0; i < mNodeArray.size(); ++ i) {
    delete mNodeArray[i];
  }
  for (ymuint i = 0; i < mEdgeArray.size(); ++ i) {
    delete mEdgeArray[i];
  }
}

// @brief 述語を追加する．
// @param[in] solver SATソルバ
// @param[in] var1, var2 変数番号
// @return 命題変数を返す．
VarId
EqSolver::add_equal(SatSolver& solver,
		    ymuint var1,
		    ymuint var2)
{
  ASSERT_COND( var1 < mNodeArray.size() );
  ASSERT_COND( var2 < mNodeArray.size() );

  EqNode* node1 = mNodeArray[var1];
  EqNode* node2 = mNodeArray[var2];

  EqEdge* edge = NULL;
  const vector<EqEdge*>& edge_list1 = node1->edge_list();
  const vector<EqEdge*>& edge_list2 = node2->edge_list();
  if ( edge_list1.size() < edge_list2.size() ) {
    for (ymuint i = 0; i < edge_list1.size(); ++ i) {
      EqEdge* edge1 = edge_list1[i];
      if ( edge1->alt_node(node1) == node2 ) {
	edge = edge1;
	break;
      }
    }
  }
  else {
    for (ymuint i = 0; i < edge_list2.size(); ++ i) {
      EqEdge* edge1 = edge_list2[i];
      if ( edge1->alt_node(node2) == node1 ) {
	edge = edge1;
	break;
      }
    }
  }

  if ( edge == NULL ) {
    VarId var = solver.new_var();
    edge = new EqEdge(node1, node2, var);
    mEdgeArray.push_back(edge);
    node1->mEdgeList.push_back(edge);
    node2->mEdgeList.push_back(edge);
  }

  return edge->var();
}

// @brief 推移的な等価関係を表す制約を加える．
// @param[in] solver SATソルバ
void
EqSolver::make_constr(SatSolver& solver)
{
  for (ymuint id = 0; id < mNodeArray.size(); ++ id) {
    EqNode* node = mNodeArray[id];
    const vector<EqEdge*>& src_edge_list = node->edge_list();
    vector<EqEdge*> edge_list;
    vector<EqNode*> adj_list;
    adj_list.reserve(src_edge_list.size());
    for (ymuint i = 0; i < src_edge_list.size(); ++ i) {
      EqEdge* edge = src_edge_list[i];
      EqNode* node1 = edge->alt_node(node);
      if ( node1->id() < id ) {
	continue;
      }
      edge_list.push_back(edge);
      adj_list.push_back(node1);
    }

    for (ymuint i1 = 0; i1 < adj_list.size(); ++ i1) {
      EqEdge* edge1 = edge_list[i1];
      EqNode* node1 = adj_list[i1];
      for (ymuint i2 = i1 + 1; i2 < adj_list.size(); ++ i2) {
	EqEdge* edge2 = edge_list[i2];
	EqNode* node2 = adj_list[i2];
	VarId var3 = add_equal(solver, node1->id(), node2->id());

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
}

END_NAMESPACE_YM_NLINK


/// @file NlSolver.cc
/// @brief NlSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver.h"
#include "NlProblem.h"
#include "NlGraph.h"
#include "NlSolution.h"
#include "MazeRouter.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlSolver::NlSolver()
{
}

// @brief デストラクタ
NlSolver::~NlSolver()
{
}

BEGIN_NONAMESPACE

inline
void
one_hot2(SatSolver& solver,
	 Literal lit1,
	 Literal lit2)
{
  solver.add_clause(~lit1, ~lit2);
  solver.add_clause( lit1,  lit2);
}

inline
void
one_hot3(SatSolver& solver,
	 Literal lit1,
	 Literal lit2,
	 Literal lit3)
{
  solver.add_clause(~lit1, ~lit2);
  solver.add_clause(~lit1, ~lit3);
  solver.add_clause(~lit2, ~lit3);
  solver.add_clause( lit1,  lit2,  lit3);
}

inline
void
one_hot4(SatSolver& solver,
	 Literal lit1,
	 Literal lit2,
	 Literal lit3,
	 Literal lit4)
{
  solver.add_clause(~lit1, ~lit2);
  solver.add_clause(~lit1, ~lit3);
  solver.add_clause(~lit1, ~lit4);
  solver.add_clause(~lit2, ~lit3);
  solver.add_clause(~lit2, ~lit4);
  solver.add_clause(~lit3, ~lit4);
  solver.add_clause( lit1,  lit2,  lit3,  lit4);
}

inline
void
zero_two_hot2(SatSolver& solver,
	      Literal lit1,
	      Literal lit2)
{
  solver.add_clause(~lit1,  lit2);
  solver.add_clause( lit1, ~lit2);
}

inline
void
zero_two_hot3(SatSolver& solver,
	      Literal lit1,
	      Literal lit2,
	      Literal lit3)
{
  solver.add_clause(~lit1, ~lit2, ~lit3);
  solver.add_clause( lit1, ~lit2,  lit3);
  solver.add_clause( lit1,  lit2, ~lit3);
  solver.add_clause(~lit1,  lit2,  lit3);
}

inline
void
zero_two_hot4(SatSolver& solver,
	      Literal lit1,
	      Literal lit2,
	      Literal lit3,
	      Literal lit4)
{
  solver.add_clause( lit1,  lit2,  lit3, ~lit4);
  solver.add_clause( lit1,  lit2, ~lit3,  lit4);
  solver.add_clause( lit1, ~lit2,  lit3,  lit4);
  solver.add_clause( lit1, ~lit2, ~lit3, ~lit4);
  solver.add_clause(~lit1,  lit2,  lit3,  lit4);
  solver.add_clause(~lit1,  lit2, ~lit3, ~lit4);
  solver.add_clause(~lit1, ~lit2,  lit3, ~lit4);
  solver.add_clause(~lit1, ~lit2, ~lit3,  lit4);
  solver.add_clause(~lit1, ~lit2, ~lit3, ~lit4);
}

END_NONAMESPACE


// @brief 問題を解く
// @param[in] problem 問題
// @param[out] solution 解
void
NlSolver::solve(const NlProblem& problem,
		NlSolution& solution)
{
  SatSolver solver("minisat2", string(), NULL);
  //SatSolver solver("satrec", string(), &cout);

  NlGraph graph;
  graph.set_problem(problem);

  make_base_cnf(solver, graph);

  solution.init(problem);

  vector<Bool3> model;
  Bool3 stat = solver.solve(model);
  switch ( stat ) {
  case kB3True:
    cout << "SAT" << endl;
    setup_solution(graph, model, solution);
    break;

  case kB3False:
    cout << "UNSAT" << endl;
    break;

  case kB3X:
    cout << "ABORT" << endl;
    break;
  }
}

// @brief 基本的な制約を作る．
void
NlSolver::make_base_cnf(SatSolver& solver,
			const NlGraph& graph)
{
  ymuint width = graph.width();
  ymuint height = graph.height();
  mNum = graph.num();

  // 枝の変数を作る．
  ymuint max_edge_id = graph.max_edge_id();
  mEdgeVarArray.clear();
  mEdgeVarArray.resize(max_edge_id);
  for (ymuint i = 0; i < max_edge_id; ++ i) {
    VarId var = solver.new_var();
    mEdgeVarArray[i] = var;
  }

  // ノードの変数を作る．
  ymuint max_node_id = graph.max_node_id();
  mNodeVarArray.clear();
  mNodeVarArray.resize(max_node_id * mNum);
  for (ymuint i = 0; i < mNodeVarArray.size(); ++ i) {
    VarId var = solver.new_var();
    mNodeVarArray[i] = var;
  }

  // 始点と終点に印をつける．
  for (ymuint i = 0; i < mNum; ++ i) {
    const NlNode* node1 = graph.start_node(i);
    Literal lit1(node_var(node1, i));
    solver.add_clause(lit1);

    const NlNode* node2 = graph.end_node(i);
    Literal lit2(node_var(node2, i));
    solver.add_clause(lit2);
  }

  // one-hot 制約を作る．
  // 一つのノードでは高々1つの変数しか1にならない．
  for (ymuint x = 0; x < width; ++ x) {
    for (ymuint y = 0; y < height; ++ y) {
      const NlNode* node = graph.node(x, y);
      for (ymuint k1 = 0; k1 < mNum; ++ k1) {
	Literal lit1(node_var(node, k1));
	for (ymuint k2 = k1 + 1; k2 < mNum; ++ k2) {
	  Literal lit2(node_var(node, k2));
	  solver.add_clause(~lit1, ~lit2);
	}
      }
    }
  }

  // 枝の条件を作る．
  for (ymuint x = 0; x < width; ++ x) {
    for (ymuint y = 0; y < height; ++ y) {
      const NlNode* node = graph.node(x, y);

      const vector<ymuint>& edge_list = node->edge_list();
      ymuint ne = edge_list.size();
      vector<Literal> lit_list(ne);
      for (ymuint i = 0; i < ne; ++ i) {
	lit_list[i] = Literal(edge_var(edge_list[i]));
      }

      ymuint term_id = node->terminal_id();
      if ( term_id > 0 ) {
	// 端点の場合
	// 必ずただ1つの枝が選ばれる．
	switch ( ne ) {
	case 2:
	  one_hot2(solver, lit_list[0], lit_list[1]);
	  break;

	case 3:
	  one_hot3(solver, lit_list[0], lit_list[1], lit_list[2]);
	  break;

	case 4:
	  one_hot4(solver, lit_list[0], lit_list[1], lit_list[2], lit_list[3]);
	  break;

	default:
	  ASSERT_NOT_REACHED;
	}
      }
      else {
	// そうでない場合
	// 0個か2個の枝が選ばれる．
	switch ( ne ) {
	case 2:
	  zero_two_hot2(solver, lit_list[0], lit_list[1]);
	  break;

	case 3:
	  zero_two_hot3(solver, lit_list[0], lit_list[1], lit_list[2]);
	  break;

	case 4:
	  zero_two_hot4(solver, lit_list[0], lit_list[1], lit_list[2], lit_list[3]);
	  break;

	default:
	  ASSERT_NOT_REACHED;
	}
      }
    }
  }

  // 横方向の隣接関係の条件を作る．
  for (ymuint x = 0; x < width - 1; ++ x) {
    for (ymuint y = 0; y < height; ++ y) {
      ymuint edge = graph.right_edge(x, y);
      Literal e_lit(edge_var(edge));
      const NlNode* node1 = graph.node(x, y);
      const NlNode* node2 = graph.node(x + 1, y);
      for (ymuint k = 0; k < mNum; ++ k) {
	Literal v_lit1(node_var(node1, k));
	Literal v_lit2(node_var(node2, k));
	solver.add_clause(~e_lit,  v_lit1, ~v_lit2);
	solver.add_clause(~e_lit, ~v_lit1,  v_lit2);
      }
    }
  }

  // 縦方向の隣接関係の条件を作る．
  for (ymuint y = 0; y < height - 1; ++ y) {
    for (ymuint x = 0; x < width; ++ x) {
      ymuint edge = graph.lower_edge(x, y);
      Literal e_lit(edge_var(edge));
      const NlNode* node1 = graph.node(x, y);
      const NlNode* node2 = graph.node(x, y + 1);
      for (ymuint k = 0; k < mNum; ++ k) {
	Literal v_lit1(node_var(node1, k));
	Literal v_lit2(node_var(node2, k));
	solver.add_clause(~e_lit,  v_lit1, ~v_lit2);
	solver.add_clause(~e_lit, ~v_lit1,  v_lit2);
      }
    }
  }
}

// @brief 解を出力する．
// @param[in] model SATの解
// @param[in] solution 解
void
NlSolver::setup_solution(const NlGraph& graph,
			 const vector<Bool3>& model,
			 NlSolution& solution)
{
  ymuint width = graph.width();
  ymuint height = graph.height();
  for (ymuint y = 0; y < height; ++ y) {
    for (ymuint x = 0; x < width; ++ x) {
      if ( solution.get(x, y) < 0 ) {
	continue;
      }
      const NlNode* node = graph.node(x, y);
      for (ymuint k = 0; k < mNum; ++ k) {
	VarId var = node_var(node, k);
	if ( model[var.val()] == kB3True ) {
	  solution.set(x, y, k + 1);
	  break;
	}
      }
    }
  }
}

// @brief ノードの変数を得る．
VarId
NlSolver::node_var(const NlNode* node,
		   ymuint idx) const
{
  return mNodeVarArray[node->id() * mNum + idx];
}

// @brief 枝の変数を得る．
VarId
NlSolver::edge_var(ymuint edge) const
{
  return mEdgeVarArray[edge - 1];
}

END_NAMESPACE_YM_NLINK
